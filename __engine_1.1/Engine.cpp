#include "Engine.hpp"

//Engine static fields
EngineDestroyer Engine::destroyer{};
Engine* Engine::self_ = nullptr;

//Default directory with object files(.obj)
const std::string Engine::kDefaultDirectory = "";

using namespace std::literals;
const std::chrono::high_resolution_clock::duration Engine::kTimeToUpdateLogic = 10ms;
const size_t Engine::kDefaultFps = 30;
const std::chrono::milliseconds Engine::ms_to_frame = std::chrono::milliseconds(1000 / kDefaultFps);
const StartPositionFunc Engine::kDefaultStartPositionFunc = []() {};

//Default costructor
Engine::Engine()noexcept(true):
	main_window_{},
	data_base_{},
	options_{

		kTimeToUpdateLogic,
		ms_to_frame,
		"__obj_models_\\"

	},
	//Initialize memory manager
	thread_{},
	log_console_{ nullptr },
	loop_is_running_{ false },
	frame_{ nullptr },
	graphic_engine_{  },
	start_pos_func_{ kDefaultStartPositionFunc }{

	data_base_.ObjFilesDir(kDefaultDirectory);

}

//Get engine reference
Engine& Engine::Instance()noexcept(false) {

	if (self_ == nullptr) {

		try {

			self_ = (Engine*) new Engine;
			destroyer.PlugEngine(self_);

		}
		catch (std::bad_alloc) {

			throw EngineException{ "Engine allocation exception. std::bad_alloc exception." };

		}

	}

	return *self_;

}

void Engine::InitCuda() const noexcept(false){

	//...

}
//Path format: "D:\\C++\\ProjectEngine\\__obj_models\\"
void Engine::DefaultDir(const std::string& dir_path) const noexcept(true){

	options_.dir_path_ = dir_path;
	data_base_.ObjFilesDir(options_.dir_path_);

}

void Engine::LoadModels(const std::initializer_list<std::string>& list) noexcept(true){

	data_base_.IncludeObjFileName(list);

}

void Engine::CreateConsole(const std::string& console_name) const noexcept(true){

	if (log_console_ == nullptr) {

		const std::wstring wconsole_name = ConvertStringToWstring(console_name);
		log_console_ = new Console{ wconsole_name };
		
	}

}

void Engine::InitWindow(const HINSTANCE hInstance) const noexcept(true){

	main_window_.PlugHInstance(hInstance);

}

void Engine::InitKeystrProcFunc(const KeystrProcFunc& func) noexcept(true){

	logic_engine_.PlugKeystrProcFunction(func);

}

void Engine::InitLogicFunc(const LogicFunc& func) noexcept(true){

	logic_engine_.PlugLogicFunction(func);

}

void Engine::InitWindowSize(const size_t width, const size_t height) const noexcept(true){


	//...

}

void Engine::StartMainLoop()noexcept(true){

	main_window_.RegisterWindowClass();
	main_window_.Create();

	data_base_.LoadObjFiles();
	LoadModelsToGpu();

	graphic_engine_.PlugCamera(&camera_);
	graphic_engine_.PlugDeviceData(data_base_.GetDeviceData());

	logic_engine_.PlugModels(&data_base_.Models());
	logic_engine_.PlugKeystrokesQueue(main_window_.KeystrokesQueue());

	start_pos_func_();
	
	//Start second thread for engine main loop
	thread_ = std::thread{ &Engine::StartLoop, this };

}

void Engine::ShowWindow() noexcept(true){
	
	main_window_.Show();
	main_window_.StartMessageLoop();
	loop_is_running_ = false;
	thread_.join();

}

void Engine::Stop() noexcept(true){

	//...

}

void Engine::StartLoop()noexcept(true){

	using namespace std::chrono;
	using duration = high_resolution_clock::duration;
	using time_point = high_resolution_clock::time_point;

	time_point cycle_beg, cycle_end, render_and_update_end;
	duration elapsed, cycle_duration, render_and_update_duration;

	//lag accumulates cycle durations
	duration lag = duration::zero();
	time_point previous = high_resolution_clock::now();

	loop_is_running_ = true;

	while (loop_is_running_) {

		cycle_beg = high_resolution_clock::now();
		elapsed = cycle_beg - previous;
		previous = cycle_beg;
		lag += elapsed;
		
		//off
		ProcessInput();

		while (lag >= options_.time_to_update_logic) {

		//off
			UpdateLogic();
			lag -= options_.time_to_update_logic;

		}

		Render();
		ShowFrame();

		render_and_update_end = high_resolution_clock::now();
		render_and_update_duration = render_and_update_end - cycle_beg;

		if (render_and_update_duration < options_.fps_limit)
		std::this_thread::sleep_for(options_.fps_limit - render_and_update_duration);

	}

}

void Engine::ProcessInput() const noexcept(true){

	

}

void Engine::UpdateLogic()noexcept(true){

	logic_engine_.UpdateWorldLogic();

}

void Engine::Render()noexcept(true){

	frame_ = &graphic_engine_.RenderMeshFrame();
	
}

void Engine::ShowFrame()noexcept(true){

	main_window_.ShowFrame(*frame_);

}


void Engine::LoadModelsToGpu()noexcept(true){

	AllocateMemory();
	CopyModels();

}

void Engine::AllocateMemory()noexcept(true){

	data_base_.AllocateGpuMemoryForModels();

}

void Engine::CopyModels()noexcept(true){

	data_base_.CopyModelsToGpu();

}


void Engine::RotateModel(ModelId model_id, const float alpha_degree, const Vector3D& around_vector, const Vertex3D& around_point)noexcept(true) {

	logic_engine_.RotateModel(model_id, alpha_degree, around_vector, around_point);
}

void Engine::TranslateModel(const ModelId model_id, const Vertex3D& translate_vertex)noexcept(true){

	logic_engine_.TranslateModel(model_id, translate_vertex);


}

void Engine::TranslateCamera(const CameraId camera_id, const Vertex3D& delta_vertex)noexcept(true) {

	camera_.Translate(delta_vertex);

}

void Engine::RotateCamera(const CameraId camera_id, const float alpha_degree, const Vector3D& around_vector, const Vertex3D& around_point)noexcept(true){

	camera_.Rotate(alpha_degree, around_vector, around_point);

}

void Engine::ScaleModel(const ModelId model_id, const float coefficient)noexcept(true) {

	logic_engine_.ScaleModel(model_id, coefficient);

}

const Vertex3D& Engine::CameraPosition(const ModelId camera_id) const noexcept(true){

	return camera_.GetPosition();

}
