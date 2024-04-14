#include "Utils/Logger/Logging.h"
#include "Detections/Info/Info.h"

int main() {
	//Logger::EnableFileOutput();

	Logger::Info("Setting Seismic Anti Cheat up...", nullptr);
	Info::GetUserInfo();

	return 0;
}