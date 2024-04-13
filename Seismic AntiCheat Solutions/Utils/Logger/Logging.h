#pragma once

#include <mutex>
#include <stdio.h>

enum LogPriority
{
	TracePriority,
	DebugPriority,
	InfoPriority,
	WarnPriority,
	ErrorPriority,
	CriticalPriority,
};

class Logger
{
private:
	static LogPriority Priority_;
	static std::mutex LogMutex_;

public:
	static void SetPriority(LogPriority NewPriority)
	{
		Priority_ = NewPriority;
	}

	template<typename... Args>
	static void Trace(const char* Message, Args... Arguments)
	{
		if (Priority_ <= TracePriority)
		{
			std::scoped_lock Lock(LogMutex_);
			printf("[Trace]\t");
			printf(Message, Arguments...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Debug(const char* Message, Args... Arguments)
	{
		if (Priority_ <= DebugPriority)
		{
			std::scoped_lock Lock(LogMutex_);
			printf("[Debug]\t");
			printf(Message, Arguments...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Info(const char* Message, Args... Arguments)
	{
		if (Priority_ <= InfoPriority)
		{
			std::scoped_lock Lock(LogMutex_);
			printf("[Info]\t");
			printf(Message, Arguments...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Warn(const char* Message, Args... Arguments)
	{
		if (Priority_ <= WarnPriority)
		{
			std::scoped_lock Lock(LogMutex_);
			printf("[Warn]\t");
			printf(Message, Arguments...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Error(const char* Message, Args... Arguments)
	{
		if (Priority_ <= ErrorPriority)
		{
			std::scoped_lock Lock(LogMutex_);
			printf("[Error]\t");
			printf(Message, Arguments...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Critical(const char* Message, Args... Arguments)
	{
		if (Priority_ <= CriticalPriority)
		{
			std::scoped_lock Lock(LogMutex_);
			printf("[Critical]\t");
			printf(Message, Arguments...);
			printf("\n");
		}
	}
};

LogPriority Logger::Priority_ = InfoPriority;