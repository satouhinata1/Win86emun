#include <windows.h>

static HMODULE hMsvcrt = NULL;

// 関数ポインタ型定義
typedef errno_t (*fn__dupenv_s)(char**, size_t*, const char*);
typedef errno_t (*fn__wdupenv_s)(wchar_t**, size_t*, const wchar_t*);
typedef char* (*fn_getenv)(const char*);
typedef int (*fn_putenv)(const char*);
typedef wchar_t* (*fn_wgetenv)(const wchar_t*);
typedef int (*fn__putenv)(const char*);
typedef int (*fn__wputenv)(const wchar_t*);
typedef void (*fn__searchenv)(const char*, const char*, char*);
typedef void (*fn__wsearchenv)(const wchar_t*, const wchar_t*, wchar_t*);
typedef errno_t (*fn__dupenv_s_dbg)(char**, size_t*, const char*, int, const char*);
typedef errno_t (*fn__wdupenv_s_dbg)(wchar_t**, size_t*, const wchar_t*, int, const wchar_t*);
typedef char** (*fn___p__environ)(void);
typedef wchar_t** (*fn___p__wenviron)(void);

// 関数ポインタ変数
static fn__dupenv_s p__dupenv_s = NULL;
static fn__wdupenv_s p__wdupenv_s = NULL;
static fn_getenv p_getenv = NULL;
static fn_putenv p_putenv = NULL;
static fn_wgetenv p_wgetenv = NULL;
static fn__putenv p__putenv = NULL;
static fn__wputenv p__wputenv = NULL;
static fn__searchenv p__searchenv = NULL;
static fn__wsearchenv p__wsearchenv = NULL;
static fn__dupenv_s_dbg p__dupenv_s_dbg = NULL;
static fn__wdupenv_s_dbg p__wdupenv_s_dbg = NULL;
static fn___p__environ p___p__environ = NULL;
static fn___p__wenviron p___p__wenviron = NULL;

static FARPROC GetMsvcrtFunc(const char* name) {
    if (!hMsvcrt) return NULL;
    return GetProcAddress(hMsvcrt, name);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    (void)lpReserved;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hMsvcrt = GetModuleHandleA("msvcrt.dll");
        if (!hMsvcrt) hMsvcrt = GetModuleHandleA("msvcrt.DLL");
        if (hMsvcrt) {
            p__dupenv_s = (fn__dupenv_s)GetMsvcrtFunc("_dupenv_s");
            p__wdupenv_s = (fn__wdupenv_s)GetMsvcrtFunc("_wdupenv_s");
            p_getenv = (fn_getenv)GetMsvcrtFunc("getenv");
            p_putenv = (fn_putenv)GetMsvcrtFunc("putenv");
            p_wgetenv = (fn_wgetenv)GetMsvcrtFunc("wgetenv");
            p__putenv = (fn__putenv)GetMsvcrtFunc("_putenv");
            p__wputenv = (fn__wputenv)GetMsvcrtFunc("_wputenv");
            p__searchenv = (fn__searchenv)GetMsvcrtFunc("_searchenv");
            p__wsearchenv = (fn__wsearchenv)GetMsvcrtFunc("_wsearchenv");
            p__dupenv_s_dbg = (fn__dupenv_s_dbg)GetMsvcrtFunc("_dupenv_s_dbg");
            p__wdupenv_s_dbg = (fn__wdupenv_s_dbg)GetMsvcrtFunc("_wdupenv_s_dbg");
            p___p__environ = (fn___p__environ)GetMsvcrtFunc("__p__environ");
            p___p__wenviron = (fn___p__wenviron)GetMsvcrtFunc("__p__wenviron");
        }
    }
    return TRUE;
}

// エクスポート関数実装
extern "C" __declspec(dllexport) errno_t _dupenv_s(char** buf, size_t* size, const char* name) { return p__dupenv_s ? p__dupenv_s(buf, size, name) : 0; }
extern "C" __declspec(dllexport) errno_t _wdupenv_s(wchar_t** buf, size_t* size, const wchar_t* name) { return p__wdupenv_s ? p__wdupenv_s(buf, size, name) : 0; }
extern "C" __declspec(dllexport) char* getenv(const char* name) { return p_getenv ? p_getenv(name) : NULL; }
extern "C" __declspec(dllexport) int putenv(const char* str) { return p_putenv ? p_putenv(str) : 0; }
extern "C" __declspec(dllexport) wchar_t* wgetenv(const wchar_t* name) { return p_wgetenv ? p_wgetenv(name) : NULL; }
extern "C" __declspec(dllexport) int _putenv(const char* str) { return p__putenv ? p__putenv(str) : 0; }
extern "C" __declspec(dllexport) int _wputenv(const wchar_t* str) { return p__wputenv ? p__wputenv(str) : 0; }
extern "C" __declspec(dllexport) void _searchenv(const char* file, const char* var, char* buf) { if (p__searchenv) p__searchenv(file, var, buf); }
extern "C" __declspec(dllexport) void _wsearchenv(const wchar_t* file, const wchar_t* var, wchar_t* buf) { if (p__wsearchenv) p__wsearchenv(file, var, buf); }
extern "C" __declspec(dllexport) errno_t _dupenv_s_dbg(char** buf, size_t* size, const char* name, int dbg, const char* file) { return p__dupenv_s_dbg ? p__dupenv_s_dbg(buf, size, name, dbg, file) : 0; }
extern "C" __declspec(dllexport) errno_t _wdupenv_s_dbg(wchar_t** buf, size_t* size, const wchar_t* name, int dbg, const wchar_t* file) { return p__wdupenv_s_dbg ? p__wdupenv_s_dbg(buf, size, name, dbg, file) : 0; }
extern "C" __declspec(dllexport) char** __p__environ(void) { return p___p__environ ? p___p__environ() : NULL; }
extern "C" __declspec(dllexport) wchar_t** __p__wenviron(void) { return p___p__wenviron ? p___p__wenviron() : NULL; }
