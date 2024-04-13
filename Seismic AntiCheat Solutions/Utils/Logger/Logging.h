#pragma once

#include <mutex>
#include <stdio.h>
#include <ctime>

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
	LogPriority Priority_ = InfoPriority;
	std::mutex LogMutex_;
	const char* FilePath_ = 0;
	FILE* File_ = 0;

public:
	static void SetPriority(LogPriority NewPriority)
	{
		GetInstance().Priority_ = NewPriority;
	}

	static void EnableFileOutput()
	{
		Logger& LoggerInstance = GetInstance();
		LoggerInstance.FilePath_ = "log.txt";
		LoggerInstance.SetFileOutput();
	}

	static void EnableFileOutput(const char* NewFilePath)
	{
		Logger& LoggerInstance = GetInstance();
		LoggerInstance.FilePath_ = NewFilePath;
		LoggerInstance.SetFileOutput();
	}

	template<typename... Args>
	static void Trace(const char* Message, Args... Arguments)
	{
		GetInstance().Log("[Trace]\t", TracePriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Debug(const char* Message, Args... Arguments)
	{
		GetInstance().Log("[Debug]\t", DebugPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Info(const char* Message, Args... Arguments)
	{
		GetInstance().Log("[Info]\t", InfoPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Warn(const char* Message, Args... Arguments)
	{
		GetInstance().Log("[Warn]\t", WarnPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Error(const char* Message, Args... Arguments)
	{
		GetInstance().Log("[Error]\t", ErrorPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Critical(const char* Message, Args... Arguments)
	{
		GetInstance().Log("[Critical]\t", CriticalPriority, Message, Arguments...);
	}
private:
	Logger() {}

	Logger(const Logger&) = delete;
	Logger& operator= (const Logger&) = delete;

	~Logger()
	{
		FreeFile();
	}

	static Logger& GetInstance()
	{
		static Logger PLogger;
		return PLogger;
	}

	template<typename... Args>
	void Log(const char* MessagePriorityString, LogPriority MessagePriority, const char* Message, Args... Arguments)
	{
		if (Priority_ <= MessagePriority)
		{
			std::time_t CurrentTime = std::time(0);
			std::tm* TimeStamp = std::localtime(&CurrentTime);
			char Buffer[80];
			strftime(Buffer, 80, "%c", TimeStamp);

			std::scoped_lock Lock(LogMutex_);

			printf("%s\t", Buffer);
			printf(MessagePriorityString);
			printf(Message, Arguments...);
			printf("\n");

			if (File_)
			{
				fprintf(File_, "%s\t", Buffer);
				fprintf(File_, MessagePriorityString);
				fprintf(File_, Message, Arguments...);
				fprintf(File_, "\n");
			}
		}
	}

	void SetFileOutput()
	{
		if (File_ != 0)
		{
			fclose(File_);
		}

		File_ = fopen(FilePath_, "a");

		if (File_ == 0)
			printf("Failed to open file at %s", FilePath_);
	}

	void FreeFile()
	{
		fclose(File_);
		File_ = 0;
	}
};
