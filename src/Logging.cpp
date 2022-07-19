#include <Windows.h>
#include <stdio.h>
#include "stdafx.h"
#include "Logging.h"
#include "Util.h"
#include "Exceptions.h"

namespace Logging
{
	HANDLE gLogFile = nullptr;
	bool gLogToExternalConsole = true;
	bool gLogToFile = true;
	bool gLogToGameConsole = false;

	void LogToFile(const char* Buff, const int Len)
	{
		if (gLogFile != INVALID_HANDLE_VALUE)
		{
			DWORD bytesWritten = 0;

			std::string f = Buff;
			if (f.find("\n", 0) == std::string::npos) {
				f += "\n";
			}
			const char* outc = f.c_str();
			int Length = strlen(outc);

			WriteFile(gLogFile, outc, Length, &bytesWritten, nullptr);
		}
	}

	void LogWinConsole(const char* Buff, const int Len)
	{
		const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD bytesWritten = 0;

		std::string f = Buff;
		if (f.find("\n", 0) == std::string::npos) {
			f += "\n";
		}
		const char* outc = f.c_str();
		int Length = strlen(outc);
		WriteFile(output, f.c_str(), Length, &bytesWritten, nullptr);

	}

	void LogIgnoreUE(const char* fmt, ...) {

		va_list args;
		va_start(args, fmt);
		std::string formatted = Util::FormatInternal(fmt, args);
		va_end(args);

		std::string out = formatted + "\n";
		const char* outc = out.c_str();
		int Length = strlen(outc);

		if (gLogToExternalConsole) LogWinConsole(outc, Length);
		if (gLogToFile) LogToFile(outc, Length);
	}

	void Log(const char* Formatted, int Length)
	{
		std::string out = Formatted;

		// UE4 Logging doesn't actually want a line break
		#ifndef UE4	
		if (Formatted[strlen(Formatted) - 1] != '\n') 
			out += "\n";
		#endif

		const char* outc = out.c_str();

		if (Length == 0) Length = strlen(outc);

		if (gLogToExternalConsole) LogWinConsole(outc, Length);

		if (gLogToFile) LogToFile(outc, Length);

		if (UnrealSDK::gameConsole != nullptr)
		{
			// It seems that Unreal will automatically put a newline on the end of a 
			// console output, but if there's already a \n at the end, then it won't
			// add this \n onto the end. So if we're printing just a \n by itself, 
			// just don't do anything.
			if (!(Length == 1))
			{
				std::wstring wfmt = Util::Widen(outc);
				const bool doInjectedNext = UnrealSDK::gInjectedCallNext;
				UnrealSDK::DoInjectedCallNext();

				#ifdef UE4
				if (UnrealSDK::gameConsole->Scrollback.Data != NULL) {
					UnrealSDK::gameConsole->OutputTextCpp((wchar_t*)wfmt.c_str());
				}
				#else
				UnrealSDK::gameConsole->OutputText(FString(wfmt.c_str()));
				#endif

				if (doInjectedNext) UnrealSDK::DoInjectedCallNext();
			}
		}
	}

	void LogW(wchar_t* Formatted, const signed int Length)
	{
		char* output = (char *)calloc(Length + 1, sizeof(char));
		size_t ret;
		wcstombs_s(&ret, output, Length, Formatted, Length);
		Log(output, 0);
	}

	void LogPy(std::string formatted)
	{
		Log(formatted.c_str(), 0);
	}

	void LogF(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		std::string formatted = Util::FormatInternal(fmt, args);
		va_end(args);

		Log(formatted.c_str(), formatted.length());
	}

	enum LogLevel
	{
		DEBUG,
		INFO,
		WARNING,
		EXCEPTION,
		CRITICAL
	};

	LogLevel Level = WARNING;

	void LogD(const char* fmt, ...)
	{
		if (Level == DEBUG)
		{
			va_list args;
			va_start(args, fmt);
			std::string formatted = "[DEBUG] " + Util::FormatInternal(fmt, args);
			va_end(args);

			Log(formatted.c_str(), formatted.length());
		}
	}

	void SetLoggingLevel(const char* NewLevel)
	{
		std::string str = NewLevel;
		std::transform(str.begin(), str.end(), str.begin(), toupper);
		if (str == "DEBUG")
		{
			Level = DEBUG;
		}
		else if (str == "INFO")
		{
			Level = INFO;
		}
		else if (str == "WARNING")
		{
			Level = WARNING;
		}
		else if (str == "EXCEPTION")
		{
			Level = EXCEPTION;
		}
		else if (str == "CRITICAL")
		{
			Level = CRITICAL;
		}
		else
		{
			LogF("Unknown logging level '%s'\n", NewLevel);
		}
	}

	void InitializeExtern()
	{
		BOOL result = AllocConsole();
		if (result)
		{
			gLogToExternalConsole = true;
		}
	}

	// Everything else can fail, but InitializeFile must work.
	void InitializeFile(const std::wstring& fileName)
	{
		gLogFile = CreateFile(fileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
		                     FILE_ATTRIBUTE_NORMAL, nullptr);
		if (gLogFile == INVALID_HANDLE_VALUE)
		{
			std::string errMsg = Util::Format("Failed to initialize log file (INVALID_HANDLE_VALUE, LastError = %d)",
			                                  GetLastError());
			throw FatalSDKException(1000, errMsg);
		}

		gLogToFile = true;
	}

	void PrintLogHeader()
	{
#ifndef UE4
		LogF("======== UnrealEngine PythonSDK Loaded (Version %d) ========\n", UnrealSDK::EngineVersion);
#else
		LogF("======== UnrealEngine PythonSDK Loaded (Version %s) ========\n", UnrealSDK::EngineBuild);
#endif
	}

	void Cleanup()
	{
		if (gLogFile != INVALID_HANDLE_VALUE)
		{
			FlushFileBuffers(gLogFile);
			CloseHandle(gLogFile);
		}
	}
}
