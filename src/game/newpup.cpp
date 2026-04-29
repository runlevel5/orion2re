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

    // Patch: { 0x00487130, &NEWPUP::Draw_Troop_Popup_ },
    void __cdecl Draw_Troop_Popup_() {
        s_invasion_side_info combat_info;
        int16_t row_count;

        COLONY::Set_Colony_Font_();

        if (COLONY::_full_draw != 0) {
            COLONY::_full_draw = 1;

            MOX2::Execute_Auto_Function_(&NEWPUP::_saved_autofunction);

            s_animation_header* lbx = (s_animation_header*)Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg);
            graphics::Darken_Fill_(3, 0x14A, animate::Get_Width_(lbx) + 1, 0x1D9);

            _anims = (s_animation_header*)Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg);
            animate::Draw_(2, 0x149, _anims);

            row_count = 0;

            int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
            int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

            invasion::Compute_Colony_Ground_Combat_Info_(&MOX::_colony[colony_idx], &combat_info);

            Print_Best_Summary_(&combat_info, &row_count);
            Print_Race_Summary_(&combat_info, &row_count);

            row_count++;

            Print_Troop_Totals_(2, (row_count * 0xB) + 0x153, (int16_t*)&combat_info);

            video::Copy_Off_To_Back_();

            COLONY::_full_draw = 0;
        }
    }

    // Patch: { 0x00487270, &NEWPUP::Print_Best_Summary_ },
    void __cdecl Print_Best_Summary_(s_invasion_side_info* side_info, int16_t* y_pos) {
        if (side_info->combat_modifiers.antigrav_bonus != 0) {
            COLGCBT::Print_Data_(
                TECHDATA::_technology_applications[9].name,
                static_cast<uint8_t>(side_info->combat_modifiers.antigrav_bonus),
                y_pos,
                side_info
            );
        }

        if (side_info->combat_modifiers.best_armor_tech_app_id != 0) {
            COLGCBT::Print_Data_(
                TECHDATA::_technology_applications[side_info->combat_modifiers.best_armor_tech_app_id].name,
                static_cast<uint8_t>(side_info->combat_modifiers.troop_bonus),
                y_pos,
                side_info
            );
        }

        if (side_info->combat_modifiers.personal_shield_tech_app_id != 0) {
            COLGCBT::Print_Data_(
                TECHDATA::_technology_applications[side_info->combat_modifiers.personal_shield_tech_app_id].name,
                static_cast<uint8_t>(side_info->combat_modifiers.personal_shield_bonus),
                y_pos,
                side_info
            );
        }

        if (side_info->combat_modifiers.rifle_tech_app_id != 0) {
            COLGCBT::Print_Data_(
                TECHDATA::_technology_applications[side_info->combat_modifiers.rifle_tech_app_id].name,
                static_cast<uint8_t>(side_info->combat_modifiers.rifle_bonus),
                y_pos,
                side_info
            );
        }
    }

    // Patch: { 0x004873C0, &NEWPUP::Print_Race_Summary_ },
    void __cdecl Print_Race_Summary_(s_invasion_side_info* side_info, int16_t* summary_buffer) {
        char officer_name[52];

        if (side_info->combat_modifiers.ground_combat_bonus != 0) {
            COLGCBT::Print_Data_(
                ESTRINGS::E_Strings_(0x1B5),
                static_cast<int16_t>(side_info->combat_modifiers.ground_combat_bonus),
                summary_buffer,
                side_info);
        }

        if (side_info->combat_modifiers.low_g_penalty != 0) {
            COLGCBT::Print_Data_(
                ESTRINGS::E_Strings_(0x16C),
                static_cast<int16_t>(side_info->combat_modifiers.low_g_penalty),
                summary_buffer,
                side_info);
        }

        const int16_t subterranean_bonus =
            static_cast<int16_t>(static_cast<uint8_t>(side_info->combat_modifiers.subterranean_bonus));
        if (subterranean_bonus != 0) {
            COLGCBT::Print_Data_(
                ESTRINGS::E_Strings_(0x1F1),
                subterranean_bonus,
                summary_buffer,
                side_info);
        }

        if (side_info->combat_modifiers.difficulty_bonus > 0 &&
            capture::Check_Release_Version_() != 0) {
            COLGCBT::Print_Data_(
                ESTRINGS::E_Strings_(0x007B),
                static_cast<int16_t>(side_info->combat_modifiers.difficulty_bonus),
                summary_buffer,
                side_info);
        }

        if (side_info->combat_modifiers.officer_idx > 0) {
            OFFICER::Officer_Name_(
                officer_name,
                side_info->player_id,
                static_cast<int16_t>(side_info->combat_modifiers.officer_idx));

            COLGCBT::Print_Data_(
                officer_name,
                side_info->combat_modifiers.officer_bonus,
                summary_buffer,
                side_info);
        }
    }

    // Patch: { 0x004870f0, &NEWPUP::Reset_Fields_ },
    void __cdecl Reset_Fields_() {
        fields::Clear_Fields_();
        COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
        fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551100, 0);
    }
}