#include "Window.hpp"
//
#include <chrono>
//


//Window constants
const std::wstring Window::kDefaultTitle{ L"Default window title" };
const std::size_t Window::kDefaultWidth{ 1920 };
const std::size_t Window::kDefaultHeight{ 1080 };
const std::size_t Window::kDefaultX{ 0 };
const std::size_t Window::kDefaultY{ 0 };
const std::wstring Window::kClassName{ L"Main window class name." };

Window::Window()noexcept(true):
	hInstance_{ nullptr },
	params_{ kDefaultWidth, 
	kDefaultHeight, 
	kDefaultX, 
	kDefaultY, 
	kDefaultTitle },
	device_context_{ nullptr },
	self_hWnd_{ nullptr }{}


void Window::PushKeystroke(KeyType key)noexcept(true) {

	using namespace std::chrono;
	high_resolution_clock::time_point now_time_point = high_resolution_clock::now();
	long long  now = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
	const Keystroke keystroke = Keystroke{ key, now };
	keystrokes_queue_.Push(keystroke);

}

LRESULT CALLBACK Window:: WndProc(HWND hWnd, UINT Message, WPARAM  wParam, LPARAM lParam) {

	Window* this_wnd = reinterpret_cast<Window*>(GetWindowLong(hWnd, 0));

	switch (Message) {

	case WM_KEYDOWN: {

		const unsigned int key = wParam;

		switch (key) {

			case VK_UP: { this_wnd->PushKeystroke(KeyType::ArrowUp); break; }
			case VK_DOWN: { this_wnd->PushKeystroke(KeyType::ArrowDown); break; }
			case VK_LEFT: { this_wnd->PushKeystroke(KeyType::ArrowLeft); break; };
			case VK_RIGHT: { this_wnd->PushKeystroke(KeyType::ArrowRight); break; }
			case VK_A: { this_wnd->PushKeystroke(KeyType::A); break; };
			case VK_D: { this_wnd->PushKeystroke(KeyType::D); break; };
			case VK_S: { this_wnd->PushKeystroke(KeyType::S); break; };
			case VK_W: { this_wnd->PushKeystroke(KeyType::W); break; };

		}
		break;
			
	}

	case WM_MOUSEWHEEL: {

		static int wheel_delta = 0;
		wheel_delta += GET_WHEEL_DELTA_WPARAM(wParam);
				
		while (wheel_delta > WHEEL_DELTA) {
					//PushKeystroke(WheelUp);
			wheel_delta -= WHEEL_DELTA;
		}
				
		while (wheel_delta < 0) {
					//PushKeystroke(WheelDown);
			wheel_delta += WHEEL_DELTA;
		}

		break;
	}

	case WM_CLOSE: { DestroyWindow(hWnd);break; }

	case WM_DESTROY: { PostQuitMessage(0);break; }
		
	default: return DefWindowProc(hWnd, Message, wParam, lParam); 
		
	}

}

void Window::PlugHInstance(const HINSTANCE hInstance)const noexcept(true){

	if (hInstance_ == 0)
		hInstance_ = hInstance;

}
		
Window::~Window() {}

void Window::RegisterWindowClass()noexcept(true) {

	WNDCLASSEX wc; //Создаем экземпляр класса WNDCLASSEX
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = this->WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 4;
	wc.hInstance = hInstance_;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = kClassName.c_str();
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {

		MessageBox(nullptr,
			L"Не удалось зарегестрировать класс",
			L"Ошибка регистрации класса!",
			MB_OK);

	}

}

