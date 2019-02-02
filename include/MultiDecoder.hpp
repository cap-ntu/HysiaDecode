/*
 * Author : Wang Yongjie
 * Email  : yongjie.wang@ntu.edu.sg
 * Description: Video decoder
 */

#ifndef _MULTIDECODER_HPP
#define _MULTIDECODER_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "BaseDec.h"
#include "CpuDec.h"
#include "GpuDec.h"
#include "AudioDec.h"
#include "DecodeQueue.hpp"
#include "CheckDevice.h"
#include <sys/stat.h>
#include <cstring>
#include <exception>

class MultiDecoder{
public:
	//default constructor using GPU 
	MultiDecoder(char *filename){
		if(check_device(1) == 0){
			std::cout<<"Using GPU"<<std::endl;
			vdec.reset(new GPUDecoder(0));
		}else{
			vdec.reset(new CPUDecoder());
		}
		adec.reset(new AudioDecoder());
		this->filename = filename;
	}

	MultiDecoder(char *filename, char *device, int device_id = 0){
		if(strcmp(device, "CPU") == 0){
			vdec.reset(new CPUDecoder());
		}else if(strcmp(device, "GPU") == 0){
			try{
				vdec.reset(new GPUDecoder(device_id));
			}catch(std::exception &e){
				std::cout<<"exception in create GPU decoder:\t"<<e.what()<<std::endl;
			}
		}
		this->filename = filename;
		adec.reset(new AudioDecoder());
	}

	~MultiDecoder(){ // release allocated resources
		vdec.release();
		adec.release();
	}

	int GetFrames(){ // video decode
		vdec->IngestVideo(this->filename);
		vdec->DecodeFrames(this->queue);
	}

	int GetAudios(){ // audio decode
		adec->IngestVideo(this->filename);
		adec->DecodeClips(&this->audio_buffer, &this->size);
	}

	int SaveFile(const char *path){
		// path the path saved the decoded videos
		//save the decoded file into disk
		if(path == NULL) return -1;
		char directory[128] = {0};
		sprintf(directory, "%s/%s", path, this->filename);

		if(! mkdir(directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){
			std::cout<<"create "<<directory<<" failed"<<std::endl;
		}
		char audio_dir[128], video_dir[128];
		sprintf(audio_dir, "%s/%s", directory, "pcm");
		sprintf(video_dir, "%s/%s", directory, "frame");

		if(! mkdir(audio_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){// create video directory
			std::cout<<"create "<<audio_dir<<" failed"<<std::endl;
		}

		if(! mkdir(video_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){ // create audio directory
			std::cout<<"create "<<video_dir<<" failed"<<std::endl;
		}

		char audio_filename[128] = {0};
		char video_filename[128] = {0};
		
		sprintf(audio_filename, "%s/%s.pcm", audio_dir, this->filename);

		// save audio pcm file
		FILE *fp_pcm = fopen(audio_filename, "wb");
		fwrite(this->audio_buffer, 1, this->size, fp_pcm);
		fclose(fp_pcm);

		// save video frames
		int cnt = 0;

		while(queue.get_size() > 0){
			sprintf(video_filename, "%s/%s-%08d.jpg", video_dir, this->filename, cnt);
			printf("%s\n", video_filename);
			cnt = cnt + 1;
			cv::Mat *tmp = queue.pop();
			if(!tmp){continue;}
			cv::imwrite(video_filename, *tmp);
		}

	}

	int SaveMemory(const char *ip = NULL, const int port = 0){
		// save data into redis
		// left empty temperoraily
	}

private:
	DecodeQueue<cv::Mat* > queue = DecodeQueue<cv::Mat* >(1000000); // video decode queue
	std::unique_ptr<BaseDecoder> vdec = nullptr;  // video codec
	std::unique_ptr<AudioDecoder> adec = nullptr; // audio codec
	uint8_t *audio_buffer = nullptr;  // audio decode buffer
	int size;  // audio buffer length
	char *filename = nullptr; // video files to be decoded
};
#endif
