#include <../include/CpuDec.h>

int main(int argc, char **argv){
	filename = argv[1];
	test = CPUDecoder();
	test.IngestVideo(filename);
	return 0;
}


