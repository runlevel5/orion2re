#pragma once

namespace MOX2 {
    void __cdecl Set_Game_Random_Seed_();

    inline void __cdecl Set_Game_Seed() {
        Set_Game_Random_Seed_();
    }

    void __cdecl Init_Map_Defaults_();
    void __cdecl Init_Map_Pictures_Default__();
    extern int8_t _user_wants_n_space_monsters;
    extern uint8_t _pan_main_menu_panel;
    extern uint8_t _skip_intro;
    extern uint8_t _quick_start;
    extern char _leave_game_message[100];

    void __cdecl Set_Mox_Language_();

    void __cdecl Set_Mox_Alt_Path_();

    void __cdecl Screen_Control_();

    extern const char s_MOX_SET[8]; // "MOX.SET"
    extern const char s_language_ini[13]; // "language.ini"
    extern const char s_language_ini_write_mode[3];
    extern const char s_rt[3]; // "rt"
    extern const char s_Thanks_English[38];
    extern const char s_Thanks_German[49];
    extern const char s_Thanks_French[36];
    extern const char s_Thanks_Spanish[41];
    extern const char s_Thanks_Italian[47];

    extern const char s_Loading_English[44];
    extern const char s_Loading_German[50];
    extern const char s_Loading_French[66];
    extern const char s_Loading_Spanish[50];
    extern const char s_Loading_Italian[55];

    extern const char s_Non_trovo_il_file_ORIONCD_INI_005578ec[62];
    extern const char s_ORIONCD_INI_no_se_ha_encontrado_0055792c[61];
    extern const char s_ORIONCD_INI_introuvable_0055796c[56];
    extern const char s_ORIONCD_INI_nicht_gefunden_005579a4[61];
    extern const char s_ORIONCD_INI_not_found_005579e4[51];
    extern const char s_Non_riesco_a_trovare_qualche_fil_00557a18[85];
    extern const char s_No_se_puede_localizar_todos_los_f_00557a70[99];
    extern const char s_Echec_de_la_localisation_de_tous_00557ad4[93];
    extern const char s_Nicht_alle_Spieldateien_auffindb_00557b34[77];
    extern const char s_Cannot_locate_all_game_files_00557b84[63];
    extern const char s_diplomat_lbx_00557bc4[13];
    extern const char s__sdiplomat_lbx_00557bd4[15];
    extern const char s_backslash_00557be4[2];
    extern const char s_orioncd_ini_00557bec[12];

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
