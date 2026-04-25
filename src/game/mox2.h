#pragma once

namespace MOX2 {
    void __cdecl Set_Game_Seed();

    void __cdecl Init_Map_Defaults_();
    void __cdecl Init_Map_Pictures_Default__();
    inline uint8_t& _user_wants_n_space_monsters = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A6378 });
    inline uint8_t& _pan_main_menu_panel = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A6379 });
    inline uint8_t& _skip_intro = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005662B4 });
    inline uint8_t& _quick_start = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005662AC });
    inline char* _leave_game_message = reinterpret_cast<char*>(std::uintptr_t{ 0x005A6380 });

    // Suggested name: MOX2::Release_Version_
    inline void __cdecl Release_Version_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00520200 });
        fn();
    }

    void __cdecl Set_Mox_Language_();

    inline void __cdecl Set_Mox_Alt_Path_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483CE0 });
        fn();
    }

    void __cdecl Screen_Control_();

    inline const char* s_MOX_SET = reinterpret_cast<const char*>(0x00556918); // "MOX.SET"
    inline const char* s_language_ini = reinterpret_cast<const char*>(0x005576D8); // "language.ini"
    inline const char* s_rt = reinterpret_cast<const char*>(0x00557BE8);           // "rt"
    inline const char* s_Thanks_English = reinterpret_cast<const char*>(0x005577A8);
    inline const char* s_Thanks_German = reinterpret_cast<const char*>(0x00557774);
    inline const char* s_Thanks_French = reinterpret_cast<const char*>(0x00557750);
    inline const char* s_Thanks_Spanish = reinterpret_cast<const char*>(0x00557724);
    inline const char* s_Thanks_Italian = reinterpret_cast<const char*>(0x005576F4);

    inline const char* s_Loading_English = reinterpret_cast<const char*>(0x005578B4);
    inline const char* s_Loading_German = reinterpret_cast<const char*>(0x00557880);
    inline const char* s_Loading_French = reinterpret_cast<const char*>(0x0055783C);
    inline const char* s_Loading_Spanish = reinterpret_cast<const char*>(0x00557808);
    inline const char* s_Loading_Italian = reinterpret_cast<const char*>(0x005577D0);

    void main2_();

    // Suggested name: MOX2::worker_thread_main
    inline void __cdecl thunk_main2_(void* arg) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004c1b20 });
        fn(arg);
    }
}

inline void FUN_004d7d80() {
    using Fn = void(*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D7D80 });
    fn();
}

// Suggested name: MOX2::Finalize_Main_Screen
inline void FUN_004d7da0() {
    using Fn = void(*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D7DA0 });
    fn();
}

namespace MOX2 {
    typedef void(__cdecl* action_func_t)();

    void __cdecl Get_Auto_Function_(action_func_t* param_1);

    void __cdecl Execute_Auto_Function_(action_func_t function_entry);

    void __cdecl Put_Auto_Function_(action_func_t* param_1);
}
