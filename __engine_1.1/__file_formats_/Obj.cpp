#include "Obj.hpp"
#pragma warning(disable : 6386)

ObjFile::ObjFile(const std::string&& file_name)noexcept(true):
	file_name_{ file_name },
	vertexs_{ nullptr, 0 },
	normals_{ nullptr, 0 },
	polygons_{ nullptr, 0 },
	rgb_colors_{ nullptr, 0 },
	file_object_{ (std::ifstream*) new std::ifstream }{

	
	file_object_->open(file_name_, std::ios::in, std::ios::_Nocreate);

}

ObjFile::~ObjFile()
{

	file_object_->close();

}

void ObjFile::Open()noexcept(true) {
	
	ReserveMemory();

}
inline Primitive ObjFile::PrimitiveType(const std::string& line) const noexcept(true)
{
	if (std::regex_match(line, kVertexsLineReg))
		return Primitive::VERTEX;
	else
		if (std::regex_match(line, kNormalsLineReg))
			return Primitive::NORMAL;
		else
			if (std::regex_match(line, kPolygonsLineReg))
				return Primitive::POLYGON;
			else
				if (std::regex_match(line, kRgbColorLineReg))
					return Primitive::RGB_COLOR;
				else
					return Primitive::RESERVED;

};

void ObjFile::ReadFile() {



}

void ObjFile::ReserveMemory()noexcept(true){

	std::string line;
	size_t n_of_vertexs = 0, n_of_normals = 0, n_of_polygons = 0, n_of_rgb_colors = 0;

	n_of_lines_ = 0;

	while (getline(*file_object_, line)) {

		n_of_lines_++; 

		const Primitive primitive = PrimitiveType(line);

		switch (primitive) {

		case Primitive::VERTEX:
			n_of_vertexs++;
			break;
		case Primitive::NORMAL:
			n_of_normals++;
			break;
		case Primitive::POLYGON:
			n_of_polygons++;
			break;
		case Primitive::RGB_COLOR:
			n_of_rgb_colors++;
			break;
		}
		
	}
	
	vertexs_.Reserve(n_of_vertexs);
	normals_.Reserve(n_of_normals);
	polygons_.Reserve(n_of_polygons);
	rgb_colors_.Reserve(n_of_rgb_colors);

}

