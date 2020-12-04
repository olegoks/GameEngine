#include "Console.hpp"

#pragma warning(disable : 4996) // <------

const std::string kDefaultTitle = "Default log console title.";

Console::Console(const std::wstring& title)noexcept(true){

	title_ = title;

	if (AllocConsole()) {

		freopen("CONOUT$", "wt", stdout);
		SetConsoleTitle((LPWSTR)title_.c_str());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		
		//Hide carrete
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(out, &cursorInfo);

	}
	else
		MessageBox(NULL,(LPWSTR) L"Failed log console creation.", NULL, NULL);

}

Console::~Console()
{

	FreeConsole();

}
