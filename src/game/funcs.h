#pragma once

#include <stdio.h>

// CRT Internal Function Wrappers
inline int __cdecl __heap_init() {
    using Fn = int(__cdecl*)();
    return reinterpret_cast<Fn>(std::uintptr_t{ 0x005470e0 })();
}

inline void __cdecl __amsg_exit(int code) {
    using Fn = void(__cdecl*)(int);
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00542490 })(code);
}

inline int __cdecl __mtinit() {
    using Fn = int(__cdecl*)();
    return reinterpret_cast<Fn>(std::uintptr_t{ 0x00548520 })();
}

inline void __cdecl __ioinit() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00548340 })();
}

inline void __cdecl ___initmbctable() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00548330 })();
}

inline void* __cdecl ___crtGetEnvironmentStringsA() {
    using Fn = void* (__cdecl*)();
    return reinterpret_cast<Fn>(std::uintptr_t{ 0x00547e90 })();
}

inline void __cdecl __exit(int code) {
    using Fn = void(__cdecl*)(int);
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00541630 })(code);
}

inline void __cdecl __setargv() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00547c20 })();
}

inline void __setenvp() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00547b30 })();
}

inline void __cdecl __cinit() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00541600 })();
}

inline int __cdecl __ismbblead(uint32_t c) {
    using Fn = int(__cdecl*)(uint32_t);
    return reinterpret_cast<Fn>(std::uintptr_t{ 0x00547ad0 })(c);
}

// User-defined WinMain entry point
inline int __stdcall WinMain_Proxy(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd) {
    using Fn = int(__stdcall*)(void*, void*, char*, int);
    return reinterpret_cast<Fn>(std::uintptr_t{ 0x005277b0 })(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

inline char* __cdecl find_substring(const char* haystack, const char* needle) {
    using Fn = char* (__cdecl*)(const char*, const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00541b30 });
    return fn(haystack, needle);
}

inline void* __cdecl _malloc(size_t size) {
    using Fn = void* (__cdecl*)(size_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005421a0 });
    return fn(size);
}

inline void __cdecl _free(void* ptr) {
    using Fn = void(__cdecl*)(void*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00542270 });
    fn(ptr);
}

inline char* __cdecl __strlwr(char* str) {
    using Fn = char* (__cdecl*)(char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0054ecd0 });
    return fn(str);
}

inline int __cdecl _atoi(const char* str) {
    using Fn = int(__cdecl*)(const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005415a0 });
    return fn(str);
}

// Function wrappers
 // Suggested name: allocate_screen_buffers
inline void __cdecl FUN_528740(uint32_t stride, uint32_t height) {
    using Fn = void(__cdecl*)(uint32_t, uint32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528740 });
    fn(stride, height);
}

// Suggested name: cleanup_video
inline void __cdecl FUN_533630() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00533630 });
    fn();
}

// Suggested name: setup_screen_workspace
inline void __cdecl FUN_528680() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528680 });
    fn();
}

// Suggested name: setup_screen_workspace_alt
inline void __cdecl FUN_5286a0() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005286a0 });
    fn();
}

// Suggested name: init_video_subsystem_1
inline void __cdecl FUN_5287a0() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005287a0 });
    fn();
}

// Suggested name: init_video_subsystem_2
inline void __cdecl FUN_5287c0() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005287c0 });
    fn();
}

// Suggested name: final_video_setup
inline void __cdecl FUN_528c90() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528c90 });
    fn();
}

inline void __cdecl Fill_(short x1, short y1, short x2, short y2, short color) {
    using Fn = void(__cdecl*)(short, short, short, short, short);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528cc0 });
    fn(x1, y1, x2, y2, color);
}

inline HRESULT __stdcall DirectDrawCreate(GUID* lpGUID, LPDIRECTDRAW* lplpDD, IUnknown* pUnkOuter) {
    using Fn = HRESULT(__stdcall*)(GUID*, LPDIRECTDRAW*, IUnknown*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0053a8ca });
    return fn(lpGUID, lplpDD, pUnkOuter);
}

inline void __cdecl FUN_526F80() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526f80 });
    fn();
}

