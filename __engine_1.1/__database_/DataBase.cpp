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

	Model model{ obj_file.DataMove() };
	models_.PushBack(std::move(model));

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

void DataBase::OutputModelsInfo()noexcept(true){

}

