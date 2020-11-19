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
//		
//void Window::PushKeystroke(KeyType key) {
//
//	using namespace std::chrono;
//	high_resolution_clock::time_point now_time_point = high_resolution_clock::now();
//	unsigned int now = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
//	const Keystroke keystroke = { key, now };
//	This->keys_.push_back(keystroke);
//
//}

Window::Window()noexcept(true) :
	hInstance_{ 0 },
	title_{ kDefaultTitle },
	width_{ kDefaultWidth },
	height_{ kDefaultHeight },
	x_{ kDefaultX },
	y_{ kDefaultY }{}

		LRESULT CALLBACK Window:: WndProc(HWND hWnd, UINT Message, WPARAM  wParam, LPARAM lParam) {

			Window* wnd_ptr = reinterpret_cast<Window*>(GetWindowLong(hWnd, 0));

			switch (Message) {

			case WM_KEYDOWN: {

				const unsigned int key = wParam;

				/*switch (key) {

				case VK_UP: { PushKeystroke(KeyType::ArrowUp);break;}
				case VK_DOWN: { PushKeystroke(KeyType::ArrowDown);break;}
				case VK_LEFT: { PushKeystroke(ArrowLeft);break;};
				case VK_RIGHT: { PushKeystroke(ArrowRight);break;}
				case VK_A: { PushKeystroke(A); break; };
				case VK_D: { PushKeystroke(D); break; };
				case VK_S: { PushKeystroke(S); break; };
				case VK_W: { PushKeystroke(W); break; };

				}*/
				
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

			case WM_CLOSE: {DestroyWindow(hWnd);break;}

			case WM_DESTROY: {PostQuitMessage(0);break;}
		
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
				title_.c_str(),
				WS_POPUP,
				x_,
				y_, 
				width_, 
				height_, 
				HWND(NULL), 
				HMENU(NULL), 
				(HINSTANCE)hInstance_, 
				NULL);
			
			if (!self_hWnd_) {
	
				MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);

			}
			
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

		//void Window::DisplayFrame() {
		//	
		//	//using namespace std::chrono;
		//	//typedef high_resolution_clock::time_point time_point;
		//	//typedef high_resolution_clock::duration duration;

		//	//time_point second_time_point_count_fps = high_resolution_clock::now();

		//	//duration render_frame_duration = second_time_point_count_fps - first_time_point_count_fps_;
		//	//int current_fps = 0;
		//	//current_fps = 1000.0f / (float)duration_cast<milliseconds>(render_frame_duration).count();

		//	//if (current_fps > 100) { current_fps = 99; }
		//	//	else
		//	//		if (current_fps < 0) { current_fps = 0; };
		//	////fps = 30;
		//	//char fps_char[3] = { "00" };
		//	//_itoa(current_fps, fps_char, 10);
		//	//wchar_t* string = ConvertString(fps_char);

		//	SetDIBitsToDevice(
		//			DeviceContext,//HDC hdc
		//			0,//int XDest,
		//			0,//int YDest,+
		//			size_.width ,//DWORD dwWidth, 
		//			size_.height ,//DWORD dwHeight,+
		//			0,//int XSrc,
		//			0,//int YSrc,
		//			0,// UINT uStartScan
		//			size_.height,//UINT cScanLines
		//			this->display_buffer_,
		//			&this->display_buffer_info_,
		//		DIB_PAL_COLORS//DIB_RGB_COLORS/*,
		//			//SRCCOPY
		//	);
		//	/*TextOut(DeviceContext, 5, 5, string, 2);

		//	first_time_point_count_fps_ = high_resolution_clock::now();*/

		//		

		//		//int SetDIBitsToDevice(
		//		//	HDC hdc,                 // дескриптор DC
		//		//	int XDest,               // x-коорд. верхнего левого угла приемника
		//		//	int YDest,               // y-коорд. верхнего левого угла приемника
		//		//	DWORD dwWidth,           // ширина прямоугольника источника
		//		//	DWORD dwHeight,          // высота прямоугольника источника
		//		//	int XSrc,                // x-коорд. нижнего левого угла источника
		//		//	int YSrc,                // y-коорд. нижнего левого угла источника
		//		//	UINT uStartScan,         // первая строка развертки в массиве
		//		//	UINT cScanLines,         // число строк развертки
		//		//	CONST VOID * lpvBits,     // массив битов DIB
		//		//	CONST BITMAPINFO * lpbmi, // информация о точечном рисунке
		//		//	UINT fuColorUse          // RGB или индексы палитры
		//		//);
		//	

		//}

		//void Window::SetDisplayBuffer(RgbPixel* display_buffer, const unsigned int display_buffer_size){
		//
		//	this->display_buffer_ = display_buffer;

		//	ZeroMemory(&this->display_buffer_info_.bmiHeader, sizeof(BITMAPINFO));

		//	this->display_buffer_info_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//	this->display_buffer_info_.bmiHeader.biWidth = this->size_.width;
		//	this->display_buffer_info_.bmiHeader.biHeight = -((int) (this->size_.height));
		//	this->display_buffer_info_.bmiHeader.biPlanes = 1;
		//	this->display_buffer_info_.bmiHeader.biBitCount = 32;//this->kBitsPerPixel;
		//	this->display_buffer_info_.bmiHeader.biCompression = BI_RGB;
		//	this->display_buffer_info_.bmiHeader.biSizeImage = ((1920 * 24 + 31) & ~31) / 8 * 1080;//display_buffer_size;
		//	this->display_buffer_info_.bmiHeader.biXPelsPerMeter = 0;
		//	this->display_buffer_info_.bmiHeader.biYPelsPerMeter = 0;
		//	this->display_buffer_info_.bmiHeader.biClrUsed = 0;
		//	this->display_buffer_info_.bmiHeader.biClrImportant = 0;
		//	
		//}

		//Window* Window::This = nullptr;
