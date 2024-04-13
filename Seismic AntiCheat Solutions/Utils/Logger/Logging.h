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
	static LogPriority Priority_;
	static std::mutex LogMutex_;
	static const char* FilePath_;
	static FILE* File_;

public:
	static void SetPriority(LogPriority NewPriority)
	{
		Priority_ = NewPriority;
	}

	static void EnableFileOutput()
	{
		FilePath_ = "log.txt";
		SetFileOutput();
	}

	static void EnableFileOutput(const char* NewFilePath)
	{
		FilePath_ = NewFilePath;
		SetFileOutput();
	}

	static void CloseFileOutput()
	{
		FreeFile();
	}

	template<typename... Args>
	static void Trace(const char* Message, Args... Arguments)
	{
		Log("[Trace]\t", TracePriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Debug(const char* Message, Args... Arguments)
	{
		Log("[Debug]\t", DebugPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Info(const char* Message, Args... Arguments)
	{
		Log("[Info]\t", InfoPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Warn(const char* Message, Args... Arguments)
	{
		Log("[Warn]\t", WarnPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Error(const char* Message, Args... Arguments)
	{
		Log("[Error]\t", ErrorPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Critical(const char* Message, Args... Arguments)
	{
		Log("[Critical]\t", CriticalPriority, Message, Arguments...);
	}
private:
	template<typename... Args>
	static void Log(const char* MessagePriorityString, LogPriority MessagePriority, const char* Message, Args... Arguments)
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

	static void SetFileOutput()
	{
		if (File_ != 0)
		{
			fclose(File_);
		}

		File_ = fopen(FilePath_, "a");

		if (File_ == 0)
			printf("Failed to open file at %s", FilePath_);
	}

	static void FreeFile()
	{
		fclose(File_);
		File_ = 0;
	}
};

LogPriority Logger::Priority_ = InfoPriority;
std::mutex Logger::LogMutex_;
const char* Logger::FilePath_ = 0;
FILE* Logger::File_ = 0;