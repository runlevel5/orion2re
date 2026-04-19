#include "pch.h"

namespace NEWPUP {
    // Patch: { 0x00487060, &NEWPUP::Troop_Popup_ },
    void __cdecl Troop_Popup_() {
        COLONY::_full_draw = 1;
        fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);

        MOX2::Get_Auto_Function_(&NEWPUP::_saved_autofunction);
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(NEWPUP::Draw_Troop_Popup_, 1);

        ERICHELP::Set_Transport_Popup_Help_List_();

        NEWPUP::Draw_Troop_Popup_();
        video::Set_Refresh_Stencil_();
        NEWPUP::Reset_Fields_();
        RUSS::Mox_Sync_Update_();

        int16_t input;
        do {
            input = fields::Get_Input_();
            NEWPUP::Draw_Troop_Popup_();
            NEWPUP::Reset_Fields_();
            RUSS::Mox_Sync_Update_();
        } while (input == 0);

        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        MOX2::Put_Auto_Function_(&NEWPUP::_saved_autofunction);
    }
}