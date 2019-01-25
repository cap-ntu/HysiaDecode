#include "AudioDec.h"
#include <iostream>


int main(int argc, char **argv){
	DecodeQueue<uint8_t *> myqueue = DecodeQueue<uint8_t* >(10000);
	char* filename = argv[1];
	AudioDecoder test = AudioDecoder();
	test.IngestVideo(filename);
	test.DecodeClips(myqueue);

	for(int i = 0; i < myqueue.get_size(); i++)
	{
		uint8_t* tmp = myqueue.pop();
//		av_free(tmp);
	}
	return 0;

}

