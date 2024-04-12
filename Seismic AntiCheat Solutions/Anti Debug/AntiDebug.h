#pragma once
#include "../Flagging/Flagging.h"

class AntiDebug : public Flagging
{
protected:
	bool IsDebuggerAttached_ = false;
	enum DebuggerType
	{
		BreakPoint,
		Exception,
		Context,
		VmHandler,
		GetString,
		PebHeader,
	};

private:
	static void SetupDebuggerDetections();

	static void AddToSeverity(DetectionSeverity Severity);


public:
	[[nodiscard]] bool GetDebugger() const
	{
		return IsDebuggerAttached_;
	}

};