#pragma once

struct HardwareIdentifier
{
	const char* BaseBoardId;
	const char* BaseBoardVendor;
	const char* ComputerName;
	const char* WindowsLicense;
	const char* TpmKey;
};

class User
{
private:
	HardwareIdentifier HardwareIdentifier_ = {};
	int UserId_ = 0;

	User(){}

	User(HardwareIdentifier Hwid, const int UserId)
	{
		this->HardwareIdentifier_ = Hwid;
		this->UserId_ = UserId;
	}

	User(const User&) = delete;
	User& operator= (const User&) = delete;

	~User()
	{
		FreeUser();
	}

	void FreeUser()
	{
		this->HardwareIdentifier_ = {};
		this->UserId_ = 0;
	}

public:
	static void AppendToUser(const HardwareIdentifier Hwid, const int UserId)
	{
		User(Hwid, UserId);
	}

	static User& GetUser(const int UserId)
	{
		static User PUser;
		return PUser;
	}
};

