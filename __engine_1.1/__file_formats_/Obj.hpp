#pragma once
#ifndef _OBJ_H_
#define _OBJ_H_

#include "..\engine_types.hpp"

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

class ObjFile final{

private:

	ArrayOf<Vertex3D> vertexs_;
	ArrayOf<Normal3D> normals_;
	ArrayOf<Polygon3D> polygons_;
	ArrayOf<RgbColor> rgb_colors_;

	//Number of lines in .obj file. Need for counting % loading.
	size_t n_of_lines_;
	std::string file_name_;
	std::ifstream* file_object_;

	template<class PrimitiveType>
	PrimitiveType ProcessLine(const std::string& line)const noexcept(true);
	template<class PrimitiveType>
	PrimitiveType ProcessPrimitive(const std::string& primitive_line)const noexcept(true);
	inline Primitive PrimitiveType(const std::string& line)const noexcept(true);
	
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
	template<class PrimtiveType>
	static const std::string kLineStrRegExpr;

public:

	explicit ObjFile(const std::string&& file_name)noexcept(true);
	~ObjFile();

	void Open()noexcept(true);
	void Read()noexcept(true);
	void Close()noexcept(true);

	template<class PrimitiveType>
	inline PrimitiveType* const Ptr()noexcept(true) { return nullptr;};

};
#endif

//Specializations for template function Ptr()
template<>
inline Vertex3D* const ObjFile::Ptr<Vertex3D>()noexcept(true) { return vertexs_.Ptr(); };
template<>
inline Normal3D* const ObjFile::Ptr<Normal3D>()noexcept(true) { return normals_.Ptr(); };
template<>
inline Polygon3D* const ObjFile::Ptr<Polygon3D>()noexcept(true) { return polygons_.Ptr(); };
template<>
inline RgbColor* const ObjFile::Ptr<RgbColor>()noexcept(true) { return rgb_colors_.Ptr(); };

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
PrimitiveType ObjFile::ProcessPrimitive(const std::string& primitive_line)const noexcept(true) {

	return PrimitiveType();
}
