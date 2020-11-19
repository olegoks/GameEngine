#pragma once
#ifndef ENGINE_TYPES_HPP_
#define ENGINE_TYPES_HPP_

#include <cstdint>
#include <string>
#include <vector>

struct Ratio {

	unsigned int vertex_n;
	unsigned int normal_n;

};

struct RgbColor {

	union {

		struct {

			uint8_t rgba_blue;
			uint8_t rgba_green;
			uint8_t rgba_red;
			uint8_t rgba_alpha;

		};

		uint8_t bgra[4];

	};

};

struct RgbPixel : public RgbColor {


};

enum class Primitive :uint8_t {

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





//Constant initialized int engine_types.cpp

//class Model final{
//private:
//	
//	std::string model_name_;
//	std::string file_name_;
//	size_t a = kNPrimitives;
//	Data data_;
//
//	const Vertex3D* d_vertexs_;
//	const Normal3D* d_normals_;
//	const Polygon3D* d_polygons_;
//	const RgbColor* d_rgb_colors_;
//
//public:
//	
//	explicit Model()noexcept(true) {
//
//	}
//
//	template<class PrimitiveType>
//	PrimitiveType* Ptr()const noexcept(true) { return nullptr; };
//	template<>
//	Vertex3D* Ptr<Vertex3D>() const noexcept(true) { return data_.; }
//};

#include <memory>

//In identifier names 
//n/N == number/Number
//el/El == element/Element / elements/Elements

//Abstract class for ArrayOf that are contain all common methods
//protected:
//SetSize - set array size, this only for child classes, realization some methods.
//SetCapacity - set array capacity.
//Two constructors are protected because the class is abstract, there will not construct no one objects of this class.
//They are need child class for initialize of size_ and capacity.
//public:
//Ptr (no const/const) - returns pointer to memory block with array elements.
//Concat - there are two versions of this method: argument pointer to memory block or reference to class object. This method concats two arrays in one resulting.
//operator[] - access to array elements. 
//Reserve - increases factual array size. Reserve memory for future adds.
//PushBack - adds element to array end. If necessary reserves memory.
//Clear - removes all array elements.
//Size - returns number of elements in the array.
//Capacity - returns factual array size.
//Empty - if array doesnt contain elements return true else false.

using ArrElement = void;
class AbstractArray abstract{
private:
protected:

	//mutable reason check ptr_ field. 
	mutable size_t size_;
	mutable size_t capacity_;

	virtual inline void  SetSize(const size_t new_size)const noexcept(true) final { size_ = new_size; };
	virtual inline void SetCapacity(const size_t new_capacity)const noexcept(true) final { capacity_ = new_capacity; };

	explicit AbstractArray(const size_t size)noexcept(true) :
		size_{ size },
		capacity_{ size } {}
	explicit AbstractArray()noexcept(true) :size_{ 0 }, capacity_{ 0 } {}

public:

	virtual void Reserve(const size_t new_capacity)const noexcept(true) = 0;
	virtual void Clear()const noexcept(false) = 0;
	virtual inline size_t Size()const noexcept(true) final { return size_; };
	virtual inline size_t Capacity()const noexcept(true) final { return capacity_; };
	virtual inline bool Empty()const noexcept(true)final { return (Size() == 0); }
	virtual ~AbstractArray() {};

};

template<class DataType>
class ArrayOf : public AbstractArray{
private:

	//"ptr_" is "mutable" because some methods, for example, Reserve we have to do non const.
	//Method reserve should be const, elements of array dont change.
	mutable DataType* ptr_;

protected:
public:

	//Copy constructor
	explicit ArrayOf(const ArrayOf<DataType>& arr)noexcept(true):
		AbstractArray{ arr.Size() }{
	
		ptr_ = (DataType*) new DataType[arr.Size()];

		for (size_t i = 0; i < arr.Size(); i++)
			ptr_[i] = arr[i];

	}
	//Move constructor
	explicit ArrayOf(ArrayOf<DataType>&& arr)noexcept(true) :
		ArrayOf{} {

		std::swap(ptr_, arr.ptr_);
		std::swap(size_, arr.size_);
		std::swap(capacity_, arr.capacity_);

	}


	//Default constructor
	explicit ArrayOf()noexcept(true) :
		AbstractArray{ 0 },
		ptr_{ nullptr }{}

	//Copy operator=
	ArrayOf<DataType>& operator=(const ArrayOf<DataType>& arr)noexcept(true) {

		Clear();

		const size_t arr_size = arr.Size();
		Reserve(arr_size);

		for (size_t i = 0; i < AbstractArray::Capacity(); i++)
			ptr_[i] = arr[i];

		AbstractArray::SetSize(arr_size);
	
		return *this;
	}

	//Move operator=
	ArrayOf<DataType>& operator=(ArrayOf<DataType>&& arr)noexcept(true) {

		std::swap(ptr_, arr.ptr_);
		std::swap(size_, arr.size_);
		std::swap(capacity_, arr.capacity_);

		return *this;

	}

