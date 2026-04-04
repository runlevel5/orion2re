#pragma once

#include <stdio.h>
#include <cstdint>

inline char* s_space_005510f0 = reinterpret_cast<char*>(std::uintptr_t{0x005510F0}); // " "
inline char* s_cmbtsfx_lbx_00551228 = reinterpret_cast<char*>(std::uintptr_t{0x00551228});
inline const char* const s_cmbtsfx_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00551228});
inline char* s_combat_lbx_00551244 = reinterpret_cast<char*>(std::uintptr_t{0x00551244});
inline char* s__00559998 = reinterpret_cast<char*>(std::uintptr_t{0x00559998});
inline char (&s__00559488)[64] = *reinterpret_cast<char(*)[64]>(std::uintptr_t{0x00559488});
inline char* s_monster_lbx_00551250 = reinterpret_cast<char*>(std::uintptr_t{0x00551250});
inline char* s_cmbtshp_lbx_0055125c = reinterpret_cast<char*>(std::uintptr_t{0x0055125C});
inline char* s_AI_trade_goods_00557670 = reinterpret_cast<char*>(std::uintptr_t{0x00557670}); // "AI trade goods"

inline const char* s_fmt_comma_00555ff0 = reinterpret_cast<const char*>(std::uintptr_t{0x00555FF0}); // "%i,%i"
inline const char* s_fmt_dot_00555ff8 = reinterpret_cast<const char*>(std::uintptr_t{0x00555FF8}); // "%i.%i"
inline const char* s_INFO_LBX_00556000 = reinterpret_cast<const char*>(std::uintptr_t{0x00556000}); // "INFO.LBX"
inline const char* s_SCIENCE_LBX_00557d68 = reinterpret_cast<const char*>(0x00557D68); // "SCIENCE.LBX"
inline const char* s_SR_R_x_SC_LBX_00557d74 = reinterpret_cast<const char*>(0x00557D74); // "SR_R%x_SC.LBX"
inline char* const s_app_pics_lbx_00556028 = reinterpret_cast<char*>(0x00556028);
inline char* const s_APP_PICS_LBX_00556018 = reinterpret_cast<char*>(0x00556018);

inline char* const s_a = reinterpret_cast<char*>(std::uintptr_t{0x00552414});
inline char* const s_an = reinterpret_cast<char*>(std::uintptr_t{0x00552418});

inline uint32_t& DAT_00565e20 = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565E20});
inline uint32_t& g_screen_res_x = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B1A8});

inline int32_t& DAT_0056178c = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056178C});
inline uint16_t*& PTR_DAT_00561580 = *reinterpret_cast<uint16_t**>(std::uintptr_t{0x00561580});

inline const char* s_percent_0055633c = reinterpret_cast<const char*>(0x0055633C); // "%"

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

inline char* __cdecl __strupr(char* str) {
    using Fn = char* (__cdecl*)(char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0054e910 });
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
inline void __cdecl Set_Page_Off_() {
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

inline int16_t __cdecl Proximity_Bonus_(int16_t player_idx, int16_t planet_idx) {
    using Fn = int16_t(__cdecl*)(int16_t, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ac040});
    return fn(player_idx, planet_idx);
}

inline int16_t __cdecl Owned_Officer_Level_(int32_t leader_idx) {
    using Fn = int16_t(__cdecl*)(int32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043e780});
    return fn(leader_idx);
}

inline bool __cdecl Player_Has_Colony_At_Star_(int16_t player_idx, int16_t star_idx) {
    using Fn = bool(__cdecl*)(int16_t, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ac120});
    return fn(player_idx, star_idx);
}

inline int __cdecl hgetch_() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AB400});
    return fn();
}

inline int __cdecl __getch() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0054EAC0});
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

inline int16_t (&_screen_data)[5] = *reinterpret_cast<int16_t (*)[5]>(std::uintptr_t{0x005A1B20});

inline const char* s_fmt_to_00557e00 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E00});

