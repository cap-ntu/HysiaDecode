#include "CpuDec.h"

int main(int argc, char **argv){
	char* filename = argv[1];
	CPUDecoder test = CPUDecoder();
	test.IngestVideo(filename);
	return 0;
}


