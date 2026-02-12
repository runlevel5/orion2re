#pragma once

#include <stdio.h>

inline uint32_t& DAT_00565e20 = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565E20});

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
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00541630 });
    fn(code);
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
    using Fn = int(__cdecl*)(uint32_t c);
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

inline uint32_t __cdecl _time(uint32_t* timer) {
    using Fn = uint32_t(__cdecl*)(uint32_t*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00541a20});
    return fn(timer);
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

inline int __cdecl hgetch_() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AB400});
    return fn();
}

inline int16_t __cdecl Mouse_Button_() {
    using Fn = int16_t(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00527680});
    return fn();
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

inline void __cdecl FUN_0052b660() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052b660 });
    fn();
}

inline uintptr_t __cdecl begin_thread(void(__cdecl* start_address)(void*), unsigned stack_size, void* arglist) {
    using Fn = uintptr_t(__cdecl*)(void(__cdecl*)(void*), unsigned, void*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005424C0 });
    return fn(start_address, stack_size, arglist);
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

inline auto _sprintf =
    reinterpret_cast<int(__cdecl*)(char*, const char*, ...)>(std::uintptr_t{ 0x00540550 });

inline uint8_t __cdecl Hire_Officer_Popup_(int16_t player_num, int16_t officer_id, char* message) {
    using Fn = uint8_t(__cdecl*)(int16_t, int16_t, char*);
    return reinterpret_cast<Fn>(std::uintptr_t{0x0045A680})(player_num, officer_id, message);
}

inline int __cdecl _clock() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540F20 });
    return fn();
}

inline char* __cdecl __itoa(int value, char* str, int radix) {
    using Fn = char*(__cdecl*)(int, char*, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00542d10});
    return fn(value, str, radix);
}

inline void __cdecl Reload_Language_Strings_() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{0x004f2470})();
}

inline void __cdecl Write_Language_Setting_File_() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{0x004f25c0})();
}

inline void __cdecl Get_File_Date_String_(const char* filename, char* date_out) {
    using Fn = void(__cdecl*)(const char*, char*);
    reinterpret_cast<Fn>(std::uintptr_t{0x0050f650})(filename, date_out);
}

inline void __cdecl Wrapper_For_Normal_System_Fields_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00497540 });
    fn();
}

inline int32_t __cdecl Get_Advanced_Government_Type_(int16_t player_idx) {
    using Fn = int32_t(__cdecl*)(int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043dea0});
    return fn(player_idx);
}

inline void __cdecl _memcpy(void* dest, const void* src, uint32_t n) {
    using Fn = void* (__cdecl*)(void*, const void*, uint32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540f70 });
    fn(dest, src, n);
}

inline void __cdecl _qsort(void* base, uint32_t num, uint32_t size, int(__cdecl* compar)(const void*, const void*)) {
    using Fn = void(__cdecl*)(void*, uint32_t, uint32_t, int(__cdecl*)(const void*, const void*));
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540C20 });
    fn(base, num, size, compar);
}

inline char& DAT_00556dac = *reinterpret_cast<char*>(std::uintptr_t{ 0x00556DAC });

inline void __cdecl No_Op1_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fbef0 });
    fn();
}

inline uint32_t __cdecl isqrt_(uint32_t val) {
    using Fn = uint32_t(__cdecl*)(uint32_t);
    return reinterpret_cast<Fn>(std::uintptr_t{0x00531AA0})(val);
}

inline void __cdecl Set_Memory_(void* dest, uint32_t size, uint8_t value) {
    using Fn = void(__cdecl*)(void*, uint32_t, uint8_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052ced8});
    fn(dest, size, value);
}

inline void* __cdecl Buffer_Reload_(const char* filename, int32_t index, void* segment) {
    using Fn = void*(__cdecl*)(const char*, int32_t, void*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C2E0});
    return fn(filename, index, segment);
}

