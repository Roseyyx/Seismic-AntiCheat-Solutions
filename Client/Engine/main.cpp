#include "../Logging/Logging.h"

int main() {
	Logger::EnableFileOutput();
	Logger::Info("Hello from Engine");

	return 0;
}