#pragma once
#ifndef ENGINE_TYPES_HPP_
#define ENGINE_TYPES_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <functional>


using ModelId = size_t;
using CameraId = size_t;

struct Ratio {
	union {
		struct {
			unsigned int vertex_n;
			unsigned int normal_n;
		};

		unsigned int v_n[2];

	};

};

struct Vector2D {

	union {

		struct {

			float x;
			float y;

		};

		float xy[2];
	
	};
};

struct Vector3D {

	union {

		struct {

			float x;
			float y;
			float z;

		};
	
		float xyz[3];

	};
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

	//explicit operator FramePixel(){
	//	
	//	//Huinya code
	//	FramePixel pixel;
	//	pixel.rgba_red = rgba_red;
	//	pixel.rgba_green = rgba_green;
	//	pixel.rgba_blue = rgba_blue;
	//	pixel.rgba_alpha = rgba_alpha;

	//	return pixel;
	//
	//}

};

using FramePixel = RgbColor;

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
	RgbColor color;

};

struct Vertex2D {

	union {

		struct {

			float x;
			float y;

		};

		float xy[2];

	};

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

template<class Type>
static void Swap(Type& first, Type& second) noexcept(true) {

	Type ref{ std::move(first) };
	first = std::move(second);
	second = std::move(ref);

}

template<class PrimitiveType>
class Location final {
private:

	const std::shared_ptr<PrimitiveType> data_;
	size_t size_;

public:

	explicit Location()noexcept(true) :
		data_{ nullptr },
		size_{ 0 }{};

	explicit Location(const std::shared_ptr<PrimitiveType> data_ptr, const size_t size)noexcept(true) :
		data_{ data_ptr },
		size_{ size }{}

	inline const std::shared_ptr<PrimitiveType> const Data()const noexcept(true) { return data_; };
	inline size_t Size()const noexcept(true) { return size; };

};

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

#include "../__engine_1.1/__gpu_manager_/GpuManager.hpp"

template<class DataType>
class HostArrayOf;

template<class DataType>
class DeviceArrayOf final : public AbstractArray {
private:

	template<class Type>
	struct deleter
	{
		
		void operator ()(Type* const data_ptr){

			FreeMemory(static_cast<void*>(data_ptr));
		
		}

	};

	mutable DataType* ptr_;
	
public:

	explicit DeviceArrayOf(DeviceArrayOf<DataType>&& arr)noexcept(true):
		ptr_{ nullptr },
		AbstractArray::AbstractArray{}{

		Swap(this->ptr_, arr.ptr_);
		Swap(this->capacity_, arr.capacity_);
		Swap(this->size_, arr.size_);

	}

	explicit DeviceArrayOf(DataType* ptr, size_t size)noexcept(true) :
		ptr_{ ptr },
		AbstractArray{ size } {}


	explicit DeviceArrayOf()noexcept(true) :
		ptr_{ nullptr } {}

	void Reserve(const size_t new_capacity)const noexcept(true) {

		if (AbstractArray::Capacity() < new_capacity) {

			DataType* new_ptr = static_cast<DataType*>(AllocateMemory(static_cast<long long>(new_capacity) * sizeof(DataType)));
			const size_t cpy_end = AbstractArray::Size();

			for (size_t i = 0; i < cpy_end; i++) {

				new_ptr[i] = std::move(ptr_[i]);

			}

			Clear();
			ptr_ = new_ptr;

			AbstractArray::SetSize(cpy_end);
			AbstractArray::SetCapacity(new_capacity);

		}

	}

	void Clear()const noexcept(true) {
	
		if (ptr_ != nullptr) {

			FreeMemory(static_cast<void*>(ptr_));
			AbstractArray::SetCapacity(0);
			AbstractArray::SetSize(0);

		}

	}

	void Copy(const HostArrayOf<DataType>& host_array)noexcept(true) {

		const size_t host_array_size = host_array.Size();
		if (host_array_size > this->Capacity()) 
			Reserve(host_array_size);

		MemoryCopy(static_cast<const void*>(host_array.Data()), static_cast<void*>(this->ptr_), host_array_size * sizeof(DataType));

		this->SetSize(host_array_size);

	}

