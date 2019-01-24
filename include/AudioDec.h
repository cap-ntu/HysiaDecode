/*
 * Author:		Wang Yongjie
 * Email:		yongjie.wang@ntu.edu.sg
 * Description:	audio decode with CPU
 */

#ifndef _AUDIODEC_H 
#define _AUDIODEC_H
#include "DecodeQueue.hpp"
#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

#ifdef __cplusplus
}
#endif



class AudioDecocer
{
private:
	AVCodec *pVideoCodec = NULL;
	AVCodec *pAudioCodec = NULL;
	AVCodecContext *pAudioCodecCtx = NULL;
	AVIOContext *pb = NULL;
	AVInputFormat *piFmt = NULL;
	AVFormatContext *pFmt = NULL;
	AVStream *pVst,*pAst;
	int videoindex = -1;
	int audioindex = -1;

public:
	AudioDecocer();
	~AudioDecocer();
	int IngestVideo(const char*);
	int DecodeFrames(DecodeQueue<cv::Mat*> &queue) override;
};

#endif
