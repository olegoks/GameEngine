#include"__engine_1.1/Engine.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	Engine& engine = Engine::Instance();

	engine.CreateConsole("Log console");
	engine.InitCuda();
	engine.DefaultDir("D:\\C++\\ProjectEngine\\__obj_models\\");
	engine.LoadModels({"Parallel.obj", "ColorCube.obj"});
	engine.InitWindow(hInstance);
	engine.StartMainLoop();
	engine.ShowWindow();
	engine.Stop();

	return 0;
}