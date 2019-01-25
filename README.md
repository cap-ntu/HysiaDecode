# HysiaDecode

This repository aims to provide a highly efficient and user-friendly video preprocessing library. 

The whole pipeline of the HysiaDecode can be illustrated in the following diagram. The Decode module firstly detect the GPU exist. If GPU exists, the module will select the GPU decoding function priorly. Otherwise, the module will select the CPU decoding function. It depends on user‘s hardware configuration.So    it support various types of users. After decoding, we save the data into a queue, and the applications can fetch the frames from the queue. To facilitate Python users,  we  warp the C++ function to Python using Pybind11.  You can follow the example you provided in test directory.  Enjoy your experience and don't forget to report the issue you encountered! 



![](https://github.com/iversonicter/HysiaDecode/blob/develop/images/pipeline.png)



| pATH    | DESCRIPTION                                     |
| ------- | ----------------------------------------------- |
| include | header files(include the dependencies)          |
| lib     | required libraries(E.g., OpenCV, FFmpeg, Cuvid) |
| src     | implementation of decoding function             |
| python  | warp the C++ function to Python                 |
| test    | test files                                      |
| build   | save the generated files(.o and .so) after make |
| utils   | some pre-implemented functions in NvDecode      |

# Required Packages and Platform

Requirement | Minimal Version
---|---
Platform | Ubutun 16/14.04, Manjaro 17.10 etc.
Nvidia GPU | Titan X, M6000 etc. |
Driver | nvidia-396 or later|
CUDA   | CUDA 8.0 or later  |


# Installation

Before using our repository, you should install Nvidia-driver first. For ubuntu user, you can install it under this instruction belowing:
```
sudo apt-get purge nvidia*
sudo add-apt-repository ppa:graphics-drivers 
sudo apt-get update
sudo apt-get install nvidia-396 
lsmod | grep nvidia 
```

Pybind11 is also necessary, because we use Pybind wrap the C++ function to Python

```
pip install pybind11
```
Then: 

```
git clone https://github.com/iversonicter/HysiaDecode.git

cd HysiaDecode; make

```

After compliation and link, this reporistory will generate a shared file like this 'PyDecoder.cpython-3xm-x86_64-linux-gnu.so' in build directory. You can import it in Python like this, don't forget add it into the system path unless you will meet this error " no moudle named PyDecoder ":

```
import PyDecoder
```

# Prefermance 

we test the decoding speed on our server(Titan X, Intel(R) Xeon(R) CPU E5-2630 v3 @ 2.40GHz) using several video clips from The Big Bang Series. 

Hardware | Speed
---|---
Titan X | ~800 frames/s
Intel E5-2630 | ~300 frames/s


# Issues

Some issues we met in develop and test. If you meet the same error, you can refer the  solution we provide here.

- issue 1

```
GPU in use: Quadro M6000
Traceback (most recent call last):
  File "test_gpu_decoder.py", line 19, in <module>
    dec.ingestVideo(video_path)
RuntimeError: NvDecoder : cuvidCtxLockCreate(&m_ctxLock, cuContext) returned error -1282253296 at src/NvDecoder/NvDecoder.cpp:524
```
 Solution : This error comes from cuvid link conflicts. You should link 

- Issue 2

```
the decoding processing is blocked and does not start the decoded process
```

Solution: When it happens, maybe your GPU does support NVDecode. You should try to consult the Nvidia documents to check whether you graphical card are suitable. In this repository, we detached the decoded function from Video_Codec_SDK_8.2.16. 