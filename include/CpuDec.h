/*
 * Author:		Wang Yongjie
 * Email:		yongjie.wang@ntu.edu.sg
 * Description:	video decoded with CPU
 */

#ifdef __CPUDEC_H
#define __CPUDEC_H

#include "BaseDec.h"
#include "DecodeQueue.hpp"
#include "opencv/opencv.hpp"

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

using namespace cv2;

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
		decode_queue = DecodeQueue(100); // queue for video decoding

	public:
		CPUDecoder();
		~CPUDecoder();
		void IngestVideo(const char*) override;
		cv::Mat FetchFrame() override;

};

#endif
