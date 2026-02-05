#include "deps.h"

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
            mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
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
                    MOX::Mox_Update_();
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
}