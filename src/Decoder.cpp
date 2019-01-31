/*
 * Author: Wang Yongjie
 * Email : yongjie.wang@ntu.edu.sg
 * Description: Video decoder
 */ 

#include "Decoder.h"
#include <iostream>

using namespace std;

template<class T>
Decoder<T>::Decoder(){
	// empty constructor
}

template<class T>
Decoder<T>::Decoder(const char* filename){
	this->filename = filename;
}

template<class T>
Decoder<T>::~Decoder(){
	// empty destoryor
}

template<class T>
int Decoder<T>::decode()
{
	//if(check_device(1)){// GPUs exist
	if(0){// GPUs exist
		; // GPU decoder

	}else{ // no GPUs
		CPUDecoder decoder = CPUDecoder();
		decoder.IngestVideo(filename);
		//decoder.FetchFrame(queue);

		int ret = pthread_create(&thread, NULL, decoder.FetchFrame, &queue);
		if(ret != 0){
			cout<<"create thread error"<<endl;
			return -1;
		}

	}
	return 0;
}


template<class T>
T Decoder<T>::fetcher()
{
	if(queue.size() > 1){
		return queue.pop();
	}
	return NULL;
}


template<class T>
int Decoder<T>::get_size()
{
	return queue.size();
}


