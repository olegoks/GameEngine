#include "Obj.hpp"
#pragma warning(disable : 6386)

const std::string ObjFile::kSpace{ " " };
const std::string ObjFile::kRetCarr{ "\r" };
const size_t ObjFile::kFirst = 0;
const size_t ObjFile::kHexColorStrLen = 6;

//Comp == component
//StrRegExpr == string of regular expression

//Primitive component StrReg
template<>
const std::string ObjFile::kCompStrRegExpr<Vertex3D> = "([-]{0,1}[0-9]+.[0-9]+)";
template<>
const std::string ObjFile::kCompStrRegExpr<Normal3D> = kCompStrRegExpr<Vertex3D>;
template<>
const std::string ObjFile::kCompStrRegExpr<Polygon3D> = "(([0-9] + )//([0-9] + ))";
template<>
const std::string ObjFile::kCompStrRegExpr<RgbColor> = "([0-9a-z]{2})";

//Primitive StrReg
template<>
const std::string ObjFile::kStrRegExpr<Vertex3D>{ kCompStrRegExpr<Vertex3D> +kSpace + kCompStrRegExpr<Vertex3D> +kSpace + kCompStrRegExpr<Vertex3D> };
template<>
const std::string ObjFile::kStrRegExpr<Normal3D>{ kCompStrRegExpr<Normal3D> +kSpace + kCompStrRegExpr<Normal3D> +kSpace + kCompStrRegExpr<Normal3D>};
template<>
const std::string ObjFile::kStrRegExpr<Polygon3D>{ kCompStrRegExpr<Polygon3D> +kSpace + kCompStrRegExpr<Polygon3D> +kSpace + kCompStrRegExpr<Polygon3D>};
template<>
const std::string ObjFile::kStrRegExpr<RgbColor>{ kCompStrRegExpr<RgbColor> +kCompStrRegExpr<RgbColor> +kCompStrRegExpr<RgbColor>};

//Primitive line regular expression
template<>
const std::string ObjFile::kLineStrRegExpr<Vertex3D> { "(v" + kSpace + kStrRegExpr<Vertex3D> +")" };
template<>
const std::string ObjFile::kLineStrRegExpr<Normal3D> { "(vn" + kSpace + kStrRegExpr<Normal3D> +")" };
template<>
const std::string ObjFile::kLineStrRegExpr<Polygon3D> { "(f" + kSpace + kStrRegExpr<Polygon3D> +")" };
template<>
const std::string ObjFile::kLineStrRegExpr<RgbColor> { "(usemtl" + kSpace + kStrRegExpr<RgbColor>+")" };


Primitive ObjFile::PrimitiveType(const std::string& line) const noexcept(true)
{
	std::string str_v{ kLineStrRegExpr<Vertex3D> };
	std::string str_n{ kLineStrRegExpr<Normal3D> };
	std::string str_p{ kLineStrRegExpr<Polygon3D> };
	std::string str_r{ kLineStrRegExpr<RgbColor> };

	if (std::regex_match(line, std::regex{ kLineStrRegExpr<Vertex3D> }))
		return Primitive::VERTEX;
	else
		if (std::regex_match(line, std::regex{ kLineStrRegExpr<Normal3D> }))
			return Primitive::NORMAL;
		else
			if (std::regex_match(line, std::regex{ kLineStrRegExpr<Polygon3D> }))
				return Primitive::POLYGON;
			else
				if (std::regex_match(line, std::regex{ kLineStrRegExpr<RgbColor> }))
					return Primitive::RGB_COLOR;
				else
					// ...
					return Primitive::RESERVED_0;

}


ObjFile::ObjFile(const std::string&& file_name)noexcept(true):
	file_name_{ file_name },
	vertexs_{ nullptr, 0 },
	normals_{ nullptr, 0 },
	polygons_{ nullptr, 0 },
	rgb_colors_{ nullptr, 0 },
	file_object_{ (std::ifstream*) new std::ifstream },
	n_of_lines_{ 0 }{

	
	

}

ObjFile::~ObjFile(){

	//...

}

void ObjFile::Open()noexcept(true) {
	
	file_object_->open(file_name_, std::ios::in, std::ios::_Nocreate);

}

