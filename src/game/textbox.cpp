#include "pch.h"

namespace TEXTBOX {
    void __cdecl Get_Help_Lbx_Name_(char* lbx_name) {
        switch (MOX::_settings.language) {
            case 1:
                strcpy(lbx_name, s_GER_HELP_LBX_00557480);
                break;
            case 2:
                strcpy(lbx_name, s_FRE_HELP_LBX_00557470);
                break;
            case 3:
                strcpy(lbx_name, s_SPA_HELP_LBX_00557450);
                break;
            case 4:
                strcpy(lbx_name, s_ITA_HELP_LBX_00557460);
                break;
            default:
                strcpy(lbx_name, s_HELP_LBX_00557490);
                break;
        }
    }

    void __cdecl Do_Text_Box_(char* title, char* body_text, int16_t input_param) {
        int16_t x = 0;
        int16_t y = 0;
        uint8_t color_1 = 0;
        uint8_t color_2 = 0;
        s_colors style_colors;
        s_colors base_colors;
        int16_t text_x, text_y;
        int16_t window_status;

        Get_Mox_Text_Box_Base_Colors_(0, &color_1, &color_2);
        Text_Box_Startup_(60000 /* 0xea60 */);

        style_colors = *MISC::Font_Colors2_(5, color_1, color_2);
        base_colors = *MISC::Font_Colors2_(3, color_1, color_2);

        fonts::Set_Font_Style_(3, &style_colors);

        int16_t height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(339 /* 0x153 */, body_text);
        int16_t width = FMTPARA::Get_Last_Formatted_Paragraph_Max_Width_();
        if (width > 339 /* 0x153 */) {
            width = 339 /* 0x153 */;
        }

        Box_Centered_On_XY_(&x, &y, height, title != nullptr);

        s_animation_header* anim = (s_animation_header*)farload::Far_Reload_Next_(s_textbox_lbx_00557444, 3, (void*)_textbox_seg);

        if (title != nullptr) {
            fonts::Set_Remap_Font_Style_(5, &style_colors);
            fonts::Get_String_Width_(title);
            MISC::Set_Fitted_Font_Style_(title, &style_colors, 5, 2, -1, 380 /* 0x17c */, 3);
            
            int16_t current_style = MISC::Get_Font_Style_();
            int16_t current_spacing = MISC::Get_Font_Spacing_();
            
            style_colors = *MISC::Font_Colors2_(current_style, color_1, color_2);
            fonts::Set_Remap_Font_Style_(current_style, &style_colors);
            fonts::Set_Font_Spacing_(current_spacing);
            
            fonts::Print_Centered_(x + 190 /* 0xbe */, y + 11 /* 0xb */, title);
        }

        fonts::Set_Remap_Font_Style_(3, &base_colors);

        if (title != nullptr) {
            text_x = x + 20 /* 0x14 */;
            text_y = y + 42 /* 0x2a */;
        } else {
            text_x = x + 8 + (362 /* 0x16a */ - (int32_t)width) / 2;
            text_y = y + 12 + ((int32_t)height + 43 /* 0x2b */ - (int32_t)height) / 2;
        }

        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(x + 20 /* 0x14 */, y + 42 /* 0x2a */, x + 359 /* 0x167 */, y + 418 /* 0x1a2 */);
        video::Clip_On_();

        FMTPARA::Print_Formatted_Paragraph_(text_x, text_y, width, body_text, 0);

        MISC::Restore_Window_Status_(window_status);
        video::Copy_Off_To_Back_();

        fields::Add_Hidden_Field_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, s__00551120, 41 /* 0x29 */);

        Text_Box_Get_Input_(input_param);

        animate::Set_Animation_Frame_(anim, 1);
        animate::Remap_Draw_(x + 158 /* 0x9e */, _bottom_y - 27 /* 0x1b */, anim);

        Text_Box_Cleanup_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
    }

    void __cdecl Text_Box_(const char* param_1, const char* param_2) {
        Do_Text_Box_(const_cast<char*>(param_1), const_cast<char*>(param_2), 0);
    }
}