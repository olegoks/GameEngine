#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <algorithm>
#include "__file_formats_/Obj.hpp"

using ObjFilesList = std::vector<std::string>;

class DataBase final{
private:

	//Default directory with object files(.obj)
	static const std::string kDefaultObjFilesDir;
	//All information about models components: vertexs, normals, polygons, rgb_colors
	ArrayOf<Model> models_;
	//All models information
	//List with included names of obj files, that will be include
	mutable ObjFilesList obj_files_list_;
	//Current obj files directory
	mutable std::string obj_files_dir_;
	//Load info from one object file
	void LoadObjFile(const std::string& file_name)noexcept(true);

public:

	//Default constructor
	explicit DataBase()noexcept(true);

	//Load ALL obj files, which names included in obj_files_list_
	void LoadObjFiles()noexcept(true);

	//Delete operators and constructors
	DataBase& operator=(DataBase&&)noexcept(true) = delete;
	DataBase& operator=(const DataBase&)noexcept(true) = delete;
	explicit DataBase(DataBase&&)noexcept(true) = delete;
	explicit DataBase(const DataBase&)noexcept(true) = delete;

	inline const Model& operator[](const size_t index)const noexcept(true) { return models_[index]; }
	inline Model& operator[](const size_t index)noexcept(true) { return models_[index]; }

	inline size_t ModelsNumber()const noexcept(true) { models_.Size(); };
	//Move all data from DataBase, after that operation DataBase will be empty

	//Set directory where will be find obj files
	inline void ObjFilesDir(const std::string& dir_path)const noexcept(true) {

		obj_files_dir_ = dir_path;

	};

	/*inline DataBase& operator+(ObjFile& obj_file)noexcept(true) {

		data_ = data_ + obj_file.Data();

	}*/

	//Include obj files name to obj_files_list
	void IncludeObjFileName(const std::initializer_list<std::string>& list)const noexcept(true);

	//virtual formal
	virtual ~DataBase() {}

	void OutputModelsInfo()noexcept(true);

};

#endif //_DATABASE_HPP_