void ObjFile::ReserveMemory()noexcept(true){

	std::string line;
	size_t n_of_vertexs = 0, n_of_normals = 0, n_of_polygons = 0, n_of_rgb_colors = 0;
	n_of_lines_ = 0;

	while (std::getline(*file_object_, line)) {

		n_of_lines_++; 
		Output(std::string{ std::to_string(n_of_lines_) + kRetCarr });

		switch (PrimitiveType(line)) {

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

		//...

		}

	}

	vertexs_.Reserve(n_of_vertexs);
	normals_.Reserve(n_of_normals);
	polygons_.Reserve(n_of_polygons);
	rgb_colors_.Reserve(n_of_rgb_colors);

}

void ObjFile::Read() noexcept(true) {

	Output("Loading file: " + file_name_);
	Output("\n");

	ReserveMemory();

	std::string line;
	file_object_->clear();
	file_object_->seekg(0, std::ios::beg);
	size_t n_of_read_lines = 0;

	while (std::getline(*file_object_, line)) {

		n_of_read_lines++;
		
		const size_t load_progress = round( ( static_cast<float>(n_of_read_lines) / static_cast<float>(n_of_lines_) ) * static_cast<float>(100) );
		
		Output(kRetCarr + std::to_string(load_progress) + "%");

		const Primitive primitive = PrimitiveType(line);

		switch (primitive) {

		case Primitive::VERTEX: {

			const Vertex3D vertex = ProcessLine<Vertex3D>(line);
			vertexs_.PushBack(vertex);
			break;
		}
		case Primitive::NORMAL: {

			const Normal3D normal = ProcessLine<Normal3D>(line);
			normals_.PushBack(normal);
			break;
		}
		case Primitive::POLYGON: {

			const Polygon3D polygon = ProcessLine<Polygon3D>(line);
			polygons_.PushBack(polygon);
			break;
		}
		case Primitive::RGB_COLOR: {

			const RgbColor rgb_color = ProcessLine<RgbColor>(line);
			rgb_colors_.PushBack(rgb_color);
			break;
		}

			//...

		}

	}

}

void ObjFile::Close()noexcept(true){

	file_object_->close();
	delete file_object_;

}

void ObjFile::Output(const std::string& output) const noexcept(true) {

	std::cout << output;

};

template<class ResultType>
ResultType ConvertStrTo(const std::string& str)noexcept(true) {

	std::stringstream convert{ str };
	ResultType result;
	convert >> result;

	return result;

}


template<>
Vertex3D ObjFile::ProcessPrimitive<Vertex3D>(const std::string& primitive_line)const noexcept(true) {

	const std::regex reg_expr{ kCompStrRegExpr<Vertex3D> };
	std::smatch reg_result;
	std::stringstream convert;
	std::regex_search(primitive_line, reg_result, reg_expr);

	Vertex3D vertex;

	vertex.x = ConvertStrTo<double>(reg_result[0]);
	vertex.y = ConvertStrTo<double>(reg_result[1]);
	vertex.z = ConvertStrTo<double>(reg_result[2]);

	return vertex;
}

template<>
Normal3D ObjFile::ProcessPrimitive<Normal3D>(const std::string& primitive_line)const noexcept(true) {

	const std::regex reg_expr{ kCompStrRegExpr<Vertex3D> };
	std::smatch reg_result;
	std::stringstream convert;
	std::regex_search(primitive_line, reg_result, reg_expr);

	Normal3D normal;

	normal.x = ConvertStrTo<double>(reg_result[0]);
	normal.y = ConvertStrTo<double>(reg_result[1]);
	normal.z = ConvertStrTo<double>(reg_result[2]);

	return normal;
}

template<>
Polygon3D ObjFile::ProcessPrimitive<Polygon3D>(const std::string& primitive_str)const noexcept(true) {

	const std::regex reg_expr{ kCompStrRegExpr<Polygon3D> };
	std::smatch reg_result;
	std::stringstream convert;
	std::regex_search(primitive_str, reg_result, reg_expr);

	std::smatch result;
	std::regex size_t_regex{ kCompStrRegExpr<RgbColor> };
	Polygon3D polygon;
	std::string size_t_str;

	for (size_t i = 0; i < 3; i++) {

		size_t_str = reg_result[i].str();
		std::regex_search(size_t_str, result, size_t_regex);
		polygon.ratios[i].vertex_n = ConvertStrTo<size_t>(result[0]);
		polygon.ratios[i].normal_n = ConvertStrTo<size_t>(result[1]);

	}

	return polygon;
}

