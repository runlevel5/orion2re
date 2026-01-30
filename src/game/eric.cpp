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
}