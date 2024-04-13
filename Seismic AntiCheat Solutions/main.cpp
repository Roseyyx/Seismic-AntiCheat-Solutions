#define _CRT_SECURE_NO_WARNINGS

#include "Utils/Logger/Logging.h"
int main() {
	const char* Name = "User";
	LOG_INFO("Hello", Name);

	return 0;
}