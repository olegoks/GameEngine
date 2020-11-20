#pragma once
#ifndef ENGINE_TYPES_HPP_
#define ENGINE_TYPES_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

enum class KeyType: uint32_t {

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

};

#include <queue>
#include <mutex>

class KeystrokesQueue final {
private:

	std::queue<Keystroke> queue_;
	std::mutex mutex_;

protected:
public:

	explicit KeystrokesQueue()noexcept(true) :
		queue_{} {}

	Keystroke Pop()noexcept(true) {

		mutex_.lock();

		if (!queue_.empty()) {

			Keystroke keystroke = queue_.back();
			queue_.pop();

		}

		mutex_.unlock();

	}

	void Push(const Keystroke keystroke)noexcept(true) {

		mutex_.lock();

		queue_.push(keystroke);

		mutex_.unlock();

	}

	inline bool Empty()const noexcept(true) { return queue_.empty(); }

};

struct Ratio {
	union {
		struct {
			unsigned int vertex_n;
			unsigned int normal_n;
		};

		unsigned int v_n[2];

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

	const PrimitiveType* data_;
	size_t size_;

public:

	explicit Location()noexcept(true) :
		data_{ nullptr },
		size_{ 0 }{};

	explicit Location(const PrimitiveType* data_ptr, const size_t size)noexcept(true) :
		data_{ data_ptr },
		size_{ size }{}

	inline const PrimitiveType* const Data()const noexcept(true) { return data_; };
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
	ArrayOf<DataType>& operator=(ArrayOf<DataType>&& arr)noexcept(true) {

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

	void Concat(const ArrayOf<DataType>& arr)noexcept(true);

	inline const DataType* const Data() const noexcept(true) { return ptr_; }

	//Virtual destructor
	virtual ~ArrayOf()noexcept(true){
	
		if (ptr_ != nullptr)
			delete[] ptr_;

	}

	ArrayOf<DataType>& operator+(const ArrayOf<DataType>& arr)noexcept(true) {

		Concat(arr);

		return *this;
	}

};

//ArrayOf<DataType> methods realizations.
template<class DataType>
void ArrayOf<DataType>::Reserve(const size_t new_capacity)const noexcept(true){

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
void ArrayOf<DataType>::PushBack(const DataType& data)noexcept(true) {

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
void ArrayOf<DataType>::PushBack(DataType&& data)noexcept(true) {

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

class DeviceData final {
private:

	const Vertex3D* vertexs_;
	const Normal3D* normals_;
	const Polygon3D* polygons_;
	const RgbColor* rgb_colors_;

protected:
public:

	explicit DeviceData(const Vertex3D* const vertexs,
		const Normal3D* const  normals,
		const Polygon3D* const polygons,
		const RgbColor* const rgb_colors)noexcept(true) :
		vertexs_{ vertexs },
		normals_{ normals },
		polygons_{ polygons },
		rgb_colors_{ rgb_colors }{}

	//Default costructor
	explicit DeviceData()noexcept(true) :
		vertexs_{ nullptr },
		normals_{ nullptr },
		polygons_{ nullptr },
		rgb_colors_{ nullptr }{}

	template<class PrimitiveType>
	inline const PrimitiveType* const Ptr()const noexcept(true) { return nullptr; };
	template<>
	inline const Vertex3D* const Ptr<Vertex3D>() const noexcept(true) { return vertexs_; };
	template<>
	inline const Normal3D* const Ptr<Normal3D>()const noexcept(true) { return normals_; };
	template<>
	inline const Polygon3D* const Ptr<Polygon3D>()const noexcept(true) { return polygons_; };
	template<>
	inline const RgbColor* const Ptr<RgbColor>()const noexcept(true) { return rgb_colors_; };

};

class HostData final{
private:

	ArrayOf<Vertex3D> vertexs_;
	ArrayOf<Normal3D> normals_;
	ArrayOf<Polygon3D> polygons_;
	ArrayOf<RgbColor> rgb_colors_;
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
	inline const ArrayOf<PrimitiveType>& Data()const noexcept(true) { return nullptr; };
	template<>
	inline const ArrayOf<Vertex3D>& Data<Vertex3D>()const noexcept(true) { return vertexs_; };
	template<>
	inline const ArrayOf<Normal3D>& Data<Normal3D>()const noexcept(true) { return normals_; };
	template<>
	inline const ArrayOf<Polygon3D>& Data<Polygon3D>()const noexcept(true) { return polygons_; };
	template<>
	inline const ArrayOf<RgbColor>& Data<RgbColor>()const noexcept(true) { return rgb_colors_; };

	//
	template<class PrimitiveType>
	inline ArrayOf<PrimitiveType>& Data()noexcept(true) { return nullptr; };
	template<>
	inline ArrayOf<Vertex3D>& Data<Vertex3D>()noexcept(true) { return vertexs_; };
	template<>
	inline ArrayOf<Normal3D>& Data<Normal3D>()noexcept(true) { return normals_; };
	template<>
	inline ArrayOf<Polygon3D>& Data<Polygon3D>()noexcept(true) { return polygons_; };
	template<>
	inline ArrayOf<RgbColor>& Data<RgbColor>()noexcept(true) { return rgb_colors_; };

	//
	virtual ~HostData()noexcept(true) {}

};

using ModelId = size_t;

class Model final {
private:

	static const std::string kDefaultFileName;
	static const std::string kDefaultModelName;
	static ModelId last_id;

	std::string model_name_;
	std::string file_name_;

	HostData host_data_;
	DeviceData device_data_;

public:

	//Default constructor
	explicit Model()noexcept(true) :
		host_data_{},
		device_data_{}{
		
		++last_id;
	
	}

	explicit Model(HostData&& data)noexcept(true) {

		host_data_ = data;

	}

	Model& operator=(Model&& model)noexcept(true) {

		if (&model == this)
			return *this;

		Swap(model_name_, model.model_name_ );
		Swap(file_name_, model.file_name_);
		Swap(host_data_, model.host_data_);
		Swap(device_data_, model.device_data_);

		return *this;
	}

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



#endif //ENGINE_TYPES_HPP_