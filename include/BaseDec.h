//
// Created by WENBO JIANG on 14/12/18.
//

#ifndef _BaseDec_H
#define _BaseDec_H

class BaseDecoder
{
public:
    BaseDecoder() = default;
    // Ingest a video file
    virtual void IngestVideo(const char*) = 0;
    // Fetch frame as mat object
    virtual cv::Mat FetchFrame() = 0;
};


#endif //end _BaseDec_H
