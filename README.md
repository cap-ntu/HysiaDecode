# HysiaDecode

This repository aims to provide a highly effienent and user-friendly video preprocessing library. 

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
- Solution 1:

This error comes from cuvid link conflicts. You should link 

- Issue 2

the decoding processing is blocking after a long time. When it happens, maybe your GPU does support NVDecode. You should try to consult the Nvidia documents to check whether you graphical card are suitable.
