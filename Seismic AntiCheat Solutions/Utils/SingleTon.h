#pragma once

class S
{
public:
	static S& getInstance()
	{
		static S	instance; // destroyed

		return		instance; // init on first use
	}
private:
	S() {} // constructor

public:
	S(S const&) = delete;
	void operator=(S const&) = delete;
};