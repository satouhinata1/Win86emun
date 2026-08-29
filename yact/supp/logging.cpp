// logging.cpp : Debug logging functionality for Win86emun
//

#include "stdafx.h"
#include "logging.h"
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

static const char* LOG_FILE_NAME = "win86emu.log";
static int LogFileHandle = 0;
static bool LogInitialized = false;
static CRITICAL_SECTION LogCriticalSection;

// Get current timestamp as formatted string
static void GetTimestamp(char* buffer, size_t bufferSize)
{
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, bufferSize, "%a %b %d %H:%M:%S %Y", timeinfo);
}

void InitDebugLog(void)
{
    if (LogInitialized)
        return;
    
    // Initialize CRITICAL_SECTION for thread-safe logging
    InitializeCriticalSection(&LogCriticalSection);
    
    EnterCriticalSection(&LogCriticalSection);
    
    // Open log file in append mode, create if not exists
    if (_sopen_s(&LogFileHandle, LOG_FILE_NAME, _O_RDWR | _O_APPEND | _O_CREAT, SH_DENYNO, S_IREAD | S_IWRITE))
    {
        LogFileHandle = 0;
        LeaveCriticalSection(&LogCriticalSection);
        return;
    }
    
    LogInitialized = true;
    
    // Write header with start timestamp
    char timestamp[64];
    GetTimestamp(timestamp, sizeof(timestamp));
    
    char header[256];
    sprintf(header, "=== Win86emun Debug Log ===\nStarted at: %s\n", timestamp);
    _write(LogFileHandle, header, (unsigned int)strlen(header));
    _commit(LogFileHandle);
    
    LeaveCriticalSection(&LogCriticalSection);
}

void ShutdownDebugLog(void)
{
    if (!LogInitialized || LogFileHandle == 0)
        return;
    
    EnterCriticalSection(&LogCriticalSection);
    
    // Write closing timestamp
    char timestamp[64];
    GetTimestamp(timestamp, sizeof(timestamp));
    
    char footer[128];
    sprintf(footer, "Closed at: %s\n", timestamp);
    _write(LogFileHandle, footer, (unsigned int)strlen(footer));
    _commit(LogFileHandle);
    
    _close(LogFileHandle);
    LogFileHandle = 0;
    LogInitialized = false;
    
    LeaveCriticalSection(&LogCriticalSection);
    DeleteCriticalSection(&LogCriticalSection);
}

void WriteDebugLog(const char* format, ...)
{
    if (!LogInitialized || LogFileHandle == 0)
        return;
    
    EnterCriticalSection(&LogCriticalSection);
    
    static char Buffer[2048];
    
    va_list args;
    va_start(args, format);
    vsnprintf(Buffer, sizeof(Buffer), format, args);
    va_end(args);
    
    // Add newline if not present
    size_t len = strlen(Buffer);
    if (len > 0 && Buffer[len - 1] != '\n')
    {
        strcat(Buffer, "\n");
        len++;
    }
    
    _write(LogFileHandle, Buffer, (unsigned int)len);
    _commit(LogFileHandle);
    
    LeaveCriticalSection(&LogCriticalSection);
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
