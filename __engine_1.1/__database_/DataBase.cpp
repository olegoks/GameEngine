#include "DataBase.hpp"

DataBase::DataBase()noexcept(true):
	data_{},
	obj_files_list_{}
{}

DataBase::~DataBase()noexcept(true){}

void DataBase::LoadObjFiles()noexcept(true){

	for (auto& obj_file_name : obj_files_list_)
		LoadObjFile(obj_file_name);

}

void DataBase::LoadObjFile(const std::string& file_name)noexcept(true) {

	ObjFile obj_file{ file_name };
	obj_file.Open();
	obj_file.Read();

	data_.Get<Vertex3D>().Concat(obj_file.Get<Vertex3D>());
	data_.Get<Normal3D>().Concat(obj_file.Get<Normal3D>());
	data_.Get<Polygon3D>().Concat(obj_file.Get<Polygon3D>());
	data_.Get<RgbColor>().Concat(obj_file.Get<RgbColor>());
	

	obj_file.Close();

	//data_.models.PushBack();

	//longSize newSizeOfArrayOfModels = this->data_info_.numberOfModels + 1;
	//ModelInfo3D* newArrayOfModels = (ModelInfo3D*) new ModelInfo3D[newSizeOfArrayOfModels];
	//memcpy(newArrayOfModels, this->data_info_.allModels, this->data_info_.numberOfModels * sizeof(ModelInfo3D) );
	//newArrayOfModels[newSizeOfArrayOfModels - 1].file_name = file_name;
	//newArrayOfModels[newSizeOfArrayOfModels - 1].numberOfNormals = obj_file.GetNumberOfNormals();
	//newArrayOfModels[newSizeOfArrayOfModels - 1].numberOfPolygons = obj_file.GetNumberOfPolygons();
	//newArrayOfModels[newSizeOfArrayOfModels - 1].numberOfVertexs = obj_file.GetNumberOfVertexs();
	//newArrayOfModels[newSizeOfArrayOfModels - 1].numberOfColors = obj_file.GetNumberOfRgbColors();

	//newArrayOfModels[newSizeOfArrayOfModels - 1].model_name = nullptr; //<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//newArrayOfModels[newSizeOfArrayOfModels - 1].vertexs_pointer = data_info_.allVertexs + data_info_.numberOfVertexs - obj_file.GetNumberOfVertexs();
	//newArrayOfModels[newSizeOfArrayOfModels - 1].normals_pointer = data_info_.allNormals + data_info_.numberOfNormals - obj_file.GetNumberOfNormals();
	//newArrayOfModels[newSizeOfArrayOfModels - 1].polygons_pointer = data_info_.allPolygons + data_info_.numberOfPolygons - obj_file.GetNumberOfPolygons();
	//newArrayOfModels[newSizeOfArrayOfModels - 1].colors_pointer = data_info_.allRgbColors + data_info_.numberOfRgbColors - obj_file.GetNumberOfRgbColors();
	//
	//delete[] this->data_info_.allModels;
	//this->data_info_.allModels = newArrayOfModels;
	//this->data_info_.numberOfModels++;


}


