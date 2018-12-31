//
// Created by WENBO JIANG on 14/12/18.
//
#include "GpuDec.h"

GPUDecoder::GPUDecoder(int device_id) {
    ck(cuInit(0));
    int nGpu = 0;
    ck(cuDeviceGetCount(&nGpu));
    if (device_id < 0 || device_id > nGpu - 1) {
        throw std::invalid_argument("Invalid devide id");
    }
    CUdevice cuDevice = 0;
    ck(cuDeviceGet(&cuDevice, device_id));
    char szDeviceName[80];
    ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
    std::cout << "GPU in use: " << szDeviceName << std::endl;
    cuContext = NULL;
    ck(cuCtxCreate(&cuContext, 0, cuDevice));
}


GPUDecoder::~GPUDecoder() {
    if(pTmpImage) {
        cuMemFree(pTmpImage);
    }
}

GPUDecoder& GPUDecoder::operator=(GPUDecoder&& gd) {
	dec.swap(gd.dec);
	demuxer.swap(gd.demuxer);
	return *this;
}

GPUDecoder& GPUDecoder::operator=(GPUDecoder& gd) {
	dec.swap(gd.dec);
	demuxer.swap(gd.demuxer);
	return *this;
}


int GPUDecoder::GetHeight() {
    return (demuxer)? demuxer->GetHeight() : 0;
}

int GPUDecoder::GetWidth() {
    return (demuxer)? demuxer->GetWidth() : 0;
}

void GPUDecoder::IngestVideo(const char* videoFile) {
    demuxer.reset(new FFmpegDemuxer(videoFile));
    dec.reset(new NvDecoder(cuContext, demuxer->GetWidth(), demuxer->GetHeight(), true, FFmpeg2NvCodecId(demuxer->GetVideoCodec())));
    frameSize = 3 * demuxer->GetWidth() * demuxer->GetHeight();
    // Delete last image
    pImage.reset(new uint8_t[frameSize]);
    // Free last cuda buffer
    if(pTmpImage) {
        cuMemFree(pTmpImage);
    }
    // Create new frame container
    bgrImage.reset(new cv::Mat(demuxer->GetWidth(), demuxer->GetHeight(), CV_8UC3));
    cuMemAlloc(&pTmpImage, frameSize);
}


void GPUDecoder::GetImage(CUdeviceptr dpSrc, uint8_t *pDst, int nWidth, int nHeight)
{
    CUDA_MEMCPY2D m = { 0 };
    m.WidthInBytes = nWidth;
    m.Height = nHeight;
    m.srcMemoryType = CU_MEMORYTYPE_DEVICE;
    m.srcDevice = (CUdeviceptr)dpSrc;
    m.srcPitch = m.WidthInBytes;
    m.dstMemoryType = CU_MEMORYTYPE_HOST;
    m.dstDevice = (CUdeviceptr)(m.dstHost = pDst);
    m.dstPitch = m.WidthInBytes;
    cuMemcpy2D(&m);
}


void GPUDecoder::ToMat(uint8_t* rawData) {
    cv::Mat channelB(dec->GetHeight(), dec->GetWidth(), CV_8UC1, rawData);
    cv::Mat channelG(dec->GetHeight(), dec->GetWidth(), CV_8UC1, rawData + dec->GetHeight() * dec->GetWidth());
    cv::Mat channelR(dec->GetHeight(), dec->GetWidth(), CV_8UC1, rawData + 2 * dec->GetHeight() * dec->GetWidth());

    std::vector<cv::Mat> channels{channelB, channelG, channelR};
    cv::merge(channels, *(bgrImage.get()));
}


void GPUDecoder::FetchFrame(unsigned char** framePtr) {
    while (frameQueue.empty() && nVideoBytes) {
        uint8_t** ppFrame;
        demuxer->Demux(&pVideo, &nVideoBytes);
        int nFrameReturned = 0;
        dec->Decode(pVideo, nVideoBytes, &ppFrame, &nFrameReturned);

        for(int i = 0; i < nFrameReturned; i++) {
            frameQueue.push(ppFrame[i]);
        }
    }
    if(frameQueue.empty()) {
        nVideoBytes = 1;
        *framePtr = nullptr;
    }
    else {
        uint8_t* pFrame = frameQueue.back();
        frameQueue.pop();
        Nv12ToBgrPlanar((uint8_t*)pFrame, dec->GetWidth(), (uint8_t*)pTmpImage, dec->GetWidth(), dec->GetWidth(), dec->GetHeight());
        GetImage(pTmpImage, reinterpret_cast<uint8_t*>(pImage.get()), dec->GetWidth(), 3 * dec->GetHeight());
        ToMat(pImage.get());
		*framePtr = reinterpret_cast<unsigned char*>(bgrImage.get());
    }
}


