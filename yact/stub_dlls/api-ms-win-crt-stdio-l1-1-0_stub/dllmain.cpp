#include <windows.h>
#include <cstdio>

static HMODULE hMsvcrt = NULL;

typedef FILE* (*fn__acrt_iob_func)(unsigned int);
typedef int (*fn__fcloseall)(void);
typedef FILE* (*fn__fdopen)(int, const char*);
typedef int (*fn__fgetchar)(void);
typedef wint_t (*fn__fgetwchar)(void);
typedef int (*fn__fileno)(FILE*);
typedef int (*fn__flushall)(void);
typedef int (*fn__fputchar)(int);
typedef wint_t (*fn__fputwchar)(wchar_t);
typedef int (*fn__fseeki64)(FILE*, __int64, int);
typedef __int64 (*fn__ftelli64)(FILE*);
typedef int (*fn__getw)(FILE*);
typedef int (*fn__putw)(int, FILE*);
typedef int (*fn__ungetch)(int);
typedef wint_t (*fn__ungetwch)(wint_t);
typedef void (*fn_clearerr)(FILE*);
typedef int (*fn_fclose)(FILE*);
typedef int (*fn_feof)(FILE*);
typedef int (*fn_ferror)(FILE*);
typedef int (*fn_fflush)(FILE*);
typedef int (*fn_fgetc)(FILE*);
typedef int (*fn_fgetpos)(FILE*, fpos_t*);
typedef char* (*fn_fgets)(char*, int, FILE*);
typedef FILE* (*fn_fopen)(const char*, const char*);
typedef int (*fn_fprintf_fn)(FILE*, const char*, ...);
typedef int (*fn_fputc)(int, FILE*);
typedef int (*fn_fputs)(const char*, FILE*);
typedef size_t (*fn_fread)(void*, size_t, size_t, FILE*);
typedef FILE* (*fn_freopen)(const char*, const char*, FILE*);
typedef int (*fn_fscanf_fn)(FILE*, const char*, ...);
typedef int (*fn_fseek)(FILE*, long, int);
typedef int (*fn_fsetpos)(FILE*, const fpos_t*);
typedef long (*fn_ftell)(FILE*);
typedef size_t (*fn_fwrite)(const void*, size_t, size_t, FILE*);
typedef int (*fn_getc)(FILE*);
typedef int (*fn_getchar)(void);
typedef char* (*fn_gets)(char*);
typedef void (*fn_perror)(const char*);
typedef int (*fn_printf_fn)(const char*, ...);
typedef int (*fn_putc)(int, FILE*);
typedef int (*fn_putchar)(int);
typedef int (*fn_puts)(const char*);
typedef int (*fn_remove)(const char*);
typedef int (*fn_rename)(const char*, const char*);
typedef void (*fn_rewind)(FILE*);
typedef int (*fn_scanf_fn)(const char*, ...);
typedef void (*fn_setbuf)(FILE*, char*);
typedef int (*fn_setvbuf)(FILE*, char*, int, size_t);
typedef int (*fn_snprintf_fn)(char*, size_t, const char*, ...);
typedef int (*fn_sprintf_fn)(char*, const char*, ...);
typedef int (*fn_sscanf_fn)(const char*, const char*, ...);
typedef FILE* (*fn_tmpfile)(void);
typedef char* (*fn_tmpnam)(char*);
typedef int (*fn_ungetc)(int, FILE*);
typedef int (*fn_vfprintf)(FILE*, const char*, void*);
typedef int (*fn_vfscanf)(FILE*, const char*, void*);
typedef int (*fn_vprintf)(const char*, void*);
typedef int (*fn_vscanf)(const char*, void*);
typedef int (*fn_vsnprintf)(char*, size_t, const char*, void*);
typedef int (*fn_vsprintf)(char*, const char*, void*);
typedef int (*fn_vsscanf)(const char*, const char*, void*);

