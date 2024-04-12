#include "AntiDebug.h"

void AntiDebug::SetupDebuggerDetections()
{
	
}

void AntiDebug::AddToSeverity(DetectionSeverity Severity)
{
	const Flagging UserFlag;
	int Flags = UserFlag.Flags;
	Flagging(Flags++, Severity);
}
