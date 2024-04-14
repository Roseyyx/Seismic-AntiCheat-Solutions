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
				// Create process and set it to suspended
				if (!CreateProcessA(Entry.path().string().c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &StartUpInfo, &ProcessInformation))
				{
					Logger::Error("Failed to create process.", nullptr);
					return false;
				}

				Logger::Info("Created process: %s", Entry.path().string().c_str());

				HANDLE HModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessInformation.dwProcessId);

				if (!HModuleSnap || HModuleSnap == INVALID_HANDLE_VALUE)
				{
					Logger::Error("Failed to create snapshot.", nullptr);
					return false;
				}

				MODULEENTRY32 ModuleEntry = MODULEENTRY32{ sizeof(MODULEENTRY32) };

				if (Module32First(HModuleSnap, &ModuleEntry))
				{
					do
					{
						if (ModuleEntry.hModule == GetModuleHandle(nullptr))
							continue;

						PIMAGE_DOS_HEADER PimageDosHeader = (PIMAGE_DOS_HEADER)ModuleEntry.modBaseAddr;
						PIMAGE_NT_HEADERS PimageNtHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)PimageDosHeader + PimageDosHeader->e_lfanew);
						PIMAGE_SECTION_HEADER PimageSectionHeader = IMAGE_FIRST_SECTION(PimageNtHeader + 1);

					}
					while (Module32Next(HModuleSnap, &ModuleEntry));
				}
				else
				{
					Logger::Error("Failed to get first module.", nullptr);
					return false;
				}
			}
		}
	}

	return true;
}