	DeviceArrayOf<DataType>& operator=(DeviceArrayOf<DataType>&& arr)noexcept(true) {

		if (this == &arr)
			return *this;

		Swap(this->ptr_, arr.ptr_);
		Swap(this->capacity_, arr.capacity_);
		Swap(this->size_, arr.size_);

		return *this;

	}

	inline const DataType* const Ptr()const noexcept(true) { return ptr_; }
	inline DataType* const Ptr()noexcept(true) { return ptr_; }

	~DeviceArrayOf()noexcept(true) {

		FreeMemory(static_cast<void*>(ptr_));

	}

};

template<class DataType>
class HostArrayOf : public AbstractArray{
private:

	//"ptr_" is "mutable" because some methods, for example, Reserve we have to do non const.
	//Method reserve should be const, elements of array dont change.
	mutable DataType* ptr_;

protected:
public:

	//Copy constructor
	explicit HostArrayOf(const HostArrayOf<DataType>& arr)noexcept(true):
		AbstractArray{ arr.Size() }{
	
		ptr_ = (DataType*) new DataType[arr.Size()];

		for (size_t i = 0; i < arr.Size(); i++)
			ptr_[i] = arr[i];

	}

	//Move constructor
	explicit HostArrayOf(HostArrayOf<DataType>&& arr)noexcept(true) :
		HostArrayOf{} {

		std::swap(ptr_, arr.ptr_);
		std::swap(size_, arr.size_);
		std::swap(capacity_, arr.capacity_);

	}

	//Default constructor
	explicit HostArrayOf()noexcept(true) :
		AbstractArray{ 0 },
		ptr_{ nullptr }{}

	//Copy operator=
	HostArrayOf<DataType>& operator=(const HostArrayOf<DataType>& arr)noexcept(true) {

		if (&arr == this) 
			return *this;

		Clear();

		const size_t arr_size = arr.Size();
		Reserve(arr_size);

		for (size_t i = 0; i < AbstractArray::Capacity(); i++)
			ptr_[i] = arr[i];

		AbstractArray::SetSize(arr_size);
	
		return *this;
	}

