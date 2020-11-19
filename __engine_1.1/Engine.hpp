#pragma once
#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include <string>
#include <windows.h>
#include <functional>
#include <codecvt>
#include <exception>
#include "__window_/Window.hpp"
#include "__database_/DataBase.hpp"
#include "__console_/Console.hpp"

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

	//Holy of all saints
	static Engine* self_;
	static EngineDestroyer destroyer;

	//Default KeystrProcFunc anf LogicFunc
	static const KeystrProcFunc kDefaultKeystrProcFunc;
	static const LogicFunc kDefaultLogicFunc;

	//
	static const std::string kDefaultDirectory;


	KeystrProcFunc keystr_proc_func_;
	LogicFunc logic_func_;
	
	//
	mutable std::string dir_path_;
	
	DataBase data_base_;
	Window main_window_;
	mutable Console* log_console_;
	
	//Default constructor
	explicit Engine()noexcept(true);
	std::wstring ConvertStringtToWstring(const std::string& str)const noexcept(true);

public:

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

	friend class EngineDestroyer;


};



#endif //_ENGINE_HPP_

