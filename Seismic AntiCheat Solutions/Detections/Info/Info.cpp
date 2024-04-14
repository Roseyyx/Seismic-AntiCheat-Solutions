#include "Info.h"
#include "../../Utils/Logger/Logging.h"
#include <map>

void DmiSystemUuidConversion(const BYTE* PByte, short Version)
{
	// Check if input byte array contains at least 16 bytes for the UUID
	if (PByte == nullptr) {
		Logger::Error("Input byte array is null.", nullptr);
		return;
	}

	bool only0xff = true;
	bool only0x00 = true;
	int i{};

	//
	// 16 because of the byte uuid
	// this process also makes so that the uuid isn't all
	// 1's and 0's
	//


	for (i = 0; i < 16 && (only0x00 || only0xff); i++) {
		if (PByte[i] != 0x00) { only0x00 = false; }
		if (PByte[i] != 0xFF) { only0xff = false; }
	}

	if (only0x00) { std::puts("Not present : 0x00"); return; }
	if (only0xff) { std::puts("Not Settable: 0xff"); return; }

	// Print UUID in the correct format based on the UUID version
	if (Version >= 0x0206) {
		Logger::Info("\t\t UUID:\t\t\t%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			PByte[3], PByte[2], PByte[1], PByte[0], PByte[5], PByte[4], PByte[7], PByte[6],
			PByte[8], PByte[9], PByte[10], PByte[11], PByte[12], PByte[13], PByte[14], PByte[15]);
	}
	else {

		Logger::Info("\t\t UUID:\t\t\t%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			PByte[0], PByte[1], PByte[2], PByte[3], PByte[4], PByte[5], PByte[6], PByte[7],
			PByte[8], PByte[9], PByte[10], PByte[11], PByte[12], PByte[13], PByte[14], PByte[15]);
	}
}

auto DmiStringConversion(
	const DmiHeader* PDmiHeader, BYTE Source
) -> const char*
{
	std::size_t Length{};
	std::size_t Index{};

	char* BytePointer = (char*)PDmiHeader;

	if (Source == 0) { return "Not specified"; }
	BytePointer += PDmiHeader->Length;

	while (Source > 1 && *BytePointer) {
		BytePointer += std::strlen(BytePointer);
		BytePointer++;
		Source--;
	}

	if (!*BytePointer) { return "BAD_INDEXING"; }

	Length = std::strlen(BytePointer);
	for (int i = 0; i < Length; i++) {
		if (BytePointer[i] < 32 || BytePointer[i] == 127) { BytePointer[i] = '.'; }
	}
	return BytePointer;

}

const char* DmiMemoryDeviceType(BYTE Code)
{
	std::map<int, const char*> MemoryDeviceType_;

	MemoryDeviceType_[DMT_OTHER] = "Other"; /* 0x01 */
	MemoryDeviceType_[DMT_UNKNOWN] = "Unknown";
	MemoryDeviceType_[DMT_DRAM] = "DRAM";
	MemoryDeviceType_[DMT_EDRAM] = "EDRAM";
	MemoryDeviceType_[DMT_VRAM] = "VRAM";
	MemoryDeviceType_[DMT_SRAM] = "SRAM";
	MemoryDeviceType_[DMT_RAM] = "RAM";
	MemoryDeviceType_[DMT_ROM] = "ROM";
	MemoryDeviceType_[DMT_FLASH] = "Flash";
	MemoryDeviceType_[DMT_EEPROM] = "EEPROM";
	MemoryDeviceType_[DMT_FEPROM] = "FEPROM";
	MemoryDeviceType_[DMT_EPROM] = "EPROM";
	MemoryDeviceType_[DMT_CDRAM] = "CDRAM";
	MemoryDeviceType_[DMT_3DRAM] = "3DRAM";
	MemoryDeviceType_[DMT_SDRAM] = "SDRAM";
	MemoryDeviceType_[DMT_SGRAM] = "SGRAM";
	MemoryDeviceType_[DMT_RDRAM] = "RDRAM";
	MemoryDeviceType_[DMT_DDR] = "DDR";
	MemoryDeviceType_[DMT_DDR2] = "DDR2";
	MemoryDeviceType_[DMT_DDR2_FB_DIMM] = "DDR2 FB-DIMM";
	MemoryDeviceType_[DMT_RESERVED1] = "Reserved";
	MemoryDeviceType_[DMT_RESERVED2] = "Reserved";
	MemoryDeviceType_[DMT_RESERVED3] = "Reserved";
	MemoryDeviceType_[DMT_DDR3] = "DDR3";
	MemoryDeviceType_[DMT_FBD2] = "FBD2";
	MemoryDeviceType_[DMT_DDR4] = "DDR4";
	MemoryDeviceType_[DMT_LPDDR] = "LPDDR";
	MemoryDeviceType_[DMT_LPDDR2] = "LPDDR2";
	MemoryDeviceType_[DMT_LPDDR3] = "LPDDR3";
	MemoryDeviceType_[DMT_LPDDR4] = "LPDDR4";
	MemoryDeviceType_[DMT_LOGICAL_NONVOLATILE_DEVICE] = "Logical non-volatile device";
	MemoryDeviceType_[DMT_HBM] = "HBM";
	MemoryDeviceType_[DMT_HBM2] = "HBM2"; /* 0x21 */


	if (Code >= DMT_OTHER
		&& Code <= DMT_HBM2) {
		return MemoryDeviceType_[Code - 0x01];
	}

	return "Out of spec";
}


