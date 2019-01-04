/*
 * Author: Wang Yongjie
 * Email : yongjie.wang@ntu.edu.sg
 * Description: test file of decoder
 * 
 */

#include "../include/Decoder.hpp"
#include "../include/opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv){
	char* filename = argv[1];
	Decoder<cv::Mat> test = Decoder<cv::Mat>(filename);
	test.decode();
	while(1)
	{
		
		Mat tmp = test.fetcher();
		cout<<tmp.rows<<"\t"<<tmp.cols<<endl;
		if(tmp.empty()){
			cout<<"empty image"<<endl;
		}
	}
};
