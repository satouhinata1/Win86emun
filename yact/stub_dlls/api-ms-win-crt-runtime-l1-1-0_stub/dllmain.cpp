#include <windows.h>

static HMODULE hMsvcrt = NULL;

// 関数ポインタ型定義
typedef int* (*fn___p___argc)(void);
typedef char*** (*fn___p___argv)(void);
typedef wchar_t*** (*fn___p___wargv)(void);
typedef int* (*fn___p___winitenv)(void);
typedef int* (*fn___p___initenv)(void);
typedef size_t* (*fn___p___mb_cur_max)(void);
typedef char** (*fn___p___pgmptr)(void);
typedef wchar_t** (*fn___p___wpgmptr)(void);
typedef int (*fn__configthreadlocale)(int);
typedef int (*fn__configure_narrow_argv)(int, char***);
typedef int (*fn__configure_wide_argv)(int, wchar_t***);
typedef int (*fn__crt_atexit)(void (*)(void));
typedef int (*fn__crt_at_quick_exit)(void (*)(void));
typedef int* (*fn__errno)(void);
typedef int (*fn__execute_onexit_table)(void*);
typedef void (__cdecl *fn__exit)(int);
typedef errno_t (*fn__get_doserrno)(unsigned long*);
typedef errno_t (*fn__get_errno)(int*);
typedef int (*fn__initialize_narrow_environment)(void);
typedef int (*fn__initialize_onexit_table)(void*);
typedef int (*fn__initialize_wide_environment)(void);
typedef void (*fn__initterm)(void*, void*);
typedef int (*fn__initterm_e)(void*, void*);
typedef int (*fn__register_onexit_function)(void*, void*);
typedef errno_t (*fn__set_doserrno)(unsigned long);
typedef int (*fn__set_errno)(int);
typedef void (__cdecl *fn_abort)(void);
typedef int (*fn_atexit)(void (*)(void));
typedef void (__cdecl *fn_exit)(int);
typedef void (__cdecl *fn_quick_exit)(int);

// 関数ポインタ変数
static fn___p___argc p___p___argc = NULL;
static fn___p___argv p___p___argv = NULL;
static fn___p___wargv p___p___wargv = NULL;
static fn___p___winitenv p___p___winitenv = NULL;
static fn___p___initenv p___p___initenv = NULL;
static fn___p___mb_cur_max p___p___mb_cur_max = NULL;
static fn___p___pgmptr p___p___pgmptr = NULL;
static fn___p___wpgmptr p___p___wpgmptr = NULL;
static fn__configthreadlocale p__configthreadlocale = NULL;
static fn__configure_narrow_argv p__configure_narrow_argv = NULL;
static fn__configure_wide_argv p__configure_wide_argv = NULL;
static fn__crt_atexit p__crt_atexit = NULL;
static fn__crt_at_quick_exit p__crt_at_quick_exit = NULL;
static fn__errno p__errno = NULL;
static fn__execute_onexit_table p__execute_onexit_table = NULL;
static fn__exit p__exit = NULL;
static fn__get_doserrno p__get_doserrno = NULL;
static fn__get_errno p__get_errno = NULL;
static fn__initialize_narrow_environment p__initialize_narrow_environment = NULL;
static fn__initialize_onexit_table p__initialize_onexit_table = NULL;
static fn__initialize_wide_environment p__initialize_wide_environment = NULL;
static fn__initterm p__initterm = NULL;
static fn__initterm_e p__initterm_e = NULL;
static fn__register_onexit_function p__register_onexit_function = NULL;
static fn__set_doserrno p__set_doserrno = NULL;
static fn__set_errno p__set_errno = NULL;
static fn_abort p_abort = NULL;
static fn_atexit p_atexit = NULL;
static fn_exit p_exit = NULL;
static fn_quick_exit p_quick_exit = NULL;

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
            p___p___argc = (fn___p___argc)GetMsvcrtFunc("__p___argc");
            p___p___argv = (fn___p___argv)GetMsvcrtFunc("__p___argv");
            p___p___wargv = (fn___p___wargv)GetMsvcrtFunc("__p___wargv");
            p___p___winitenv = (fn___p___winitenv)GetMsvcrtFunc("__p___winitenv");
            p___p___initenv = (fn___p___initenv)GetMsvcrtFunc("__p___initenv");
            p___p___mb_cur_max = (fn___p___mb_cur_max)GetMsvcrtFunc("__p___mb_cur_max");
            p___p___pgmptr = (fn___p___pgmptr)GetMsvcrtFunc("__p___pgmptr");
            p___p___wpgmptr = (fn___p___wpgmptr)GetMsvcrtFunc("__p___wpgmptr");
            p__configthreadlocale = (fn__configthreadlocale)GetMsvcrtFunc("_configthreadlocale");
            p__configure_narrow_argv = (fn__configure_narrow_argv)GetMsvcrtFunc("_configure_narrow_argv");
            p__configure_wide_argv = (fn__configure_wide_argv)GetMsvcrtFunc("_configure_wide_argv");
            p__crt_atexit = (fn__crt_atexit)GetMsvcrtFunc("_crt_atexit");
            p__crt_at_quick_exit = (fn__crt_at_quick_exit)GetMsvcrtFunc("_crt_at_quick_exit");
            p__errno = (fn__errno)GetMsvcrtFunc("_errno");
            p__execute_onexit_table = (fn__execute_onexit_table)GetMsvcrtFunc("_execute_onexit_table");
            p__exit = (fn__exit)GetMsvcrtFunc("_exit");
            p__get_doserrno = (fn__get_doserrno)GetMsvcrtFunc("_get_doserrno");
            p__get_errno = (fn__get_errno)GetMsvcrtFunc("_get_errno");
            p__initialize_narrow_environment = (fn__initialize_narrow_environment)GetMsvcrtFunc("_initialize_narrow_environment");
            p__initialize_onexit_table = (fn__initialize_onexit_table)GetMsvcrtFunc("_initialize_onexit_table");
            p__initialize_wide_environment = (fn__initialize_wide_environment)GetMsvcrtFunc("_initialize_wide_environment");
            p__initterm = (fn__initterm)GetMsvcrtFunc("_initterm");
            p__initterm_e = (fn__initterm_e)GetMsvcrtFunc("_initterm_e");
            p__register_onexit_function = (fn__register_onexit_function)GetMsvcrtFunc("_register_onexit_function");
            p__set_doserrno = (fn__set_doserrno)GetMsvcrtFunc("_set_doserrno");
            p__set_errno = (fn__set_errno)GetMsvcrtFunc("_set_errno");
            p_abort = (fn_abort)GetMsvcrtFunc("abort");
            p_atexit = (fn_atexit)GetMsvcrtFunc("atexit");
            p_exit = (fn_exit)GetMsvcrtFunc("exit");
            p_quick_exit = (fn_quick_exit)GetMsvcrtFunc("quick_exit");
        }
    }
    return TRUE;
}

