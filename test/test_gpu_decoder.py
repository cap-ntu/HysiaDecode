import PyHysiaDec
import cv2
import time                                                                                                                                                                   
 
 
if __name__ == "__main__":
    video_path = "bigbang.mp4"
    # Test cv2 video capture
    cap = cv2.VideoCapture(video_path)
    tick = time.time()
    while True:
        ret, frame = cap.read()
        if not ret:
            break
    tock = time.time()
    print("time taken for cv2 video reader is %.3f" % (tock - tick))

    # Test GPU decoder
    dec = PyHysiaDec.GPUDecoder(1)
    dec.IngestVideo(video_path)
    tick = time.time()
    while True:
        frame = dec.FetchFrame()
        if frame.size == 0:
            break
    tock = time.time()
    print("time taken for gpu video reader is %.3f" % (tock - tick))

