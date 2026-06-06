#include "pch.h"

namespace ANTAROOM {
    s_colors _antaran_room_normal_colors = {{250, 254, 255, 251, 252, 253, 254, 255}};
    char* _antaran_room_message;
    uint8_t* _antaran_room_mouse_bitmap;
    s_mouse_list _antaran_room_blank_mouse[1] = {{0, 0, 0, 0, 639, 479}};

    void __cdecl Draw_Main_Antaran_Room_Screen_() {
        video::Set_Page_Off_();
        fonts::Set_Outline_Color_(0xFA, 0xFA);

        if (MOX::_settings.animations_on == 1) {
            file_ani::Draw_File_Animation_Stencil_();
        }

        fonts::Set_Outline_Color_(0x0, 0xFA);
        fonts::Set_Font_Style_Outline_(4, &_antaran_room_normal_colors);

        int16_t max_height = BILL::_Get_Paragraph_Max_Height_(0x226, _antaran_room_message);
        BILL::_Print_Paragraph_(0x37, 0x1D6 - max_height, 0x226, _antaran_room_message, 2);

        JIM::Play_Streaming_Music_(0x78, -1, 1);
    }

    void __cdecl Main_Antaran_Room_Screen_(int16_t message_set_index) {
        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        fields::Enable_Draw_All_Fields_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        mouse::Set_Mouse_List_(_antaran_room_blank_mouse, 1);
        JIM::Play_Streaming_Music_(120, -1, 1);
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();

        MOX2::Get_Auto_Function_(&EVENTS::_jim_saved_autofunction);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();

        Reload_Main_Antaran_Room_Screen_();
        Get_Antaran_Room_Message_(1 - message_set_index);

        bool is_done = false;
        fields::Assign_Auto_Function_(Draw_Main_Antaran_Room_Screen_, 2);

        while (!is_done) {
            timer::Mark_Time_();
            fields::Clear_Fields_();

            int16_t hidden_field = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);

            if (fields::Get_Input_() == hidden_field || key::Keyboard_Status_()) {
                is_done = true;
            }

            if (!is_done) {
                Draw_Main_Antaran_Room_Screen_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
            }
        }

        MOX::_auto_turn_exit = 1;
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        file_ani::Reset_File_Animation_Buffer_();
        mouse::Set_Mouse_List_(_antaran_room_blank_mouse, 1);
        JIM::Play_Background_Music_();
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();

        RUSS::Release_All_Blocks_(MOX::_screen_seg);
        MOX2::Put_Auto_Function_(&EVENTS::_jim_saved_autofunction);
    }

    void __cdecl Reload_Main_Antaran_Room_Screen_() {
        _antaran_room_mouse_bitmap = (uint8_t*)allocate::Allocate_First_Block_(MOX::_screen_seg, 0x395);
        s_animation_header* pic = (s_animation_header*)farload::Far_Reload_Next_("antaroom.LBX", 0, MOX::_screen_seg);
        bitmap::Clear_Mouse_Image_(0);
        bitmap::Create_Blank_Bitmap_(30, 30, _antaran_room_mouse_bitmap);
        bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)_antaran_room_mouse_bitmap, pic);
        video::Set_Page_Off_();
        animate::Draw_Palette_(pic);
        palette::Set_Palette_Changes_(0, 255);
        graphics::Fill_(0, 0, 639, 479, 0);
        _antaran_room_message = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 210);

        int32_t remaining = allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        uint8_t* anim_buf = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, remaining - 100);
        file_ani::Set_File_Animation_Buffer_(anim_buf);

        video::Set_Refresh_Stencil_();
        file_ani::Open_File_Animation_("antaroom.LBX", 1);
        video::Set_Page_Off_();
        file_ani::Draw_File_Animation_();
        palette::Refresh_Palette_();
        video::Copy_Off_To_Back_();

        bitmap::Draw_Bitmap_To_Mouse_(0, 0, (s_animation_header*)_antaran_room_mouse_bitmap, 0);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }

    void __cdecl Get_Antaran_Room_Message_(int16_t message_index) {
        char buffer[200];

        uint32_t random_val = random::Random_(4);
        int16_t message_lbx_index = (int16_t)(random_val + (message_index + (uint8_t)MOX::_settings.language * 2) * 4 - 1);

        farload::Farload_Data_Static_("antarmsg.LBX", message_lbx_index, buffer, 0, 1, sizeof(buffer));

        strcpy(_antaran_room_message, buffer);
    }
}