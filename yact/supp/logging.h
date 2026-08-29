// logging.h : Debug logging functionality for Win86emun
//

#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize logging subsystem (called during EmuInitialize)
void InitDebugLog(void);

// Shutdown logging subsystem (called during process detach)
void ShutdownDebugLog(void);

// Write formatted debug log message
void WriteDebugLog(const char* format, ...);

// Log DLL load failure
void LogDllLoadFailure(const char* dllName, DWORD errorCode);

// Log emulation start
void LogEmulationStart(DWORD entryPoint, int nParams);

// Log emulation end
void LogEmulationEnd(DWORD returnValue);

#ifdef __cplusplus
}
#endif