static fn__acrt_iob_func p__acrt_iob_func = NULL;
static fn__fcloseall p__fcloseall = NULL;
static fn__fdopen p__fdopen = NULL;
static fn__fgetchar p__fgetchar = NULL;
static fn__fgetwchar p__fgetwchar = NULL;
static fn__fileno p__fileno = NULL;
static fn__flushall p__flushall = NULL;
static fn__fputchar p__fputchar = NULL;
static fn__fputwchar p__fputwchar = NULL;
static fn__fseeki64 p__fseeki64 = NULL;
static fn__ftelli64 p__ftelli64 = NULL;
static fn__getw p__getw = NULL;
static fn__putw p__putw = NULL;
static fn__ungetch p__ungetch = NULL;
static fn__ungetwch p__ungetwch = NULL;
static fn_clearerr p_clearerr = NULL;
static fn_fclose p_fclose = NULL;
static fn_feof p_feof = NULL;
static fn_ferror p_ferror = NULL;
static fn_fflush p_fflush = NULL;
static fn_fgetc p_fgetc = NULL;
static fn_fgetpos p_fgetpos = NULL;
static fn_fgets p_fgets = NULL;
static fn_fopen p_fopen = NULL;
static fn_fprintf_fn p_fprintf_fn_ptr = NULL;
static fn_fputc p_fputc = NULL;
static fn_fputs p_fputs = NULL;
static fn_fread p_fread = NULL;
static fn_freopen p_freopen = NULL;
static fn_fscanf_fn p_fscanf_fn_ptr = NULL;
static fn_fseek p_fseek = NULL;
static fn_fsetpos p_fsetpos = NULL;
static fn_ftell p_ftell = NULL;
static fn_fwrite p_fwrite = NULL;
static fn_getc p_getc = NULL;
static fn_getchar p_getchar = NULL;
static fn_gets p_gets = NULL;
static fn_perror p_perror = NULL;
static fn_printf_fn p_printf_fn_ptr = NULL;
static fn_putc p_putc = NULL;
static fn_putchar p_putchar = NULL;
static fn_puts p_puts = NULL;
static fn_remove p_remove = NULL;
static fn_rename p_rename = NULL;
static fn_rewind p_rewind = NULL;
static fn_scanf_fn p_scanf_fn_ptr = NULL;
static fn_setbuf p_setbuf = NULL;
static fn_setvbuf p_setvbuf = NULL;
static fn_snprintf_fn p_snprintf_fn_ptr = NULL;
static fn_sprintf_fn p_sprintf_fn_ptr = NULL;
static fn_sscanf_fn p_sscanf_fn_ptr = NULL;
static fn_tmpfile p_tmpfile = NULL;
static fn_tmpnam p_tmpnam = NULL;
static fn_ungetc p_ungetc = NULL;
static fn_vfprintf p_vfprintf = NULL;
static fn_vfscanf p_vfscanf = NULL;
static fn_vprintf p_vprintf = NULL;
static fn_vscanf p_vscanf = NULL;
static fn_vsnprintf p_vsnprintf = NULL;
static fn_vsprintf p_vsprintf = NULL;
static fn_vsscanf p_vsscanf = NULL;

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
            p__acrt_iob_func = (fn__acrt_iob_func)GetMsvcrtFunc("__acrt_iob_func");
            p__fcloseall = (fn__fcloseall)GetMsvcrtFunc("_fcloseall");
            p__fdopen = (fn__fdopen)GetMsvcrtFunc("_fdopen");
            p__fgetchar = (fn__fgetchar)GetMsvcrtFunc("_fgetchar");
            p__fgetwchar = (fn__fgetwchar)GetMsvcrtFunc("_fgetwchar");
            p__fileno = (fn__fileno)GetMsvcrtFunc("_fileno");
            p__flushall = (fn__flushall)GetMsvcrtFunc("_flushall");
            p__fputchar = (fn__fputchar)GetMsvcrtFunc("_fputchar");
            p__fputwchar = (fn__fputwchar)GetMsvcrtFunc("_fputwchar");
            p__fseeki64 = (fn__fseeki64)GetMsvcrtFunc("_fseeki64");
            p__ftelli64 = (fn__ftelli64)GetMsvcrtFunc("_ftelli64");
            p__getw = (fn__getw)GetMsvcrtFunc("_getw");
            p__putw = (fn__putw)GetMsvcrtFunc("_putw");
            p__ungetch = (fn__ungetch)GetMsvcrtFunc("_ungetch");
            p__ungetwch = (fn__ungetwch)GetMsvcrtFunc("_ungetwch");
            p_clearerr = (fn_clearerr)GetMsvcrtFunc("clearerr");
            p_fclose = (fn_fclose)GetMsvcrtFunc("fclose");
            p_feof = (fn_feof)GetMsvcrtFunc("feof");
            p_ferror = (fn_ferror)GetMsvcrtFunc("ferror");
            p_fflush = (fn_fflush)GetMsvcrtFunc("fflush");
            p_fgetc = (fn_fgetc)GetMsvcrtFunc("fgetc");
            p_fgetpos = (fn_fgetpos)GetMsvcrtFunc("fgetpos");
            p_fgets = (fn_fgets)GetMsvcrtFunc("fgets");
            p_fopen = (fn_fopen)GetMsvcrtFunc("fopen");
            p_fprintf_fn_ptr = (fn_fprintf_fn)GetMsvcrtFunc("fprintf");
            p_fputc = (fn_fputc)GetMsvcrtFunc("fputc");
            p_fputs = (fn_fputs)GetMsvcrtFunc("fputs");
            p_fread = (fn_fread)GetMsvcrtFunc("fread");
            p_freopen = (fn_freopen)GetMsvcrtFunc("freopen");
            p_fscanf_fn_ptr = (fn_fscanf_fn)GetMsvcrtFunc("fscanf");
            p_fseek = (fn_fseek)GetMsvcrtFunc("fseek");
            p_fsetpos = (fn_fsetpos)GetMsvcrtFunc("fsetpos");
            p_ftell = (fn_ftell)GetMsvcrtFunc("ftell");
            p_fwrite = (fn_fwrite)GetMsvcrtFunc("fwrite");
            p_getc = (fn_getc)GetMsvcrtFunc("getc");
            p_getchar = (fn_getchar)GetMsvcrtFunc("getchar");
            p_gets = (fn_gets)GetMsvcrtFunc("gets");
            p_perror = (fn_perror)GetMsvcrtFunc("perror");
            p_printf_fn_ptr = (fn_printf_fn)GetMsvcrtFunc("printf");
            p_putc = (fn_putc)GetMsvcrtFunc("putc");
            p_putchar = (fn_putchar)GetMsvcrtFunc("putchar");
            p_puts = (fn_puts)GetMsvcrtFunc("puts");
            p_remove = (fn_remove)GetMsvcrtFunc("remove");
            p_rename = (fn_rename)GetMsvcrtFunc("rename");
            p_rewind = (fn_rewind)GetMsvcrtFunc("rewind");
            p_scanf_fn_ptr = (fn_scanf_fn)GetMsvcrtFunc("scanf");
            p_setbuf = (fn_setbuf)GetMsvcrtFunc("setbuf");
            p_setvbuf = (fn_setvbuf)GetMsvcrtFunc("setvbuf");
            p_snprintf_fn_ptr = (fn_snprintf_fn)GetMsvcrtFunc("snprintf");
            p_sprintf_fn_ptr = (fn_sprintf_fn)GetMsvcrtFunc("sprintf");
            p_sscanf_fn_ptr = (fn_sscanf_fn)GetMsvcrtFunc("sscanf");
            p_tmpfile = (fn_tmpfile)GetMsvcrtFunc("tmpfile");
            p_tmpnam = (fn_tmpnam)GetMsvcrtFunc("tmpnam");
            p_ungetc = (fn_ungetc)GetMsvcrtFunc("ungetc");
            p_vfprintf = (fn_vfprintf)GetMsvcrtFunc("vfprintf");
            p_vfscanf = (fn_vfscanf)GetMsvcrtFunc("vfscanf");
            p_vprintf = (fn_vprintf)GetMsvcrtFunc("vprintf");
            p_vscanf = (fn_vscanf)GetMsvcrtFunc("vscanf");
            p_vsnprintf = (fn_vsnprintf)GetMsvcrtFunc("vsnprintf");
            p_vsprintf = (fn_vsprintf)GetMsvcrtFunc("vsprintf");
            p_vsscanf = (fn_vsscanf)GetMsvcrtFunc("vsscanf");
        }
    }
    return TRUE;
}

