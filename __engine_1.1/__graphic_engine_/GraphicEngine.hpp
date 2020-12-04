#pragma once
#ifndef _GRAPHICENGINE_H_
#define _GRAPHICENGINE_H_

#include <chrono>
#include <list>
#include <cstdlib>
#include <thread>
#include <windows.h>
#include "Camera/Camera.hpp"
#include "CudaGraphicEngine.cuh"
#include "../engine_types.hpp"

//#include "../__graphic_engine_/GraphicEngineCUDA.hpp"

class GraphicEngine final{
private:

	static const size_t kThreadsPerTriangle;
	static const size_t kDefaultFrameWidth;
	static const size_t kDefaultFrameHeight;

	const Camera* camera_;
	FrameDevice device_frame_;
	FrameHost host_frame_;
	DeviceData* device_data_;

	uint8_t RandomUint8_t()const noexcept(true) {

		return static_cast<uint8_t>(rand()) % 255;

	}

	RgbColor&& RandomColor() const noexcept(true) {

		return RgbColor{ RandomUint8_t(), RandomUint8_t(), RandomUint8_t(), RandomUint8_t() };
	}

	void FillDeviceFrame(const FramePixel& pixel)noexcept(true);
	
public:

	explicit GraphicEngine(const size_t frame_width = kDefaultFrameWidth, const size_t frame_height = kDefaultFrameHeight)noexcept(true):
		camera_{ nullptr },
		device_frame_{ frame_width, frame_height },
		host_frame_{ frame_width, frame_height, RgbColor{20, 0, 250} },
		device_data_{}{}

	FrameHost& RenderMeshFrame()noexcept(true);

	inline void PlugCamera(const Camera* camera)noexcept(true) { camera_ = camera; };
	inline void PlugDeviceData(DeviceData* device_data)noexcept(true) { device_data_ = device_data; }

};

#endif