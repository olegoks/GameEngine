#pragma once

#ifndef _CONSOLE_HPP_
#define _CONSOLE_HPP_

#include <stdio.h>
#include <windows.h>
#include <string>

class Console {
private:

	static const std::wstring kDefaultTitle;
	std::wstring title_;

public:

	explicit Console(const std::wstring& title)noexcept(true);
	~Console();

};

#endif
