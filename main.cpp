#include "__engine_1.1/Engine.hpp"

Engine& engine = Engine::Instance();

void ProcessKeyboard(KeyType key)noexcept(true) {

	const Vector3D rotate_vector{ 0.0f, 1.0f, 0.0f };
	const Vertex3D rotate_vertex{ 0.0f, 0.0f, 0.0f };

	//const Vertex3D translate_vertex_left(-1.0f, 0.0f, 0.0f);
	//const Vertex3D translate_vertex_right(1.0f, 0.0f, 0.0f);
	//const Vertex3D translate_vertex_down(0.0f, 0.0f, -1.0f);
	//const Vertex3D translate_vertex_up(0.0f, 0.0f, 1.0f);

	//const Vertex3D* const camera_position = engineGetCameraPosition(0);
	//const Vertex3D origin(0.0f, 0.0f, 0.0f);

	//const Vector3D dir_vector = engineCalculateVector(camera_position, &origin);

	//const Vertex3D delta_vertex_first = { dir_vector.x, dir_vector.y , dir_vector.z };
	//const Vertex3D delta_vertex_second = { -dir_vector.x, -dir_vector.y , -dir_vector.z };

	if (key == KeyType::D) {

		engine.RotateModel(0, 10.0f, rotate_vector, rotate_vertex);

	}

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	engine.CreateConsole("Log console");
	engine.InitCuda();
	engine.InitKeystrProcFunc(ProcessKeyboard);
	engine.DefaultDir("D:\\C++\\ProjectEngine\\__obj_models\\");
	engine.LoadModels( { "skeleton.obj" } );
	engine.InitWindow(hInstance);
	engine.StartMainLoop();
	engine.ShowWindow();
	engine.Stop();

	return 0;
}