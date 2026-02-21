#pragma once
#include <cstdint>

namespace FLEETPOP {
    inline const char* s_BUFFER0_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00551124}); // "BUFFER0.LBX"

    void __cdecl Reload_Fleet_Box_Data_(void);

    bool __cdecl Ship_Should_Be_Auto_Selected_(int16_t ship_idx);

    int16_t __cdecl N_Fleet_Icons_(int16_t node_idx);

    inline void __cdecl Fleet_Box_Data_(int16_t icon_count) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00404D10});
        fn(icon_count);
    }

    int16_t __cdecl Fleet_Buttons_Chosen_();

    void __cdecl Set_Fleet_Movement_Icon_Selections_(int16_t state, uint8_t auto_select_flag);

    bool __cdecl Some_Icon_Selected_();

    void __cdecl Reload_Ship_Selection_Status_();

    int16_t __cdecl First_Unselected_Icon_();

    void __cdecl Save_Ship_Selection_Status_();

    int16_t __cdecl Fleet_Box_Grid_Height_(int16_t grid_type);
    int16_t __cdecl Fleet_Box_Width_();
    int16_t __cdecl Fleet_Box_Height_(int16_t fleet_count);

    void __cdecl Set_Fleet_Movement_Box_Dimensions_(int16_t count);

    bool __cdecl Can_Select_Fleet_Box_Ship_(int16_t index);

    int16_t __cdecl Icon_Selection_Status_(int16_t icon_field_id);

    void __cdecl Clear_Ship_Node_Selection_Flags_();

    void __cdecl Reload_Fleet_Box_Palette_(int16_t player_idx, uint32_t is_not_player);
}