bool Info::GetDmi(std::vector<uint8_t>& Buffer)
{
	const BYTE ByteSignature[] = { 'B', 'M', 'S', 'R' };
	const DWORD Signature = *((DWORD*)ByteSignature);

	DWORD Size = GetSystemFirmwareTable(Signature, 0, nullptr, 0);
	if (Size == 0) {
		LOG_WARN("GetSystemFirmwareTable returned 0", 0);
		return false;
	}

	Buffer.resize(Size, 0);

	if (Size != GetSystemFirmwareTable(Signature, 0, Buffer.data(), Size))
	{
		Buffer.clear();
		LOG_WARN("GetSystemFirmwareTable size mismatch", 0);
		return false;
	}

	SMBIOSData = reinterpret_cast<RawSMBIOSData*>(Buffer.data());

	return true;
}

void Info::GetUserInfo()
{
	std::vector<uint8_t> Buffer;
	bool Result = false;
	DWORD Flag = { 1 };

	Result = GetDmi(Buffer);

	if (!Result || !SMBIOSData)
	{
		LOG_ERROR("Unable to retrieve SMBIOS table", 0);
		return;
	}

	BYTE* B_ = SMBIOSData->SMBIOSTableData;
	for (DWORD Index{}; Index < SMBIOSData->Length; Index++)
	{
		DmiHeader* Header = reinterpret_cast<DmiHeader*>(B_);

		// Check if moving beyond the allocated memory
		if (B_ + Header->Length >= SMBIOSData->SMBIOSTableData + SMBIOSData->Length) {
			break;  // Exit loop to avoid going out of bounds
		}

		if (Header->Type == DMI_ENTRY_BIOS && Flag)
		{
			Logger::Info("Type %02d - [Bios Device Type]", Header->Type);
			Logger::Info("\t\t BIOS Vendor:\t\t%s",		DmiStringConversion(Header, B_[0x4]));
			Logger::Info("\t\t BIOS Version:\t\t%s",		DmiStringConversion(Header, B_[0x5]));
			Logger::Info("\t\t Release Date:\t\t%s",		DmiStringConversion(Header, B_[0x8]));

			Flag = 0;
		}
		else if (Header->Type == DMI_ENTRY_BASEBOARD)
		{
			Logger::Info("Type %02d - [Baseboard Device Type]", Header->Type);
			Logger::Info("\t\t Manufacturer:\t\t%s",		DmiStringConversion(Header, B_[0x4]));
			Logger::Info("\t\t Product Number:\t%s",		DmiStringConversion(Header, B_[0x5]));
			Logger::Info("\t\t Version:\t\t%s",			DmiStringConversion(Header, B_[0x6]));
			Logger::Info("\t\t Serial Number:\t\t%s",	DmiStringConversion(Header, B_[0x7]));

			// UUID gets printed in the function already
			DmiSystemUuidConversion(B_ + 0x8,SMBIOSData->SMBIOSMajorVersion * 0x100 + SMBIOSData->SMBIOSMinorVersion);

		}
		else if (Header->Type == DMI_ENTRY_MEM_DEVICE)
		{
			if ((DmiMemoryDeviceType(B_[0x12])) != "Other")
			{
				Logger::Info("Type %02d - [Memory Device Type]", Header->Type);
				Logger::Info("\t\t Memory Type:\t\t%s",  DmiMemoryDeviceType(B_[0x12]));
				Logger::Info("\t\t Size of Ram:\t\t%s",  DmiStringConversion(Header, B_[0xC]));
				Logger::Info("\t\t Manufacturer:\t\t%s", DmiStringConversion(Header, B_[0x17]));
				Logger::Info("\t\t Serial Number:\t\t%s",DmiStringConversion(Header, B_[0x18]));
			}
		}

		// Move the pointer within bounds
		Index += Header->Length;
		B_ += Header->Length;

		// Skip padding
		while (B_ + sizeof(WORD) <= SMBIOSData->SMBIOSTableData + SMBIOSData->Length && (*(WORD*)B_) != 0) {
			B_++;
			Index++;
		}
		B_ += 2;  // Move past the NULL terminator
	}

	Buffer.clear();

	return;
}