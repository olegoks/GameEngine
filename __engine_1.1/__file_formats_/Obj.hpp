#pragma once
#ifndef _OBJ_H_
#define _OBJ_H_

#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <regex>
#include "..\engine_types.hpp"
#include <memory>

static const std::string kSpace{ " " };


// StrReg = string regular expression, no std::regex
static const std::string kFloatStrReg{ "([0-9]+.[0-9]+)" };
static const std::string kSize_tStrReg{ "([0-9]+)" };
static const std::string kHexRgbColorStrReg{ "([a-f0-9]{1,6})" };
static const std::string kRatioStrReg{ "(" + kSize_tStrReg + "//" + kSize_tStrReg + ")" };
static const std::string kVertexsStrReg{ "(v" + kSpace + kFloatStrReg + kSpace + kFloatStrReg + kSpace + kFloatStrReg + ")" };
static const std::string kNormalsStrReg{ "(vn" + kSpace + kFloatStrReg + kSpace + kFloatStrReg + kSpace + kFloatStrReg + ")" };
static const std::string kPolygonsStrReg{ "(f" + kSpace + kRatioStrReg + kSpace + kRatioStrReg + kSpace + kRatioStrReg + ")" };
static const std::string kRgbColorStrReg{ "(usemtl" + kSpace + kHexRgbColorStrReg + ")" };

static const std::regex kVertexsLineReg{ kVertexsStrReg };
static const std::regex kNormalsLineReg{ kNormalsStrReg };
static const std::regex kPolygonsLineReg{ kPolygonsStrReg };
static const std::regex kRgbColorLineReg{ kRgbColorStrReg };

static const char kRetCarr = 'r';
static const size_t kFirst = 0;

enum class Primitive:uint8_t {

	VERTEX,
	NORMAL,
	POLYGON,
	RGB_COLOR,
	RESERVED

};

class ObjFile final{

private:

	ArrayOf<Vertex3D> vertexs_;
	ArrayOf<Normal3D> normals_;
	ArrayOf<Polygon3D> polygons_;
	ArrayOf<RgbColor> rgb_colors_;
	size_t n_of_lines_;
	std::string file_name_;
	std::ifstream* file_object_;

	void ReserveMemory()noexcept(true);
	void CopyData();

	template<class PrimitiveType>
	PrimitiveType ProcessLine(const std::string& line)const noexcept(true);
	inline Primitive PrimitiveType(const std::string& line)const noexcept(true);
	void ReadFile();


public:

	class FileOpenException {
	public:

		int codeError;
		const char* fileName;

		FileOpenException(int openCodeError, const char* unopenedFileName):
			codeError(openCodeError),
			fileName(unopenedFileName) {
		
		}

	};

	class AllocationMemoryException {};
	class DoubleReadingFileException {
	public:
		const char* firstFileName;
		const char* secondFileName;

		DoubleReadingFileException(const char* firstFileName, const char* secondFileName) {

			this->firstFileName = firstFileName;
			this->secondFileName = secondFileName;

		}
	};
	
	explicit ObjFile(const std::string&& file_name)noexcept(true);
	~ObjFile();
	void Open()noexcept(true);

};

#endif

template<class ComponentType>
inline ComponentType ProcessElement(const std::string&& element_str)noexcept(true){

	std::stringstream convert{ element_str };
	ComponentType component;
	convert >> component;

	return component;
}

template<class PrimitiveType>
inline PrimitiveType ObjFile::ProcessLine(const std::string& line) const noexcept(true) {

	/*PrimitiveType result{};*/




	return PrimitiveType();
}

template<>
inline Ratio ProcessElement<Ratio>(const std::string&& element_str)noexcept(true) {

	std::stringstream convert{ element_str.substr(std::ios::beg, element_str.find('/')) };
	Ratio component;
	convert >> component.vertex_n;
	size_t lst_pos = element_str.rfind('/');
	convert << element_str.substr(lst_pos, element_str.length() - 1 - lst_pos);
	convert >> component.vertex_n;
	return component;

}

template<>
inline RgbColor ProcessElement<RgbColor>(const std::string&& element_str)noexcept(true) {

	
	return RgbColor{};

}