extern "C" __declspec(dllexport) FILE* my_acrt_iob_func(unsigned int a) { return p__acrt_iob_func ? p__acrt_iob_func(a) : NULL; }
extern "C" __declspec(dllexport) int _fcloseall(void) { return p__fcloseall ? p__fcloseall() : 0; }
extern "C" __declspec(dllexport) FILE* _fdopen(int a, const char* b) { return p__fdopen ? p__fdopen(a, b) : NULL; }
extern "C" __declspec(dllexport) int _fgetchar(void) { return p__fgetchar ? p__fgetchar() : 0; }
extern "C" __declspec(dllexport) wint_t _fgetwchar(void) { return p__fgetwchar ? p__fgetwchar() : 0; }
extern "C" __declspec(dllexport) int _fileno(FILE* a) { return p__fileno ? p__fileno(a) : -1; }
extern "C" __declspec(dllexport) int _flushall(void) { return p__flushall ? p__flushall() : 0; }
extern "C" __declspec(dllexport) int _fputchar(int a) { return p__fputchar ? p__fputchar(a) : 0; }
extern "C" __declspec(dllexport) wint_t _fputwchar(wchar_t a) { return p__fputwchar ? p__fputwchar(a) : 0; }
extern "C" __declspec(dllexport) int _fseeki64(FILE* a, __int64 b, int c) { return p__fseeki64 ? p__fseeki64(a, b, c) : 0; }
extern "C" __declspec(dllexport) __int64 _ftelli64(FILE* a) { return p__ftelli64 ? p__ftelli64(a) : 0; }
extern "C" __declspec(dllexport) int _getw(FILE* a) { return p__getw ? p__getw(a) : 0; }
extern "C" __declspec(dllexport) int _putw(int a, FILE* b) { return p__putw ? p__putw(a, b) : 0; }
extern "C" __declspec(dllexport) int _ungetch(int a) { return p__ungetch ? p__ungetch(a) : 0; }
extern "C" __declspec(dllexport) wint_t _ungetwch(wint_t a) { return p__ungetwch ? p__ungetwch(a) : 0; }
extern "C" __declspec(dllexport) void clearerr(FILE* a) { if (p_clearerr) p_clearerr(a); }
extern "C" __declspec(dllexport) int fclose(FILE* a) { return p_fclose ? p_fclose(a) : 0; }
extern "C" __declspec(dllexport) int feof(FILE* a) { return p_feof ? p_feof(a) : 0; }
extern "C" __declspec(dllexport) int ferror(FILE* a) { return p_ferror ? p_ferror(a) : 0; }
extern "C" __declspec(dllexport) int fflush(FILE* a) { return p_fflush ? p_fflush(a) : 0; }
extern "C" __declspec(dllexport) int fgetc(FILE* a) { return p_fgetc ? p_fgetc(a) : EOF; }
extern "C" __declspec(dllexport) int fgetpos(FILE* a, fpos_t* b) { return p_fgetpos ? p_fgetpos(a, b) : 0; }
extern "C" __declspec(dllexport) char* fgets(char* a, int b, FILE* c) { return p_fgets ? p_fgets(a, b, c) : NULL; }
extern "C" __declspec(dllexport) FILE* fopen(const char* a, const char* b) { return p_fopen ? p_fopen(a, b) : NULL; }
extern "C" __declspec(dllexport) int fputc(int a, FILE* b) { return p_fputc ? p_fputc(a, b) : EOF; }
extern "C" __declspec(dllexport) int fputs(const char* a, FILE* b) { return p_fputs ? p_fputs(a, b) : EOF; }
extern "C" __declspec(dllexport) size_t fread(void* a, size_t b, size_t c, FILE* d) { return p_fread ? p_fread(a, b, c, d) : 0; }
extern "C" __declspec(dllexport) FILE* freopen(const char* a, const char* b, FILE* c) { return p_freopen ? p_freopen(a, b, c) : NULL; }
extern "C" __declspec(dllexport) int fseek(FILE* a, long b, int c) { return p_fseek ? p_fseek(a, b, c) : 0; }
extern "C" __declspec(dllexport) int fsetpos(FILE* a, const fpos_t* b) { return p_fsetpos ? p_fsetpos(a, b) : 0; }
extern "C" __declspec(dllexport) long ftell(FILE* a) { return p_ftell ? p_ftell(a) : 0; }
extern "C" __declspec(dllexport) size_t fwrite(const void* a, size_t b, size_t c, FILE* d) { return p_fwrite ? p_fwrite(a, b, c, d) : 0; }
extern "C" __declspec(dllexport) int getc(FILE* a) { return p_getc ? p_getc(a) : EOF; }
extern "C" __declspec(dllexport) int getchar(void) { return p_getchar ? p_getchar() : EOF; }
extern "C" __declspec(dllexport) char* gets(char* a) { return p_gets ? p_gets(a) : NULL; }
extern "C" __declspec(dllexport) void perror(const char* a) { if (p_perror) p_perror(a); }
extern "C" __declspec(dllexport) int putc(int a, FILE* b) { return p_putc ? p_putc(a, b) : EOF; }
extern "C" __declspec(dllexport) int putchar(int a) { return p_putchar ? p_putchar(a) : EOF; }
extern "C" __declspec(dllexport) int puts(const char* a) { return p_puts ? p_puts(a) : EOF; }
extern "C" __declspec(dllexport) int remove(const char* a) { return p_remove ? p_remove(a) : 0; }
extern "C" __declspec(dllexport) int rename(const char* a, const char* b) { return p_rename ? p_rename(a, b) : 0; }
extern "C" __declspec(dllexport) void rewind(FILE* a) { if (p_rewind) p_rewind(a); }
extern "C" __declspec(dllexport) void setbuf(FILE* a, char* b) { if (p_setbuf) p_setbuf(a, b); }
extern "C" __declspec(dllexport) int setvbuf(FILE* a, char* b, int c, size_t d) { return p_setvbuf ? p_setvbuf(a, b, c, d) : 0; }
extern "C" __declspec(dllexport) FILE* tmpfile(void) { return p_tmpfile ? p_tmpfile() : NULL; }
extern "C" __declspec(dllexport) char* tmpnam(char* a) { return p_tmpnam ? p_tmpnam(a) : NULL; }
extern "C" __declspec(dllexport) int ungetc(int a, FILE* b) { return p_ungetc ? p_ungetc(a, b) : EOF; }
extern "C" __declspec(dllexport) int vfprintf(FILE* a, const char* b, void* c) { return p_vfprintf ? p_vfprintf(a, b, c) : 0; }
extern "C" __declspec(dllexport) int vfscanf(FILE* a, const char* b, void* c) { return p_vfscanf ? p_vfscanf(a, b, c) : 0; }
extern "C" __declspec(dllexport) int vprintf(const char* a, void* b) { return p_vprintf ? p_vprintf(a, b) : 0; }
extern "C" __declspec(dllexport) int vscanf(const char* a, void* b) { return p_vscanf ? p_vscanf(a, b) : 0; }
extern "C" __declspec(dllexport) int vsnprintf(char* a, size_t b, const char* c, void* d) { return p_vsnprintf ? p_vsnprintf(a, b, c, d) : 0; }
extern "C" __declspec(dllexport) int vsprintf(char* a, const char* b, void* c) { return p_vsprintf ? p_vsprintf(a, b, c) : 0; }
extern "C" __declspec(dllexport) int vsscanf(const char* a, const char* b, void* c) { return p_vsscanf ? p_vsscanf(a, b, c) : 0; }