// エクスポート関数実装（_imp_プレフィックスを付けて衝突回避）
extern "C" __declspec(dllexport) int* __p___argc(void) { return p___p___argc ? p___p___argc() : NULL; }
extern "C" __declspec(dllexport) char*** __p___argv(void) { return p___p___argv ? p___p___argv() : NULL; }
extern "C" __declspec(dllexport) wchar_t*** __p___wargv(void) { return p___p___wargv ? p___p___wargv() : NULL; }
extern "C" __declspec(dllexport) int* __p___winitenv(void) { return p___p___winitenv ? p___p___winitenv() : NULL; }
extern "C" __declspec(dllexport) int* __p___initenv(void) { return p___p___initenv ? p___p___initenv() : NULL; }
extern "C" __declspec(dllexport) size_t* __p___mb_cur_max(void) { return p___p___mb_cur_max ? p___p___mb_cur_max() : NULL; }
extern "C" __declspec(dllexport) char** __p___pgmptr(void) { return p___p___pgmptr ? p___p___pgmptr() : NULL; }
extern "C" __declspec(dllexport) wchar_t** __p___wpgmptr(void) { return p___p___wpgmptr ? p___p___wpgmptr() : NULL; }
extern "C" __declspec(dllexport) int _configthreadlocale(int a) { return p__configthreadlocale ? p__configthreadlocale(a) : 0; }
extern "C" __declspec(dllexport) int _configure_narrow_argv(int a, char*** b) { return p__configure_narrow_argv ? p__configure_narrow_argv(a, b) : 0; }
extern "C" __declspec(dllexport) int _configure_wide_argv(int a, wchar_t*** b) { return p__configure_wide_argv ? p__configure_wide_argv(a, b) : 0; }
extern "C" __declspec(dllexport) int _crt_atexit(void (*a)(void)) { return p__crt_atexit ? p__crt_atexit(a) : 0; }
extern "C" __declspec(dllexport) int _crt_at_quick_exit(void (*a)(void)) { return p__crt_at_quick_exit ? p__crt_at_quick_exit(a) : 0; }
extern "C" __declspec(dllexport) int* _errno(void) { return p__errno ? p__errno() : NULL; }
extern "C" __declspec(dllexport) int _execute_onexit_table(void* a) { return p__execute_onexit_table ? p__execute_onexit_table(a) : 0; }
extern "C" __declspec(dllexport) void _exit(int a) { if (p__exit) p__exit(a); for(;;); }
extern "C" __declspec(dllexport) int _get_doserrno(unsigned long* a) { return p__get_doserrno ? p__get_doserrno(a) : 0; }
extern "C" __declspec(dllexport) int _get_errno(int* a) { return p__get_errno ? p__get_errno(a) : 0; }
extern "C" __declspec(dllexport) int _initialize_narrow_environment(void) { return p__initialize_narrow_environment ? p__initialize_narrow_environment() : 0; }
extern "C" __declspec(dllexport) int _initialize_onexit_table(void* a) { return p__initialize_onexit_table ? p__initialize_onexit_table(a) : 0; }
extern "C" __declspec(dllexport) int _initialize_wide_environment(void) { return p__initialize_wide_environment ? p__initialize_wide_environment() : 0; }
extern "C" __declspec(dllexport) void _initterm(void* a, void* b) { if (p__initterm) p__initterm(a, b); }
extern "C" __declspec(dllexport) int _initterm_e(void* a, void* b) { return p__initterm_e ? p__initterm_e(a, b) : 0; }
extern "C" __declspec(dllexport) int _register_onexit_function(void* a, void* b) { return p__register_onexit_function ? p__register_onexit_function(a, b) : 0; }
extern "C" __declspec(dllexport) int _set_doserrno(unsigned long a) { return p__set_doserrno ? p__set_doserrno(a) : 0; }
extern "C" __declspec(dllexport) int _set_errno(int a) { return p__set_errno ? p__set_errno(a) : 0; }
extern "C" __declspec(dllexport) void my_abort(void) { if (p_abort) p_abort(); for(;;); }
extern "C" __declspec(dllexport) int my_atexit(void (*a)(void)) { return p_atexit ? p_atexit(a) : 0; }
extern "C" __declspec(dllexport) void my_exit(int a) { if (p_exit) p_exit(a); for(;;); }
extern "C" __declspec(dllexport) void my_quick_exit(int a) { if (p_quick_exit) p_quick_exit(a); for(;;); }