void ObjFile::CopyData() {

	std::string line;
	std::string designation;
	file_object_->clear();
	file_object_->seekg(std::ios::beg);

	unsigned int n_of_current_line = 0;
	
	std::cout << "Loading file : " << file_name_ << std::endl;

	while (std::getline(*file_object_, line)) {

		n_of_current_line++;
		std::cout.setf(std::ios::right);
		const unsigned int load_progress = round(static_cast<float>(n_of_current_line) / (static_cast<float>(n_of_lines_) * 100));
		std::cout << kRetCarr << std::setw(5)  << load_progress << '%';
		designation = line.substr(std::ios::beg, 2);

		/*if (designation == kVertexs) { 

			std::unique_ptr< ArrayOf< Vertex3D > > vertexs;
			vertexs = ProcessVertexs(line); 
			vertexs_.Concat(vertexs->GetPtr(), vertexs->);

		}
		else
			if (designation == "vn") ProcessLineWithNormal(line);
			else
				if (designation == "f ") ProcessLineWithPolygon(line);
				else
					if (designation == "us") ProcessLineWithRgbColor(line);*/

	}

	std::cout << std::endl;

}
//std::unique_ptr<ArrayOf<Vertex3D>> ObjFile::ProcessVertexs(std::string& line)const noexcept(true) {
//
//	line.erase(0, 2);
//	line.push_back(' ');
//
//	std::stringstream stream;
//	size_t pos;
//	std::string str_float;
//
//	std::unique_ptr<ArrayOf<Vertex3D>> vertexs{ std::make_unique< ArrayOf<Vertex3D> >() };
//	vertexs->Reserve(1);
//	float xyz[3];
//
//	for (size_t i = 0; i < 3; i++){
//
//		pos = line.find(kSpace);
//
//		for (size_t j = 0; j < pos; j++)
//			str_float.push_back( line[i] );
//
//		line.erase(std::ios::beg, pos + 1);
//
//		stream << str_float;
//		stream >> xyz[i];
//
//	}
//
//	(*vertexs)[kFirst].x = xyz[0];
//	(*vertexs)[kFirst].y = xyz[1];
//	(*vertexs)[kFirst].z = xyz[2];
//	   
//	return std::move(vertexs);
//}
//
//void ObjFile::ProcessLineWithNormal(std::string& newLine)noexcept(true) {
//
//	newLine->erase(0, 3);
//	newLine->push_back(' ');
//
//	std::stringstream stream;
//
//	size_t pos;
//	std::string strFloat;
//	float xyz[3];
//
//	for (size_t j = 0; j < 3; j++)
//	{
//		pos = newLine->find(' ');
//
//		strFloat.clear();
//		for (size_t i = 0; i < pos; i++) {
//
//			strFloat += newLine->at(i);
//
//		}
//
//		newLine->erase(0, pos + 1);
//
//		stream << strFloat;
//		stream >> xyz[j];
//		stream.clear();
//
//	}
//
//	normals[currentNumberOfNormals].x = xyz[0];
//	normals[currentNumberOfNormals].y = xyz[1];
//	normals[currentNumberOfNormals].z = xyz[2];
//
//	currentNumberOfNormals++;
//
//}
//void ObjFile::ProcessLineWithPolygon(string* newLine) {
//	
//	newLine->erase(0, 2);
//	newLine->push_back(' ');
//
//	unsigned int pos;
//	std::string strInt;
//	std::stringstream stream;
//
//	for (size_t i = 0; i < 3; i++)
//	{
//		pos = newLine->find('/');
//
//		strInt.clear();
//		for (size_t j = 0; j < pos; j++)
//		{
//			strInt += newLine->at(j);
//		}
//
//		stream.clear();
//		stream << strInt;
//		stream >> this->polygons[currentNumberOfPolygons].ratios[i].vertexNumber;
//
//		newLine->erase(0, pos + 1);
//		pos = newLine->find('/');
//		newLine->erase(0, pos + 1);
//
//		pos = newLine->find(' ');
//
//		strInt.clear();
//		for (size_t j = 0; j < pos; j++)
//		{
//			strInt += newLine->at(j);
//		}
//
//
//		stream.clear();
//		stream << strInt;
//		stream >> polygons[currentNumberOfPolygons].ratios[i].normalNumber;
//
//		newLine->erase(0, pos + 1);
//	}
//
//	polygons[currentNumberOfPolygons].color = (RgbPixel)current_polygon_color_;
//	this->currentNumberOfPolygons++;
//
//}
//void ObjFile::ProcessLineWithRgbColor(string* newLine) {
//
//	newLine->erase(0, 7);
//	unsigned int length_of_the_line = newLine->size();
//
//	if (length_of_the_line <= 6) {
//
//		if (length_of_the_line < 6) {
//			for (size_t i = 0; i <  6 - length_of_the_line; i++) newLine->insert(0, "0");	
//		}
//
//		this->rgbColors[this->currentNumberOfRgbColors];
//		std::string strRGB;
//		unsigned int intTemp;
//		std::stringstream convertStream;
//
//		for (size_t i = 0; i < 6 - length_of_the_line; i++)
//		{
//			(*newLine) = "0" + (*newLine);
//		}
//
//		//R
//		strRGB = newLine->substr(0, 2);
//		convertStream << strRGB;
//		convertStream >> std::hex >> intTemp;
//		convertStream.clear();
//		//rgbColors[currentNumberOfRgbColors].rgb_red = intTemp;
//		current_polygon_color_.rgb_red = intTemp;
//		//G
//		strRGB = newLine->substr(2, 2);
//		convertStream << strRGB;
//		convertStream >> std::hex >> intTemp;
//		convertStream.clear();
//		//rgbColors[currentNumberOfRgbColors].rgb_green = intTemp;
//		current_polygon_color_.rgb_green = intTemp;
//		//B
//		strRGB = newLine->substr(4, 2);
//		convertStream << strRGB;
//		convertStream >> std::hex >> intTemp;
//		convertStream.clear();
//
//		//rgbColors[currentNumberOfRgbColors].rgb_blue = intTemp;
//		current_polygon_color_.rgb_blue= intTemp;
//		currentNumberOfRgbColors++;
//	}
//	else
//	{
//		
//		rgbColors[currentNumberOfRgbColors].rgb_red = 0;
//		rgbColors[currentNumberOfRgbColors].rgb_green = 0;
//		rgbColors[currentNumberOfRgbColors].rgb_blue = 0;
//
//	}
//}