	//Override methods
	inline void Clear()const noexcept(true)override {

		if (ptr_ != nullptr)delete[] ptr_;
		AbstractArray::SetSize(0);
		AbstractArray::SetCapacity(0);

	}

	void Reserve(const size_t new_capacity)const noexcept(true) override;
	
	//Not override methods
	void PushBack(const DataType& data)noexcept(true);

	inline DataType& operator[](const size_t index)noexcept(true) { 
		return *(ptr_ + index);
	};

	inline const DataType& operator[](const size_t index)const noexcept(true) {
		return *(ptr_ + index);
	};

	void Concat(const ArrayOf<DataType>& arr)noexcept(true);

	inline const DataType* const Data() const noexcept(true) { return ptr_; }

	//Virtual destructor
	virtual ~ArrayOf()noexcept(true){
	
		if (ptr_ != nullptr)
			delete[] ptr_;

	}

};

//ArrayOf<DataType> methods realizations.
template<class DataType>
void ArrayOf<DataType>::Reserve(const size_t new_capacity)const noexcept(true){

	if (AbstractArray::Capacity() < new_capacity) {

		DataType* new_ptr = (DataType*) new DataType[new_capacity];
		const size_t cpy_end = AbstractArray::Size();

		for (size_t i = 0; i < cpy_end; i++) {

			new_ptr[i] = ptr_[i];

		}

		Clear();
		ptr_ = new_ptr;

		AbstractArray::SetCapacity(new_capacity);

	}

};

template<class DataType>
void ArrayOf<DataType>::PushBack(const DataType& data)noexcept(true) {

	//Reserves memory if necessary.
	if (AbstractArray::Size() + 1 > AbstractArray::Capacity())
		Reserve(AbstractArray::Capacity() + 1);

	if (Empty())
		ptr_[0] = data;
	else
		ptr_[AbstractArray::Size() - 1] = data;

	AbstractArray::SetSize(AbstractArray::Size() + 1);

}

template<class DataType>
void ArrayOf<DataType>::Concat(const ArrayOf<DataType>& arr)noexcept(true){

	//Pointer to concat memory block.
	const DataType* const concat_ptr = arr.Data();
	
	//Final capacity(== size) of this array
	const size_t final_size = AbstractArray::Size() + arr.Size();

	//Check this class capacity. If this class capacity is enough 
	//copy new elements.
	if (final_size <= AbstractArray::Capacity()) {

		const size_t cpy_start = AbstractArray::Size();

		for (size_t i = cpy_start, j = 0; j < arr.Size(); i++, j++) {

			ptr_[i] = concat_ptr[j];

		}

		AbstractArray::SetSize(final_size);

	}
	//If need more memory, alloc new
	//memory block and copy old elements and new element after.
	else {

		DataType* const new_ptr = (DataType* const) new DataType[final_size];

		for (size_t i = 0; i < AbstractArray::Size(); i++) {

			new_ptr[i] = ptr_[i];

		}

		const size_t cpy_start = AbstractArray::Size();

		for (size_t i = cpy_start, j = 0; j < arr.Size(); i++, j++) {

			new_ptr[i] = concat_ptr[j];

		}

		Clear();

		ptr_ = new_ptr;

		AbstractArray::SetSize(final_size);
		AbstractArray::SetCapacity(final_size);

	}

}

class Data {
private:

	ArrayOf<Vertex3D> vertexs_;
	ArrayOf<Normal3D> normals_;
	ArrayOf<Polygon3D> polygons_;
	ArrayOf<RgbColor> rgb_colors_;
	//...
protected:

	template<class Type>
	void Swap(Type& first, Type& second)const noexcept(true) {

		Type&& ref = std::move(first);
		first = std::move(second);
		second = std::move(ref);

	}

public:

	//Copy constructor
	explicit Data(const Data& data)noexcept(true) {

		vertexs_ = data.vertexs_;
		normals_ = data.normals_;
		polygons_ = data.polygons_;
		rgb_colors_ = data.rgb_colors_;

	}

	//Move costructor
	explicit Data(Data&& data)noexcept(true) :Data{} {

		Swap(vertexs_, data.vertexs_);
		Swap(normals_, data.normals_);
		Swap(polygons_, data.polygons_);
		Swap(rgb_colors_, data.rgb_colors_);

	}

	//Default constructor
	explicit Data()noexcept(true) :
		vertexs_{},
		normals_{},
		polygons_{},
		rgb_colors_{}
		//...
	{  }

	template<class PrimitiveType>
	ArrayOf<PrimitiveType>& Get()noexcept(true) { return nullptr; };
	template<>
	ArrayOf<Vertex3D>& Get<Vertex3D>()noexcept(true) { return vertexs_; };
	template<>
	ArrayOf<Normal3D>& Get<Normal3D>()noexcept(true) { return normals_; };
	template<>
	ArrayOf<Polygon3D>& Get<Polygon3D>()noexcept(true) { return polygons_; };
	template<>
	ArrayOf<RgbColor>& Get<RgbColor>()noexcept(true) { return rgb_colors_; };

	virtual ~Data()noexcept(true) {}

};


#endif //ENGINE_TYPES_HPP_