inline void __cdecl Timed_Text_Box_(const char* title, const char* message, int16_t duration) {
    using Fn = void(__cdecl*)(const char*, const char*, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471090});
    fn(title, message, duration);
}

inline void __cdecl Build_Captain_Id_List_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043D470});
    fn();
}

inline void __cdecl Get_Officer_Pictures_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00441A40});
    fn();
}

inline const char** g_race_names_ptrs = reinterpret_cast<const char**>(std::uintptr_t{ 0x0059D660 });
inline char**& DAT_0059d660 = *reinterpret_cast<char***>(std::uintptr_t{ 0x0059D660 });
inline const char* const s_RACENAME_LBX = reinterpret_cast<const char*>(0x00557614);
inline const char* const s_RACESTUF_LBX = reinterpret_cast<const char*>(0x00557624);
inline uint8_t* g_trait_modifiers_table = reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0055903A });

inline void __cdecl FUN_00443C60() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(std::uintptr_t{ 0x00443C60 })();
}

// Suggested name: s_Null_Pointer
inline const char* const s__d_c_d_0055996c = reinterpret_cast<const char*>(0x0055996C); // "%d%c%d"
inline const char* const s__s_s__s_00559964 = reinterpret_cast<const char*>(0x00559964); // "%s%s %s"
inline const char* const s__s__s_00558014 = reinterpret_cast<const char*>(0x00558014); // "%s %s"
inline const char* const s__s_d___d__00559958 = reinterpret_cast<const char*>(0x00559958); // "%s%d (%d)"
inline const char* const s__s_d_00556134 = reinterpret_cast<const char*>(0x00556134); // "%s%d"
inline const char* const s___d__00559950 = reinterpret_cast<const char*>(0x00559950); // " (%d)"
inline const char* const s__s_s_00556aec = reinterpret_cast<const char*>(0x00556AEC); // "%s%s"
inline const char* const s__d___0055612c = reinterpret_cast<const char*>(0x0055612C); // "%d%%"
inline const char* const s___s_0055994c = reinterpret_cast<const char*>(0x0055994C); // "@%s"
inline const char* const s___00559948 = reinterpret_cast<const char*>(0x00559948); // "@"
inline const char* const s_0__s_00559940 = reinterpret_cast<const char*>(0x00559940); // "0 %s"
inline const char* const s__d__s_005569f0 = reinterpret_cast<const char*>(0x005569F0); // "%d %s"
inline const char* s__00551100 = reinterpret_cast<const char*>(0x00551100);   // ""
inline const char* s_plus_00551118 = reinterpret_cast<const char*>(0x00551118);  // "+"
inline const char* s_minus_0055111c = reinterpret_cast<const char*>(0x0055111C);  // "-"

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

inline const char* s_SAVE_005568fc = reinterpret_cast<const char*>(0x005568FC); // "SAVE"
inline const char* s__GAM_005568f4 = reinterpret_cast<const char*>(0x005568F4); // ".GAM"
inline const char* s_rb_0055117c = reinterpret_cast<const char*>(0x0055117C); // "rb"
inline const char* s_wb_00551178 = reinterpret_cast<const char*>(std::uintptr_t{ 0x00551178 }); // "wb"
inline const char* s_mox_set_0055760c = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055760C }); // "mox.set"
inline const char* s_Unable_to_open_mox_set_005575f4 = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575F4 }); // "Unable to open mox.set"
inline const char* s_MOX_SET_00556918 = reinterpret_cast<const char*>(std::uintptr_t{0x00556918}); // "MOX.SET"
inline const char* s_Settings_file_error_00556950 = reinterpret_cast<const char*>(std::uintptr_t{0x00556950}); // "Settings file (MOX.SET) is missing..."

// "Orion"
inline const char* s_Orion_0055a738 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A738});

