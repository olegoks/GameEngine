#pragma once
#ifndef _GPUALLOCATOR_HPP_
#define _GPUALLOCATOR_HPP_

#include "../engine_types.hpp"
#include "../__gpu_manager_/GpuManager.hpp"
#include "../__gpu_manager_/CUDAGpuManager.cuh"

class GpuMemoryManager final{
private:
public:

	explicit GpuMemoryManager()noexcept(true){}

	template<class Type>
	static void Free(Type* data)noexcept(true) {
		 
		FreeMemory(static_cast<void*>(data));

	}


	template<class Type>
	static inline Type* Allocate(const long long n_bytes)noexcept(true) {

		return static_cast<Type*>(AllocateMemory(n_bytes));

	}

	template<class Type>
	static void Copy(const Type* host_data, Type* device_ptr, const long long size)noexcept(true) {

		MemoryCopy(static_cast<const void*>(host_data), static_cast<void*>(device_ptr), size);

	}

};
#endif //_GPUALLOCATOR_HPP_

