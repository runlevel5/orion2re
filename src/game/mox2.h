#pragma once

namespace MOX2 {
    void __cdecl Set_Game_Random_Seed_();

    inline void __cdecl Set_Game_Seed() {
        Set_Game_Random_Seed_();
    }

    void __cdecl Init_Map_Defaults_();
    void __cdecl Init_Map_Pictures_Default__();
    inline uint8_t& _user_wants_n_space_monsters = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A6378 });
    inline uint8_t& _pan_main_menu_panel = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A6379 });
    inline uint8_t& _skip_intro = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005662B4 });
    inline uint8_t& _quick_start = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005662AC });
    inline char* _leave_game_message = reinterpret_cast<char*>(std::uintptr_t{ 0x005A6380 });

    void __cdecl Set_Mox_Language_();

    void __cdecl Set_Mox_Alt_Path_();

    void __cdecl Screen_Control_();

    inline const char* s_MOX_SET = reinterpret_cast<const char*>(0x00556918); // "MOX.SET"
    inline const char* s_language_ini = reinterpret_cast<const char*>(0x005576D8); // "language.ini"
    inline const char* s_language_ini_write_mode = reinterpret_cast<const char*>(0x0055A804);
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

    inline const char* s_Non_trovo_il_file_ORIONCD_INI_005578ec = reinterpret_cast<const char*>(0x005578EC);
    inline const char* s_ORIONCD_INI_no_se_ha_encontrado_0055792c = reinterpret_cast<const char*>(0x0055792C);
    inline const char* s_ORIONCD_INI_introuvable_0055796c = reinterpret_cast<const char*>(0x0055796C);
    inline const char* s_ORIONCD_INI_nicht_gefunden_005579a4 = reinterpret_cast<const char*>(0x005579A4);
    inline const char* s_ORIONCD_INI_not_found_005579e4 = reinterpret_cast<const char*>(0x005579E4);
    inline const char* s_Non_riesco_a_trovare_qualche_fil_00557a18 = reinterpret_cast<const char*>(0x00557A18);
    inline const char* s_No_se_puede_localizar_todos_los_f_00557a70 = reinterpret_cast<const char*>(0x00557A70);
    inline const char* s_Echec_de_la_localisation_de_tous_00557ad4 = reinterpret_cast<const char*>(0x00557AD4);
    inline const char* s_Nicht_alle_Spieldateien_auffindb_00557b34 = reinterpret_cast<const char*>(0x00557B34);
    inline const char* s_Cannot_locate_all_game_files_00557b84 = reinterpret_cast<const char*>(0x00557B84);
    inline const char* s_diplomat_lbx_00557bc4 = reinterpret_cast<const char*>(0x00557BC4);
    inline const char* s__sdiplomat_lbx_00557bd4 = reinterpret_cast<const char*>(0x00557BD4);
    inline const char* s_backslash_00557be4 = reinterpret_cast<const char*>(0x00557BE4);
    inline const char* s_orioncd_ini_00557bec = reinterpret_cast<const char*>(0x00557BEC);

    void __cdecl main2_();
    void __cdecl thunk_main2_(void*);
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

    void __cdecl Execute_Auto_Function_(action_func_t* function_entry);

    void __cdecl Put_Auto_Function_(action_func_t* param_1);
}