void Window::Create()noexcept(true){
			
			self_hWnd_ = CreateWindowEx(WS_EX_TOPMOST, kClassName.c_str(),
				params_.title_.c_str(),
				WS_OVERLAPPEDWINDOW,//WS_POPUP,
				params_.x_,
				params_.y_, 
				params_.width_, 
				params_.height_, 
				HWND(NULL), 
				HMENU(NULL), 
				(HINSTANCE)hInstance_, 
				NULL);
			
			if (!self_hWnd_) {
	
				MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);

			}

			//Что-то нужно сделать))
			SetWindowLong(self_hWnd_, 0, (LONG)this);

			SetProcessDPIAware();//Вызывая эту функцию (SetProcessDPIAware), вы сообщаете системе, что интерфейс вашего приложения умеет сам правильно масштабироваться при высоких значениях DPI (точки на дюйм). Если вы не выставите этот флаг, то интерфейс вашего приложения может выглядеть размыто при высоких значениях DPI.
			device_context_ = GetDC(self_hWnd_);

		}

		void Window::Show()const noexcept(true)
		{
			ShowWindow(self_hWnd_, SW_MAXIMIZE);
		}

		WPARAM Window::StartMessageLoop()const noexcept(true){
			
			/*
			typedef struct tagMSG {
				HWND hwnd; // дескриптор окна, которому адресовано сообщение
				UINT message; // номер (идентификатор ) сообщения
				WPARAM wParam; // параметр сообщения wParam
				LPARAM lParam; //параметр сообщения lParam
				DWORD time; // время отправки сообщения
				POINT pt; //позиция курсора(в экранных координатах) в момент
				//отправки сообщения
			} MSG;
			*/
	
			MSG msg; // создём экземпляр структуры MSG для обработки сообщений

			msg = { 0 }; // Инициализация структуры сообщения 

			while (GetMessage(&msg, NULL, NULL, NULL) != 0) {
					
					TranslateMessage(&msg);
					DispatchMessage(&msg);

			}

			// BOOL GetMessage(
			// MSG lpMsg, - адрес структуры куда поместить сообщение
			// HWND hWnd, - значение этого параметра равно NULL, 
			// UINT wMsgFilterMin, - Определяет целочисленную величину самого маленького значения сообщения, которое будет извлечено.
			// UINT wMsgFilterMax - Определяет целочисленную величину самого большого значения сообщения, которое будет извлечено.
			// *Если wMsgFilterMin и wMsgFilterMax являются оба нулевыми, функция GetMessage возвращает все доступные сообщения (то есть никакой фильтрации в диапазоне значений не выполняется).
			
			//Функция TranslateMessage переводит сообщения виртуальных клавиш в символьные сообщения. 
			//Символьные сообщения помещаются в очереди сообщений вызывающего потока для прочтения в следующий раз, 
			//когда поток вызовет функцию GetMessage или PeekMessage.

			//Функция DispatchMessage распределяет сообщение оконной процедуре WndProc.
			return msg.wParam;

		}

		void Window::ShowFrame(const FrameHost& frame)noexcept(true) {
			
			//using namespace std::chrono;
			//typedef high_resolution_clock::time_point time_point;
			//typedef high_resolution_clock::duration duration;

			//time_point second_time_point_count_fps = high_resolution_clock::now();

			//duration render_frame_duration = second_time_point_count_fps - first_time_point_count_fps_;
			//int current_fps = 0;
			//current_fps = 1000.0f / (float)duration_cast<milliseconds>(render_frame_duration).count();

			//if (current_fps > 100) { current_fps = 99; }
			//	else
			//		if (current_fps < 0) { current_fps = 0; };
			////fps = 30;
			//char fps_char[3] = { "00" };
			//_itoa(current_fps, fps_char, 10);
			//wchar_t* string = ConvertString(fps_char);

			SetDIBitsToDevice(
					device_context_,
					0,
					0,
					params_.width_, 
					params_.height_,
					0,
					0,
					0,
					params_.height_,
					static_cast<const void*>(frame),
					&frame.Info(),
					DIB_PAL_COLORS
			);

			/*TextOut(DeviceContext, 5, 5, string, 2);

			first_time_point_count_fps_ = high_resolution_clock::now();*/

				//int SetDIBitsToDevice(
				//	HDC hdc,                 // дескриптор DC
				//	int XDest,               // x-коорд. верхнего левого угла приемника
				//	int YDest,               // y-коорд. верхнего левого угла приемника
				//	DWORD dwWidth,           // ширина прямоугольника источника
				//	DWORD dwHeight,          // высота прямоугольника источника
				//	int XSrc,                // x-коорд. нижнего левого угла источника
				//	int YSrc,                // y-коорд. нижнего левого угла источника
				//	UINT uStartScan,         // первая строка развертки в массиве
				//	UINT cScanLines,         // число строк развертки
				//	CONST VOID * lpvBits,     // массив битов DIB
				//	CONST BITMAPINFO * lpbmi, // информация о точечном рисунке
				//	UINT fuColorUse          // RGB или индексы палитры
				//);
			
		}
