//
// Created by WENBO JIANG on 14/12/18.
//

#ifndef _BaseDec_H
#define _BaseDec_H

#include "opencv2/opencv.hpp"


class BaseDecoder
{
public:
    BaseDecoder() = default;
    // Ingest a video file
    virtual int IngestVideo(const char*) = 0;
    // Fetch frame as mat object
    virtual int FetchFrame() = 0;
};


#endif //end _BaseDec_H
