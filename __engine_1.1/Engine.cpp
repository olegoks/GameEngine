#include "Engine.hpp"

//Engine static fields
EngineDestroyer Engine::destroyer{};
Engine* Engine::self_ = nullptr;
const KeystrProcFunc Engine::kDefaultKeystrProcFunc = [](const Keystroke&){};
const LogicFunc Engine::kDefaultLogicFunc = [](){};

//Default directory with object files(.obj)
const std::string Engine::kDefaultDirectory = "";

//Default costructor
Engine::Engine()noexcept(true):
	main_window_{},
	data_base_{},
	//Initialize options struct
	log_console_{ nullptr },
	loop_is_running_{ false }{

	data_base_.ObjFilesDir(kDefaultDirectory);

}

std::wstring Engine::ConvertStringtToWstring(const std::string& str) const noexcept(true)
{

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	
	return converter.from_bytes(str);
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
	data_base_.LoadObjFiles();

}

void Engine::CreateConsole(const std::string& console_name) const noexcept(true){

	if (log_console_ == nullptr) {

		const std::wstring wconsole_name = ConvertStringtToWstring(console_name);
		log_console_ = new Console{ wconsole_name };
		
	}

}

void Engine::InitWindow(const HINSTANCE hInstance) const noexcept(true){

	main_window_.PlugHInstance(hInstance);

}

void Engine::InitKeystrProcFunc(const KeystrProcFunc& func) noexcept(true){

	keystr_proc_func_ = func;

}

void Engine::InitLogicFunc(const LogicFunc& func) noexcept(true){

	logic_func_ = func;

}

void Engine::InitWindowSize(const size_t width, const size_t height) const noexcept(true){

	//...

}

void Engine::StartMainLoop() noexcept(true){

	main_window_.RegisterWindowClass();
	main_window_.Create();

	LoadModelsToGpu();

	StartLoop();

}

void Engine::ShowWindow() noexcept(true){

	main_window_.Show();
	main_window_.StartMessageLoop();

}

void Engine::Stop() noexcept(true){

	//...

}

//void Engine::OutputModelsInfo() noexcept(true){
//	
//	data_base_.OutputModelsInfo();
//
//}

using namespace std::literals;
const std::chrono::high_resolution_clock::duration Engine::kTimeToUpdateLogic = 10ms;
const size_t Engine::kDefaultFps = 60;
const std::chrono::milliseconds Engine::ms_to_frame = std::chrono::milliseconds(1000 / kDefaultFps);

void Engine::StartLoop()noexcept(true){

	using namespace std::chrono;
	using duration = high_resolution_clock::duration;
	using time_point = high_resolution_clock::time_point;

	time_point cycle_beg, cycle_end, render_and_update_end;
	duration elapsed, cycle_duration, render_and_update_duration;

	//lag accumulates cycle durations
	duration lag = duration::zero();
	time_point previous = high_resolution_clock::now();

	while (loop_is_running_) {

		cycle_beg = high_resolution_clock::now();
		elapsed = cycle_beg - previous;
		previous = cycle_beg;
		lag += elapsed;

		ProcessInput();

		while ( lag >= options_.time_to_update_logic) {

			UpdateLogic();
			lag -= options_.time_to_update_logic;

		}

		Render();
		SwapBuffers();
		ShowFrame();

		render_and_update_end = high_resolution_clock::now();
		render_and_update_duration = render_and_update_end - cycle_beg;

		if (render_and_update_duration < options_.fps_limit)
			std::this_thread::sleep_for(options_.fps_limit - render_and_update_duration);

	}

}

void Engine::ProcessInput() const noexcept(true)
{
}

void Engine::UpdateLogic() const noexcept(true)
{
}

void Engine::Render()const noexcept(true){
}

void Engine::ShowFrame() const noexcept(true)
{
}

void Engine::SwapBuffers() const noexcept(true)
{
}

void Engine::LoadModelsToGpu() const noexcept(true){

	AllocateMemory();
	CopyModels();

}

void Engine::AllocateMemory() const noexcept(true)
{
}

void Engine::CopyModels() const noexcept(true)
{
}
