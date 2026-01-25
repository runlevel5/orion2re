#pragma once

namespace FILEDEF {
    void Load_Game_Settings_();
    bool Save_Game_Settings_(void);
    void Update_Game_Settings_Version_Number_();

    void __cdecl Save_Game_(short slot_index);
    uint32_t __cdecl Load_Game_(int16_t slot_index);
    void __cdecl Save_Moo_Custom_Player_Data_(s_player* player);
}
