#pragma once

class Flagging
{
protected:
	enum DetectionSeverity
	{
		HighSeverity,
		MediumSeverity,
		LowSeverity,
		NoSeverity
	};
private:
	Flagging()
	{
		this->Flags = 0;
		this->Severity = NoSeverity;
	}

	Flagging(int Flags, DetectionSeverity Severity)
	{
		this->Flags = Flags;
		this->Severity = Severity;
	}

	void Clear()
	{
		this->Flags = 0;
		this->Severity = NoSeverity;
	}

	int Flags;
	DetectionSeverity Severity;
};

