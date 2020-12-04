#pragma once
#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include <string>
#include <windows.h>
#include <functional>
#include <codecvt>
#include <exception>
#include <thread>

#include "__window_/Window.hpp"
#include "__database_/DataBase.hpp"
#include "__console_/Console.hpp"
#include "../__engine_1.1/__gpu_manager_/GpuManager.hpp"
#include "__graphic_engine_/GraphicEngine.hpp"
#include "../__engine_1.1/__logic_engine_/LogicEngine.hpp"

//Func = function
//Proc = processing, process
//Keystr = keystrokes

class EngineException final : public std::exception {
private:
	
	//Exception information
	std::string what_;

public:

	EngineException(const std::string& what)noexcept(true):
		what_{ what } {}

	inline const std::string& What()const noexcept(true) { return what_; };

};

class Engine;

class EngineDestroyer final {
private:

	Engine* engine_ptr_;

public:

	explicit EngineDestroyer()noexcept(true) :
		engine_ptr_{ nullptr } {}
	
	inline void PlugEngine(Engine* engine_ptr)const noexcept(true) { engine_ptr = engine_ptr; };

	~EngineDestroyer()noexcept(true) {}

};


//Engine options. Start options are default.
static struct Options {

	std::chrono::high_resolution_clock::duration time_to_update_logic;
	std::chrono::high_resolution_clock::duration fps_limit;
	mutable std::string dir_path_;

};

class Engine final{
private:

	//Default options 
	static const std::chrono::high_resolution_clock::duration kTimeToUpdateLogic;
	static const std::chrono::milliseconds ms_to_frame;

	//Holy of all saints
	static Engine* self_;
	static EngineDestroyer destroyer;
	
	//Engine options
	Options options_;

	//Main engine thread.
	//Loop stops if Window 
	//gets Message WM_DESTROY.
	std::thread thread_;

	//
	DeviceData device_data_;

	//Main engine subsystems
	//DataBase contains all models and their info
	DataBase data_base_;
	Window main_window_;
	GraphicEngine graphic_engine_;
	LogicEngine logic_engine_;
	Camera camera_;

	//If there is no console Console* pointer is nullptr.
	mutable Console* log_console_;
	
	//
	bool loop_is_running_;

	//
	const FrameHost* frame_;
	//Default constructor
	explicit Engine()noexcept(true);

	//RgbColor RandomColor()const noexcept(true);

	std::wstring ConvertStringtToWstring(const std::string& str)const noexcept(true);
	void StartLoop()noexcept(true);
	void ProcessInput()const noexcept(true);
	void UpdateLogic()noexcept(true);
	void Render()noexcept(true);
	void ShowFrame()noexcept(true);
	void LoadModelsToGpu()noexcept(true);
	void AllocateMemory()noexcept(true);
	void CopyModels()noexcept(true);

public:

	static const size_t kDefaultFps;
	static const std::string kDefaultDirectory;

	//Returs reference to engine object
	static Engine& Instance()noexcept(false);
	
	//Engine interface
	void InitCuda(/*...*/)const noexcept(false);
	void DefaultDir(const std::string& dir_path)const noexcept(true);
	void LoadModels(const std::initializer_list<std::string>& list)noexcept(true);
	void CreateConsole(const std::string& console_name)const noexcept(true);
	void InitWindow(const HINSTANCE hInstance)const noexcept(true);
	void InitKeystrProcFunc(const KeystrProcFunc& func)noexcept(true);
	void InitLogicFunc(const LogicFunc& func)noexcept(true);
	void InitWindowSize(const size_t width, const size_t height)const noexcept(true);
	void StartMainLoop()noexcept(true);
	void ShowWindow()noexcept(true);
	void Stop()noexcept(true);
	//void OutputModelsInfo()noexcept(true);

	//Models manipulations
	//void TranslateModel(ModelId model_id, );
	void RotateModel(ModelId model_id, const float alpha_degree, const Vector3D& around_vector, const Vertex3D& around_point);
	friend class EngineDestroyer;

};

#endif //_ENGINE_HPP_

