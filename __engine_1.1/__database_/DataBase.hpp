#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "../__file_formats_/Obj.hpp"
#include "../engine_types.hpp"

using ObjFilesList = std::vector<std::string>;

class DataBase final{
private:

	Data data_;
	ObjFilesList obj_files_list_;

	void LoadObjFile(const std::string& file_name)noexcept(true);

public:

	explicit DataBase()noexcept(true);
	~DataBase()noexcept(true);
	void LoadObjFiles()noexcept(true);

	template<class PrimitiveType>
	inline const PrimitiveType* const GetData()const noexcept(true) { return nullptr; };

	inline void IncludeObjFile(const std::string& file_name)noexcept(true) {

		obj_files_list_.push_back(file_name);
		
	}

};

#endif _DATABASE_HPP_