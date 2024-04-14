#include "Utils/Logger/Logging.h"
#include "Detections/Info/Info.h"
#include "Utils/Sideload/Sideload.h"

int main() {
	Logger::EnableFileOutput();
	Logger::Info("Setting Seismic Anti Cheat up...", nullptr);

	SideLoad::Setup();

	Info::GetUserInfo();

	return 0;
}