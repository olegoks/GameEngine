#pragma once
#ifndef _GPUALLOCATOR_HPP_
#define _GPUALLOCATOR_HPP_

#include "../engine_types.hpp"
#include "../__gpu_manager_/CUDAGpuManager.cuh"

class GpuMemoryManager final{
private:

public:

	explicit GpuMemoryManager()noexcept(true){}

	void Allocate(Model& model)const noexcept(true) {

		long long n_bytes;

		n_bytes = model.host_data_.Data<Vertex3D>().Size();
		model.device_data_.vertexs_ = AllocateMemory<Vertex3D>(n_bytes);
		
		n_bytes = model.host_data_.Data<Normal3D>().Size();
		model.device_data_.normals_ = AllocateMemory<Normal3D>(n_bytes);

		n_bytes = model.host_data_.Data<Polygon3D>().Size();
		model.device_data_.polygons_ = AllocateMemory<Polygon3D>(n_bytes);

		n_bytes = model.host_data_.Data<RgbColor>().Size();
		model.device_data_.rgb_colors_ = AllocateMemory<RgbColor>(n_bytes);
		//Allocate memory for models. Memory copy of models, but in global gpu memory space

	}


	void CopyModel(Model& model)const noexcept(true) {

		model.host_data_.Data<Vertex3D>();
		MemoryCopy<Vertex3D>(model.host_data_.Data<Vertex3D>().Data(), model.device_data_.vertexs_, model.host_data_.NPrimitiveElements<Vertex3D>() );

	}


};
#endif //_GPUALLOCATOR_HPP_

