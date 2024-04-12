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
public:
	Flagging()
	{
		this->Flags = 0;
		this->Severity = NoSeverity;
	}

	Flagging(const int Flags, const DetectionSeverity Severity)
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

