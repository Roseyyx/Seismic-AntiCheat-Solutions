#include "Sideload.h"


bool SideLoad::Setup()
{
	Logger::Info("Setting up sideload...", nullptr);

	// Get current path
	const std::filesystem::path Path = std::filesystem::current_path();
	// Get current exe name
	char ExeName[MAX_PATH];
	DWORD ExeNameSize = GetModuleFileNameA(nullptr, ExeName, MAX_PATH);
	if (ExeNameSize == 0)
	{
		Logger::Error("Failed to get current exe name.", nullptr);
		return false;
	}

	Logger::Info("Current exe name: %s", ExeName);

	if (!exists(Path))
	{
		Logger::Error("Failed to find current path.", nullptr);
		return false;
	}

	for (const auto& Entry : std::filesystem::directory_iterator(Path))
	{
		if (Entry.is_regular_file() && Entry.path().extension() == ".exe")
		{
			//Logger::Info("Found Exe: %s", Entry.path().string().c_str());
			if (Entry.path().filename().string().find("_ac_") != std::string::npos && Entry.path().filename().string() != Path.string())
			{
				Logger::Info("Found sideloaded exe: %s", Entry.path().string().c_str());

				STARTUPINFO StartUpInfo = { sizeof(StartUpInfo) };
				PROCESS_INFORMATION  ProcessInformation;

				if (!CreateProcessA(Entry.path().string().c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &StartUpInfo, &ProcessInformation))
				{
					Logger::Error("Failed to create process.", nullptr);
					return false;
				}

				Logger::Info("Process ID: %d", ProcessInformation.dwProcessId);

				// wait for the process to load modules

				if (WaitForInputIdle(ProcessInformation.hProcess, 10000) == WAIT_FAILED)
				{
					Logger::Error("Failed to wait for input idle.", nullptr);
					return false;
				}

				HANDLE HModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessInformation.dwProcessId);

				if (!HModuleSnap || HModuleSnap == INVALID_HANDLE_VALUE)
				{
					Logger::Error("Failed to create snapshot.", nullptr);
					return false;
				}

				MODULEENTRY32 ModuleEntry;
				ModuleEntry.dwSize = sizeof(MODULEENTRY32);

				if (Module32First(HModuleSnap, &ModuleEntry))
				{
					do
					{
						if (ModuleEntry.hModule == GetModuleHandle(nullptr))
							continue;

						Logger::Info("Module: %s", ModuleEntry.szModule);

					} while (Module32Next(HModuleSnap, &ModuleEntry));
				}
				else
				{
					CloseHandle(HModuleSnap);
					Logger::Error("Failed to get first module.", nullptr);
					return false;
				}
			}
		}
	}

	return true;
}
