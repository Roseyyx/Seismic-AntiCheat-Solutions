#pragma once
#include <iostream>
#include <windows.h>

#pragma region ConsoleLogColors
#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)
#pragma endregion

class Logging
{
private:
	FILE* PStream = {};

public:
	bool Attach();
	bool Detach();

	template <typename ...ArgsT>
	void PrintLog(const std::string OutputText, const ArgsT& ...Arguments)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_CYAN);
		std::cout << "[LOG]";
		if constexpr (sizeof...(Arguments) > 0)
			std::cout << std::vformat(OutputText, std::make_format_args(Arguments...)) << std::endl;
		else
			std::cout << OutputText << std::endl;
	}
	template <typename ...ArgsT>
	void PrintWarning(const std::string OutputText, const ArgsT& ...Arguments)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_YELLOW);
		std::cout << "[WARNING]";
		if constexpr (sizeof...(Arguments) > 0)
			std::cout << std::vformat(OutputText, std::make_format_args(Arguments...)) << std::endl;
		else
			std::cout << OutputText << std::endl;
	}
	template <typename ...ArgsT>
	void PrintError(const std::string OutputText, const ArgsT& ...Arguments)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_RED);
		std::cout << "[ERROR]";
		if constexpr (sizeof...(Arguments) > 0)
			std::cout << std::vformat(OutputText, std::make_format_args(Arguments...)) << std::endl;
		else
			std::cout << OutputText << std::endl;
	}
	template <typename ...ArgsT>
	void PrintDebug(const std::string OutputText, const ArgsT& ...Arguments)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_MAGENTA);
		std::cout << "[DEBUG]";
		if constexpr (sizeof...(Arguments) > 0)
			std::cout << std::vformat(OutputText, std::make_format_args(Arguments...)) << std::endl;
		else
			std::cout << OutputText << std::endl;
	}


};
