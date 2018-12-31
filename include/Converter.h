//
// Created by WENBO JIANG on 14/12/18.
//

#ifndef HYSIACODEC_CONVERTER_H
#define HYSIACODEC_CONVERTER_H
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#endif //HYSIACODEC_CONVERTER_H

cv::Mat ConvertToRGB(unsigned char *yuvBuf, int width, int height);
void ConvertToPlanar(uint8_t *pHostFrame, int nWidth, int nHeight, int nBitDepth);
