#pragma once

namespace FILEDEF {
    void Load_Game_Settings_();
    bool Save_Game_Settings_(void);
    void Update_Game_Settings_Version_Number_();

    void __cdecl Save_Game_(short slot_index);
    uint32_t __cdecl Load_Game_(int16_t slot_index);
    inline const char* s_SAVE_d_GAM_00556944 = reinterpret_cast<const char*>(std::uintptr_t{0x00556944});
    inline const char* const s_rb_0055117c = reinterpret_cast<const char*>(std::uintptr_t{0x0055117C});
    inline const char* const s_GAM_005568f4 = reinterpret_cast<const char*>(std::uintptr_t{0x005568F4});
    inline const char* const s_SAVE_005568fc = reinterpret_cast<const char*>(std::uintptr_t{0x005568FC});
    inline const char* const s_Error_saving_game_00556904 = reinterpret_cast<const char*>(std::uintptr_t{0x00556904});
    inline const char* const s_no_description_00556920 = reinterpret_cast<const char*>(std::uintptr_t{0x00556920});
    inline const char* const s_Auto_Save_00556938 = reinterpret_cast<const char*>(std::uintptr_t{0x00556938});
    inline const char* const s_invalid_save_slot_english = reinterpret_cast<const char*>(std::uintptr_t{0x005568A0});
    inline const char* const s_invalid_save_slot_german = reinterpret_cast<const char*>(std::uintptr_t{0x00556844});
    inline const char* const s_invalid_save_slot_french = reinterpret_cast<const char*>(std::uintptr_t{0x005567D4});
    inline const char* const s_invalid_save_slot_spanish = reinterpret_cast<const char*>(std::uintptr_t{0x0055677C});
    inline const char* const s_invalid_save_slot_italian = reinterpret_cast<const char*>(std::uintptr_t{0x00556720});
    int8_t __cdecl Get_Game_Type_From_Saved_Game_(int16_t save_slot);
    void __cdecl Save_Moo_Custom_Player_Data_(s_player* player);
    void __cdecl Get_Saved_Game_Descriptions_(int32_t* save_times);
    bool __cdecl Read_Moo_Custom_Player_Data_(s_player* player);
}
