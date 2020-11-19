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
const std::string ObjFile::kCompStrRegExpr<Polygon3D> = "(([0-9]+)//([0-9]+))";
template<>
const std::string ObjFile::kCompStrRegExpr<RgbColor> = "([0-9a-f]{2})";

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
const std::string ObjFile::kLineStrRegExpr<Polygon3D> { "(f[ ]?"+ kSpace + kStrRegExpr<Polygon3D> +")" };
template<>
const std::string ObjFile::kLineStrRegExpr<RgbColor> { "(usemtl" + kSpace + kStrRegExpr<RgbColor>+")" };


Primitive ObjFile::PrimitiveType(const std::string& line) const noexcept(true)
{

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

ObjFile::ObjFile(const std::string& file_name, const std::string& file_dir_path)noexcept(true):
	file_name_{ file_name },
	file_dir_path_{ file_dir_path },
	data_{},
	file_object_{ (std::ifstream*) new std::ifstream },
	n_of_lines_{ 0 }{

}

ObjFile::~ObjFile(){

	//...

}

void ObjFile::Open()noexcept(true) {
	
	file_object_->open(file_dir_path_ + file_name_, std::ios::in, std::ios::_Nocreate);

}

void ObjFile::ReserveMemory()noexcept(true){

	std::string line;
	size_t n_of_vertexs = 0, n_of_normals = 0, n_of_polygons = 0, n_of_rgb_colors = 0;
	n_of_lines_ = 0;

	file_object_->clear();
	file_object_->seekg(0, std::ios::beg);

	while (std::getline(*file_object_, line)) {

		n_of_lines_++; 

		LoadingImitation();

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

	data_.Data<Vertex3D>().Reserve(n_of_vertexs);
	data_.Data<Normal3D>().Reserve(n_of_normals);
	data_.Data<Polygon3D>().Reserve(n_of_polygons);
	data_.Data<RgbColor>().Reserve(n_of_rgb_colors);
	//...
}

void ObjFile::Read() noexcept(true) {

	//Counting file size
	file_object_->clear();
	file_object_->seekg(0, std::ios_base::end);
	const long long size_bytes = file_object_->tellg();
	const long long size_mbytes = ceil(static_cast<float>(size_bytes) / 1024.0f);

	Output("\nLoading file: " + file_name_ + "  " + std::to_string(size_mbytes) + "KB");
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

		//In function ProcessLine line can be changed! 
		//Not-const reference parametr.

		switch (primitive) {
		case Primitive::VERTEX: {

			const Vertex3D vertex = ProcessLine<Vertex3D>(line);
			data_.Data<Vertex3D>().PushBack(vertex);

			break;
		}
		case Primitive::NORMAL: {

			const Normal3D normal = ProcessLine<Normal3D>(line);
			data_.Data<Normal3D>().PushBack(normal);

			break;
		}
		case Primitive::POLYGON: {

			const Polygon3D polygon = ProcessLine<Polygon3D>(line);
			data_.Data<Polygon3D>().PushBack(polygon);

			break;
		}
		case Primitive::RGB_COLOR: {

			const RgbColor rgb_color = ProcessLine<RgbColor>(line);
			data_.Data<RgbColor>().PushBack(rgb_color);

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

void ObjFile::LoadingImitation()const noexcept(true) {

	using namespace std::chrono;
	static const size_t counter_border = 4;
	static size_t counter = 0;
	static high_resolution_clock::time_point last_paint_time = high_resolution_clock::now();
	static const milliseconds interval_millisec = milliseconds{ 400 };

	milliseconds interval = duration_cast<milliseconds>(high_resolution_clock::now() - last_paint_time);

	if (interval > interval_millisec) {

		if (counter < counter_border) {

			if(counter != 0)
				Output(".");

		}
		else {

			counter = 0;
			Output(kRetCarr + "   ");
			Output(kRetCarr);

		}

		last_paint_time = high_resolution_clock::now();

		++counter;

	}
}
//Magic number 3
//comp == points:{x, y, z}
//->3 comp in vertex
//->3 comp in normal
//->3 comp in polygon
//->3 comp in rgb_color R G B

template<>
Vertex3D ObjFile::ProcessPrimitive<Vertex3D>(std::string& primitive_line)const noexcept(true) {

	const std::regex reg_expr{ kCompStrRegExpr<Vertex3D> };
	std::smatch reg_result;
	std::stringstream convert;
	Vertex3D vertex;

	for (size_t i = 0; i < 3; i++){

		std::regex_search(primitive_line, reg_result, reg_expr);
		vertex.xyz[i] = ConvertStrTo<double>(reg_result.str());
		primitive_line = reg_result.suffix();

	}

	return vertex;
}

template<>
Normal3D ObjFile::ProcessPrimitive<Normal3D>(std::string& primitive_line)const noexcept(true) {

	const std::regex reg_expr{ kCompStrRegExpr<Vertex3D> };
	std::smatch reg_result;
	std::stringstream convert;
	Normal3D normal;

	for (size_t i = 0; i < 3; i++) {

		std::regex_search(primitive_line, reg_result, reg_expr);
		normal.xyz[i] = ConvertStrTo<double>(reg_result.str());
		primitive_line = reg_result.suffix();

	}

	return normal;
}

template<>
Polygon3D ObjFile::ProcessPrimitive<Polygon3D>(std::string& primitive_str)const noexcept(true) {

	const std::regex reg_expr{ kCompStrRegExpr<Polygon3D> };
	std::smatch reg_result;
	std::stringstream convert;
	Polygon3D polygon;
	
	for (size_t i = 0; i < 3; i++) {

		std::regex_search(primitive_str, reg_result, reg_expr);
		std::string size_t_str = reg_result.str();

		//Process "uint//uint"
		size_t pos = size_t_str.find("/");
		polygon.ratios[i].vertex_n = ConvertStrTo<size_t>(size_t_str.substr(0, pos));
		pos = size_t_str.rfind("/");
		polygon.ratios[i].normal_n = ConvertStrTo<size_t>(size_t_str.substr(pos, size_t_str.size() - pos));

		primitive_str = reg_result.suffix();
	}

	return polygon;
}

template<>
RgbColor ObjFile::ProcessPrimitive<RgbColor>(std::string& str)const noexcept(true) {

	RgbColor rgb_color;
	std::smatch reg_result;
	std::regex reg_expr{ kCompStrRegExpr<RgbColor> };
	std::stringstream convert;
	
	for (size_t i = 0; i < 3; i++)	{

		std::regex_search(str, reg_result, reg_expr);
		std::string hex_str{ reg_result.str() };
		
		int hex_int;
		convert << hex_str;
		convert >> std::hex >> hex_int;
		convert.clear();
		rgb_color.bgra[2 - i] = static_cast<uint8_t>(hex_int);
		str = reg_result.suffix();

	}

	rgb_color.rgba_alpha = 0;
	
	return rgb_color;
}

