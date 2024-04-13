#include <stdexcept>
#include <windows.h>
#include "Detections/Detections.h"

int main() {

	while (!GetAsyncKeyState(VK_END))
	{
		if (!Detections::Setup())
			throw std::runtime_error("Failed to setup detections");

	}


	return 0;
}