import PyDecoder

if __name__ == "__main__":
    video_path = "test/bigbang.mp4"
    # Test GPU decoder
    dec = PyDecoder.Decoder("GPU 1")
    dec.ingestVideo(video_path)
    dec.decode()
    tick = time.time()
    while True:
        frame = dec.fetchFrame()
        if frame.size == 0:
            break
    tock = time.time()

