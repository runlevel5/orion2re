#include "pch.h"

namespace RACERPRT {

char _TREATIES_FMT[12] = "%s\n%s\n%s\n%s";
char _PERSONALITY_FMT[6] = "%s\r%s";
s_colors _report_normal_color = {{0x14, 0xB9, 0xB8, 0xB6, 0xB8, 0xB9, 0xB9, 0xBA}};
uint8_t _race_report_jim_hack;
s_racescrn_icon_layout _spy_group_data = {24, 216, 187, 239, 24, nullptr};

void __cdecl Race_Report_Screen_(int16_t param_1) {
    int32_t local_31c;
    char acStack_318[40];
    s_animation_header* apsStack_2f0[10];
    s_racescrn_mouse_group sStack_2c8;
    char acStack_2a8[40];
    char acStack_280[128];
    char acStack_200[128];
    char local_180[128];
    char acStack_100[256];

    MOX::_cur_plyr_ptr = &MOX::_player[param_1];

    if (MOX::_cur_plyr_ptr->objectives == 100) {
        strcpy(local_180, MOX::_personality_labels[(uint8_t)MOX::_cur_plyr_ptr->personality]);
    } else {
        sprintf(local_180, RACERPRT::_PERSONALITY_FMT,
                MOX::_personality_labels[(uint8_t)MOX::_cur_plyr_ptr->personality],
                MOX::_objective_labels[MOX::_cur_plyr_ptr->objectives]);
    }

    FMTPARA::Set_Paragraph_Integer_Pointer_(0, &local_31c);
    fields::Clear_Fields_();
    BILLHELP::Report_Help_();
    fields::Enable_Draw_All_Fields_();
    RUSS::Mox_Update_();

    s_animation_header* anim = (s_animation_header*)farload::Far_Reload_(RACESCRN::s_races_lbx, MOX::_cur_plyr_ptr->race + 0x20, (void*)MOX::_screen_seg);
    s_animation_header* psVar4 = (s_animation_header*)farload::Far_Reload_Next_(RACESCRN::s_races_lbx, MOX::_player[param_1].race + 0x2E, (void*)MOX::_screen_seg);
    RUSS::Mox_Update_();

    for (uint16_t i = 0; i < 10; ++i) {
        apsStack_2f0[i] = (s_animation_header*)farload::Far_Reload_Next_(s_racerprt_lbx_005574d8, i, (void*)MOX::_screen_seg);
        RUSS::Mox_Update_();
    }

    short spy_count = BILL::Get_Their_Spy_Number_(param_1, MOX::_PLAYER_NUM);
    RACESCRN::Setup_Icon_Group_Data_(&sStack_2c8, &RACERPRT::_spy_group_data, spy_count, psVar4, nullptr);

    int16_t num_wars = 0;
    acStack_200[0] = '\0';
    int16_t num_alliances = 0;
    acStack_280[0] = '\0';

    if (MOX::_NUM_PLAYERS > 0) {
        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (i != param_1) {
                if (MOX::_cur_plyr_ptr->treaty[i] == 2) {
                    strcat(acStack_200, MOX::_player[i].race_name);
                    strcat(acStack_200, string::s_Newline);
                    num_alliances++;
                } else if (MOX::_cur_plyr_ptr->treaty[i] >= 4) {
                    strcat(acStack_280, MOX::_player[i].race_name);
                    strcat(acStack_280, string::s_Newline);
                    num_wars++;
                }
            }
        }
    }

    if (num_alliances == 0) {
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x1D, acStack_200, 0x28);
        strcat(acStack_200, string::s_Newline);
    }
    if (num_wars == 0) {
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x1E, acStack_280, 0x28);
        strcat(acStack_280, string::s_Newline);
    }

    RUSS::Mox_Update_();
    JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x1F, acStack_318, 0x28);
    JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x20, acStack_2a8, 0x28);
    RUSS::Mox_Update_();

    sprintf(acStack_100, RACERPRT::_TREATIES_FMT, acStack_318, acStack_200, acStack_2a8, acStack_280);

    INFO::_g_exit_field = fields::Add_Button_Field_(0x216, 0x1B1, s__00551100, apsStack_2f0[1], s__00551120, '(');

    MOX::_first_time = 1;

    while (true) {
        if (MOX::_FULL_DRAW_SCREEN_FLAG == 1) {
            fields::Clear_Fields_Above_(INFO::_g_exit_field);
        }
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        graphics::Tri_Interlaced_Fill_(0xD4, 0x17, 0x26C, 0x1C9, 0xB2, 0xB3, 0xB3);
        animate::Draw_(0, 0, apsStack_2f0[0]);

        if (MOX::_first_time != 0) {
            BILL::Turn_Off_Palettes_(0, 0xFF);
        }

        animate::Draw_(0x46, 0x1C, anim);
        graphics::Box_(0x46, 0x1C, 0x4C, 0x58, 1, *(uint8_t*)&RACESCRN::_banner_color_high.colors[(uint8_t)MOX::_player[param_1].color]);
        RUSS::Mox_Update_();
        graphics::Box_(0x47, 0x1D, 0x4A, 0x56, 1, *(uint8_t*)&RACESCRN::_banner_color_low.colors[(uint8_t)MOX::_player[param_1].color]);

        fonts::Set_Font_Style_(3, &RACERPRT::_report_normal_color);
        BILL::Squeeze_Paragraph_Centered_(0x2A, 0x87, 0x80, 0x1A, MOX::_cur_plyr_ptr->name, 2);

        fonts::Set_Font_Style_(4, &RACERPRT::_report_normal_color);
        BILL::Squeeze_Paragraph_Centered_(0x18, 0xA7, 0xA4, 0x2A, local_180, 2);

        if (spy_count == 0) {
            fonts::Set_Font_Style_(3, &RACERPRT::_report_normal_color);
            JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x1C, acStack_318, 0x28);
        } else {
            RACESCRN::Draw_Icon_Group_(&sStack_2c8);
            fonts::Set_Font_Style_(2, &RACERPRT::_report_normal_color);
            if (spy_count <= 1) {
                JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x4B, acStack_318, 0x28);
            } else {
                JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x4A, acStack_318, 0x28);
                JIM::Decode_Text_Stat_(acStack_318, spy_count, 0x28);
            }
        }
        
        BILL::Squeeze_Centered_Print_(0x18, 0xDE, acStack_318, 0xA4);
        RUSS::Mox_Update_();

        fonts::Set_Font_Style_(4, &RACERPRT::_report_normal_color);
        BILL::Squeeze_Formatted_Paragraph_Centered_(0x18, 0x123, 0xA4, 0xAA, acStack_100, 2);

        video::Copy_Off_To_Back_();
        ERIC::Fast_Fade_In_();
        MOX::_first_time = 0;
        RACERPRT::_race_report_jim_hack = 1;

        allocate::Mark_Block_((void*)MOX::_screen_seg);
        int16_t action = INFO::_Tech_Review_Subscreen_(apsStack_2f0 + 2, INFO::_g_exit_field + 1, param_1);
        allocate::Release_Block_((void*)MOX::_screen_seg);

        RUSS::Mox_Update_();
        RACERPRT::_race_report_jim_hack = 0;

        if (action == INFO::_g_exit_field) {
            ERIC::Fast_Fade_Out_();
            MOX::_cur_plyr_ptr = &MOX::_player[MOX::_PLAYER_NUM];
            return;
        }
    }
}

} // namespace RACERPRT