/*
 * Filename : CpuDec
 * Author	: Wang Yongjie
 * Description: video decode with CPU
 */ 

#include "CpuDec.h"
#include <iostream>
#include <cstdint>

using namespace std;

long U[256], V[256], Y1[256], Y2[256];
uint64_t RGB_SIZE = 0;

void MakeConversionTable()
{
        long i;
        for (i=0; i<256; i++)
        {
            V[i] = 15938*i-2221300;
            U[i] = 20238*i-2771300;
            Y1[i] = 11644*i;
            Y2[i] = 19837*i-311710;
        }
}

void YUV420ToRGB (uint64_t width, uint64_t height, unsigned char *pYUVBuf, unsigned char *pRGBBuf)
{
        unsigned char *pY, *pU, *pV, *pUbase, *pVbase;
        uint64_t i, j;
        unsigned char *pR, *pG, *pB;
        int tmp_width = (width / 4) * 4;

        RGB_SIZE=tmp_width*height*3;
        pUbase = pYUVBuf + width*height;
        pVbase = pUbase + width*height/4;
        for (i=0; i<height; i++)
        {
                pB = pRGBBuf+RGB_SIZE-3*tmp_width*(i+1);
                pG = pRGBBuf+RGB_SIZE-3*tmp_width*(i+1)+1;
                pR = pRGBBuf+RGB_SIZE-3*tmp_width*(i+1)+2;
                pY = pYUVBuf + i*width;
                pU = pUbase + (i/2)*width/2;
                pV = pVbase + (i/2)*width/2;
                for (j=0; j<tmp_width; j+=2)
                {
                        *pR = std::max ((long)(0), (long)std::min ((long)255, (V[*pV] + Y1[*pY])/10000) );   //R value
                        *pB = std::max ((long)0, (long)std::min ((long)255, (U[*pU] + Y1[*pY])/10000) );   //B value
                        *pG = std::max ((long)0, (long)std::min ((long)255, (Y2[*pY] - 5094*(*pR) - 1942*(*pB))/10000) ); //G value

                        pR += 3;
                        pB += 3;
                        pG += 3;
                        pY++;

                        *pR = std::max ((long)0, (long)std::min ((long)255, (V[*pV] + Y1[*pY])/10000) );   //R value
                        *pB = std::max ((long)0, (long)std::min ((long)255, (U[*pU] + Y1[*pY])/10000) );   //B value
                        *pG = std::max ((long)0, (long)std::min ((long)255, (Y2[*pY] - 5094*(*pR) - 1942*(*pB))/10000) ); //G value

                        pR += 3;
                        pB += 3;
                        pG += 3;
                        pY++;

                        pU++;
                        pV++;
                }
        }
}

unsigned char* convertYUVToRGB(unsigned char* yuv, int width, int height)
{
    unsigned char* bgr = new unsigned char[width * height * 3];

    memset(bgr, 0, width * height * 3 * sizeof(unsigned char));

    YUV420ToRGB(width, height, yuv, bgr);

    unsigned char temp = 0;
    for (int i = 0; i < height / 2; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            temp = bgr[i * width *3 + j];
            bgr[i * width * 3+ j] = bgr[(height - i - 1) * width * 3+ j];
            bgr[(height - i - 1) * width * 3 + j] = temp;
        }
    }

    return bgr;
}

CPUDecoder::CPUDecoder(){
}

CPUDecoder::~CPUDecoder(){
}


int CPUDecoder::IngestVideo(const char* filename){

	av_register_all(); //initialize decoding environments
	MakeConversionTable();
	if (avformat_open_input(&pFmt, filename, piFmt, NULL) < 0)
	{
		cerr<<"avformat open failed";
		return -1;
	}
	else
	{
		cout<<"Open stream success"<<endl;
	}

	if (avformat_find_stream_info(pFmt,NULL) < 0)
	{
		cerr<<"could not find stream";
		return -1;
	}
	av_dump_format(pFmt, 0, "", 0);

	for (int i = 0; i < pFmt->nb_streams; i++)
	{
		if ( (pFmt->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) &&
				(videoindex < 0) )
		{
			videoindex = i;
		}
		if ( (pFmt->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) &&
				(audioindex < 0) )
		{
			audioindex = i;
		}
	}

	if (videoindex < 0 || audioindex < 0)
	{
		cerr<<"video index = "<<videoindex<<"audioindex = "<<audioindex;
		return -1;
	}

	pVst = pFmt->streams[videoindex];
	pAst = pFmt->streams[audioindex];

	pVideoCodecCtx = pVst->codec;
	pAudioCodecCtx = pAst->codec;

	pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
	if (!pVideoCodec)
	{
		cerr<<"could not find video decoder";
		return -1;
	}
	if (avcodec_open2(pVideoCodecCtx, pVideoCodec,NULL) < 0)
	{
		cerr<<"could not open video codec";
		return -1;
	}

	pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
	if (!pAudioCodec)
	{
		cerr<<"could not find audio decoder";
		return -1;
	}
	if (avcodec_open2(pAudioCodecCtx, pAudioCodec, NULL) < 0)
	{
		cerr<<"could not find audio codec";
		return -1;
	}
	return 0;

}

int CPUDecoder::FetchFrame(DecodeQueue<cv::Mat> &queue){

	int got_picture;
	AVFrame *pframe = av_frame_alloc();
	int num = 0;
	int yuvheight = pVideoCodecCtx->height;
	int yuvwidth = pVideoCodecCtx->width;
	int yuvlen = (int)(yuvheight * yuvwidth * 3/2);
	AVPacket pkt;
	av_init_packet(&pkt);
	unsigned char * yuvdata = (unsigned char *)calloc(sizeof(unsigned char),sizeof(float)*yuvlen);
	unsigned char * rgbData;
	while(1)
	{
		if (av_read_frame(pFmt, &pkt) >= 0)
		{
			if (pkt.stream_index == videoindex)
			{
				avcodec_decode_video2(pVideoCodecCtx, pframe, &got_picture, &pkt);
				if (got_picture)
				{
					unsigned char* ptr = 0;
					int linesize = 0;
					int offset = 0;
					int width,height;
					width = yuvwidth;
					height = yuvheight;
					for(int mi=0;mi<3;mi++)
					{
						ptr = pframe->data[mi];
						linesize = pframe->linesize[mi];
						if (mi == 1)
						{
							width >>= 1;
							height >>= 1;
						}
						for(int j=0;j<height;j++)
						{
							memcpy(yuvdata+offset,ptr,width*sizeof(uint8_t));
							ptr += linesize;
							offset += width;
						}
					}

					rgbData=convertYUVToRGB(yuvdata,yuvwidth,yuvheight);
					Mat img(yuvheight, yuvwidth, CV_8UC3, rgbData);
					queue.push(img);
					cout<<"decode thread\t"<<queue.size()<<"\t"<<queue._head<<"\t"<<queue._end<<endl;
				}
			}
		}else{
			break;
		}
		av_free_packet(&pkt);
	}
	av_free(pframe);
	free(yuvdata);
	return 0;
}

int CPUDecoder::GetWidth(){
	return pVideoCodecCtx->width;
}


int CPUDecoder::GetHeight(){
	return pVideoCodecCtx->height;
}


