#pragma once
#ifndef ENGINE_TYPES_HPP_
#define ENGINE_TYPES_HPP_

#include <cstdint>

struct Ratio {

	unsigned int vertex_n;
	unsigned int normal_n;

};

struct RgbColor {

	union {

		struct {
			uint8_t	rgb_blue;
			uint8_t	rgb_green;
			uint8_t	rgb_red;
			uint8_t	rgb_reserved;
		};

		unsigned char rgba[4];

	};

};

struct RgbPixel: public RgbColor {


};

enum class Primitive:uint8_t {

	VERTEX,
	NORMAL,
	POLYGON,
	RGB_COLOR,
	RESERVED_0,
	RESERVED_1,
	RESERVED_2,
	RESERVED_3

};

struct Polygon3D {

	Ratio ratios[3];
	RgbPixel color;

};

struct Vertex3D {

	Vertex3D() : x(0.0f), y(0.0f), z(0.0f) {};
	Vertex3D(float x_, float y_, float z_) noexcept : x(x_), y(y_), z(z_) {};

	union {

		struct {

			float x;
			float y;
			float z;

		};

		float xyz[3];
	};
};

struct Normal3D {

	inline Normal3D() noexcept : x(0.0f), y(0.0f), z(0.0f) {};
	inline Normal3D(float x_, float y_, float z_) noexcept : x(x_), y(y_), z(z_) {};

	union {

		struct {

			float x;
			float y;
			float z;

		};

		float xyz[3];
	};

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

	virtual inline void  SetSize(const size_t new_size)noexcept(true) final { size_ = new_size; };
	virtual inline void SetCapacity(const size_t new_capacity)noexcept(true) final { capacity_ = new_capacity; };

	explicit Array(const size_t size)noexcept(true) :
		size_{ size },
		capacity_{ size } {}

public:

	virtual inline bool Empty()const noexcept(true) { return (Size() == 0); }
	virtual inline size_t Capacity()const noexcept(true) final { return capacity_; };
	virtual void Concat(const void* const ptr, const size_t size)noexcept(true) = 0;
	virtual inline size_t Size()const noexcept(true) final { return size_; };
	virtual ~Array() {};

};

template<class DataType>
class ArrayOf : public Array {
private:

	DataType* ptr_;

	using BaseClass = Array;

protected:

public:

	inline DataType* const Ptr()noexcept(true)  { return ptr_; };
	inline void Clear()noexcept(true) { if(ptr_ != nullptr)delete[] ptr_; }
	ArrayOf& operator=(const ArrayOf& arr)noexcept(true) = delete;
	ArrayOf&& operator=(ArrayOf&& arr)noexcept(true) = delete;
	explicit ArrayOf(const ArrayOf& arr)noexcept(true) = delete;
	explicit ArrayOf(ArrayOf&& arr)noexcept(true) = delete;

	explicit ArrayOf(DataType* const ptr, const size_t size)noexcept(true):
		Array{ (ptr != nullptr) ? (size) : (0) },
		ptr_{ ptr }{}

	explicit ArrayOf()noexcept(true):
		Array{ 0 },
		ptr_{ nullptr }{}
	
	inline DataType& operator[](const size_t index)const noexcept(true) { return *(ptr_ + index); };

	void Reserve(const size_t new_capacity) noexcept(true) {

		if (BaseClass::Capacity() < new_capacity) {

			DataType* const new_ptr = (DataType* const) new DataType[new_capacity];
			const size_t cpy_end = BaseClass::Size();

			for (size_t i = 0; i < cpy_end; i++){

				new_ptr[i] = ptr_[i];

			}

			Clear();
			ptr_ = new_ptr;

			BaseClass::SetCapacity(new_capacity);

		}

	};

	void Concat(const void* const ptr, const size_t size)noexcept(true) override{

		const DataType* const concat_ptr = reinterpret_cast<const DataType* const>(ptr);
		const size_t final_size = BaseClass::Size() + size;

		if (final_size <= BaseClass::Capacity()) {

			const size_t cpy_start = BaseClass::Size();

			for (size_t i = cpy_start, j = 0; j < size; i++, j++){

				ptr_[i] = concat_ptr[j];

			}

			BaseClass::SetSize(final_size);

		}
		else {

			DataType* const new_ptr = (DataType* const) new DataType[final_size];

			for (size_t i = 0; i < BaseClass::Size(); i++){

				new_ptr[i] = ptr_[i];

			}

			const size_t cpy_start = BaseClass::Size();

			for (size_t i = cpy_start, j = 0; j < size; i++, j++) {

				new_ptr[i] = concat_ptr[j];

			}

			Clear();
			ptr_ = new_ptr;

		}

	}

	void PushBack(const DataType& data)noexcept(true){

		if (BaseClass::Size() + 1 > BaseClass::Capacity())
			Reserve(BaseClass::Capacity() + 1);
		
		if(Empty())
			ptr_[0] = data;
		else
			ptr_[BaseClass::Size() - 1] = data;

		BaseClass::SetSize(BaseClass::Size() + 1);

	}

	~ArrayOf()noexcept(true){}

};

#endif //ENGINE_TYPES_HPP_