inline const char* s_fmt_modifier_full_005523a4 = reinterpret_cast<const char*>(std::uintptr_t{0x005523A4}); // "%d.%d"
inline const char* s_fmt_score_simple_00556104 = reinterpret_cast<const char*>(std::uintptr_t{0x00556104}); // "%d"
inline const char* s_dot_0055632c = reinterpret_cast<const char*>(std::uintptr_t{0x0055632C}); // "."
inline char* s_RACEOPT_LBX_00556320 = reinterpret_cast<char*>(std::uintptr_t{0x00556320}); // "RACEOPT.LBX"
inline char* s___0055632c = reinterpret_cast<char*>(std::uintptr_t{0x0055632C}); // "."
inline char* s_patch13_lbx_00556330 = reinterpret_cast<char*>(std::uintptr_t{0x00556330}); // "patch13.lbx"
inline const char* s_fmt_score_complex_0055a518 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A518}); // "(%d x %s) %d"
inline const char* s_racestuf_lbx_00556310 = reinterpret_cast<const char*>(std::uintptr_t{0x00556310});

inline auto _sprintf =
    reinterpret_cast<int(__cdecl*)(char*, const char*, ...)>(std::uintptr_t{ 0x00540550 });

inline int __cdecl _clock() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00540F20 });
    return fn();
}

inline int __cdecl _strncmp(const char* s1, const char* s2, size_t n) {
    using Fn = int(__cdecl*)(const char*, const char*, size_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00541C30});
    return fn(s1, s2, n);
}

inline char* __cdecl __itoa(int value, char* str, int radix) {
    using Fn = char*(__cdecl*)(int, char*, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00542d10});
    return fn(value, str, radix);
}

inline void __cdecl Set_Fade_Speed_Perc(int32_t speed) {
    using Fn = void(__cdecl*)(int32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BA650});
    fn(speed);
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

inline char* __cdecl Option_String_(int16_t id) {
    using Fn = char*(__cdecl*)(int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00509880});
    return fn(id);
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
inline char& DAT_00566440 = *reinterpret_cast<char*>(std::uintptr_t{0x00566440});

inline void __cdecl No_Op1_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fbef0 });
    fn();
}

inline uint32_t __cdecl isqrt_(uint32_t val) {
    using Fn = uint32_t(__cdecl*)(uint32_t);
    return reinterpret_cast<Fn>(std::uintptr_t{0x00531AA0})(val);
}

inline int16_t __cdecl Range_(int16_t y1, int16_t x1, int16_t y2, int16_t x2) {
    using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00531A50});
    return fn(y1, x1, y2, x2);
}

inline int16_t __cdecl Best_Missile_Damage_Special_(int16_t player_idx) {
    using Fn = int16_t(__cdecl*)(int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481cf0});
    return fn(player_idx);
}

inline int16_t __cdecl Get_Won_Council_Score_(int16_t player_idx) {
    using Fn = int16_t(__cdecl*)(int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004E2630});
    return fn(player_idx);
}

inline void __cdecl Set_Memory_(void* dest, uint32_t size, uint8_t value) {
    using Fn = void(__cdecl*)(void*, uint32_t, uint8_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052ced8});
    fn(dest, size, value);
}

inline void* __cdecl Allocate_First_Block_(void* segment, uint32_t size) {
    using Fn = void*(__cdecl*)(void*, uint32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052E930});
    return fn(segment, size);
}

inline void* __cdecl Allocate_Next_Block_(void* segment, uint32_t size) {
    using Fn = void*(__cdecl*)(void*, uint32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052E950});
    return fn(segment, size);
}

inline void __cdecl Copy_Structure_(void* dest, const void* src, int32_t size) {
    using Fn = void(__cdecl*)(void*, const void*, int32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052d0fa});
    fn(dest, src, size);
}

inline void __cdecl Swap_Structure_(void* ptr1, void* ptr2, int size) {
    using Fn = void(__cdecl*)(void*, void*, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052D169});
    fn(ptr1, ptr2, size);
}

namespace switchD_004d0ea1 {
    inline uint32_t* switchdataD_004d1930 = reinterpret_cast<uint32_t*>(std::uintptr_t{0x004D1930});
}
namespace switchD_004d1457 {
    inline uint32_t* switchdataD_004d1948 = reinterpret_cast<uint32_t*>(std::uintptr_t{0x004D1948});
}
namespace switchD_004d174c {
    inline uint32_t* switchdataD_004d1960 = reinterpret_cast<uint32_t*>(std::uintptr_t{0x004D1960});
}

