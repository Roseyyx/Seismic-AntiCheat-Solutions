#pragma once

#include <mutex>
#include <stdio.h>
#include <time.h>
#include "Xor.h"

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
	const char* FilePath_ = nullptr;
	FILE* File_ = nullptr;

public:
	static void SetPriority(LogPriority NewPriority)
	{
		GetInstance().Priority_ = NewPriority;
	}

	static void EnableFileOutput()
	{
		Logger& LoggerInstance = GetInstance();
		LoggerInstance.FilePath_ = xorstr_("log.txt");
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
		GetInstance().Log(xorstr_("[Trace]\t"), TracePriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Debug(const char* Message, Args... Arguments)
	{
		GetInstance().Log(xorstr_("[Debug]\t"), DebugPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Info(const char* Message, Args... Arguments)
	{
		GetInstance().Log(xorstr_("[Info]\t"), InfoPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Warn(const char* Message, Args... Arguments)
	{
		GetInstance().Log(xorstr_("[Warn]\t"), WarnPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Error(const char* Message, Args... Arguments)
	{
		GetInstance().Log(xorstr_("[Error]\t"), ErrorPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Critical(const char* Message, Args... Arguments)
	{
		GetInstance().Log(xorstr_("[Critical]\t"), CriticalPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Trace(int Line, const char* SourceFile, const char* Message, Args... Arguments)
	{
		GetInstance().Log(Line, SourceFile, xorstr_("[Trace]\t"), TracePriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Debug(int Line, const char* SourceFile, const char* Message, Args... Arguments)
	{
		GetInstance().Log(Line, SourceFile, xorstr_("[Debug]\t"), DebugPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Info(int Line, const char* SourceFile, const char* Message, Args... Arguments)
	{
		GetInstance().Log(Line, SourceFile, xorstr_("[Info]\t"), InfoPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Warn(int Line, const char* SourceFile, const char* Message, Args... Arguments)
	{
		GetInstance().Log(Line, SourceFile, xorstr_("[Warn]\t"), WarnPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Error(int Line, const char* SourceFile, const char* Message, Args... Arguments)
	{
		GetInstance().Log(Line, SourceFile, xorstr_("[Error]\t"), ErrorPriority, Message, Arguments...);
	}

	template<typename... Args>
	static void Critical(int Line, const char* SourceFile, const char* Message, Args... Arguments)
	{
		GetInstance().Log(Line, SourceFile, xorstr_("[Critical]\t"), CriticalPriority, Message, Arguments...);
	}
public:
	Logger() = default;

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
			const std::time_t CurrentTime = std::time(nullptr);
			const std::tm* TimeStamp = std::localtime(&CurrentTime);
			char Buffer[80];
			strftime(Buffer, 80, "%c", TimeStamp);

			std::scoped_lock Lock(LogMutex_);

			//printf("%s\t", Buffer);
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

	template<typename... Args>
	void Log(int LineNumber, const char* SourceFile, const char* MessagePriorityString, LogPriority MessagePriority, const char* Message, Args... Arguments)
	{
		if (Priority_ <= MessagePriority)
		{
			const std::time_t CurrentTime = std::time(nullptr);
			const std::tm* TimeStamp = std::localtime(&CurrentTime);
			char Buffer[80];
			strftime(Buffer, 80, "%c", TimeStamp);

			std::scoped_lock Lock(LogMutex_);

			//printf("%s\t", Buffer);
			printf(MessagePriorityString);
			printf(Message, Arguments...);
			printf(xorstr_(" on line %d in %s"), LineNumber, SourceFile);
			printf("\n");

			if (File_)
			{
				fprintf(File_, "%s\t", Buffer);
				fprintf(File_, MessagePriorityString);
				fprintf(File_, Message, Arguments...);
				fprintf(File_, xorstr_(" on line %d in %s"), LineNumber, SourceFile);
				fprintf(File_, "\n");
			}
		}
	}

	void SetFileOutput()
	{
		if (File_ != nullptr)
		{
			fclose(File_);
		}

		fopen_s(&File_, FilePath_, "a");

		if (File_ == nullptr)
			printf("Failed to open file at %s", FilePath_);
	}

	void FreeFile()
	{
		if (File_ == nullptr) return;
		fclose(File_);
		File_ = nullptr;
	}
};

#define LOG_TRACE(Message, ...) (Logger::Trace(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_DEBUG(Message, ...) (Logger::Debug(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_INFO(Message, ...) (Logger::Info(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_WARN(Message, ...) (Logger::Warn(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_ERROR(Message, ...) (Logger::Error(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_CRITICAL(Message, ...) (Logger::Critical(__LINE__, __FILE__, Message, __VA_ARGS__))