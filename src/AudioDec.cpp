/*
 * Author:		Wang Yongjie
 * Email:		yongjie.wang@ntu.edu.sg
 * Description:	audio decode with CPU
 */

#include "AudioDec.h"
#include <iostream>

AudioDecoder::AudioDecoder(){//constuctor function
	//empty
}

AudioDecoder::~AudioDecoder(){
	//empty
}


AudioDecoder::IngestVideo(const char* filename){
	av_register_all();
	if (avformat_open_input(&pFmt, filename, piFmt, NULL) < 0)
	{
		std::cerr<<"avformat open failed.";
		return -1;
	}
	else
	{
		std::cout<<"open stream successfully!"<<std::endl;
	}

	if (avformat_find_stream_info(pFmt,NULL) < 0)
	{
		std::cerr<<"could not find stream.";
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
		std::cerr<<"video index = "<<videoindex <<", audio index = "<<audioindex;
		return -1;
	}

	AVStream *pVst,*pAst;
	pVst = pFmt->streams[videoindex];
	pAst = pFmt->streams[audioindex];

	pAudioCodecCtx = pAst->codec;

	pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
	if (!pAudioCodec)
	{
		std::cerr<<"could not find audio codec";
		return -1;
	}
	if (avcodec_open2(pAudioCodecCtx, pAudioCodec,NULL) < 0)
	{
		std::cerr<<"could not open audio codec";
		return -1;
	}
	return 0;

}

AudioDecoder::DecodeClips(DecodeQueue<uint8_t* > &queue){

	AVFrame *pframe = av_frame_alloc();
	AVPacket pkt;
	av_init_packet(&pkt);
	SwrContext *swrCtr = swr_alloc();
	enum AVSampleFormat in_sample_fmt = pAudioCodecCtx->sample_fmt;
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int in_sample_rate = pAudioCodecCtx->sample_rate;
	int out_sample_rate = 44100;
	uint64_t in_ch_layout = pAudioCodecCtx->channel_layout;
	uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO; int frame_num = 0;
	swr_alloc_set_opts(swrCtr,out_ch_layout,out_sample_fmt,out_sample_rate,in_ch_layout,in_sample_fmt,in_sample_rate,0,NULL);
	swr_init(swrCtr);

	int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
	uint8_t *out_buffer = (uint8_t *)av_malloc(48000 * 4);
	int got_frame = 0, idex = 0, ret;
	while(av_read_frame(pFmt, &pkt) >= 0){
		if(pkt.stream_index == audioindex)
		{
			ret = avcodec_decode_audio4(pAudioCodecCtx, pframe, &got_frame, &pkt);
			if(got_frame > 0){
				std::cout<<"audio decoding"<<std::endl;
			}
			swr_convert(swrCtr, &out_buffer, 48000*4, pframe->data, pframe->nb_samples);
			int out_buffer_size = av_samples_get_buffer_size(NULL, out_channel_nb, pframe->nb_samples, out_sample_fmt, 1);
			std::cout<<out_buffer_size<<std::endl;

		}else{
			std::cout<<"no audio stream"<<std::endl;
		}
		queue.push(out_buffer);
		av_frame_unref(pframe);
		av_free_packet(&pkt);
	}


	av_free(pframe);
	//av_free(out_buffer);  free the memory after dequeue
	avcodec_close(pAudioCodecCtx);
	swr_free(&swrCtr);
	avformat_close_input(&pFmt);
	avformat_free_context(pFmt);

	return 0;

}