inline uintptr_t __cdecl begin_thread(void(__cdecl* start_address)(void*), unsigned stack_size, void* arglist) {
    using Fn = uintptr_t(__cdecl*)(void(__cdecl*)(void*), unsigned, void*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005424C0 });
    return fn(start_address, stack_size, arglist);
}

// Suggested name: Release_Resources
inline void __cdecl FUN_00521080() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00521080 });
    fn();
}

// Suggested name: Reset_Graphics_State
inline void __cdecl FUN_0052d290() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052d290 });
    fn();
}

// Suggested name: Finalize_System_Shutdown
inline void __cdecl FUN_00533630() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00533630 });
    fn();
}

inline FILE* __cdecl FID_conflict_wfopen(const char* filename, const char* mode) {
    using Fn = FILE * (__cdecl*)(const char*, const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540c00 });
    return fn(filename, mode);
}

inline uint32_t __cdecl _fread(void* buffer, uint32_t size, uint32_t count, FILE* stream) {
    using Fn = uint32_t(__cdecl*)(void*, uint32_t, uint32_t, FILE*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540a30 });
    return fn(buffer, size, count, stream);
}

inline uint32_t __cdecl _fwrite(const void* ptr, uint32_t size, uint32_t nmemb, FILE* stream) {
    using Fn = uint32_t(__cdecl*)(const void*, uint32_t, uint32_t, FILE*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540860 });
    return fn(ptr, size, nmemb, stream);
}

inline int32_t __cdecl _fclose(FILE* stream) {
    using Fn = int32_t(__cdecl*)(FILE*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005405f0 });
    return fn(stream);
}

inline int __cdecl _fflush(FILE* stream) {
    using Fn = int(__cdecl*)(FILE*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540690 });
    return fn(stream);
}

inline char* __cdecl _fgets(char* str, int num, FILE* stream) {
    using Fn = char*(__cdecl*)(char*, int, FILE*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00541BB0 });
    return fn(str, num, stream);
}

inline FILE* __cdecl _wfopen(const char* filename, const char* mode) {
    using Fn = FILE*(__cdecl*)(const char*, const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540C00 });
    return fn(filename, mode);
}

inline int __cdecl _sprintf(char* o, const char* s, ...)
{
    using VFn = int(__cdecl*)(char*, const char*, va_list);
    auto vfn = reinterpret_cast<VFn>(std::uintptr_t{ 0x00540550 });

    va_list ap;
    va_start(ap, s);
    int rc = vfn(o, s, ap);
    va_end(ap);

    return rc;
}

inline int __cdecl _clock() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540F20 });
    return fn();
}

inline void __cdecl _memcpy(void* dest, const void* src, uint32_t n) {
    using Fn = void* (__cdecl*)(void*, const void*, uint32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540f70 });
    fn(dest, src, n);
}

inline void __cdecl Mox_Update_() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x004d7820 })();
}

inline void __cdecl FUN_00443C60() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00443C60 })();
}

// Suggested name: s_Null_Pointer
inline const char* const s_Null_Pointer_0055b658 =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B658 }); // "Null Pointer"

// Suggested name: s_allocation_error_fmt
inline const char* const s_Couldn_t_Allocate_0055B668 =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B668 }); // "Couldn't Allocate %d. Increase Virtual Memory."

inline const char* const s_Unable_to_Allocate_bytes =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B690 }); // "Unable to Allocate %d bytes (%s, %d)\n"

// Suggested name: g_mouse_buttons_state
inline uint16_t& g_mouse_buttons_state =
    *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578DE0 });

inline int __cdecl __strcmpi(const char* s1, const char* s2) {
    using Fn = int(__cdecl*)(const char*, const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005428d0 });
    return fn(s1, s2);
}

inline int __cdecl _fseek(FILE* stream, long offset, int origin) {
    using Fn = int(__cdecl*)(FILE*, long, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00541770 });
    return fn(stream, offset, origin);
}

inline const char* s_rb_0055117c = reinterpret_cast<const char*>(0x0055117C); // "rb"
inline const char* s_wb_00551178 = reinterpret_cast<const char*>(std::uintptr_t{ 0x00551178 }); // "wb"
inline const char* s_mox_set_0055760c = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055760C }); // "mox.set"
inline const char* s_Unable_to_open_mox_set_005575f4 = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575F4 }); // "Unable to open mox.set"


