#include "pch.h"

namespace CMBTDRW1 {

    void __cdecl Detailed_View_Ship_(int16_t ship_idx) {
        char ship_name[16];
        bool is_editable = (ship_idx != -1);

        EVANHELP::Save_Help_List_();
        MOX2::Get_Auto_Function_(&_k_save);
        video::Set_Page_Back_();
        MOX::_ship_frame--;
        MOX2::Execute_Auto_Function_(&_k_save);
        fields::Draw_Visible_Fields_Back_();
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();
        fields::Assign_Auto_Function_(Combat_Draw_View_Ship_, 1);
        allocate::Push_Block_(MOX::_screen_seg);

        if (is_editable) {
            MOX::_ship_data_id = -1;
            COMBINIT::Load_Display_Combat_Ship_(ship_idx);
            
            // BUG: Potential buffer overflow if name is longer than 15 characters
            strcpy(ship_name, MOX::_global_combat_data.name);

            INITSHIP::Get_Ship_Combat_Bonuses_(
                ship_idx, 
                &MOX::_global_combat_data.ship_defense, 
                &MOX::_global_combat_data.ship_missle_defense, 
                &MOX::_global_combat_data.ship_attack
            );

            uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(98, 98);
            MOX::_draw_ship_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);
            MOX::_combat_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);

            s_animation_header* anim = (s_animation_header*)Buffer_Reload_("Combat.lbx", 0xB, MOX::_global_cache_seg);
            remap::Calculate_Picture_Remap_Colors_(anim);

            NEWGAME::_color_ken.colors[0] = remap::Get_Remap_Palette_Color_(0x84);
            NEWGAME::_color_ken.colors[1] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[2] = remap::Get_Remap_Palette_Color_(0x8A);
            NEWGAME::_color_ken.colors[3] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[4] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[5] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[6] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[7] = remap::Get_Remap_Palette_Color_(0x8C);
        }

        if (MOX::_global_combat_data.officer_idx == -1) {
            int16_t p_idx = (int16_t)MOX::_global_combat_data.player_idx;
            if (p_idx > 7) p_idx = 0;
            
            uint8_t race = (uint8_t)MOX::_player[p_idx].race;
            MOX::_detail_fleet_icon_seg = (uint32_t)farload::Far_Reload_Next_("flticons.lbx", race, MOX::_screen_seg);
        } else {
            int16_t mode = is_editable ? 0 : 2;
            MOX::_detail_fleet_icon_seg = OFFICER::Load_Officer_Picture_(0, MOX::_global_combat_data.officer_idx, (uint32_t)MOX::_screen_seg, 0, mode);
        }

        fields::Mark_Fields_();

        if (is_editable) {
            fonts::Set_Font_Style_(4, &NEWGAME::_color_ken);
            fields::Add_Continuous_String_Input_Field_(0x106, 0x32, 0x67, 0xC, ship_name, 0xF, 0, 0, 0, 0);
            fields::Add_Hidden_Field_(0x10C, 0x32, 0x17F, 0x40, "", 0);
        }

        int16_t exit_field_id = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, "", 0);
        bool exit_triggered = false;
        do {
            int16_t input = fields::Get_Input_();
            if (input == exit_field_id) {
                exit_triggered = true;
            }
            video::Set_Page_Off_();
            Combat_Draw_View_Ship_();
            RUSS::Mox_Sync_Update_();
        } while (!exit_triggered);

        if (is_editable) {
            strings::Trim_(ship_name);
            if (ship_name[0] != '\0') {
                strcpy(MOX::_ship[ship_idx].d.name, ship_name);
            }
        }

        fields::Release_Fields_();
        EVANHELP::Restore_Help_List_();
        MOX2::Put_Auto_Function_(&_k_save);
        allocate::Pop_Block_(MOX::_screen_seg);
    }
}