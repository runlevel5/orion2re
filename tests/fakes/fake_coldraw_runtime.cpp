#include "pch.h"

namespace {
    s_colony colonies[MAX_COLONIES];
    uint8_t production_animation;
    uint8_t import_animation;
    uint8_t shortage_animation;
    int short_anim_calls;
    int draw_calls;
}

namespace TEST_COLDRAW {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        COLONY::_step_squish = 0;
        short_anim_calls = 0;
        draw_calls = 0;
    }

    int Short_Anim_Calls_() {
        return short_anim_calls;
    }

    int Draw_Calls_() {
        return draw_calls;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
}

namespace COLONY {
    int16_t _step_squish;

    s_animation_header* __cdecl Prod_Anims_(int16_t, int16_t, int8_t) {
        return reinterpret_cast<s_animation_header*>(&production_animation);
    }

    s_animation_header* __cdecl Import_Anims_(int16_t, int16_t, int8_t) {
        return reinterpret_cast<s_animation_header*>(&import_animation);
    }

    s_animation_header* __cdecl Short_Anims_(int16_t, int16_t, uint8_t) {
        short_anim_calls++;
        return reinterpret_cast<s_animation_header*>(&shortage_animation);
    }
}

namespace animate {
    void __cdecl Draw_(int16_t, int16_t, s_animation_header*) {
        draw_calls++;
    }
}

namespace graphics {
    void __cdecl Set_Window_(int16_t, int16_t, int16_t, int16_t) {
    }
}

namespace video {
    void __cdecl Clip_On_() {
    }

    void __cdecl Clip_Off_() {
    }
}