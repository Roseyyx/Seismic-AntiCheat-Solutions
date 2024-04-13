#define _CRT_SECURE_NO_WARNINGS

#include "Utils/Logger/Logging.h"

void LogTest(int n)
{
	Logger::Info("%d", n);
}

int main() {
	Logger::EnableFileOutput();

	std::thread Threads[10];

	for (int i = 0; i < 10; i++) Threads[i] = std::thread(LogTest, i);
	for (int i = 0; i < 10; i++) Threads[i].join();

	Logger::CloseFileOutput();

	return 0;
}