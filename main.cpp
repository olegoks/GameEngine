
#include "__engine_1.1/engine_types.hpp"
#include <iostream>
#include "__engine_1.1/__file_formats_/Obj.hpp"

int main(const size_t argc, char** argv) {

	ObjFile file{"D:\\C++\\ProjectEngine\\__obj_models\\ColorCube.obj"};
	file.Open();
	return 0;
}