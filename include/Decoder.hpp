/*
 * Author : Wang Yongjie
 * Email  : yongjie.wang@ntu.edu.sg
 * Description: Video decoder
 */
 

#ifndef _DECODER_H
#define _DECODER_H

#include "CpuDec.h"
//#include "GpuDec.h"
#include "DecodeQueue.hpp"
#include "CheckDevice.h"
#include <pthread.h>
#include <thread>

using namespace std;

template<class T>
class Decoder{
public:
	Decoder(){
		//empty constructor
	};

	Decoder(char* filename){
		this->filename = filename;
	}
	~Decoder(){
		//empty destoryer
	}
	int decodeThread(){
		//if(check_device(1)){// GPUs exist
		if(0){// GPUs exist
			; // GPU decoder

		}else{ // no GPUs
			CPUDecoder decoder = CPUDecoder();
			decoder.IngestVideo(filename);
			decoder.FetchFrame(queue);
		}
		return 0;
	}

	int decode(){
		thread _thread(&Decoder::decodeThread, this);
		_thread.join();
	}

	T fetcher(){
		return this->queue.pop();
		/*
		if(this->queue.get_size() > 1){
			return this->queue.pop();
		}
		return T();
		*/
	}
	int get_size(){
		return  queue.get_size();
	}
	
private:
	char* filename; // video filename
	DecodeQueue<T> queue = DecodeQueue<T>(100000); // video decode queue

};

#endif
