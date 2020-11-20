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
//#define VK_A 65
//#define VK_B 66
//#define VK_D 68
//#define VK_W 87
//#define VK_S 83

class Window final{
private:

	static const std::wstring kClassName;

	std::wstring title_;
	HWND self_hWnd_;
	mutable HINSTANCE hInstance_;
	HDC device_context_;
	size_t width_;
	size_t height_;
	size_t x_;
	size_t y_;
	KeystrokesQueue keystrokes_queue_;


	//RgbPixel* display_buffer_;
	//RgbPixel* now_is_displaying_buffer_;
	//DISPLAYBUFFERINFO display_buffer_info_;
	//const unsigned int kBitsPerPixel = 32;
	//win_coordinats win_coordinats_;

	//high_resolution_clock::time_point first_time_point_count_fps_;

	//static void PushKeystroke(KeyType key);

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

			//void SetDisplayBuffer(RgbPixel* display_buffer, const unsigned int display_buffer_size);

			//void DisplayFrame();

			//std::vector<Keystroke>* GetKeystrokesBuffer() noexcept { return &keys_; }
			//RgbPixel* GetHostDisplayBuffer() const noexcept { return display_buffer_; };
	void Create()noexcept(true);
	void Show()const noexcept(true);
	WPARAM StartMessageLoop()const noexcept(true);
	void RegisterWindowClass()noexcept(true);

};

#endif // !_WINDOW_HPP_