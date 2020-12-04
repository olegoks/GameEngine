#pragma once

#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <Windows.h>
#include "../engine_types.hpp"

//enum SizeMode {
//
//	SIZEBLE,
//	NOT_SIZEBLE,
//	FULL_SCREEN
//
//};
//
#define VK_A 65
#define VK_B 66
#define VK_D 68
#define VK_W 87
#define VK_S 83

static struct Parametrs {

	size_t width_;
	size_t height_;
	size_t x_;
	size_t y_;
	std::wstring title_;

};

class Window final{
private:

	static const std::wstring kClassName;
	HWND self_hWnd_;
	mutable HINSTANCE hInstance_;
	HDC device_context_;
	KeystrokesQueue keystrokes_queue_;
	Parametrs params_;

	void PushKeystroke(KeyType key)noexcept(true);

public:

	//Window class constants
	static const std::wstring kDefaultTitle;
	static const std::size_t kDefaultWidth;
	static const std::size_t kDefaultHeight;
	static const std::size_t kDefaultX;
	static const std::size_t kDefaultY;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM  wParam, LPARAM lParam);
	void PlugHInstance(const HINSTANCE hInstance)const noexcept(true);
	explicit Window()noexcept(true);
	~Window();
	void ShowFrame(const FrameHost& frame)noexcept(true);
	inline Keystroke PopKeystroke()noexcept(true) { keystrokes_queue_.Pop(); };
	void Create()noexcept(true);
	void Show()const noexcept(true);
	WPARAM StartMessageLoop()const noexcept(true);
	void RegisterWindowClass()noexcept(true);
	inline size_t Width()const noexcept(true) { return params_.width_; };
	inline size_t Height()const noexcept(true) { return params_.height_; };
	inline void Width(const size_t width)noexcept(true) { params_.width_ = width; };
	inline void Height(const size_t height)noexcept(true) { params_.height_ = height; };
	inline KeystrokesQueue* KeystrokesQueue()noexcept(true) { return &keystrokes_queue_; };
};

#endif // !_WINDOW_HPP_