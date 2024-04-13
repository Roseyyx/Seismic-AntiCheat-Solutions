#include "Utils/Logger/Logging.h"
int main() {
	Logger::EnableFileOutput();

	Logger::Info("Setting Seismic Anti Cheat up...", nullptr);

	return 0;
}