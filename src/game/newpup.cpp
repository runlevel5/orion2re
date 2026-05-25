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
        int16_t summary_row;
        s_invasion_side_info invasion_info;
        int16_t planet_index;

        COLONY::Set_Colony_Font_();

        if (COLONY::_full_draw != 0) {
            COLONY::_full_draw = 1;
            MOX2::Execute_Auto_Function_(&NEWPUP::_saved_autofunction);

            s_animation_header* anim = (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg);
            graphics::Darken_Fill_(3, 0x14A, (int16_t)(animate::Get_Width_(anim) + 1), 0x1D9);

            NEWPUP::_anims = (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg);
            animate::Draw_(2, 0x149, NEWPUP::_anims);

            summary_row = 0;
            planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);

            invasion::Compute_Colony_Ground_Combat_Info_(
                &MOX::_colony[MOX::_planet[planet_index].colony_index],
                &invasion_info);

            Print_Best_Summary_(&invasion_info, &summary_row);
            Print_Race_Summary_(&invasion_info, &summary_row);

            ++summary_row;
            Print_Troop_Totals_(2, summary_row * 11 + 0x153, &invasion_info);

            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }
    }

    void __cdecl Print_Data_(char* str, int16_t value, int16_t* y_pos, s_invasion_side_info* side_info) {
        (void)side_info;

        char text_buffer[500];
        s_animation_header* anim;

        sprintf(
            text_buffer,
            ESTRINGS::E_Strings_(0x4B),
            s_0_0055110c,
            str,
            s_1_00551110,
            value);

        COLONY::Set_Colony_Font_To_(2);

        anim = (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg);

        FMTPARA::Print_Formatted_Paragraph_(11, *y_pos * 11 + 0x153, animate::Get_Width_(anim) - 0x12, text_buffer, 0);

        ++(*y_pos);
    }

    void __cdecl Print_Best_Summary_(s_invasion_side_info* side_info, int16_t* y_pos) {
        if (side_info->combat_modifiers.antigrav_bonus != 0) {
            Print_Data_(
                TECHDATA::_technology_applications[9].name,
                (uint8_t)side_info->combat_modifiers.antigrav_bonus,
                y_pos,
                side_info
            );
        }

        if (side_info->combat_modifiers.best_armor_tech_app_id != 0) {
            uint8_t tech_app_id = side_info->combat_modifiers.best_armor_tech_app_id;

            Print_Data_(
                TECHDATA::_technology_applications[tech_app_id].name,
                (uint8_t)side_info->combat_modifiers.troop_bonus,
                y_pos,
                side_info
            );
        }

        if (side_info->combat_modifiers.personal_shield_tech_app_id != 0) {
            uint8_t tech_app_id = side_info->combat_modifiers.personal_shield_tech_app_id;

            Print_Data_(
                TECHDATA::_technology_applications[tech_app_id].name,
                (uint8_t)side_info->combat_modifiers.personal_shield_bonus,
                y_pos,
                side_info
            );
        }

        if (side_info->combat_modifiers.rifle_tech_app_id != 0) {
            uint8_t tech_app_id = side_info->combat_modifiers.rifle_tech_app_id;

            Print_Data_(
                TECHDATA::_technology_applications[tech_app_id].name,
                (uint8_t)side_info->combat_modifiers.rifle_bonus,
                y_pos,
                side_info
            );
        }
    }

    // Patch: { 0x004873C0, &NEWPUP::Print_Race_Summary_ },
    void __cdecl Print_Race_Summary_(s_invasion_side_info* side_info, int16_t* summary_buffer) {
        char officer_name[52];

        if (side_info->combat_modifiers.ground_combat_bonus != 0) {
            Print_Data_(
                ESTRINGS::E_Strings_(0x1B5),
                static_cast<int16_t>(side_info->combat_modifiers.ground_combat_bonus),
                summary_buffer,
                side_info);
        }

        if (side_info->combat_modifiers.low_g_penalty != 0) {
            Print_Data_(
                ESTRINGS::E_Strings_(0x16C),
                static_cast<int16_t>(side_info->combat_modifiers.low_g_penalty),
                summary_buffer,
                side_info);
        }

        const int16_t subterranean_bonus =
            static_cast<int16_t>(static_cast<uint8_t>(side_info->combat_modifiers.subterranean_bonus));
        if (subterranean_bonus != 0) {
            Print_Data_(
                ESTRINGS::E_Strings_(0x1F1),
                subterranean_bonus,
                summary_buffer,
                side_info);
        }

        if (side_info->combat_modifiers.difficulty_bonus > 0 &&
            capture::Check_Release_Version_() != 0) {
            Print_Data_(
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

            Print_Data_(
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

    // Patch: { 0x004874B0, &NEWPUP::Print_Troop_Totals_ },
    void __cdecl Print_Troop_Totals_(int16_t x_base, int16_t unused_param_2, s_invasion_side_info* invasion_side) {
        (void)unused_param_2;

        int16_t troop_type = 0;
        int16_t displayed_column_base = 0;
        char text_buffer[500];

        do {
            const int16_t player_id = invasion_side->player_id;
            const uint16_t race =
                static_cast<uint16_t>(static_cast<uint8_t>(MOX::_player[player_id].race));
            s_animation_header* troop_anim =
                COLONY::Player_Troop_Anim_(player_id, troop_type, race);
            const int16_t displayed_column = static_cast<int16_t>(displayed_column_base + 1);
            const int16_t row_index = 2;

            COLONY::Set_Colony_Font_To_Player_(1, (uint8_t)player_id);

            sprintf(
                text_buffer,
                ESTRINGS::E_Strings_(0x0d),
                static_cast<int32_t>(invasion_side->unit_strengths[troop_type]) -
                static_cast<int32_t>(invasion_side->combat_modifiers.difficulty_bonus));

            {
                const int32_t width = static_cast<int32_t>(animate::Get_Width_(
                    (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg)));
                const int16_t x = (int16_t)(
                    (((width + ((width >> 31) & 3)) >> 2) * displayed_column) + x_base);
                fonts::Print_Centered_(x, 0x1d1, text_buffer);
            }

            {
                const uint8_t hit_points =
                    static_cast<uint8_t>(invasion_side->unit_hps[troop_type]);

                sprintf(
                    text_buffer,
                    ESTRINGS::E_Strings_((hit_points > 1) ? 0x17 : 0x16),
                    static_cast<uint32_t>(hit_points));

                const int32_t width = static_cast<int32_t>(animate::Get_Width_(
                    (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg)));
                const int16_t x = (int16_t)(
                    (((width + ((width >> 31) & 3)) >> 2) * displayed_column) + x_base);
                fonts::Print_Centered_(x, 0x1c8, text_buffer);
            }

            {
                const int32_t unit_count =
                    static_cast<int32_t>(invasion_side->unit_counts[troop_type]);
                char* troop_name = COLONY::Player_Troop_Name_(player_id, troop_type, race);

                sprintf(
                    text_buffer,
                    ESTRINGS::E_Strings_(0x0e),
                    unit_count,
                    troop_name);

                const int32_t width = static_cast<int32_t>(animate::Get_Width_(
                    (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg)));
                const int16_t x = (int16_t)(
                    (((width + ((width >> 31) & 3)) >> 2) * displayed_column) + x_base);
                fonts::Print_Centered_(x, 0x1d1 - row_index * 9, text_buffer);
            }

            {
                const int32_t width = static_cast<int32_t>(animate::Get_Width_(
                    (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLGCBT_LBX, 0x15, MOX::_global_cache_seg)));
                const int16_t x = (int16_t)(
                    (((width + ((width >> 31) & 3)) >> 2) * displayed_column) + x_base);
                ERIC::Draw_Bottom_Centered_(x, 0x1d9 - (row_index + 1) * 9, troop_anim);
            }

            {
                const int32_t unit_count =
                    static_cast<int32_t>(invasion_side->unit_counts[troop_type]);
                sprintf(text_buffer, ESTRINGS::E_Strings_(0x7c), unit_count);
            }

            ++troop_type;
            displayed_column_base = displayed_column;
        } while (troop_type < 3);
    }
}