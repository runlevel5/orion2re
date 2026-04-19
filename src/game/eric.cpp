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
                    current_fade += fade_step;
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
        int32_t current_time = _clock();
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

    void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, const char* text) {
        int16_t text_height = BILL::_Get_Paragraph_Max_Height_(width, text);

        int16_t y_centered = static_cast<int16_t>(
            static_cast<int32_t>(y) +
            (static_cast<uint16_t>(height) >> 1) -
            (static_cast<int32_t>(text_height) / 2));

        BILL::_Print_Paragraph_(x, y_centered, width, text, 2);
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
}