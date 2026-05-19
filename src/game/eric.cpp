#include "pch.h"

namespace ERIC {
    uint16_t __cdecl Get_Fade_Percent_() {
        return _fade_percent;
    }

    void __cdecl Eric_Defaults_() {
    }

    void __cdecl Eric_Init_() {
        COLONY::Init_Colonies_();
        EVENTS::Init_Events_();
    }

    void __cdecl Set_Fade_Percent_(int16_t percent) {
        _fade_percent = static_cast<uint8_t>(percent);

        if (percent == 0) {
            mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        }
    }

    void __cdecl Set_Fade_Speed_Percent_(uint8_t percentage) {
        _fade_speed_in_percent = percentage;
    }

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count) {
        if (count == 0) {
            return -1;
        }

        for (uint16_t i = 0; i < count; ++i) {
            if (list[i] == value) {
                return static_cast<int16_t>(i);
            }
        }

        return -1;
    }

    void __cdecl Fast_Fade_In_() {
        if (_fade_percent != 0) {
            video::Set_Refresh_Full_();

            int16_t fade_level = static_cast<int16_t>(_fade_percent);

            while (fade_level >= 0) {
                palstore::Darken_Palette_(static_cast<int32_t>(fade_level));
                _fade_percent = static_cast<uint8_t>(fade_level);
                RUSS::Mox_Update_();
                fade_level -= 10;
            }
        }

        video::Set_Refresh_Stencil_();
    }

    void Fast_Fade_Out_() {
        bool faded = false;
        capture::Check_Release_Version_();

        if (_fade_percent == 0) {
            video::Set_Refresh_Full_();

            uint16_t current_fade = _fade_percent;
            if (current_fade < 101) {
                faded = true;
                do {
                    palstore::Darken_Palette_(current_fade);
                    _fade_percent = static_cast<uint8_t>(current_fade);
                    RUSS::Mox_Update_();
                    current_fade += _fade_speed_in_percent;
                } while (current_fade < 101);
            }

            if (faded) {
                video::Set_Page_Back_();
                graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
                video::Copy_Back_To_Off_();
                palette::Clear_Palette_Changes_(0, 0xFF);
                RUSS::Mox_Sync_Update_();
            }

            video::Set_Refresh_Stencil_();
        }
    }

    int16_t __cdecl Global_Cycler_(int16_t divisor, int16_t modulus) {
        int32_t current_time = clock();
        int32_t cycle_index = current_time / static_cast<int32_t>(divisor);
        return static_cast<int16_t>(cycle_index % static_cast<int32_t>(modulus));
    }

    bool __cdecl Fast_Fade_Out_Needed_For_Colony_Palette_() {
        if (_hacky_need_to_fade_to_colony_palette_flag != 0) {
            return true;
        }

        uint8_t previous_screen = static_cast<uint8_t>(MOX::_previous_screen);

        switch (MOX::_current_screen) {
            case SCREEN_COLONY:
                if (previous_screen == SCREEN_QUEUE_POPUP ||
                    previous_screen == SCREEN_COLONY_LANDING) {
                    return false;
                }
                return true;

            case SCREEN_COLONY_SUMMARY:
                return previous_screen != SCREEN_QUEUE_POPUP;

            case SCREEN_QUEUE_POPUP:
                if (previous_screen == SCREEN_COLONY ||
                    previous_screen == SCREEN_COLONY_LANDING ||
                    previous_screen == SCREEN_COLONY_SUMMARY) {
                    return false;
                }
                return true;

            case SCREEN_COLONY_LANDING:
                return true;

            default:
                return true;
        }
    }

    void __cdecl Draw_Centered_(int16_t x, int16_t y, s_animation_header* animation) {
        int16_t width = animate::Get_Width_(animation);
        int16_t height = animate::Get_Height_(animation);

        int16_t x_centered = x - (width / 2);
        int16_t y_centered = y - (height / 2);

        animate::Draw_(x_centered, y_centered, animation);
    }

    void __cdecl Draw_Bottom_Centered_(int16_t x, int16_t y, s_animation_header* anim) {
        int16_t width = animate::Get_Width_(anim);
        int16_t height = animate::Get_Height_(anim);
        animate::Draw_(x - (width / 2), y - height, anim);
    }

    void __cdecl Draw_Bottom_Centered_No_Glass_(int16_t x, int16_t y, s_animation_header* anim) {
        int16_t width = animate::Get_Width_(anim);
        int16_t height = animate::Get_Height_(anim);
        animate::Draw_No_Glass_(x - (width / 2), y - height, anim);
    }

    void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, const char* text) {
        int16_t text_height = BILL::_Get_Paragraph_Max_Height_(width, text);

        int16_t y_centered = static_cast<int16_t>(
            static_cast<int32_t>(y) +
            (static_cast<uint16_t>(height) >> 1) -
            (static_cast<int32_t>(text_height) / 2));

        BILL::_Print_Paragraph_(x, y_centered, width, text, 2);
    }

    void __cdecl Print_Paragraph_Centered_Vertically_(int16_t x, int16_t y, int16_t width, uint16_t box_height, char* text, int16_t color) {
        int16_t max_height = BILL::_Get_Paragraph_Max_Height_(width, text);
        BILL::_Print_Paragraph_(x, y + (box_height >> 1) - (max_height / 2), width, text, color);
    }

    char* __cdecl Red_If_Negative_Fmt_String_(int16_t value) {
        if (value >= 0) {
            return ESTRINGS::E_Strings_(12);
        }
        return s_2_00551108;
    }

    void __cdecl Play_Dimensional_Portal_Flic_() {
        Do_Cinematic_Popup_Setup_();
        SMACK::Play_Cinematic_(COLONY::_DIMTVFIN_LBX, 0, 0);
        Do_Cinematic_Popup_Cleanup_();
    }

    void __cdecl Play_Planetary_Destruction_Flic_() {
        Do_Cinematic_Popup_Setup_();
        SMACK::Play_Cinematic_(COLONY::_PLNTDFIN_LBX, 0, 0);
        Do_Cinematic_Popup_Cleanup_();
    }

    void __cdecl Do_Cinematic_Popup_Cleanup_() {
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        fields::Deactivate_Auto_Function_();
        MOX2::Put_Auto_Function_(&ERIC::_cinematic_saved_autofunction);
        allocate::Pop_Block_(MOX::_screen_seg);

        if (ERIC::_saved_sfx_l != 0 || ERIC::_saved_music_l != 0) {
            MOX::_settings.sound_fx_level = (int8_t)ERIC::_saved_sfx_l;
            MOX::_settings.music_level = (int8_t)ERIC::_saved_music_l;
        }
    }

    bool __cdecl Sint_Is_In_List_(int16_t value, int16_t* list, uint16_t count) {
        if (count == 0) {
            return false;
        }

        for (uint16_t i = 0; i < count; ++i) {
            if (list[i] == value) {
                return true;
            }
        }

        return false;
    }

    void __cdecl Pulsing_Box_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t param_5, int32_t param_6, int16_t param_7) {
        graphics::Box_(x1, y1, x2, y2, param_5, 0xff);
        palette::Cycle_Palette_Color_(0xff, 0x13, 0x10, 0x09, 0x20, 0x28, 0x15, param_7);
    }

    void __cdecl Play_Loose_Flic_() {
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0x16B), 0, 1);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }

    void __cdecl Play_Normal_Win_Flic_() {
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0x136), 0x400, 1);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }

    void __cdecl Play_All_Flics_() {
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0xb7), 0x400, 1);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0x136), 0, 1);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }

    void __cdecl Play_Take_Orion_Flic_() {
        Do_Cinematic_Popup_Setup_();
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0x194), 0, 0);
        Do_Cinematic_Popup_Cleanup_();
    }

    void __cdecl Set_Refresh_Mode_(int16_t mode) {
        if (mode == 0) {
            video::Set_Refresh_Full_();
        } else if (mode == 2) {
            video::Set_Refresh_Differential_();
        } else if (mode == 3) {
            video::Set_Refresh_Stencil_();
        }
    }

    void __cdecl Do_Cinematic_Popup_Setup_() {
        MOX2::Get_Auto_Function_(&_cinematic_saved_autofunction);
        fields::Deactivate_Auto_Function_();

        if (MOX::_settings.sound_fx_on == 0 && MOX::_settings.music_on == 0) {
            _saved_sfx_l = 0;
            _saved_music_l = 0;
        } else {
            _saved_sfx_l = MOX::_settings.sound_fx_level;
            _saved_music_l = MOX::_settings.music_level;
            if (MOX::_settings.music_level <= MOX::_settings.sound_fx_level) {
                MOX::_settings.music_level = MOX::_settings.sound_fx_level;
            }
        }

        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        Cinematic_Autofunction_();
        fields::Assign_Auto_Function_(Cinematic_Autofunction_, 1);
        allocate::Push_Block_(MOX::_screen_seg);
        video::Copy_Off_To_Back_();
    }

    void __cdecl Play_Antarans_Attack_Flic_() {
        Do_Cinematic_Popup_Setup_();
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0xAE), 0, 0);
        Do_Cinematic_Popup_Cleanup_();
    }

    void __cdecl Play_Diplomatic_Win_Flic_() {
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0x22C), 0, 1);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        SMACK::Play_Cinematic_(ESTRINGS::E_Strings_(0x136), 0, 1);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }

    void __cdecl Play_Amoeba_Flic_() {
        Do_Cinematic_Popup_Setup_();
        SMACK::Play_Cinematic_(COLONY::_AMEBAFIN_LBX, 0, 0);
        Do_Cinematic_Popup_Cleanup_();
    }

    int8_t __cdecl Player_Has_Ship_Size_Or_Larger_At_Star_(int16_t player_idx, int16_t size, int16_t star_idx) {
        for (int16_t i = 0; i < 500; ++i) {
            if (MOX::_ship[i].location == star_idx &&
                MOX::_ship[i].status == 0 &&
                MOX::_ship[i].owner == player_idx &&
                MOX::_ship[i].d.size >= size) {
                return 1;
            }
        }
        return 0;
    }

    int8_t __cdecl Player_Can_Mind_Control_Colony_(int16_t player_idx, int16_t colony_idx) {
        if (MOX::_player[player_idx].traits[25] != 0) {
            if (Player_Has_Ship_Size_Or_Larger_At_Star_(player_idx, 2, MOX::_planet[MOX::_colony[colony_idx].planet].star_index)) {
                if (!Player_Has_Leader_With_General_Skill_At_Star_(MOX::_colony[colony_idx].owner, 0, MOX::_planet[MOX::_colony[colony_idx].planet].star_index)) {
                    // Note: The original code calls this function twice with identical arguments.
                    if (!Player_Has_Leader_With_General_Skill_At_Star_(MOX::_colony[colony_idx].owner, 0, MOX::_planet[MOX::_colony[colony_idx].planet].star_index)) {
                        if (MOX::_player[MOX::_colony[colony_idx].owner].traits[25] == 0) {
                            return 1;
                        }
                    }
                }
            }
        }
        return 0;
    }

    int8_t __cdecl Player_Can_Mind_Control_Colony_With_Help_(int16_t player_idx, int16_t colony_idx)
    {
        if (MOX::_player[player_idx].traits[25] == 0)
        {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x233));
            return 0;
        }

        int16_t planet_idx = MOX::_colony[colony_idx].planet;
        int16_t star_idx = MOX::_planet[planet_idx].star_index;

        if (Player_Has_Ship_Size_Or_Larger_At_Star_(player_idx, 2, star_idx) == 0)
        {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x24C));
            return 0;
        }

        int16_t owner_idx = MOX::_colony[colony_idx].owner;

        // The original code redundantly checks the exact same condition twice
        if (Player_Has_Leader_With_General_Skill_At_Star_(owner_idx, 0, star_idx) != 0 ||
            Player_Has_Leader_With_General_Skill_At_Star_(owner_idx, 0, star_idx) != 0)
        {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x200));
            return 0;
        }

        if (MOX::_player[owner_idx].traits[25] != 0)
        {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x201));
            return 0;
        }

        return 1;
    }

    void __cdecl Force_Fade_To_Colony_Palette_(uint8_t flag) {
        _hacky_need_to_fade_to_colony_palette_flag = flag;
    }

    int16_t __cdecl Player_Capitol_World_(int16_t player_idx) {
        for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            if (MOX::_colony[i].buildings[9] != 0 && MOX::_colony[i].owner == player_idx) {
                return i;
            }
        }
        return Pick_Random_Colony_(player_idx);
    }

    int16_t __cdecl Pick_Random_Colony_(int16_t owner) {
        int16_t count = 0;
        int16_t chosen_colony = -1;

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
            if (MOX::_colony[i].owner == owner) {
                count++;
                if (random::Random_(count) == 1) {
                    chosen_colony = i;
                }
            }
        }

        return chosen_colony;
    }

    void __cdecl Cinematic_Autofunction_() {
        video::Set_Page_Off_();

        s_animation_header* anim = (s_animation_header*)farload::Far_Reload_Next_(
            COLONY::_MAINPUPS_LBX, 0x53, MOX::_screen_seg);
        int16_t width = animate::Get_Width_(anim);
        int16_t x = ((640 - width) / 2) + 3;

        anim = (s_animation_header*)farload::Far_Reload_Next_(
            COLONY::_MAINPUPS_LBX, 0x53, MOX::_screen_seg);
        int16_t height = animate::Get_Height_(anim);
        int16_t y = ((480 - height) / 2) + 1;

        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
        MOX::_dont_draw_main_screen_to_back = 1;

        MAINSCR::Draw_Mini_Main_Screen_();

        anim = (s_animation_header*)farload::Far_Reload_Next_(
            COLONY::_MAINPUPS_LBX, 0x53, MOX::_screen_seg);
        animate::Draw_(x, y, anim);

        palette::Clear_Palette_Changes_(0, 255);
    }

    void __cdecl String_Builder2_(char* dest, int16_t mode, const char* format, ...) {
        char local_buf[1000];
        va_list args;

        if (mode >= 0) {
            va_start(args, format);
            uint32_t len = vsprintf(local_buf, format, args);
            va_end(args);

            if (len > 1000) {
                exitm::Exit_With_Message(ESTRINGS::E_Strings_(408 /* ESTR_PFMT__STRING_BUILDER */));
                return;
            }

            if (mode == 0) {
                strcpy(dest, local_buf);
            } else {
                sprintf(dest, ESTRINGS::E_Strings_(71), dest, 13, local_buf);
            }
        }
    }

    int8_t __cdecl Player_Has_Leader_With_General_Skill_At_Star_(short player_idx, short skill_idx, short star_idx) {
        for (short i = 0; i < 0x43; ++i) {
            if (MOX::_leaders[i].player_index == player_idx &&
                MOX::_leaders[i].status == 1 &&
                MOX::_leaders[i].eta == 0) {

                if ((MOX::_leaders[i].type == 0 && MOX::_ship[MOX::_leaders[i].location].location == star_idx) ||
                    (MOX::_leaders[i].type == 1 && MOX::_leaders[i].location == star_idx)) {

                    if (OFFICER::Officer_Has_General_Skill_(player_idx, i, skill_idx)) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
}