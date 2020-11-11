#pragma once
#ifndef ENGINE_TYPES_HPP_
#define ENGINE_TYPES_HPP_

struct Ratio {

	unsigned int vertexNumber;
	unsigned int normalNumber;

};

struct RgbPixel {
	unsigned char	rgb_blue;
	unsigned char   rgb_green;
	unsigned char   rgb_red;
	unsigned char   rgb_reserved;
};

struct Polygon3D {

	Ratio ratios[3];
	RgbPixel color;

};

struct Vertex3D {

	Vertex3D() : x(0.0f), y(0.0f), z(0.0f) {};
	Vertex3D(float x_, float y_, float z_) noexcept : x(x_), y(y_), z(z_) {};
	float x;
	float y;
	float z;

};

struct Normal3D {

	inline Normal3D() noexcept : x(0.0f), y(0.0f), z(0.0f) {};
	inline Normal3D(float x_, float y_, float z_) noexcept : x(x_), y(y_), z(z_) {};
	float x;
	float y;
	float z;

};

#include <memory>

//In identifier names 
//n/N == number/Number
//el/El == element/Element / elements/Elements

class Array abstract {
private:

	size_t size_;
	size_t capacity_;

protected:

	virtual inline size_t Capacity()const noexcept(true) final { return capacity_; };
	virtual inline void  SetSize(const size_t new_size)noexcept(true) final { this->size_ = new_size; };
	virtual inline void SetCapacity(const size_t new_capacity)noexcept(true) final { this->capacity_ = new_capacity; };

	explicit Array(const size_t size)noexcept(true) :
		size_{ size },
		capacity_{ size } {}

public:

	virtual void Concat(const void* const ptr, const size_t size)noexcept(true) = 0;
	virtual inline size_t Size()const noexcept(true) final { return size_; };
	virtual ~Array() {};

};

template<class DataType>
class ArrayOfPrimitives : public ArrayOf {
private:

	DataType* ptr_;

	using BaseClass = Array;

	void Reserve(const size_t new_capacity) noexcept(true) {

		DataType* const new_ptr = (DataType* const) new DataType[new_capacity];
		
		const size_t last_index = ( (new_capacity >= BaseClass::Size()) ? (BaseClass::Size()) : (new_capacity) )- 1;

		for (size_t index = 0; index <= last_index; index++)
			*(new_ptr + index) = *(ptr_ + index);

		BaseClass::SetCapacity(new_capacity);
		
		if (ptr_ != nullptr) delete[] ptr_;
		ptr_ = new_ptr;

	};

protected:

public:

	DataType* const GetPtr()const noexcept(true)  { return ptr_; };

	explicit ArrayOfPrimitives(DataType* const ptr, const size_t size)noexcept(true):
		ArrayOf( ( ptr != nullptr ) ? ( size )  :( 0 ) ){

		if (ptr != nullptr) ptr_ = ptr;
		else
			ptr_ = ptr;
		
	}
	
	inline DataType& operator[](const size_t index)const noexcept(true) { return *(ptr_ + index); };

	void Concat(const void* const ptr, const size_t size)noexcept(true) override{

		const DataType* const concat_ptr = reinterpret_cast<const DataType* const>(ptr);
		const size_t new_size = BaseClass::Size() + size;
		Reserve(new_size);
		const size_t first_index = BaseClass::Size();

		for (size_t i = first_index, j = 0; j < size; j++, i++)
			operator[](i) = concat_ptr[j];

		BaseClass::SetSize(new_size);

	}

	~ArrayOfPrimitives()noexcept(true){
	
		if (ptr_ != nullptr)
			delete[] ptr_;

	}

};

#endif //ENGINE_TYPES_HPP_