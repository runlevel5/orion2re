#include "pch.h"

namespace {
    const char s_caret_00556074[2] = "^";

    int16_t _turn_sum_list_y[16] = {
        0, 10, 20, 30, 40, 50, 60, 70,
        80, 90, 100, 110, 120, 130, 140, 0,
    };

    int16_t _turn_sum_field_y2[16] = {
        9, 19, 29, 39, 49, 59, 69, 79,
        89, 99, 109, 119, 129, 139, 149, 0,
    };

    void __cdecl Draw_Turn_Summary_Popup_App_(int16_t x, int16_t y, uint8_t msg_idx)
    {
        if (msg_idx <= 0xFA)
        {
            char buffer[256];

            fonts::Set_Font_LF_(10);
            MSG_::sprintf_msg_(buffer, &MSG_::_msgs[msg_idx]);
            fonts::Print_(x, y, s_caret_00556074);
            FMTPARA::Print_Formatted_Paragraph_(x + 13, y, 320, buffer, 0);
        }
    }
}

namespace s_turnsum {
    const char lbx_00558c70[12] = "turnsum.lbx";
}

namespace TURNSUM {
    s_colors _turn_sum_normal_color = {{110, 114, 113, 113, 113, 114, 114, 115}};

    s_colors _turn_sum_high_color = {{110, 117, 116, 115, 116, 116, 117, 118}};

    s_list_state _g_turn_sum_header = {
        92,
        103,
        320,
        373,
        _turn_sum_list_y,
        _turn_sum_list_y,
        _turn_sum_field_y2,
        8,
        0,
        nullptr,
        nullptr,
        0,
        0,
        4,
        4,
        reinterpret_cast<void*>(Draw_Turn_Summary_Popup_App_),
        13,
        0,
        &_turn_sum_high_color,
        {&_turn_sum_normal_color, &_turn_sum_normal_color, &_turn_sum_normal_color, &_turn_sum_normal_color},
        &_turn_sum_normal_color,
    };

    void __cdecl Draw_Turn_Summary_Popup_()
    {
        _g_turn_sum_header.group_colors[0] = &_turn_sum_high_color;

        int16_t input = fields::Scan_Input_();

        if (input >= INFO::_g_first_list_field && input <= INFO::_g_last_list_field)
        {
            LIST::Set_Selected_List_Entry_(&_g_turn_sum_header, input);
        }

        _g_selected_msg_p = LIST::Get_Selected_List_Entry_(&_g_turn_sum_header);
        LIST::Display_List_Text_(&_g_turn_sum_header, _g_selected_msg_p);

        _g_turn_sum_header.group_colors[0] = &_turn_sum_normal_color;
    }

