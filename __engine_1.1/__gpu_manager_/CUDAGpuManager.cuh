
#include <memory>
#include "cuda_runtime.h"

template<class PrimitiveType>
PrimitiveType* AllocateMemory(const long long n_bytes)noexcept(true) {

	PrimitiveType* ptr = nullptr;
	cudaMalloc((void**) &ptr, n_bytes);
	return ptr;
}

template<class PrimtiveType>
void MemoryCopy(const PrimtiveType* host_data, PrimtiveType* device_ptr, const long long size)noexcept(true) {

	cudaMemcpy((void*)device_ptr, (const void*)host_data, size, cudaMemcpyHostToDevice);

}