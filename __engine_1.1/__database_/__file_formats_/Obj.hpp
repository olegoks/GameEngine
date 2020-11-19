#pragma once
#ifndef _OBJ_HPP_
#define _OBJ_HPP_

#include "../../engine_types.hpp"

#include <string>
template<class ResultType>
ResultType ConvertStrTo(const std::string& str)noexcept(true);

#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <regex>
#include <memory>
#include <map>
#include <chrono>

class ObjFile final{
private:

	DataHost data_;

	//Number of lines in .obj file. Need for counting % loading.
	size_t n_of_lines_;
	std::string file_name_;
	std::string file_dir_path_;
	std::ifstream* file_object_;

	template<class PrimitiveType>
	PrimitiveType ProcessLine(const std::string& line)const noexcept(true);
	template<class PrimitiveType>
	PrimitiveType ProcessPrimitive(std::string& primitive_line)const noexcept(true);
	inline Primitive PrimitiveType(const std::string& line)const noexcept(true);
	void LoadingImitation()const noexcept(true);

protected:

	void ReserveMemory()noexcept(true);
	void Output(const std::string& output)const noexcept(true);

	//String constants for creation regular expressions
	static const std::string kSpace;
	static const std::string kRetCarr;
	static const size_t kFirst;
	static const size_t kHexColorStrLen;
	template<class PrimitiveType>
	static const std::string kCompStrRegExpr;
	template<class PrimtiveType>
	static const std::string kStrRegExpr;


public:
	template<class PrimtiveType>
	static const std::string kLineStrRegExpr;

	explicit ObjFile(const std::string& file_name, const std::string& file_dir_path)noexcept(true);
	~ObjFile();

	void Open()noexcept(true);
	void Read()noexcept(true);
	void Close()noexcept(true);

	template<class PrimitiveType>
	inline ArrayOf<PrimitiveType>& Data()noexcept(true) { return ArrayOf<PrimitiveType>{}; };

	
};
#endif // _OBJFILE_HPP_

//
template<>
inline ArrayOf<Vertex3D>& ObjFile::Data<Vertex3D>()noexcept(true) { return data_.Data<Vertex3D>(); };
template<>
inline ArrayOf<Normal3D>& ObjFile::Data<Normal3D>()noexcept(true) { return data_.Data<Normal3D>(); };
template<>
inline ArrayOf<Polygon3D>& ObjFile::Data<Polygon3D>()noexcept(true) { return data_.Data<Polygon3D>(); };
template<>
inline ArrayOf<RgbColor>& ObjFile::Data<RgbColor>()noexcept(true) { return data_.Data<RgbColor>(); };

//Primitive component StrReg
template<class PrimitiveType>
const std::string ObjFile::kCompStrRegExpr{ "(.*)" };

//Primitive StrReg
template<class PrimtiveType>
const std::string ObjFile::kStrRegExpr{ "(.*)" };

//Primitive line regular expression
template<class PrimtiveType>
const std::string ObjFile::kLineStrRegExpr{ "(.*)" };

template<class PrimitiveType>
PrimitiveType ObjFile::ProcessLine(const std::string& line) const noexcept(true) {

	const std::regex reg_expr{ kStrRegExpr<PrimitiveType> };
	std::smatch result;
	std::regex_search(line, result, reg_expr);
	std::string result_str = result.str();

	return ProcessPrimitive<PrimitiveType>(result_str);
}

template <class PrimitiveType>
PrimitiveType ObjFile::ProcessPrimitive(std::string& primitive_line)const noexcept(true) {

	return PrimitiveType();
}
