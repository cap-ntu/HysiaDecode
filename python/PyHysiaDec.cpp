#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include "../HysiaDec/HysiaDec.h"
#include <iostream>

namespace py = pybind11;


// Wrap the class method 
py::array_t<uint8_t> DecodeFrames(GPUDecoder* dec) {
	unsigned char* data = nullptr;
	dec->DecodeFrames(&data);
	if(data) {
		// Avoid python from gabbage collecting the numpy array
		py::capsule doNothing(data, [](void* d){
			// Do not delete the data pointer
			// It is managed by the GPUDecoder object
		});
		// Construct Numpy array from data pointer without copying
		// With the help of capsule
		return py::array_t<uint8_t>(
			{dec->GetHeight(), dec->GetWidth(), 3},
			{dec->GetWidth() * 3, 3, 1},
			data,
			doNothing
		);
	}
	else {
		return py::array_t<uint8_t>();
	}
}



PYBIND11_MODULE(PyHysiaDec, m) {
	py::class_<GPUDecoder>(m, "GPUDecoder")
		.def(py::init<int>())
		.def("IngestVideo", &GPUDecoder::IngestVideo)
		.def("DecodeFrames", &DecodeFrames);
}


