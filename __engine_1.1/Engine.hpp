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

using KeystrProcFunc = std::function<void(const Keystroke&)>;
using LogicFunc = std::function<void()>;

class Engine final{
private:

	//Engine options. Start options are default.
	struct Options {

		std::chrono::high_resolution_clock::duration time_to_update_logic;
		std::chrono::high_resolution_clock::duration fps_limit;
		mutable std::string dir_path_;
		
	};

	//Default options 
	static const std::chrono::high_resolution_clock::duration kTimeToUpdateLogic;
	static const std::chrono::milliseconds ms_to_frame;


	//Default KeystrProcFunc anf LogicFunc
	static const KeystrProcFunc kDefaultKeystrProcFunc;
	static const LogicFunc kDefaultLogicFunc;

	KeystrProcFunc keystr_proc_func_;
	LogicFunc logic_func_;
	
	//Holy of all saints
	static Engine* self_;
	static EngineDestroyer destroyer;
	
	//Engine options
	Options options_;

	//Main engine thread
	std::thread thread_;

	//Main engine subsystems
	DataBase data_base_;
	Window main_window_;
	GpuMemoryManager gpu_memory_manager_;
	
	mutable Console* log_console_;
	
	//
	bool loop_is_running_;

	//Default constructor
	explicit Engine()noexcept(true);

	//Private methods
	std::wstring ConvertStringtToWstring(const std::string& str)const noexcept(true);
	void StartLoop()noexcept(true);
	void ProcessInput()const noexcept(true);
	void UpdateLogic()const noexcept(true);
	void Render()const noexcept(true);
	void ShowFrame()const noexcept(true);
	void SwapBuffers()const noexcept(true);
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

	friend class EngineDestroyer;

};

#endif //_ENGINE_HPP_

