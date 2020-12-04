#include "../../__engine_1.1/__gpu_manager_/CUDAGpuManager.cuh"


void* AllocateMemory(const long long n_bytes) noexcept(true)
{

	void* ptr = nullptr;
	cudaMalloc((void**)&ptr, n_bytes);
	return ptr;

}

void MemoryCopy(const void* host_data, void* device_ptr, const long long size)noexcept(true) {

	cudaMemcpy((void*)device_ptr, (const void*)host_data, size, cudaMemcpyHostToDevice);

}

void FreeMemory(void* device_data)noexcept(true) {

	cudaFree(static_cast<void*>(device_data));

}