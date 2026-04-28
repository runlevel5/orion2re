#pragma once

namespace FILEDEF {
    void Load_Game_Settings_();
    bool Save_Game_Settings_(void);
    void Update_Game_Settings_Version_Number_();

    void __cdecl Save_Game_(short slot_index);
    uint32_t __cdecl Load_Game_(int16_t slot_index);
    inline const char* s_SAVE_d_GAM_00556944 = reinterpret_cast<const char*>(std::uintptr_t{0x00556944});
    int8_t __cdecl Get_Game_Type_From_Saved_Game_(int16_t save_slot);
    void __cdecl Save_Moo_Custom_Player_Data_(s_player* player);
    void __cdecl Get_Saved_Game_Descriptions_(int32_t* save_times);
    bool __cdecl Read_Moo_Custom_Player_Data_(s_player* player);
}
