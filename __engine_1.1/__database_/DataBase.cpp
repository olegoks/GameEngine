#include "DataBase.hpp"

//Constants
const std::string DataBase::kDefaultObjFilesDir = "";

DataBase::DataBase()noexcept(true):
	models_{},
	obj_files_list_{},
	obj_files_dir_{ kDefaultObjFilesDir }{}

void DataBase::LoadObjFiles()noexcept(true){

	//FIX
	std::cout << "Directory with object file: " << obj_files_dir_ << std::endl;

	for (auto& obj_file_name : obj_files_list_)
		LoadObjFile(obj_file_name);

}

void DataBase::LoadObjFile(const std::string& file_name)noexcept(true) {

	ObjFile obj_file{ file_name, obj_files_dir_ };

	obj_file.Open();
	obj_file.Read();

	Model model{ file_name };
	model.SetHostData(obj_file.DataMove());
	models_.PushBack( std::move(model) );

	obj_file.Close();

}

void DataBase::IncludeObjFileName(const std::initializer_list<std::string>& list)const noexcept(true) {

	for (auto& obj : list) {

		//Twice including test
		auto name_it = std::find(obj_files_list_.cbegin(), obj_files_list_.cend(), obj);
		bool list_contains_name_yet = !(name_it == obj_files_list_.cend());

		if (!list_contains_name_yet)
			obj_files_list_.push_back(obj);

	}

}

void DataBase::AllocateGpuMemoryForModels() noexcept(true){

	NPrimitives n_model_primitives{ 0 };

	for (size_t i = 0; i < models_.Size(); i++) {

		const Model& model = models_[i];
		n_model_primitives = n_model_primitives + model.PrimitivesNumber();

	}

	common_gpu_memory_.Reserve(n_model_primitives);

	DevicePtrs ptrs = common_gpu_memory_.Ptrs();

	for (size_t i = 0; i < models_.Size(); i++) {

		Model& model = models_[i];
		DeviceData device_data{ ptrs, n_model_primitives };
		model.SetDeviceData(std::move(device_data));
		ptrs = ptrs + model.PrimitivesNumber();
	}

}

void DataBase::CopyModelToGpu(Model& model)noexcept(true) {
	
	//????

	model.device_data_.vertexs_.Copy(model.host_data_.Data<Vertex3D>());
	model.device_data_.normals_.Copy(model.host_data_.Data<Normal3D>());
	model.device_data_.polygons_.Copy(model.host_data_.Data<Polygon3D>());
	model.device_data_.rgb_colors_.Copy(model.host_data_.Data<RgbColor>());

}

void DataBase::CopyModelsToGpu() noexcept(true){

	DevicePtrs ptrs;
	ptrs = common_gpu_memory_.Ptrs();

	for (size_t model_index = 0; model_index < models_.Size(); model_index++) {

		Model& model = models_[model_index];
		CopyModelToGpu(model);

	}

}

void DataBase::OutputModelsInfo()noexcept(true){}