	//Move operator=
	HostArrayOf<DataType>& operator=(HostArrayOf<DataType>&& arr)noexcept(true) {

		if (&arr == this)
			return *this;

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
	void PushBack(DataType&& data)noexcept(true);

	inline DataType& operator[](const size_t index)noexcept(true) { 
		return *(ptr_ + index);
	};

	inline const DataType& operator[](const size_t index)const noexcept(true) {
		return *(ptr_ + index);
	};

	void Concat(const HostArrayOf<DataType>& arr)noexcept(true);

	inline const DataType* const Data() const noexcept(true) { return ptr_; }

	//Virtual destructor
	virtual ~HostArrayOf()noexcept(true){
	
		if (ptr_ != nullptr)
			delete[] ptr_;

	}

	HostArrayOf<DataType>& operator+(const HostArrayOf<DataType>& arr)noexcept(true) {

		Concat(arr);

		return *this;
	}

};

//ArrayOf<DataType> methods realizations.
template<class DataType>
void HostArrayOf<DataType>::Reserve(const size_t new_capacity)const noexcept(true){

	if (AbstractArray::Capacity() < new_capacity) {

		DataType* new_ptr = (DataType*) new DataType[new_capacity];
		const size_t cpy_end = AbstractArray::Size();

		for (size_t i = 0; i < cpy_end; i++) {

			new_ptr[i] = std::move(ptr_[i]);

		}

		Clear();
		ptr_ = new_ptr;

		AbstractArray::SetSize(cpy_end);
		AbstractArray::SetCapacity(new_capacity);

	}

};

template<class DataType>
void HostArrayOf<DataType>::PushBack(const DataType& data)noexcept(true) {

	//Reserves memory if necessary.
	if (AbstractArray::Size() + 1 > AbstractArray::Capacity())
		Reserve(AbstractArray::Capacity() + 1);

	if (Empty())
		ptr_[0] = data;
	else
		ptr_[AbstractArray::Size()] = data;

	AbstractArray::SetSize(AbstractArray::Size() + 1);

}

template<class DataType>
void HostArrayOf<DataType>::PushBack(DataType&& data)noexcept(true) {

	//Reserves memory if necessary.
	if (AbstractArray::Size() + 1 > AbstractArray::Capacity())
		Reserve(AbstractArray::Capacity() + 1);

	if (Empty())
		ptr_[0] = std::move(data);
	else
		ptr_[AbstractArray::Size()] = std::move(data);

	AbstractArray::SetSize(AbstractArray::Size() + 1);

}

template<class DataType>
void HostArrayOf<DataType>::Concat(const HostArrayOf<DataType>& arr)noexcept(true){

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

struct NPrimitives {

	size_t n_vertexs;
	size_t n_normals;
	size_t n_polygons;
	size_t n_rgb_colors;

	NPrimitives& operator+(const NPrimitives& n_primitives)noexcept(true) {

		n_vertexs += n_primitives.n_vertexs;
		n_normals += n_primitives.n_normals;
		n_polygons += n_primitives.n_polygons;
		n_rgb_colors += n_primitives.n_rgb_colors;

		return *this;
	}
};

struct DevicePtrs {

	Vertex3D* vertexs;
	Normal3D* normals;
	Polygon3D* polygons;
	RgbColor* rgb_colors;

	DevicePtrs& operator+(const NPrimitives& n_primitives)noexcept(true) {

		vertexs += n_primitives.n_vertexs;
		normals += n_primitives.n_normals;
		polygons += n_primitives.n_polygons;
		rgb_colors += n_primitives.n_rgb_colors;

		return *this;

	}

};

class DeviceData final {
private:

	DeviceArrayOf<Vertex3D> vertexs_;
	DeviceArrayOf<Normal3D> normals_;
	DeviceArrayOf<Polygon3D> polygons_;
	DeviceArrayOf<RgbColor> rgb_colors_;

protected:
public:

	//explicit DeviceData(Vertex3D* const vertexs,
	//	Normal3D* const  normals,
	//	Polygon3D* const polygons,
	//	RgbColor* const rgb_colors)noexcept(true) :
	//	vertexs_{ vertexs },
	//	normals_{ normals },
	//	polygons_{ polygons },
	//	rgb_colors_{ rgb_colors }{}

	explicit DeviceData(const size_t vertexs_size,
		const size_t normals_size,
		const size_t polygons_size,
		const size_t rgb_colors_size)noexcept(true) {


	}

	explicit DeviceData(DeviceData&& device_data)noexcept(true) {

		Swap(vertexs_, device_data.vertexs_);
		Swap(normals_, device_data.normals_);
		Swap(polygons_, device_data.polygons_);
		Swap(rgb_colors_, device_data.rgb_colors_);

	}

	explicit DeviceData(const DevicePtrs& ptrs, const NPrimitives& n_primitives)noexcept(true):
		vertexs_{ ptrs.vertexs, n_primitives.n_vertexs },
		normals_{ ptrs.normals, n_primitives.n_normals },
		polygons_{ ptrs.polygons, n_primitives.n_polygons },
		rgb_colors_{ ptrs.rgb_colors, n_primitives.n_rgb_colors }{ }

	explicit DeviceData()noexcept(true):
		vertexs_{},
		normals_{},
		polygons_{},
		rgb_colors_{}{}

	NPrimitives PrimitivesNumber()const noexcept(true) {

		return NPrimitives{ vertexs_.Size(), normals_.Size(), polygons_.Size(), rgb_colors_.Size() };

	}

	NPrimitives PrimitivesCapacity()const noexcept(true) {

		return NPrimitives{ vertexs_.Capacity(), normals_.Capacity(), polygons_.Capacity(), rgb_colors_.Capacity() };

	}

	void Reserve(const NPrimitives& n_primitives)noexcept(true) {

		vertexs_.Reserve(n_primitives.n_vertexs);
		normals_.Reserve(n_primitives.n_normals);
		polygons_.Reserve(n_primitives.n_polygons);
		rgb_colors_.Reserve(n_primitives.n_rgb_colors);

	}

	DeviceData& operator=(DeviceData&& device_data)noexcept(true) {

		if (this == &device_data)
			return *this;

		Swap(vertexs_, device_data.vertexs_);
		Swap(normals_, device_data.normals_);
		Swap(polygons_, device_data.polygons_);
		Swap(rgb_colors_, device_data.rgb_colors_);

		return *this;
	}

	const DevicePtrs Ptrs()noexcept(true) {

		const DevicePtrs ptrs{

			vertexs_.Ptr(),
			normals_.Ptr(),
			polygons_.Ptr(),
			rgb_colors_.Ptr()

		};
		
		return ptrs;

	}


	////Default costructor
	//explicit DeviceData()noexcept(true) :
	//	vertexs_{ nullptr },
	//	normals_{ nullptr },
	//	polygons_{ nullptr },
	//	rgb_colors_{ nullptr }{}

	template<class PrimitiveType>
	inline const PrimitiveType* const Ptr()const noexcept(true) { return nullptr; };
	template<>
	inline const Vertex3D* const Ptr<Vertex3D>() const noexcept(true) { return vertexs_.Ptr(); };
	template<>
	inline const Normal3D* const Ptr<Normal3D>()const noexcept(true) { return normals_.Ptr(); };
	template<>
	inline const Polygon3D* const Ptr<Polygon3D>()const noexcept(true) { return polygons_.Ptr(); };
	template<>
	inline const RgbColor* const Ptr<RgbColor>()const noexcept(true) { return rgb_colors_.Ptr(); };

	friend class DataBase;
};

class HostData final{
private:

	HostArrayOf<Vertex3D> vertexs_;
	HostArrayOf<Normal3D> normals_;
	HostArrayOf<Polygon3D> polygons_;
	HostArrayOf<RgbColor> rgb_colors_;
	//...

protected:

public:

	//Copy constructor
	explicit HostData(const HostData& data)noexcept(true) {

		vertexs_ = data.vertexs_;
		normals_ = data.normals_;
		polygons_ = data.polygons_;
		rgb_colors_ = data.rgb_colors_;
		//...
	}

	//Move costructor
	explicit HostData(HostData&& data)noexcept(true):HostData{} {

		Swap(vertexs_, data.vertexs_);
		Swap(normals_, data.normals_);
		Swap(polygons_, data.polygons_);
		Swap(rgb_colors_, data.rgb_colors_);
		//...
	}

	//Default constructor
	explicit HostData()noexcept(true) :
		vertexs_{},
		normals_{},
		polygons_{},
		rgb_colors_{}
		//...
	{  }

	HostData& operator=(const HostData& data)noexcept(true) {

		if (&data == this)
			return *this;

		vertexs_ = data.vertexs_;
		normals_ = data.normals_;
		polygons_ = data.polygons_;
		rgb_colors_ = data.rgb_colors_;
		//...

		return *this;

	}

	HostData& operator=(HostData&& data)noexcept(true) {

		if (&data == this)
			return *this;

		Swap(vertexs_, data.vertexs_);
		Swap(normals_, data.normals_);
		Swap(polygons_, data.polygons_);
		Swap(rgb_colors_, data.rgb_colors_);
		//...

		return *this;
	}

	HostData& operator+(const HostData& data_host)noexcept(true) {

		vertexs_ = vertexs_ + data_host.vertexs_;
		normals_ = normals_ + data_host.normals_;
		polygons_ = polygons_ + data_host.polygons_;
		rgb_colors_ = rgb_colors_ + data_host.rgb_colors_;
		//...

		return *this;
	}

	void CorrectPolygonOffset(const NPrimitives& polygon_offset)noexcept(true) {

		for (size_t i = 0; i < polygons_.Size(); i++){
			
			for (size_t j = 0; j < 3; j++) {

				polygons_[i].ratios[j].vertex_n += polygon_offset.n_vertexs;
				polygons_[i].ratios[j].normal_n += polygon_offset.n_normals;

			}
			
		}
		
	}

	/*template<class PrimitiveType>
	inline Location<PrimitiveType> RetLocation()const noexcept(true) { return Location<PrimitiveType>{}; };
	template<>
	inline Location<Vertex3D> RetLocation()const noexcept(true) { return Location<Vertex3D>{ vertexs_.Data(), vertexs_.Size() }; };
	template<>
	inline Location<Normal3D> RetLocation()const noexcept(true) { return Location<Normal3D>{ normals_.Data(), normals_.Size() }; };
	template<>
	inline Location<Polygon3D> RetLocation()const noexcept(true) { return Location<Polygon3D>{ polygons_.Data(), polygons_.Size() }; };
	template<>
	inline Location<RgbColor> RetLocation()const noexcept(true) { return Location<RgbColor>{ rgb_colors_.Data(), rgb_colors_.Size() }; };*/
	//

	
	template<class PrimitiveType>
	inline const HostArrayOf<PrimitiveType>& Data()const noexcept(true) { return nullptr; };
	template<>
	inline const HostArrayOf<Vertex3D>& Data<Vertex3D>()const noexcept(true) { return vertexs_; };
	template<>
	inline const HostArrayOf<Normal3D>& Data<Normal3D>()const noexcept(true) { return normals_; };
	template<>
	inline const HostArrayOf<Polygon3D>& Data<Polygon3D>()const noexcept(true) { return polygons_; };
	template<>
	inline const HostArrayOf<RgbColor>& Data<RgbColor>()const noexcept(true) { return rgb_colors_; };

	//
	template<class PrimitiveType>
	inline HostArrayOf<PrimitiveType>& Data()noexcept(true) { return nullptr; };
	template<>
	inline HostArrayOf<Vertex3D>& Data<Vertex3D>()noexcept(true) { return vertexs_; };
	template<>
	inline HostArrayOf<Normal3D>& Data<Normal3D>()noexcept(true) { return normals_; };
	template<>
	inline HostArrayOf<Polygon3D>& Data<Polygon3D>()noexcept(true) { return polygons_; };
	template<>
	inline HostArrayOf<RgbColor>& Data<RgbColor>()noexcept(true) { return rgb_colors_; };


	template<class PrimitiveType>
	inline size_t NumberOf()const noexcept(true) { return 0; }

	template<>
	inline size_t NumberOf<Vertex3D>()const noexcept(true) { return Data<Vertex3D>().Size(); }

	template<>
	inline size_t NumberOf<Normal3D>()const noexcept(true) { return Data<Normal3D>().Size(); }

	template<>
	inline size_t NumberOf<Polygon3D>()const noexcept(true) { return Data<Polygon3D>().Size(); }

	template<>
	inline size_t NumberOf<RgbColor>()const noexcept(true) { return Data<RgbColor>().Size(); }

	//
	virtual ~HostData()noexcept(true) {}

};


class Model final {
private:

	static const std::string kDefaultFileName;
	static const std::string kDefaultModelName;

	std::string model_name_;
	std::string file_name_;
	DeviceData device_data_;
	HostData host_data_;


	//Test
	Vertex3D model_center_;

public:

	//Default constructor
	explicit Model(const std::string& model_name = kDefaultModelName, const std::string& file_name = kDefaultFileName)noexcept(true):
		model_name_{ model_name },
		file_name_{ file_name },
		//Test
		model_center_{ 0.0, 0.0, 0.0 }{}

	inline void SetHostData(HostData&& data)noexcept(true) {

		host_data_ = std::move(data);

	}


	inline void SetDeviceData(DeviceData&& data)noexcept(true) {

		device_data_ = std::move(data);

	}

	Model& operator=(Model&& model)noexcept(true) {

		if (this == &model)
			return *this;

		Swap(model_name_, model.model_name_ );
		Swap(file_name_, model.file_name_);
		Swap(std::move(device_data_), std::move(model.device_data_));
		Swap(std::move(host_data_), std::move(model.host_data_));

		return *this;
	}

	NPrimitives PrimitivesNumber()const noexcept(true) {

		return NPrimitives{ NumberOf<Vertex3D>(), NumberOf<Normal3D>(), NumberOf<Polygon3D>(), NumberOf<RgbColor>() };

	}

	template<class PrimitiveType>
	inline size_t NumberOf()const noexcept(true) { return 0; };

	template<>
	inline size_t NumberOf<Vertex3D>() const noexcept(true) { return host_data_.NumberOf<Vertex3D>(); };

	template<>
	inline size_t NumberOf<Normal3D>() const noexcept(true) { return host_data_.NumberOf<Normal3D>(); };

	template<>
	inline size_t NumberOf<Polygon3D>() const noexcept(true) { return host_data_.NumberOf<Polygon3D>(); };

	template<>
	inline size_t NumberOf<RgbColor>() const noexcept(true) { return host_data_.NumberOf<RgbColor>(); };

	friend class DataBase;
	friend class LogicEngine;

	//template<class Vertex3D>
	//inline size_t NPrimitiveElemennts<Vertex3D>()const noexcept(true) { return ; }
	//Get device data
	/*template<class PrimitiveType>
	inline Location<PrimitiveType> const DeviceData()const noexcept(true) { return Location<PrimitiveType>{}; };
	template<>
	inline Location<Vertex3D> const DeviceData()const noexcept(true) { return d_vertexs_; }
	template<>
	inline Location<Normal3D> const DeviceData()const noexcept(true) { return d_normals_; }
	template<>
	inline Location<Polygon3D> const DeviceData()const noexcept(true) { return d_polygons_; }
	template<>
	inline Location<RgbColor> const DeviceData()const noexcept(true) { return d_rgb_colors_; }*/

};

#include <windows.h>
#include <array>

struct Coordinats {
	size_t x;
	size_t y;
};

using BUFFERINFO = BITMAPINFO;

class AbstractFrame abstract {
private:

protected:

	FramePixel* buffer_;
	static const size_t kBitsPerPixel;
	static const RgbColor kDefaultColor;
	static const size_t kDefaultWidth;
	static const size_t kDefaultHeight;
	size_t width_;
	size_t height_;

public:

	virtual inline size_t Width()const noexcept(true) { 
		
		return width_; 
	
	};

	virtual inline size_t Height()const noexcept(true) { 
		
		return height_; 
	
	};

	virtual inline explicit operator const void* ()const noexcept(true) { 
		
		return buffer_;

	}

	inline FramePixel* FramePixelPtr()noexcept(true) { return buffer_; };

	virtual inline explicit operator void* ()noexcept(true) {

		return buffer_;

	}

	explicit AbstractFrame(const size_t width = kDefaultWidth, const size_t height = kDefaultHeight)noexcept(true):
		width_{ width },
		height_{ height },
		buffer_{ nullptr }{}

	virtual ~AbstractFrame()noexcept(true) {
	

	}

};

#include "../__engine_1.1/__gpu_manager_/CUDAGpuManager.cuh"

class FrameHost;

class FrameDevice final : public AbstractFrame  {
private:

public:

	explicit FrameDevice(const size_t width, const size_t height)noexcept(true):
		AbstractFrame{ width, height }{

		buffer_ = static_cast<FramePixel*>(AllocateMemory(width * height * sizeof(FramePixel)));

	}

	~FrameDevice()noexcept(true){
	
		FreeMemory(static_cast<void*>(buffer_));
	
	}
	
	friend void CopyDeviceFrameToHostFrame(FrameHost& host_frame, FrameDevice& device_frame)noexcept(true);

};


class FrameHost final : public AbstractFrame {
private:

	BUFFERINFO buffer_info_;

public:

	explicit FrameHost(FrameHost&& frame)noexcept(true):
		AbstractFrame{} {

		Swap(buffer_, frame.buffer_);
		Swap(buffer_info_, frame.buffer_info_);
		Swap(width_, frame.width_);
		Swap(height_, frame.height_);
		//BUFFERINFI...

	}

	explicit FrameHost(const size_t width, const size_t height, const RgbColor& color = kDefaultColor)noexcept(true):
		AbstractFrame{ width, height },
		buffer_info_{ 0 }{

		AbstractFrame::buffer_ = new FramePixel[width * height];

		for (size_t i = 0; i < width * height; i++)
			buffer_[i] = color;

		buffer_info_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		buffer_info_.bmiHeader.biWidth = width_;
		buffer_info_.bmiHeader.biHeight = -(static_cast<int>(height_));
		buffer_info_.bmiHeader.biPlanes = 1;
		buffer_info_.bmiHeader.biBitCount = 32;
		buffer_info_.bmiHeader.biCompression = BI_RGB;
		buffer_info_.bmiHeader.biSizeImage = ((width_ * 24 + 31) & ~31) / 8 * height_;
		buffer_info_.bmiHeader.biXPelsPerMeter = 0;
		buffer_info_.bmiHeader.biYPelsPerMeter = 0;
		buffer_info_.bmiHeader.biClrUsed = 0;
		buffer_info_.bmiHeader.biClrImportant = 0;

	}

	FrameHost& operator=(FrameHost&& frame)noexcept(true) {

		if (this == &frame)
			return *this;

		Swap(buffer_, frame.buffer_);
		Swap(width_, frame.width_);
		Swap(height_, frame.height_);

		return *this;

	}

	FrameHost& operator=(const FrameHost& frame)noexcept(true) = delete;

	inline FramePixel& operator[](const Coordinats coord)noexcept(true){

		return buffer_[width_ * coord.y + coord.x];

	}
	
	//const FramePixel* Ptr()const noexcept(true) { return buffer_.data(); };

	~FrameHost()noexcept(true) {

		if (buffer_ != nullptr)
			delete[] buffer_;

	}

	inline const BUFFERINFO& Info()const noexcept(true) { return buffer_info_; };

	friend void CopyDeviceFrameToHostFrame(FrameHost& host_frame, FrameDevice& device_frame)noexcept(true);

};

enum class KeyType : uint32_t {

	Nothing = 0,
	ArrowLeft = 1,
	ArrowRight = 2,
	ArrowUp = 3,
	ArrowDown = 4,
	WheelDown = 5,
	WheelUp = 6,
	MouseLeft = 7,
	MouseRight = 8,
	A = 9,
	S = 10,
	D = 11,
	W = 12

};

struct Keystroke {

	KeyType key_type;
	long long time_point;

	explicit Keystroke()noexcept(true):
		key_type{ KeyType::Nothing },
		time_point{ 0 }{}

	explicit Keystroke(KeyType key, long long now)noexcept(true):
		key_type{ key },
		time_point{ now }{}

};

#include <mutex>
#include <queue>

class KeystrokesQueue final {
private:

	enum class Operation:uint8_t {

		POP = 0,
		PUSH = 1

	};

	std::queue<Keystroke> keystrokes_;
	bool empty_;
	std::mutex mutex_;

	static const Keystroke kDefaultKeystroke;

	Keystroke ProcessOperation(Operation operation, const Keystroke& keystroke = kDefaultKeystroke)noexcept(true) {

		//mutex_.lock();

		if (operation == Operation::PUSH) {

			keystrokes_.push(keystroke);

			//mutex_.unlock();
		}
		else {

			if (!keystrokes_.empty()) {

				Keystroke result = keystrokes_.back();
				keystrokes_.pop();

				//mutex_.unlock();
				return result;

			}
			//mutex_.unlock();
			return Keystroke{};

		}


	}

public:

	explicit KeystrokesQueue()noexcept(true):
		empty_{ true },
		mutex_{},
		keystrokes_{} { }

	void Push(const Keystroke& keystroke)noexcept(true) {

		mutex_.lock();
		ProcessOperation(Operation::PUSH, keystroke);
		mutex_.unlock();

	}

	Keystroke Pop()noexcept(true) {

		mutex_.lock();
		Keystroke keystroke = ProcessOperation(Operation::POP);;
		mutex_.unlock();

		return keystroke;

	}

};

using KeystrProcFunc = std::function<void(const KeyType&)>;
using LogicFunc = std::function<void()>;
using NextKeystroke = std::function<Keystroke()>;
using StartPositionFunc = std::function<void()>;

#include <string>
#include <codecvt>

std::wstring ConvertStringToWstring(const std::string& str)noexcept(true);

template<class PrimitiveType>
void RotatePrimitive(PrimitiveType& vertex, float alpha_rad, const Vector3D& around_vector, const Vertex3D& around_vertex)noexcept(true) {

	const float sin_alpha = sin(alpha_rad);
	const float cos_alpha = cos(alpha_rad);

	float VX = around_vector.x;
	float VY = around_vector.y;
	float VZ = around_vector.z;

	float XP = vertex.x - around_vertex.x;
	float YP = vertex.y - around_vertex.y;
	float ZP = vertex.z - around_vertex.z;

	vertex.x = (cos_alpha + (1 - cos_alpha) * VX * VX) * XP + ((1 - cos_alpha) * VX * VY - sin_alpha * VZ) * YP + ((1 - cos_alpha) * VX * VZ + sin_alpha * VY) * ZP + around_vertex.x;
	vertex.y = ((1 - cos_alpha) * VY * VX + sin_alpha * VZ) * XP + (cos_alpha + (1 - cos_alpha) * VY * VY) * YP + ((1 - cos_alpha) * VY * VZ - sin_alpha * VX) * ZP + around_vertex.y;
	vertex.z = ((1 - cos_alpha) * VZ * VX - sin_alpha * VY) * XP + ((1 - cos_alpha) * VZ * VY + sin_alpha * VX) * YP + (cos_alpha + (1 - cos_alpha) * VZ * VZ) * ZP + around_vertex.z;

}

#endif //ENGINE_TYPES_HPP_