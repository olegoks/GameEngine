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
	dir_path_{ kDefaultDirectory },
	log_console_{ nullptr }{

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

	dir_path_ = dir_path;
	data_base_.ObjFilesDir(dir_path_);

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
	//...

}

void Engine::ShowWindow() noexcept(true){

	main_window_.Show();
	main_window_.StartMessageLoop();

}

void Engine::Stop() noexcept(true){

	//...

}