inline const char* s_BUFFER0_LBX = reinterpret_cast<const char*>(0x00551124); // "BUFFER0.LBX"
inline const char* s_rstring_d_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00559A38}); // "rstring%d.lbx"
inline const char* s_STARBG_LBX = reinterpret_cast<const char*>(0x005598FC); // "STARBG.LBX"
inline const char* s_ships_lbx_005597b4 = reinterpret_cast<const char*>(std::uintptr_t{0x005597B4}); // "ships.lbx"

inline const char* s_______CLIMATE_modifier_is__d_Pla_00558a58 =
    reinterpret_cast<const char*>(std::uintptr_t{0x00558A58}); // "------CLIMATE modifier is %d Planets: %d index %d\n"

inline char* const s_Moo2AlreadyRunning = reinterpret_cast<char*>(std::uintptr_t{ 0x0055B1CC }); // "Moo2 is already running"
inline char* const s_Microprose = reinterpret_cast<char*>(std::uintptr_t{ 0x0055B1E8 });        // "Microprose"

inline LRESULT CALLBACK Main_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    using Fn = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00536710 });
    return fn(hwnd, uMsg, wParam, lParam);
}

inline const char* const s_Memory_Corruption_val_00558028 =
    reinterpret_cast<const char*>(std::uintptr_t{0x00558028}); // "Memory Corruption! val == %d, sh == %d, pl == %d, d == %d"

inline const char* s_File_Animation_Frame_Too_Large_0055b804 =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B804 }); // "File Animation Frame Too Large"

inline const char* s_PR_00558238 = reinterpret_cast<const char*>(std::uintptr_t{0x00558238}); // "PR"
inline const char* s_FP_00558240 = reinterpret_cast<const char*>(std::uintptr_t{0x00558240}); // "FP"
inline const char* s_RP_00558248 = reinterpret_cast<const char*>(std::uintptr_t{0x00558248}); // "RP"
inline const char* s_PI_00559994 = reinterpret_cast<const char*>(std::uintptr_t{0x00559994}); // "PI"
inline const char* s_G_0055241c = reinterpret_cast<const char*>(0x0055241C);  // "G"
inline const char* s_C_00552420 = reinterpret_cast<const char*>(0x00552420);  // "C"
inline const char* s_L_005560e8 = reinterpret_cast<const char*>(0x005560E8);  // "L"
inline const char* s_P_005569d8 = reinterpret_cast<const char*>(0x005569D8);  // "P"
inline const char* s_I_0055801c = reinterpret_cast<const char*>(0x0055801C);  // "I"
inline const char* s_R_0055824c = reinterpret_cast<const char*>(0x0055824C);  // "R"
inline const char* s_Q_005598f0 = reinterpret_cast<const char*>(0x005598F0);  // "Q"
inline const char* s_F_005598f4 = reinterpret_cast<const char*>(0x005598F4);  // "F"
inline const char* s_T_005598f8 = reinterpret_cast<const char*>(0x005598F8);  // "T"
inline const char* s__00551120 = reinterpret_cast<const char*>(0x00551120); // ""
inline const char* s_S_005569cc = reinterpret_cast<const char*>(0x005569CC); // "S"
inline const char* s_D_005569dc = reinterpret_cast<const char*>(0x005569DC); // "D"
inline const char* s_O_00556afc = reinterpret_cast<const char*>(0x00556AFC); // "O"
inline const char* s_N_0055765c = reinterpret_cast<const char*>(0x0055765C); // "N"
inline const char* s_A_00557ed4 = reinterpret_cast<const char*>(0x00557ED4); // "A"
inline char*& PTR_s_GAME_LBX_0055ac4c =
    *reinterpret_cast<char**>(std::uintptr_t{0x0055AC4C});
inline void*& PTR_s_OFFICER_LBX_00556990 =
    *reinterpret_cast<void**>(std::uintptr_t{0x00556990});
inline const char* s___INVALID___0055ac58 = reinterpret_cast<const char*>(0x0055AC58); // "* INVALID *"