#include "pch.h"

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text) {
        TEXTBOX::Do_Text_Box_(nullptr, (char*)text, 0);
    }

    void __cdecl Help_(const char* message) {
        Message_Box_(message);
    }

    void __cdecl Warning_Box_(const char* message) {
        _warning_box = 1;
        Message_Box_Exploding_((char*)message);
    }

    void __cdecl Message_Box_Exploding_Star_(const char* text, int16_t star_idx) {
        Message_Box_Exploding_((char*)text);
    }

    void __cdecl Message_Box_Exploding_(char* str) {
        Message_Box_Startup_();

        fonts::Set_Font_Style_(4, &_message_text_colors);
        fonts::Set_Font_LF_(0x12);

        _image_seg[0] = (uint8_t*)Buffer_Reload_(s_warning_lbx_00557650, 0, (uint8_t*)MOX::_global_cache_seg);
        remap::Calculate_Picture_Remap_Colors_((s_animation_header*)_image_seg[0]);

        uint32_t size = bitmap::Get_Bitmap_Size_(0x14B, 0xBF);
        _box_bitmap = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
        bitmap::Create_Blank_Bitmap_(0x14B, 0xBF, _box_bitmap);

        _message_box_y = 0x90;
        int mode = 4;
        _first_field = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551120, 0x29);

        s_colors font_colors;
        // Potential bug in original code: font_colors is passed uninitialized
        fonts::Set_Remap_Font_Style_(4, &font_colors);
        fonts::Set_Font_LF_(0x12);

        int16_t height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(0xE2, str);
        if (height > 0x78) {
            mode = 3;
            fonts::Set_Remap_Font_Style_(3, &font_colors);
            fonts::Set_Font_LF_(0x12);
        }

        uint8_t base_color1;
        uint8_t base_color2;
        TEXTBOX::Get_Mox_Text_Box_Base_Colors_(1, &base_color1, &base_color2);
        s_colors* psVar2 = MISC::Font_Colors2_(mode, base_color1, base_color2);
        font_colors = *psVar2;

        fonts::Set_Remap_Font_Style_(mode, &font_colors);
        fonts::Set_Font_LF_(0x12);

        _image_seg[0] = (uint8_t*)Buffer_Reload_(s_warning_lbx_00557650, 0, (uint8_t*)MOX::_global_cache_seg);
        animate::Remap_Draw_(0x9A, _message_box_y, (s_animation_header*)_image_seg[0]);

        Do_Print_1_Or_More_Formatted_Lines_Centered_(0xD0, 0xE8, 0xE2, str);
        video::Copy_Off_To_Back_();

        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
        RUSS::Mox_Sync_Update_();

        bitmap::Screen_Bitmap_Capture_(0xD0, _message_box_y + 0x25, 0x1B2, _message_box_y + 0x9D, (s_animation_header*)_box_bitmap);

        fields::Assign_Auto_Function_(Draw_Animating_Message_Box_, 2);

        int16_t input = fields::Get_Input_();
        while (input == 0) {
            timer::Mark_Time_();
            Draw_Animating_Message_Box_();
            setvesa::Add_Square_To_Scan_List_(5, 0x14, 0x24D, 0x1CB);
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(2);
            input = fields::Get_Input_();
        }

        Message_Box_Cleanup_();
    }

    int16_t __cdecl Confirmation_Box_(char* text) {
        uint8_t color_1;
        uint8_t color_2;
        s_colors font_colors;

        TEXTBOX::Get_Mox_Text_Box_Base_Colors_(2, &color_1, &color_2);
        s_colors* colors_ptr = MISC::Font_Colors2_(4, color_1, color_2);

        font_colors = *colors_ptr;
        fonts::Set_Remap_Font_Style_(4, &font_colors);
        Message_Box_Startup_();

        for (int i = 0; i < 3; ++i) {
            _image_seg[i] = (uint8_t*)Buffer_Reload_(s_confirm_lbx_00557664, i, (uint8_t*)MOX::_global_cache_seg);
            if (i == 0) {
                remap::Calculate_Picture_Remap_Colors_((s_animation_header*)_image_seg[0]);
            }
        }

        _first_field = fields::Add_Hidden_Field_(0xeb, 0x12e, 0x11e, 0x143, s_Y_00557660, 0x29);
        int16_t second_field = fields::Add_Hidden_Field_(0x159, 0x12e, 0x18c, 0x143, s_N_0055765c, 0x29);

        for (int i = 0; i < 3; ++i) {
            _image_seg[i] = (uint8_t*)Buffer_Reload_(s_confirm_lbx_00557664, i, (uint8_t*)MOX::_global_cache_seg);
        }

        int16_t font_idx = 4;
        animate::Remap_Draw_(0xa1, 0x75, (s_animation_header*)_image_seg[0]);

        do {
            TEXTBOX::Get_Mox_Text_Box_Base_Colors_(2, &color_1, &color_2);
            s_colors* colors_ptr2 = MISC::Font_Colors2_(font_idx, color_1, color_2);

            font_colors = *colors_ptr2;
            fonts::Set_Remap_Font_Style_(font_idx, &font_colors);
            font_idx--;

            fonts::Set_Font_LF_(0x12);

            if (FMTPARA::Get_Formatted_Paragraph_Max_Height_(0xe0, text) <= 0x7e) {
                break;
            }
        } while (font_idx > 1);

        Do_Print_1_Or_More_Formatted_Lines_Centered_(0xcc, 0xd0, 0xe0, text);
        Draw_Confirm_Box_();
        video::Copy_Off_To_Back_();
        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27f, 0x1df);
        RUSS::Mox_Sync_Update_();
        fields::Assign_Auto_Function_(Draw_Confirm_Box_, 2);

        int16_t input;
        do {
            timer::Mark_Time_();
            input = fields::Get_Input_();
            if (input == 0) {
                RUSS::Mox_Sync_Update_();
            }
            timer::Release_Time_(2);
        } while (input != _first_field && input != second_field);

        Message_Box_Cleanup_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;

        return (input == _first_field) ? 1 : 0;
    }
}