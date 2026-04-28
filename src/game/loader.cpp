#include "pch.h"

namespace LOADER {
    void __cdecl Load_Main_Screen_Pictures_() {
        allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);

        MOX::_main_game_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 1, MOX::_global_screen_seg);
        MOX::_main_next_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 2, MOX::_global_screen_seg);
        MOX::_main_colonies_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 3, MOX::_global_screen_seg);
        MOX::_main_planets_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 4, MOX::_global_screen_seg);
        MOX::_main_fleets_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 5, MOX::_global_screen_seg);
        MOX::_main_zoom_in_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 6, MOX::_global_screen_seg);
        MOX::_main_zoom_out_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 7, MOX::_global_screen_seg);
        MOX::_main_leaders_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 8, MOX::_global_screen_seg);
        MOX::_main_races_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 9, MOX::_global_screen_seg);
        MOX::_main_info_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 10, MOX::_global_screen_seg);
        MOX::_fleet_box_selection_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 11, MOX::_global_screen_seg);
        MOX::_darkened_turn_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 12, MOX::_global_screen_seg);

        allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);

        MOX::_moveable_box_pict[4] = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x52, MOX::_global_screen_seg);

        RUSS::Mox_Update_();

        allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);

        MOX::_moveable_box_pict[14] = nullptr;
        MOX::_fleet_box_scroll_up_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x3A, MOX::_global_screen_seg);
        MOX::_fleet_box_scroll_dn_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x3B, MOX::_global_screen_seg);
        MOX::_fleet_box_all_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x3C, MOX::_global_screen_seg);

        RUSS::Mox_Update_();

        MOX::_fleet_box_att_xenons_lg_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x3D, MOX::_global_screen_seg);
        MOX::_fleet_box_colonize_lg_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x3E, MOX::_global_screen_seg);
        MOX::_fleet_box_engage_lg_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x3F, MOX::_global_screen_seg);
        MOX::_fleet_box_transport_lg_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x40, MOX::_global_screen_seg);
        MOX::_fleet_box_outpost_lg_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x41, MOX::_global_screen_seg);
        MOX::_fleet_box_close_lg_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x42, MOX::_global_screen_seg);
        MOX::_fleet_box_att_xenons_sm_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x43, MOX::_global_screen_seg);
        MOX::_fleet_box_engage_sm_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x44, MOX::_global_screen_seg);
        MOX::_fleet_box_colonize_sm_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x45, MOX::_global_screen_seg);
        MOX::_fleet_box_transport_sm_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x46, MOX::_global_screen_seg);
        MOX::_fleet_box_outpost_sm_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x47, MOX::_global_screen_seg);
        MOX::_fleet_box_close_sm_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, 0x48, MOX::_global_screen_seg);

        int picture_idx = 0xF1;
        allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 4; ++j) {
                int index = j + i * 4;
                MOX::_hl_antaran_ship_icon_seg[index] = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, picture_idx, MOX::_global_screen_seg);
                MOX::_antaran_ship_icons[index] = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, picture_idx, MOX::_global_screen_seg);
                picture_idx++;
            }
        }

        allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);

        MOX::_star_width = 0x21;
        MOX::_star_height = 0x21;
        RUSS::Mox_Update_();

        int star_picture_idx = 1;
        for (int i = 0; i < 11; ++i) {
            RUSS::Mox_Update_();
            MOX::_star_colors_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(s_gstar_lbx_00558544, star_picture_idx, MOX::_global_screen_seg);
            star_picture_idx++;
        }

        SYS::_sys_data_buffer = allocate::Allocate_Next_Block_(MOX::_global_screen_seg, 0xDC);

        int player_light_idx = 0x109;
        for (int i = 0; i < 8; ++i) {
            MAINSCR::_player_lights_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(s_BUFFER0_LBX, player_light_idx, MOX::_global_screen_seg);
            player_light_idx++;
        }

        allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);
    }

    void __cdecl Load_Pictures_() {
        Load_Main_Screen_Pictures_();
        RUSS::Mox_Update_();
        COLONY::Load_Colony_Anims_();
        RUSS::Mox_Update_();
        TECHINIT::Init_Tech_();
        RUSS::Mox_Update_();

        void* block = MOX::_global_screen_seg;
        int32_t remaining_space = allocate::Get_Remaining_Block_Space_(block);

        // Allocate block and leave space for the animation header
        s_animation_header* anim_seg = (s_animation_header*)allocate::Allocate_Next_Block_(block, remaining_space - 12 /* sizeof(s_animation_header) */);
        MOX::_screen_seg = anim_seg;

        // Initialize the start of the frame offset data area
        memset(anim_seg->frame_offsets, 0, 20);
    }
}