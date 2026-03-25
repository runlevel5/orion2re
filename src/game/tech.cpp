#include "pch.h"

namespace TECH {

void __cdecl Tech_Change_() {
    allocate::Allocate_First_Block_(MOX::_screen_seg, 4);
    video::Set_Page_Off_();
    fields::Draw_Visible_Fields_();
    video::Copy_Off_To_Back_();

    TECH::_Tech_Select_(1);

    MOX::_current_screen = MOX::_return_screen;
}

void __cdecl Save_Hyper_Tech_Values_(int16_t player_idx) {
    // Copy 8 bytes of hyper_advanced_tech levels from the player record to the global storage
    memcpy(TECH::_saved_hyper_advanced_tech, MOX::_player[player_idx].hyper_advanced_tech, 8);
}

void __cdecl Promote_Hyper_Field_Values_(int16_t player_idx) {
    // Each player has 8 hyper-advanced technology fields. 
    // This function increments the level of each field by 1.
    for (int i = 0; i < 8; ++i) {
        MOX::_player[player_idx].hyper_advanced_tech[i]++;
    }
}

}