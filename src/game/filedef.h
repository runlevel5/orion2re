#pragma once

namespace FILEDEF {
    void Load_Game_Settings_();
    bool Save_Game_Settings_(void);
    void Update_Game_Settings_Version_Number_();

    void __cdecl Save_Game_(short slot_index);
    uint32_t __cdecl Load_Game_(int16_t slot_index);
    extern const char s_SAVE_d_GAM_00556944[11];
    extern const char s_rb_0055117c[3];
    extern const char s_GAM_005568f4[5];
    extern const char s_SAVE_005568fc[5];
    extern const char s_Error_saving_game_00556904[20];
    extern const char s_no_description_00556920[21];
    extern const char s_Auto_Save_00556938[12];
    extern const char s_invalid_save_slot_english[65];
    extern const char s_invalid_save_slot_german[89];
    extern const char s_invalid_save_slot_french[112];
    extern const char s_invalid_save_slot_spanish[87];
    extern const char s_invalid_save_slot_italian[91];
    int8_t __cdecl Get_Game_Type_From_Saved_Game_(int16_t save_slot);
    void __cdecl Save_Moo_Custom_Player_Data_(s_player* player);
    void __cdecl Get_Saved_Game_Descriptions_(int32_t* save_times);
    bool __cdecl Read_Moo_Custom_Player_Data_(s_player* player);
}
