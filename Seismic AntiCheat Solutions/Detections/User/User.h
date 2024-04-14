#pragma once

class User
{
public:

	User() = default;

	User(const User&) = delete;
	User(User&&) = delete;

	User& operator=(const User&) = delete;

    ~User() = default;

private:

	// User information such as Baseboard Serial, Baseboard UUID, Windows OEM key, Baseboard vendor

	const char* BaseboardSerial_ = nullptr;
	const char* WindowsOEMKey_ = nullptr;
	const char* BaseboardVendor_ = nullptr;

public:

    // Setters
    void SetBaseboardSerial(const char* BaseboardSerial)
    {
        BaseboardSerial_ = BaseboardSerial;
    }

    void SetWindowsOEMKey(const char* WindowsOEMKey)
    {
        WindowsOEMKey_ = WindowsOEMKey;
    }

    void SetBaseboardVendor(const char* BaseboardVendor)
    {
        BaseboardVendor_ = BaseboardVendor;
    }

    // Getters
    const char* GetBaseboardSerial() const
    {
        return BaseboardSerial_;
    }

    const char* GetWindowsOEMKey() const
    {
        return WindowsOEMKey_;
    }

    const char* GetBaseboardVendor() const
    {
        return BaseboardVendor_;
    }

};