namespace GLOBALS {
    inline const char* s_spaces_percent_s_0055236c = reinterpret_cast<const char*>(std::uintptr_t{0x0055236C});
    inline const char* s_percent_s_percent_s_percent_s_005574f4 = reinterpret_cast<const char*>(std::uintptr_t{0x005574F4});
}

inline void __cdecl Best_Instructor_Bonus_(int16_t* bonuses) {
    using Fn = void(__cdecl*)(int16_t*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00409370});
    fn(bonuses);
}

inline void __cdecl Do_XP_For_Ship_(int16_t ship_idx, int16_t bonus) {
    using Fn = void(__cdecl*)(int16_t, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00409410});
    fn(ship_idx, bonus);
}

inline int16_t __cdecl Base_Generic_Dcv_(int16_t target_idx) {
    using Fn = int16_t(__cdecl*)(int16_t);
    return reinterpret_cast<Fn>(std::uintptr_t{0x00508670})(target_idx);
}

inline uint32_t __cdecl Modified_Weapon_Strength_(uint32_t strength, uint16_t specials) {
    using Fn = uint32_t(__cdecl*)(uint32_t, uint16_t);
    return reinterpret_cast<Fn>(std::uintptr_t{0x00508990})(strength, specials);
}

inline int16_t __cdecl Strength_Hits_Modifier_(int16_t hits) {
    using Fn = int16_t(__cdecl*)(int16_t);
    return reinterpret_cast<Fn>(std::uintptr_t{0x00508A50})(hits);
}

inline void* __cdecl Buffer_Reload_(const char* filename, int32_t index, void* segment) {
    using Fn = void*(__cdecl*)(const char*, int32_t, void*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C2E0});
    return fn(filename, index, segment);
}

inline void __cdecl Combat_Draw_View_Ship_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d1f80});
    fn();
}

inline void __cdecl Timed_Text_Box_(const char* title, const char* message, int16_t duration) {
    using Fn = void(__cdecl*)(const char*, const char*, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471090});
    fn(title, message, duration);
}

inline void __cdecl Delete_Prod_From_Colony_Queue_(int32_t colony_idx, int32_t choice) {
    using Fn = void(__cdecl*)(int32_t, int32_t);
    reinterpret_cast<Fn>(std::uintptr_t{0x0050b300})(colony_idx, choice);
}

inline void __cdecl Build_Captain_Id_List_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043D470});
    fn();
}

inline void __cdecl Set_Multi_Player_Game_Type_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005081D0});
    fn();
}

inline void __cdecl Give_Officer_XP_(int16_t player_index, int16_t leader_index, int16_t amount) {
    using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00442ec0});
    fn(player_index, leader_index, amount);
}

inline void __cdecl Get_Officer_Pictures_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00441A40});
    fn();
}

inline const char** g_race_names_ptrs = reinterpret_cast<const char**>(std::uintptr_t{ 0x0059D660 });
inline char**& DAT_0059d660 = *reinterpret_cast<char***>(std::uintptr_t{ 0x0059D660 });
inline char* s_360_00556164 = reinterpret_cast<char*>(std::uintptr_t{0x00556164}); // "360"
inline void __cdecl Draw_Design_Screen_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042F470});
    fn();
}

inline uint8_t& DAT_00590000 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00590000});

