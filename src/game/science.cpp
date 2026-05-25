#include "pch.h"

namespace SCIENCE {
    void __cdecl Show_Off_Captured_Tech_(int16_t* captured_tech_ids, uint16_t tech_count, int16_t message_arg) {
        char text_buffer[0x100];

        allocate::Push_Block_(MOX::_screen_seg);

        void* animation_buffer = allocate::Allocate_First_Block_(MOX::_screen_seg, 0x7C830);
        file_ani::Set_File_Animation_Buffer_(animation_buffer);

        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 3, text_buffer, 0x100);
        JIM::Decode_Text_System_(text_buffer, message_arg, 0x100);

        char* trooper_anim = BILL::Cache_Player_Trooper_Anim_();
        SCIENCE::Science_Room_(trooper_anim, tech_count, captured_tech_ids, text_buffer);

        file_ani::Reset_File_Animation_Buffer_();
        allocate::Pop_Block_(MOX::_screen_seg);
        COLONY::Reset_Mouse_Picture_();
        JIM::Play_Background_Music_();
    }

    void __cdecl Update_Science_Anims_(int unused, uint8_t draw_multi_anim, uint8_t draw_anim_2, uint8_t draw_anim_1) {
        if (MOX::_settings.animations_on != 0) {
            file_ani::Draw_Multi_File_Animation_Stencil_(1);
        } else {
            file_ani::Set_File_Animation_Frame_(18);
        }

        if (draw_anim_2 == 1) {
            s_animation_header* anim = (s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 2, MOX::_global_cache_seg);
            animate::Draw_(0, 0, anim);
        }

        if (draw_multi_anim != 0) {
            file_ani::Draw_Multi_File_Animation_(0);
        }

        if (draw_anim_1 == 1) {
            s_animation_header* anim = (s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 1, MOX::_global_cache_seg);
            animate::Draw_(0, 0, anim);
        }
    }

    void __cdecl Show_Off_Stolen_Tech_(uint16_t tech_idx, int16_t other_idx) {
        char message_text[0x100];
        int16_t discovered_tech = (int16_t)tech_idx;
        void* animation_block;
        char* spy_anim;

        allocate::Push_Block_(MOX::_screen_seg);
        animation_block = allocate::Allocate_First_Block_(MOX::_screen_seg, 0x7C830);
        file_ani::Set_File_Animation_Buffer_(animation_block);
        JIM::Get_Text_Message_(SCIENCE::s_billtext_lbx_0055135c, 2, message_text, 0x100);
        JIM::Decode_Text_Tech_And_Other_Name_(message_text, MOX::_PLAYER_NUM, tech_idx, other_idx, 0x100);
        spy_anim = BILL::Cache_Player_Spy_Anim_();
        SCIENCE::Science_Room_(spy_anim, 1, &discovered_tech, message_text);
        file_ani::Reset_File_Animation_Buffer_();
        allocate::Pop_Block_(MOX::_screen_seg);
        COLONY::Reset_Mouse_Picture_();
        JIM::Play_Background_Music_();
    }

    void __cdecl Science_Room_(char* intro_animation_name, uint16_t discovery_count, int16_t* discovery_data, char* footer_text) {
        MOX2::action_func_t saved_auto_function = nullptr;
        int16_t science_hot_key;
        uint16_t remaining_discoveries;
        uint16_t current_entry_index;
        bool first_entry;
        bool room_animation_ready;
        int16_t jumble_amount;
        int16_t jumble_steps;
        s_animation_header* science_room_picture;
        void* text_data_buffer;
        void* current_text_bitmap;
        void* previous_text_bitmap;
        void* unused_strip_bitmap;
        char help_lbx_name[20];
        char bill_text[80];
        char animation_lbx_names[8][64];
        char weapon_mod_text[256];

        MOX2::Get_Auto_Function_(&saved_auto_function);
        fields::Deactivate_Help_List_();
        video::Clip_Off_();
        allocate::Push_Block_(MOX::_screen_seg);
        fields::Clear_Fields_();

        remaining_discoveries = discovery_count;
        if (discovery_count != 0) {
            for (uint16_t i = 0; i < discovery_count; ++i) {
                sprintf(animation_lbx_names[i], s_TANM__03hi_LBX_005512a0, discovery_data[i] & 0x01FF);
            }
        }

        RUSS::Mox_Update_();

        science_room_picture = (s_animation_header*)farload::Far_Reload_Next_(s_SCIENCE_LBX_00551290, 0, MOX::_screen_seg);
        SCIENCE::_science_mouse_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x395);
        text_data_buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x57C);
        current_text_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0xAF, 0xC3));
        previous_text_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0xAF, 0xC3));
        unused_strip_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x258, 0x3C));

        buffer::Flush_Buffer_Space_(MOX::_global_cache_seg);

        if (MOX::_settings.animations_on == 0) {
            animate::Set_Animation_Frame_((s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 1, MOX::_global_cache_seg), 5);
        }

        bitmap::Create_Blank_Bitmap_(0xAF, 0xC3, current_text_bitmap);
        bitmap::Create_Blank_Bitmap_(0xAF, 0xC3, previous_text_bitmap);
        bitmap::Create_Blank_Bitmap_(0x258, 0x3C, unused_strip_bitmap);

        fonts::Set_Outline_Color_(0, 0xF0);
        fonts::Set_Font_Style_Outline_(4, &SCIENCE::_science_normal_color);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        ERIC::Fast_Fade_Out_();

        file_ani::Open_Multi_File_Animation_(intro_animation_name, 0, 1);
        fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551100, 0);
        science_hot_key = fields::Add_Hot_Key_(s__00551120);

        video::Set_Page_Off_();
        fields::Assign_Auto_Function_(&SCIENCE::Draw_Science_Room_, 2);

        COLONY::Clear_Mouse_Picture_();
        bitmap::Clear_Mouse_Image_(0);
        bitmap::Create_Blank_Bitmap_(0x1E, 0x1E, (s_animation_header*)SCIENCE::_science_mouse_bitmap);
        bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)SCIENCE::_science_mouse_bitmap, science_room_picture);
        bitmap::Draw_Bitmap_To_Mouse_(0, 0, (s_animation_header*)SCIENCE::_science_mouse_bitmap, 0);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 4, bill_text, 0x50);

        buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 1, MOX::_global_cache_seg);
        palette::Set_Palette_Changes_(0, 0xFF);
        animate::Draw_Palette_(science_room_picture);
        BILL::Turn_Off_Palettes_(0, 0xFF);
        video::Set_Refresh_Stencil_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        file_ani::Draw_Multi_File_Animation_(1);
        video::Copy_Off_To_Back_();

        {
            int16_t fade_amount = 100;

            BILL::Turn_Off_Palettes_(0, 0xFF);
            fields::Disable_Mouse_Wait_();
            fields::Clear_Mouse_Buffer_();

            do {
                JIM::Play_Streaming_Music_(0x75, -2, 1);
                timer::Mark_Time_();

                fade_amount -= 0x14;
                if (fade_amount < 0) {
                    fade_amount = 0;
                }

                palette::Set_Palette_Changes_(0, 0xFF);
                palstore::Darken_Palette_(fade_amount);
                palette::Clear_Palette_Changes_(0, 0xFF);

                animate::Draw_(0, 0, (s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 2, MOX::_global_cache_seg));
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
            } while (fade_amount > 0);
        }

        ERIC::Set_Fade_Percent_(0);

        if (discovery_count != 0) {
            first_entry = true;
            room_animation_ready = false;
            current_entry_index = 0;
            memcpy(&jumble_amount, animation_lbx_names[0], sizeof(jumble_amount));
            memcpy(&jumble_steps, animation_lbx_names[0], sizeof(jumble_steps));

            bool should_close = false;

            for (;;) {
                JIM::Play_Streaming_Music_(0x75, -2, 1);
                timer::Mark_Time_();

                const int16_t input_key = fields::Get_Input_();
                if (input_key == 0 && !first_entry) {
                    SCIENCE::Update_Science_Anims_(true, true, true, true);
                } else {
                    if (remaining_discoveries == 0 || science_hot_key == input_key) {
                        should_close = true;
                        break;
                    }

                    SCIENCE::Update_Science_Anims_(true, false, true, true);
                    RUSS::Mox_Sync_Update_();

                    file_ani::Open_Multi_File_Animation_(animation_lbx_names[current_entry_index], 0, 0);
                    video::Set_Page_Off_();
                    file_ani::Draw_Multi_File_Animation_Palette_(0);
                    palette::Refresh_Palette_();

                    bitmap::Copy_Bitmap_To_Bitmap_((s_animation_header*)previous_text_bitmap, (s_animation_header*)current_text_bitmap);
                    TEXTBOX::Get_Help_Lbx_Name_(help_lbx_name);

                    char* formatted_text = (char*)farload::Far_Reload_Data_(
                            help_lbx_name,
                            0,
                            text_data_buffer,
                            discovery_data[current_entry_index] & 0x01FF,
                            1,
                            0x57B);

                    bitmap::Create_Blank_Bitmap_(0xAF, 0xC3, current_text_bitmap);

                    fonts::Set_Font_Style_(3, &SCIENCE::_science_high_color);
                    FMTPARA::Print_Formatted_Paragraph_To_Bitmap_(2, 1, 0xAB, formatted_text, 2, (s_animation_header*)current_text_bitmap);

                    const int16_t paragraph_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(0xAB, formatted_text);
                    int16_t text_y = paragraph_height + 1;

                    fonts::Set_Font_Style_(2, &SCIENCE::_science_high_color);
                    fonts::Set_Font_LF_(1);

                    if ((discovery_data[current_entry_index] & 0x4000) != 0) {
                        fonts::Print_To_Bitmap_(2, text_y, bill_text, current_text_bitmap);

                        ++current_entry_index;

                        int32_t weapon_mask = discovery_data[current_entry_index];
                        uint16_t weapon_mod_index = 1;
                        weapon_mod_text[0] = '\0';

                        while (weapon_mask != 0) {
                            if ((weapon_mask & 1) != 0) {
                                strcpy(&weapon_mod_text[strlen(weapon_mod_text)], TECHDATA::_weapon_modifications[weapon_mod_index].name);
                                strcpy(&weapon_mod_text[strlen(weapon_mod_text)], s__0055136c);
                            }

                            weapon_mask >>= 1;
                            ++weapon_mod_index;
                        }

                        formatted_text = weapon_mod_text;
                        text_y = paragraph_height + 0x10;
                    } else {
                        formatted_text += 0x67;
                    }

                    FMTPARA::Print_Formatted_Paragraph_To_Bitmap_(2, text_y, 0xAB, formatted_text, 2, (s_animation_header*)current_text_bitmap);

                    ++current_entry_index;
                    --remaining_discoveries;

                    first_entry = false;
                    jumble_amount = (MOX::_settings.animations_on != 0) ? 100 : 0;
                    jumble_steps = 6;

                    do {
                        SCIENCE::Update_Science_Anims_(true, false, true, true);
                    } while (mouse::Mouse_Button_() != 0);

                    fields::Clear_Mouse_Buffer_();
                }

                if (room_animation_ready) {
                    if (jumble_amount <= 0) {
                        animate::Draw_(0xDD, 0x2C, (s_animation_header*)current_text_bitmap);
                    } else {
                        bitmap::Jumble_Bitmap_((s_animation_header*)previous_text_bitmap, (s_animation_header*)current_text_bitmap, jumble_amount, jumble_steps);
                        jumble_amount -= 0x14;
                        animate::Draw_(0xDD, 0x2C, (s_animation_header*)previous_text_bitmap);

                        if (jumble_steps > 1) {
                            --jumble_steps;
                        }
                    }
                } else {
                    room_animation_ready = animate::Get_Current_Frame_(
                            (s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 1, MOX::_global_cache_seg)) >= 5;
                }

                fonts::Set_Outline_Color_(0, 0xF2);
                fonts::Set_Font_Style_Outline_(4, &SCIENCE::_science_normal_color);
                FMTPARA::Print_Formatted_Paragraph_(0x91, 0x1A4, 0x17C, footer_text, 2);

                timer::Release_Time_(1);
                RUSS::Mox_Sync_Update_();
            }

            if (should_close) {
                int16_t frame = animate::Get_Current_Frame_(
                        (s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 1, MOX::_global_cache_seg));
                --frame;

                if (frame >= 0 && MOX::_settings.animations_on != 0) {
                    do {
                        const int32_t frame_index = frame;

                        timer::Mark_Time_();

                        animate::Set_Animation_Frame_(
                                (s_animation_header*)buffer::Buffer_Reload_(s_SCIENCE_LBX_00551290, 1, MOX::_global_cache_seg),
                                frame_index);

                        SCIENCE::Update_Science_Anims_(false, false, true, true);

                        if (frame_index < 5) {
                            bitmap::Scale_Bitmap_(
                                    (s_animation_header*)previous_text_bitmap,
                                    (s_animation_header*)current_text_bitmap,
                                    SCIENCE::_bm_scale_x[frame_index],
                                    SCIENCE::_bm_scale_y[frame_index]);

                            animate::Draw_(
                                    SCIENCE::_bm_scaled_x1[frame_index],
                                    SCIENCE::_bm_scaled_y1[frame_index],
                                    (s_animation_header*)previous_text_bitmap);
                        } else {
                            animate::Draw_(0xDD, 0x2C, (s_animation_header*)current_text_bitmap);
                        }

                        --frame;

                        timer::Release_Time_(1);
                        RUSS::Mox_Sync_Update_();
                        JIM::Play_Streaming_Music_(0x75, -2, 1);
                    } while (frame >= 0);
                }

                SCIENCE::Update_Science_Anims_(false, false, false, false);
                RUSS::Mox_Sync_Update_();
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
        fields::Clear_Fields_();
        fields::Clear_Mouse_Buffer_();
        fields::Enable_Mouse_Wait_();
        MOX2::Put_Auto_Function_(&saved_auto_function);
        ERIC::Set_Fade_Percent_(0);
    }

    void __cdecl Draw_Science_Room_() {
    }

    void __cdecl Show_Off_Researched_Tech_(int16_t player_idx) {
        int16_t researched_tech_ids[4];
        char science_room_lbx[64];
        char decoded_text[256];
        short found_count = 0;

        MOX::_cur_plyr_ptr = &MOX::_player[player_idx];

        if (MOX::_cur_plyr_ptr->current_research_field != 0) {
            const uint8_t research_field = static_cast<uint8_t>(MOX::_cur_plyr_ptr->current_research_field);

            TECH::Technology_Fields_Name_(research_field);

            const uint16_t* field_tech_ids = SCIENCE::_technology_fields[research_field].tech;
            for (int tech_slot = 0; tech_slot < 4; ++tech_slot) {
                const uint16_t tech_id = field_tech_ids[tech_slot];
                if (tech_id > 0 && MOX::_cur_plyr_ptr->tech_applications[tech_id] == 3) {
                    researched_tech_ids[found_count++] = (int16_t)tech_id;
                }
            }

            if (found_count != 0) {
                JIM::Get_Text_Message_(SCIENCE::s_billtext_lbx_0055135c, 1, decoded_text, 0xFA);
                JIM::Decode_Text_Field_(decoded_text, research_field, 0xFA);

                sprintf(
                    science_room_lbx,
                    SCIENCE::s_SR_R_x_SC_LBX_005512b0,
                    static_cast<unsigned int>(static_cast<uint8_t>(MOX::_player[player_idx].race)));

                SCIENCE::Science_Room_(science_room_lbx, found_count, researched_tech_ids, decoded_text);
                return;
            }
        }

        decoded_text[0] = '\0';
        fields::Clear_Fields_();
        ERIC::Fast_Fade_Out_();
    }
}