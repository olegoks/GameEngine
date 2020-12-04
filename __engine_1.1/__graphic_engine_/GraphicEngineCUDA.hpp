#pragma once
#ifndef _GRAPHICENGINECUDA_H_
#define _GRAPHICENGINECUDA_H_
#include "cuda_runtime_api.h"
#include "GraphicEngine.hpp"
#include "../engine_types.hpp"
#include "Camera/Camera.hpp"

FrameHost MeshFrame(DeviceData* device_data, const Camera* camera)noexcept(true);

#endif