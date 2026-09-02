// dllmain.cpp - Stub DLL for api-ms-win-crt-time-l1-1-0.dll
// Provides time-related CRT functions for x86 emulation on Windows RT

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// Timezone data exports (data symbols, not functions)
// These are exported as both _timezone and __timezone for compatibility
long _timezone = 0;
long __timezone = 0;
int _daylight = 0;
int __daylight = 0;
char* _tzname[2] = {nullptr, nullptr};
char* __tzname[2] = {nullptr, nullptr};
