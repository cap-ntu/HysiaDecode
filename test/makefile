CPUDecoder:CPUDecoder.o
	g++ -g -o CPUDecoder CPUDecoder.o -L /usr/local/lib -lavformat -lavcodec, -lavutil

CPUDecoder.o:test_cpudecode.cpp
	g++ -g -c test_cpudecode.cpp -o CPUDecoder.o -I ../include/ffmpeg -I ../include/opencv

clean:
	rm CPUDecoder.o CPUDecoder