inline const char* const s_Raider_0055768c = reinterpret_cast<const char*>(0x0055768C); // "Raider"
inline char* s_rparen_00557f08 = reinterpret_cast<char*>(std::uintptr_t{0x00557F08}); // ")"
inline const char* const s_Marauder_00557694 = reinterpret_cast<const char*>(0x00557694); // "Marauder"
inline char* s_space_lparen_0055999c = reinterpret_cast<char*>(std::uintptr_t{0x0055999C}); // " ("
inline char* s_space_x_005599a0 = reinterpret_cast<char*>(std::uintptr_t{0x005599A0}); // " x"
inline const char* const s_Interdictor_005576ac = reinterpret_cast<const char*>(0x005576AC); // "Interdictor"
inline const char* const s_Harbinger_005576b8 = reinterpret_cast<const char*>(0x005576B8); // "Harbinger"
inline const char* s__00557e60 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E60}); // "\n\n"
inline const char* const s_RACENAME_LBX = reinterpret_cast<const char*>(0x00557614);
inline const char* const s_RACESTUF_LBX = reinterpret_cast<const char*>(0x00557624);
inline const char* s_WWWWWWW_0055137c = reinterpret_cast<const char*>(std::uintptr_t{0x0055137c}); // "WWWWWWW"
inline const char* s__s_00551130 = reinterpret_cast<const char*>(std::uintptr_t{0x00551130});
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
inline const char* s__d_BC_00552384 = reinterpret_cast<const char*>(0x00552384);
inline const char* s_plus_00551118 = reinterpret_cast<const char*>(0x00551118);  // "+"
inline const char* s_minus_0055111c = reinterpret_cast<const char*>(0x0055111C);  // "-"
inline const char* s_c_00556350 = reinterpret_cast<const char*>(std::uintptr_t{0x00556350}); // "c"
inline const char* s_o_00556af4 = reinterpret_cast<const char*>(std::uintptr_t{0x00556AF4}); // "o"
inline const char* s_oc_00556af8 = reinterpret_cast<const char*>(std::uintptr_t{0x00556AF8}); // "oc"
inline const char* s_H_005569d4 = reinterpret_cast<const char*>(0x005569D4); // "H"
inline const char* s_M_00557ed8 = reinterpret_cast<const char*>(0x00557ED8); // "M"
inline const char* DAT_00558540 = reinterpret_cast<const char*>(0x00558540);
inline const char* DAT_0055aa90 = reinterpret_cast<const char*>(0x0055AA90);

inline const char* const s_Monster_ship_id_cannot_mov_00552cd8 =
    reinterpret_cast<const char*>(std::uintptr_t{0x00552CD8}); // "Monster (ship id: %d) cannot move to %s (%d)!"

inline const char* const s_Auto_Save_00556938 =
    reinterpret_cast<const char*>(std::uintptr_t{0x00556938}); // "(Auto Save)"

inline const char* s_0_00551104 = reinterpret_cast<const char*>(0x00551104); // " 0"
inline const char* s_2_00551108 = reinterpret_cast<const char*>(0x00551108); // " 2"
inline const char* s_1_00551114 = reinterpret_cast<const char*>(0x00551114); // " 1"

inline const char* const s_e_0055ac48 = reinterpret_cast<const char*>(std::uintptr_t{0x0055AC48}); // "e"

inline const char* const s_Null_Pointer_0055b658 =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B658 }); // "Null Pointer"

// Suggested name: s_allocation_error_fmt
inline const char* const s_Couldn_t_Allocate_0055B668 =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B668 }); // "Couldn't Allocate %d. Increase Virtual Memory."

inline const char* const s_Unable_to_Allocate_bytes =
    reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B690 }); // "Unable to Allocate %d bytes (%s, %d)\n"

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

inline const char* const s_fmt_00557F18 =
    reinterpret_cast<const char*>(std::uintptr_t{0x00557F18}); // "%d> %s"

inline const char*& s_format_string = *reinterpret_cast<const char**>(std::uintptr_t{0x00558BD8});

inline const char*& PTR_s_FLEET_LBX_005584a8 = *reinterpret_cast<const char**>(std::uintptr_t{0x005584A8});

inline const char* s_RACEOPT_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559DB0}); // "RACEOPT.LBX"
inline const char* s_SAVE_005568fc = reinterpret_cast<const char*>(0x005568FC); // "SAVE"
inline const char* s__GAM_005568f4 = reinterpret_cast<const char*>(0x005568F4); // ".GAM"
inline const char* s_rb_0055117c = reinterpret_cast<const char*>(0x0055117C); // "rb"
inline const char* s_wb_00551178 = reinterpret_cast<const char*>(std::uintptr_t{ 0x00551178 }); // "wb"
inline const char* s_mox_set_0055760c = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055760C }); // "mox.set"
inline const char* s_Unable_to_open_mox_set_005575f4 = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575F4 }); // "Unable to open mox.set"
inline const char* s_MOX_SET_00556918 = reinterpret_cast<const char*>(std::uintptr_t{0x00556918}); // "MOX.SET"
inline const char* s_Settings_file_error_00556950 = reinterpret_cast<const char*>(std::uintptr_t{0x00556950}); // "Settings file (MOX.SET) is missing..."

