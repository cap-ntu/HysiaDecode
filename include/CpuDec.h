/*
 * Author:		Wang Yongjie
 * Email:		yongjie.wang@ntu.edu.sg
 * Description:	video decoded with CPU
 */

#ifndef _CPUDEC_H
#define _CPUDEC_H

#include "BaseDec.h"
#include <opencv2/opencv.hpp>
#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"

#ifdef __cplusplus
}
#endif

using namespace cv;

long U[256], V[256], Y1[256], Y2[256];
uint64_t RGB_SIZE = 0;

class CPUDecoder: public BaseDecoder
{
	private:
		AVCodec *pVideoCodec = NULL;
		AVCodec *pAudioCodec = NULL;
		AVCodecContext *pVideoCodecCtx = NULL;
		AVCodecContext *pAudioCodecCtx = NULL;
		AVIOContext *pb = NULL;
		AVInputFormat *piFmt = NULL;
		AVFormatContext *pFmt = NULL;
		AVStream *pVst,*pAst;
		AVFrame *pframe = av_frame_alloc();
		AVPacket pkt;
		int videoindex = -1;
		int audioindex = -1;

	public:
		CPUDecoder();
		~CPUDecoder();
		int IngestVideo(const char*) override;
		cv::Mat FetchFrame() override;

};

void MakeConversionTable();
void YUV420ToRGB (uint64_t width, uint64_t height, unsigned char *pYUVBuf, unsigned char *pRGBBuf);
unsigned char* convertYUVToRGB(unsigned char* yuv, int width, int height);
#endif
