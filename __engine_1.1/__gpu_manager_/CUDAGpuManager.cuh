#
#include <memory>
#include "cuda_runtime.h"
#include "GpuManager.hpp"

void* AllocateMemory(const long long n_bytes) noexcept(true);

void MemoryCopy(const void* host_data, void* device_ptr, const long long size)noexcept(true);

void FreeMemory(void* device_data)noexcept(true);
