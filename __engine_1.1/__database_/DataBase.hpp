#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "__file_formats_/Obj.hpp"
#include "../engine_types.hpp"

using ObjFilesList = std::vector<std::string>;

class DataBase final{
private:

	static const std::string kDefaultObjFilesDir;
	DataHost data_;
	mutable ObjFilesList obj_files_list_;
	mutable std::string obj_files_dir_;
	void LoadObjFile(const std::string& file_name)noexcept(true);

public:

	explicit DataBase()noexcept(true);
	void LoadObjFiles()noexcept(true);

	DataBase& operator=(DataBase&&)noexcept(true) = delete;
	DataBase& operator=(const DataBase&)noexcept(true) = delete;
	explicit DataBase(DataBase&&)noexcept(true) = delete;
	explicit DataBase(const DataBase&)noexcept(true) = delete;

	inline const DataHost& CopyData()const noexcept(true){ return data_; };
	inline DataHost&& MoveData()noexcept(true){ return std::move(data_); };

	inline void ObjFilesDir(const std::string& dir_path)const noexcept(true) {

		obj_files_dir_ = dir_path;

	};

	inline void IncludeObjFileName(const std::initializer_list<std::string>& list)const noexcept(true){

		for (auto& obj : list)
			obj_files_list_.push_back(obj);
		
	}

	//virtual formal
	virtual ~DataBase() {}

};

#endif _DATABASE_HPP_