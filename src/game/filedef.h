#pragma once

namespace FILEDEF {
    void Load_Game_Settings_();
    bool Save_Game_Settings_(void);
    void Update_Game_Settings_Version_Number_();

    void __cdecl Save_Game_(short slot_index);
    uint32_t __cdecl Load_Game_(int16_t slot_index);
    uint8_t __cdecl Get_Game_Type_From_Saved_Game_(int slot);
    void __cdecl Save_Moo_Custom_Player_Data_(s_player* player);
    void __cdecl Get_Saved_Game_Descriptions_(void* buffer);

    inline bool Read_Moo_Custom_Player_Data_(s_player* player) {
        using Fn = bool(*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043D1C0 });
        return fn(player);
    }
}
