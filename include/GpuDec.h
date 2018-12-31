//
// Created by WENBO JIANG on 14/12/18.
//

#ifndef _GPU_DECODE_ 
#define _GPU_DECODE_

#include <iostream>
#include <algorithm>
#include <queue>
#include <cuda.h>
#include "BaseDec.h"
#include "NvDecoder/NvDecoder.h"
#include "utils/NvCodecUtils.h"
#include "utils/FFmpegDemuxer.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"


class GPUDecoder: public BaseDecoder
{
private:
    CUcontext cuContext = NULL;
    std::ofstream fpOut;
    std::unique_ptr<NvDecoder> dec = nullptr;
    std::unique_ptr<FFmpegDemuxer> demuxer = nullptr;
    uint8_t* pVideo = NULL;
    int nVideoBytes = 1;
    std::queue<uint8_t*> frameQueue;
    int frameSize = 0;
    std::unique_ptr<uint8_t[]> pImage = nullptr;
    std::unique_ptr<cv::Mat> bgrImage = nullptr;
    CUdeviceptr pTmpImage = 0;
public:
    GPUDecoder() {}
    GPUDecoder(int device_id);
    ~GPUDecoder();
	GPUDecoder& operator=(GPUDecoder&&);
	GPUDecoder& operator=(GPUDecoder&);
    virtual int IngestVideo(const char*) override;
    virtual int FetchFrame(unsigned char**) override;
    virtual int GetHeight();
    virtual int GetWidth();
private:
    void GetImage(CUdeviceptr dpSrc, uint8_t *pDst, int nWidth, int nHeight);
    void ToMat(uint8_t*);
}

#endif //end _GPU_DECODE_