inline const char* s_Antares_00551288 = reinterpret_cast<const char*>(std::uintptr_t{0x00551288});
inline const char* s_cmbtplnt_lbx_00551278 = reinterpret_cast<const char*>(std::uintptr_t{0x00551278});
inline const char* s_0_0055110c = reinterpret_cast<const char*>(std::uintptr_t{0x0055110C}); /* " 0" */
inline const char* s___00556070 = reinterpret_cast<const char*>(std::uintptr_t{0x00556070}); /* "^ " */
inline const char* s__005569f8 = reinterpret_cast<const char*>(std::uintptr_t{0x005569F8}); /* "\n" */
inline const char* s_BUFFER0_LBX = reinterpret_cast<const char*>(0x00551124); // "BUFFER0.LBX"
inline const char* s_rstring_d_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00559A38}); // "rstring%d.lbx"
inline const char* s_STARBG_LBX = reinterpret_cast<const char*>(0x005598FC); // "STARBG.LBX"
inline const char* s_councmsg_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00559A80}); // "councmsg.lbx"
inline const char* s_ships_lbx_005597b4 = reinterpret_cast<const char*>(std::uintptr_t{0x005597B4}); // "ships.lbx"
inline const char* s_billtext_lbx_0055135c = reinterpret_cast<const char*>(std::uintptr_t{0x0055135C}); // "billtext.lbx"
inline char* s_racerprt_lbx_005574d8 = reinterpret_cast<char*>(std::uintptr_t{0x005574D8});
inline const char* const s_gstar_lbx_00558544 = reinterpret_cast<const char*>(std::uintptr_t{0x00558544}); // "gstar.lbx"
inline char* const s_SPA_HELP_LBX_00557450 = reinterpret_cast<char*>(std::uintptr_t{0x00557450}); // "SPA_HELP.LBX"
inline char* const s_ITA_HELP_LBX_00557460 = reinterpret_cast<char*>(std::uintptr_t{0x00557460}); // "ITA_HELP.LBX"
inline char* const s_FRE_HELP_LBX_00557470 = reinterpret_cast<char*>(std::uintptr_t{0x00557470}); // "FRE_HELP.LBX"
inline char* const s_GER_HELP_LBX_00557480 = reinterpret_cast<char*>(std::uintptr_t{0x00557480}); // "GER_HELP.LBX"
inline char* const s_HELP_LBX_00557490 = reinterpret_cast<char*>(std::uintptr_t{0x00557490}); // "HELP.LBX"
inline const char* s_DIPLOMAT_LBX_00552350 = reinterpret_cast<const char*>(std::uintptr_t{0x00552350}); // "DIPLOMAT.LBX"
inline const char* s_DIPLOMAT_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00552350}); /* "DIPLOMAT.LBX" */
inline const char* s_dipstars_lbx_00552374 = reinterpret_cast<const char*>(std::uintptr_t{0x00552374}); // "dipstars.lbx"

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

inline char* s_Cannot_color_replace_refresh_mas_0055b7a0 =
    reinterpret_cast<char*>(std::uintptr_t{0x0055B7A0}); // "Cannot color replace refresh mask."

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
inline const char* s_combat_lbx_005599c8 = reinterpret_cast<const char*>(0x005599C8); // "combat.LBX"

inline void __cdecl Set_Font_Style_(int32_t a1, const void* colors) {
    using Fn = void(__cdecl*)(int32_t, const void*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005255A0});
    fn(a1, colors);
}

inline int16_t __cdecl Get_Font_Height_(int32_t a1) {
    using Fn = int16_t(__cdecl*)(int32_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526060});
    return fn(a1);
}

inline int16_t __cdecl Get_String_Width_(const char* str) {
    using Fn = int16_t(__cdecl*)(const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00525470});
    return fn(str);
}

inline void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
    using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, uint8_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528CC0});
    fn(x1, y1, x2, y2, color);
}

