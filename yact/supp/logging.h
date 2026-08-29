// logging.h : Debug logging functionality for Win86emun
//

#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Export macro for supp.nt.dll
#ifdef SUPP_EXPORTS
#define SUPP_API __declspec(dllexport)
#else
#define SUPP_API __declspec(dllimport)
#endif

// Initialize logging subsystem (called during EmuInitialize)
SUPP_API void InitDebugLog(void);

// Shutdown logging subsystem (called during process detach)
SUPP_API void ShutdownDebugLog(void);

// Write formatted debug log message
SUPP_API void WriteDebugLog(const char* format, ...);

// Log DLL load failure
SUPP_API void LogDllLoadFailure(const char* dllName, DWORD errorCode);

// Log emulation start
SUPP_API void LogEmulationStart(DWORD entryPoint, int nParams);

// Log emulation end
SUPP_API void LogEmulationEnd(DWORD returnValue);

#ifdef __cplusplus
}
#endif
