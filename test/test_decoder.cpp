/*
 * Author: Wang Yongjie
 * Email : yongjie.wang@ntu.edu.sg
 * Description: test file of decoder
 * 
 */

#include "../include/Decoder.hpp"
#include "../include/opencv/opencv2/opencv.hpp"
#include "../include/opencv/opencv2/core.hpp"


int main(int argc, char **argv){
	char* filename = argv[1];
	Decoder<cv::Mat*> test(filename);
	test.decode();
	while(1)
	{
		
		cv::Mat* tmp = test.fetchFrame();
		if(!tmp){
			std::cout<<"empty image"<<std::endl;
			break;
		}
		std::cout<<tmp->rows<<"\t"<<tmp->cols<<std::endl;
	}
};