inline void __cdecl Print_(int16_t x, int16_t y, const char* str) {
    using Fn = void(__cdecl*)(int16_t, int16_t, const char*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00525910});
    fn(x, y, str);
}

inline void __cdecl Save_Hotseat_Map_Info_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004CBD70});
    fn();
}

inline void __cdecl Restore_Hotseat_Map_Info_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004CBC80});
    fn();
}

inline void __cdecl Fade_In_Function_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00504500});
    fn();
}

inline void __cdecl Hotseat_Draw_Next_Player_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00503E30});
    fn();
}

inline void __cdecl Hotseat_Load_Next_Player_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00504190});
    fn();
}

inline void __cdecl Hotseat_Add_Next_Player_Fields_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00503D30});
    fn();
}

inline void __cdecl _Squeeze_Print_Paragraph_(uint16_t p1, int16_t p2, int16_t p3, int16_t p4, char* p5, uint16_t p6, int16_t* p7, int8_t p8, int8_t p9) {
    using Fn = void(__cdecl*)(uint16_t, int16_t, int16_t, int16_t, char*, uint16_t, int16_t*, int8_t, int8_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407480});
    fn(p1, p2, p3, p4, p5, p6, p7, p8, p9);
}
inline const char* s_S_005569cc = reinterpret_cast<const char*>(0x005569CC); // "S"
inline const char* s_D_005569dc = reinterpret_cast<const char*>(0x005569DC); // "D"
inline const char* s_O_00556afc = reinterpret_cast<const char*>(0x00556AFC); // "O"
inline const char* s_N_0055765c = reinterpret_cast<const char*>(0x0055765C); // "N"
inline const char* s_A_00557ed4 = reinterpret_cast<const char*>(0x00557ED4); // "A"
inline const char* s_dot_d_0055a528 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A528}); // ".%d"
inline char*& PTR_s_GAME_LBX_0055ac4c =
    *reinterpret_cast<char**>(std::uintptr_t{0x0055AC4C});
inline void*& PTR_s_OFFICER_LBX_00556990 =
    *reinterpret_cast<void**>(std::uintptr_t{0x00556990});
inline const char* s___INVALID___0055ac58 = reinterpret_cast<const char*>(0x0055AC58); // "* INVALID *"

inline int16_t* _x_fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00551040});

inline char* s_fmt_d_s_s_00557e0c = reinterpret_cast<char*>(std::uintptr_t{0x00557E0C}); /* "%d %s (%s)" */
inline char* s_fmt_s_d_EP_00557e18 = reinterpret_cast<char*>(std::uintptr_t{0x00557E18}); /* "%s (%d EP)" */

inline const char* s_Too_many_ships_in_list_00557de8 =
    reinterpret_cast<const char*>(std::uintptr_t{0x00557DE8}); /* "Too many ships in list" */

inline const char* s_i_PR_00557d84 = reinterpret_cast<const char*>(0x00557D84);
inline const char* s_i_FP_00557d8c = reinterpret_cast<const char*>(0x00557D8C);
inline const char* s_i_RP_00557d94 = reinterpret_cast<const char*>(0x00557D94);
inline const char* s_jimtext_lbx_00552360 = reinterpret_cast<const char*>(0x00552360);
inline char* s_jimtext2_lbx_00552340 = reinterpret_cast<char*>(std::uintptr_t{0x00552340});
inline const char* s_jimtext2_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00552340}); // "jimtext2.lbx"
inline const char* s_s_sez__s_00552334 = reinterpret_cast<const char*>(std::uintptr_t{0x00552334}); // "%s sez: %s"
inline const char* s_percent_s_0055236c = reinterpret_cast<const char*>(0x0055236C);
inline const char* s_empty_00551100 = reinterpret_cast<const char*>(0x00551100);
inline const char* s_PR_00557d9c = reinterpret_cast<const char*>(std::uintptr_t{0x00557D9C});
inline const char* s_FP_00557da0 = reinterpret_cast<const char*>(std::uintptr_t{0x00557DA0});
inline const char* s_RP_00557da4 = reinterpret_cast<const char*>(std::uintptr_t{0x00557DA4});

struct s_mouse_list;