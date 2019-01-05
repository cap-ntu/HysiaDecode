#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include "opencv2/opencv.hpp"
#include "../include/Decoder.hpp"
#include <iostream>

namespace py = pybind11;


// Wrap the class method 
// Convert opencv mat to numpy array
py::array_t<uint8_t> fetchFrame(Decoder* dec) {
	cv::Mat frame = dec.fetchFrame();
	if(frame) {
		// Transfer resource release to Python
		py::capsule cleanUp(data, [](void* d){
			d = reinterpret_cast<cv::Mat*>(d);
			delete d;
		});
		// Construct Numpy array from data pointer without copying
		// With the help of capsule
		return py::array_t<uint8_t>(
			{dec->getHeight(), dec->getWidth(), 3},
			{dec->getWidth() * 3, 3, 1},
			frame->data,
			cleanUp
		);
	}
	else {
		return py::array_t<uint8_t>();
	}
}



PYBIND11_MODULE(PyHysiaDec, m) {
	py::class_<GPUDecoder>(m, "Decoder")
		.def(py::init<char*>())
		.def("decode", &Decoder::decode)
		.def("getWidth", &Decoder::getWidth)
		.def("getHeight", &Decoder::getHeight)
		.def("fetchFrame", &fetchFrame);
}


