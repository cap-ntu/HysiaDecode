GCC ?= g++
CCFLAGS := -std=c++11 -g

CUDA_PATH ?= /opt/cuda/

# common includes
INCLUDE := -I$(CUDA_PATH)/include # cuda includes
INCLUDE += -I./include/NvDecoder
INCLUDE += -I/usr/local/include # ffmpeg includes
INCLUDE += -I/usr/include # opencv includes
INCLUDE += $(shell python -m pybind11 --includes) # pybind11 includes




# common libraries



# Target rules

all:build

build:PyDec



