#pragma once
#ifndef _GPUALLOCATOR_HPP_
#define _GPUALLOCATOR_HPP_

#include "../engine_types.hpp"


class GpuMemoryManager final{
private:



public:

	explicit GpuMemoryManager()noexcept(true){}

	void Allocate(Model& model)const noexcept(true) {

		
		//Allocate memory for models. Memory copy of models, but in global gpu memory space

	}


};
#endif //_GPUALLOCATOR_HPP_

