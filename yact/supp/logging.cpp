// logging.cpp : Debug logging functionality for Win86emun
//

#include "stdafx.h"
#include "logging.h"
#include <stdio.h>
#include <time.h>
#include <mutex>
#include <io.h>
#include <fcntl.h>
#include <share.h>

static const char* LOG_FILE_NAME = "win86emu.log";
static int LogFileHandle = 0;
static bool LogInitialized = false;
static std::mutex LogMutex;

// Get current timestamp as formatted string
static void GetTimestamp(char* buffer, size_t bufferSize)
{
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, bufferSize, "%a %b %d %H:%M:%S %Y", timeinfo);
}

void InitDebugLog(void)
{
    std::lock_guard<std::mutex> lock(LogMutex);
    
    if (LogInitialized)
        return;
    
    // Open log file in append mode, create if not exists
    if (_sopen_s(&LogFileHandle, LOG_FILE_NAME, _O_RDWR | _O_APPEND | _O_CREAT, SH_DENYNO, _S_IREAD | _S_IWRITE))
    {
        LogFileHandle = 0;
        return;
    }
    
    LogInitialized = true;
    
    // Write header with start timestamp
    char timestamp[64];
    GetTimestamp(timestamp, sizeof(timestamp));
    
    char header[256];
    sprintf_s(header, "=== Win86emun Debug Log ===\nStarted at: %s\n", timestamp);
    _write(LogFileHandle, header, strlen(header));
    _commit(LogFileHandle);
}

void ShutdownDebugLog(void)
{
    std::lock_guard<std::mutex> lock(LogMutex);
    
    if (!LogInitialized || LogFileHandle == 0)
        return;
    
    // Write closing timestamp
    char timestamp[64];
    GetTimestamp(timestamp, sizeof(timestamp));
    
    char footer[128];
    sprintf_s(footer, "Closed at: %s\n", timestamp);
    _write(LogFileHandle, footer, strlen(footer));
    _commit(LogFileHandle);
    
    _close(LogFileHandle);
    LogFileHandle = 0;
    LogInitialized = false;
}

void WriteDebugLog(const char* format, ...)
{
    std::lock_guard<std::mutex> lock(LogMutex);
    
    if (!LogInitialized || LogFileHandle == 0)
        return;
    
    static __declspec(thread) char Buffer[2048];
    
    va_list args;
    va_start(args, format);
    vsprintf_s(Buffer, sizeof(Buffer), format, args);
    va_end(args);
    
    // Add newline if not present
    size_t len = strlen(Buffer);
    if (len > 0 && Buffer[len - 1] != '\n')
    {
        strcat_s(Buffer, "\n");
        len++;
    }
    
    _write(LogFileHandle, Buffer, (unsigned int)len);
    _commit(LogFileHandle);
}

void LogDllLoadFailure(const char* dllName, DWORD errorCode)
{
    WriteDebugLog("[ERROR] Failed to load required DLL: %s (error %u)", dllName, errorCode);
}

void LogEmulationStart(DWORD entryPoint, int nParams)
{
    WriteDebugLog("[EXEC] Starting emulation at 0x%08X, NParams=%d", entryPoint, nParams);
}

void LogEmulationEnd(DWORD returnValue)
{
    WriteDebugLog("[EXEC] Returned: 0x%08X", returnValue);
}
