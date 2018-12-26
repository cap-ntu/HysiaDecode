
class GPUDecoder: public HysiaDecoder
{
private:
    CUcontext cuContext = NULL;
    std::ofstream fpOut;
    NvDecoder* dec = nullptr;
    FFmpegDemuxer* demuxer = nullptr;
    uint8_t* pVideo = NULL;
    int nVideoBytes = 1;
    std::queue<uint8_t*> frameQueue;
    int frameSize = 0;
    std::unique_ptr<uint8_t[]> pImage = nullptr;
    CUdeviceptr pTmpImage = 0;
public:
    GPUDecoder(int device_id);
    ~GPUDecoder();
    virtual void IngestVideo(const char*) override;
    virtual cv::Mat FetchFrame() override;
private:
    void GetImage(CUdeviceptr dpSrc, uint8_t *pDst, int nWidth, int nHeight);
    cv::Mat ToMat(uint8_t*);
};

