
#include "__engine_1.1/__file_formats_/Obj.hpp"

int main(const size_t argc, char** argv) {

	/*ObjFile file{"D:\\C++\\ProjectEngine\\__obj_models\\ColorCube.obj" };
	file.Open();
	file.Read();
	file.Close();
	Vertex3D* const arr = file.Ptr<Vertex3D>();*/


	const std::regex reg_expr{ "([-]{0,1}[0-9]+.[0-9]+)" };
	std::smatch reg_result;
	std::stringstream convert;
	std::string str{ "-2.00 0.00 -2.00" };
	std::regex_search(str, reg_result, reg_expr);

	Vertex3D vertex;
	std::smatch::const_iterator it = reg_result.cbegin();


	vertex.x = ConvertStrTo<double>(*it);
	vertex.y = ConvertStrTo<double>(*(++it));
	vertex.z = ConvertStrTo<double>(*(++it));


	system("pause");
	return 0;
}