#include "__engine_1.1/Engine.hpp"

class Tree final {

private:

	ModelId id_;

public:



};

class Road final {

private:

	size_t n_parts_;
	size_t first_id_;

public:

	explicit Road(size_t first_id)noexcept(true):
		n_parts_{ 10 },
		first_id_{ first_id }{}

	void LoadRoad()const noexcept(true) {

		Engine& engine = Engine::Instance();

		/*for (size_t i = 0; i < n_parts_; i++)
			engine.LoadModels({"Doroga.obj"});*/

		float part_length = 4.0f;
		float offset = 0.0f;

		for (size_t i = first_id_; i < first_id_ + n_parts_ ; i++){

			engine.TranslateModel(i, Vertex3D{ 0.0, 0.0, -offset + part_length * 5 });
			offset += part_length;

		}

	}

};

void ProcessKeyboard(KeyType key)noexcept(true) {

	Engine& engine = Engine::Instance();

	const Vector3D rotate_vector{ 0.0f, 1.0f, 0.0f };
	const Vertex3D rotate_vertex{ 0.0f, 0.0f, 0.0f };

	switch(key){

	case KeyType::D:

		engine.TranslateModel(18, Vertex3D{0.5, 0.0, 0.0});
		break;
	case KeyType::A:

		engine.TranslateModel(18, Vertex3D{ -0.5, 0.0, 0.0 });
		break;

	}

}

void LogicFunction()noexcept(true) {

	Engine& engine = Engine::Instance();
	/*
	const Vector3D rotate_vector{ 0.0f, 1.0f, 0.0f };
	const Vertex3D rotate_vertex{ 0.0f, 0.0f, 0.0f };
	engine.RotateModel(0, 1.0f, rotate_vector, rotate_vertex);
	
	static float sign = -0.05;

	Vertex3D camera_pos = engine.CameraPosition(0);
	
	if (camera_pos.z < -25.0 || camera_pos.z > -5.0) {

		sign = -sign;

	}

	camera_pos.z += static_cast<float>(sign);*/
	//engine.CameraPosition(0, camera_pos);

	//Vertex3D offset_vertex;
	//
	//static size_t counter = 0;
	//
	//++counter;

	//if (counter == 1000) {

	//	offset_vertex = Vertex3D{ 0.0, 0.0, 20.0f };
	//	counter = 0;
	//}
	//else
	//	offset_vertex = Vertex3D{ 0.0, 0.0, -0.02f };

	//for (int i = 0; i < 14; ++i) {

	//	engine.TranslateModel(i, offset_vertex);

	//}


	const Vertex3D beg_offset{0.0, 0.0, +40.0 };
	const Vertex3D& camera_pos = engine.CameraPosition(0);

	for (size_t i = 0; i < 18; i++){

		engine.TranslateModel(i, Vertex3D{ 0.0, 0.0, -0.02 });

		const Vertex3D& model_pos = engine.ModelPosition(i);
		if (model_pos.z < camera_pos.z - 10.0) {

			engine.TranslateModel(i, beg_offset);
		
		}

	}

}

void StartPosFunc()noexcept(true) {

	Engine& engine = Engine::Instance();

	engine.CameraPosition(0, Vertex3D{0.0, 0.0, -20.0});
	engine.RotateCamera(0, 55.0f, Vector3D{ 1.0, 0.0, 0.0 }, Vertex3D{ 0.0, 0.0, 0.0 });
	engine.TranslateModel(0, Vertex3D{ -10.0, 0.0, 4.0 });
	engine.TranslateModel(1, Vertex3D{ 10.0, 0.0, 8.0 });
	engine.TranslateModel(2, Vertex3D{ -8.0, 0.0, -6.0 });
	engine.TranslateModel(3, Vertex3D{ 5.0, 0.0, 15.0 });

	engine.TranslateModel(4, Vertex3D{ -9.0, 0.0, 32.0 });
	engine.TranslateModel(5, Vertex3D{ 7.0, 0.0, 37.0 });
	engine.TranslateModel(6, Vertex3D{ -5.0, 0.0, -22.0 });
	engine.TranslateModel(7, Vertex3D{ 11.0, 0.0, 20.0 });
	engine.TranslateModel(18, Vertex3D{0.0, 0.0, -10.0});
	Road road{ 8 };
	road.LoadRoad();


}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	Engine& engine = Engine::Instance();

	engine.CreateConsole("Log console");
	engine.InitCuda();
	engine.InitKeystrProcFunc(ProcessKeyboard);
	engine.InitLogicFunc(LogicFunction);
	engine.InitStartPosFunc(StartPosFunc);
	engine.DefaultDir("D:\\C++\\ProjectEngine\\__obj_models\\");
	engine.LoadModels( { "ColorTree.obj", "ColorTree.obj" , "ColorTree.obj", "ColorTree.obj", "ColorTree.obj", "ColorTree.obj" , "ColorTree.obj", "ColorTree.obj","DorogaNew.obj", "DorogaNew.obj" , "DorogaNew.obj" , "DorogaNew.obj" , "DorogaNew.obj" , "DorogaNew.obj", "DorogaNew.obj" , "DorogaNew.obj" , "DorogaNew.obj" , "DorogaNew.obj", "NewCar.obj"} );
	engine.InitWindow(hInstance);
	engine.StartMainLoop();
	engine.ShowWindow();
	engine.Stop();

	return 0;
}