    void __cdecl Turn_Summary_Popup_()
    {
        int16_t num_msgs;
        MOX2::action_func_t auto_func;
        s_list_item list_items[100];

        MOX::_current_screen = 0;
        num_msgs = MSG_::N_Msgs_For_Player_(MOX::_PLAYER_NUM);

        if (num_msgs <= 0)
        {
            if (MOX::_current_screen == 0)
            {
                MOX::_current_screen = 0x27; // 39
            }
            return;
        }

        if (MOX::_return_screen == 0x27) // 39
        {
            _g_turn_message_ix = 0;
        }
        else
        {
            ERIC::Fast_Fade_Out_();
            fonts::Load_Palette_(0, 0, 0xFF);
            BILL::Turn_Off_Palettes_(0, 0xFF);
        }

        fields::Clear_Mouse_Buffer_();
        fields::Deactivate_Help_List_();
        MOX2::Get_Auto_Function_(&auto_func);
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();

        s_animation_header* anim = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_(s_turnsum::lbx_00558c70, 0, MOX::_screen_seg));
        s_animation_header* pic = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(s_turnsum::lbx_00558c70, 1, MOX::_screen_seg));
        s_animation_header* pic_00 = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(s_turnsum::lbx_00558c70, 2, MOX::_screen_seg));
        s_animation_header* pic_01 = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(s_turnsum::lbx_00558c70, 3, MOX::_screen_seg));

        while (true)
        {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            fields::Clear_Fields_();
            fields::Assign_Auto_Function_(TURNSUM::Draw_Turn_Summary_Popup_, 2);

            int16_t field_btn_up = fields::Add_Button_Field_(0x1B9, 0x5F, s__00551100, pic, s__00551100, 0);
            int16_t field_btn_down = fields::Add_Button_Field_(0x1B9, 0x161, s__00551100, pic_00, s__00551100, 0);

            _g_turn_sum_header.items = list_items;
            INFO::Init_Turn_Summary_List_(&_g_turn_sum_header, num_msgs);

            _g_selected_msg_p = LIST::Select_And_Page_By_Head_ID_(&_g_turn_sum_header, _g_turn_message_ix);

            MAINSCR::Draw_Mini_Main_Screen_();
            BILLHELP::Turn_Summary_Popup_Help_();

            while (true)
            {
                fields::Clear_Fields_Above_(field_btn_down);
                INFO::_g_first_list_field = fields::Get_Field_Counter_();
                INFO::_g_last_list_field = LIST::Add_Fields_To_List_Page_(&_g_turn_sum_header);

                int16_t field_btn_ok = fields::Add_Button_Field_(0xF2, 0x176, s__00551100, pic_01, s__00551120, 0);
                fields::Add_Hidden_Field_(0x54, 0x33, 0x1D0, 0x189, s__00551120, 0);
                int16_t field_hidden_bg = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551120, 0);

                animate::Draw_(0x54, 0x33, anim);
                LIST::Display_List_Page_(&_g_turn_sum_header);
                LIST::Set_List_Up_Down_Field_Drawing_(&_g_turn_sum_header, field_btn_up, field_btn_down);

                video::Copy_Off_To_Back_();
                palette::Clear_Palette_Changes_(0, 0xFF);
                RUSS::Mox_Sync_Update_();
                ERIC::Fast_Fade_In_();

                bool full_draw_requested = false;

                while (true)
                {
                    int16_t input_field = fields::Get_Input_();

                    if (MOX::_FULL_DRAW_SCREEN_FLAG != 0)
                    {
                        full_draw_requested = true;
                        break;
                    }

                    if (input_field >= INFO::_g_first_list_field && input_field <= INFO::_g_last_list_field)
                    {
                        uint8_t msg_idx = _g_selected_msg_p->group_id;
                        if (MSG_::Goto_Msg_Colony_(&MSG_::_msgs[msg_idx]) != 0)
                        {
                            input_field = field_hidden_bg;
                            MOX::_current_screen = 1;
                            MOX::_return_screen = 0x28; // 40
                            _g_turn_message_ix = msg_idx;
                        }
                    }
                    else
                    {
                        if (input_field > 0)
                        {
                            if (input_field >= field_hidden_bg || input_field == field_btn_ok)
                            {
                                // Handle loop exit later
                            }
                            else if (input_field == field_btn_down)
                            {
                                if (LIST::Next_List_Page_(&_g_turn_sum_header) != 0)
                                {
                                    break;
                                }
                            }
                            else if (input_field == field_btn_up)
                            {
                                if (LIST::Previous_List_Page_(&_g_turn_sum_header) != 0)
                                {
                                    break;
                                }
                            }
                        }
                        else if (input_field == 0)
                        {
                            timer::Mark_Time_();
                            TURNSUM::Draw_Turn_Summary_Popup_();
                            RUSS::Mox_Sync_Update_();
                            timer::Release_Time_(2);
                        }
                    }

                    if (input_field >= field_hidden_bg || input_field == field_btn_ok)
                    {
                        MOX2::Put_Auto_Function_(&auto_func);
                        fields::Clear_Fields_();
                        if (MOX::_current_screen == 0)
                        {
                            MOX::_current_screen = 0x27; // 39
                        }
                        return;
                    }
                }

                if (full_draw_requested)
                {
                    break;
                }
            }
        }
    }

}