//
// Created by WENBO JIANG on 14/12/18.
//
#include "Converter.h"
#include "../Utils/NvCodecUtils.h"

cv::Mat ConvertToRGB(unsigned char *yuvBuf, int width, int height)
{
    cv::Size actual_size(width, height);
    cv::Size half_size(width / 2, height / 2);

    unsigned char *y_ptr = yuvBuf;
    unsigned char *u_ptr = y_ptr + width * height;
    unsigned char *v_ptr = u_ptr + 1 / 4 * width * height;

    cv::Mat y(actual_size, CV_8UC1, y_ptr);
    cv::Mat u(half_size, CV_8UC1, u_ptr);
    cv::Mat v(half_size, CV_8UC1, v_ptr);

    cv::Mat u_resized, v_resized;
    cv::resize(u, u_resized, actual_size, 0, 0, cv::INTER_NEAREST);
    cv::resize(v, v_resized, actual_size, 0, 0, cv::INTER_NEAREST);

    cv::Mat yuv;
    std::vector<cv::Mat> yuv_channels = {y, u_resized, v_resized};
    cv::merge(yuv_channels, yuv);

    cv::Mat rgb;
    cv::cvtColor(yuv, rgb, cv::COLOR_YUV2RGB);
    return rgb;
}

void ConvertToPlanar(uint8_t *pHostFrame, int nWidth, int nHeight, int nBitDepth)
{
    if (nBitDepth == 8)
    {
        // nv12->iyuv
        YuvConverter<uint8_t> converter8(nWidth, nHeight);
        converter8.UVInterleavedToPlanar(pHostFrame);
    }
    else
    {
        // p016->yuv420p16
        YuvConverter<uint16_t> converter16(nWidth, nHeight);
        converter16.UVInterleavedToPlanar((uint16_t *)pHostFrame);
    }
}
