#include "pch.h"

namespace assert {

    int32_t __cdecl hgetch_() {
        fonts::PSTRS_(0, 24, "*");

        int32_t pressed_key = _getch();

        if ((pressed_key == 27 || pressed_key == 'Q' || pressed_key == 'q') && MOX::_global_var != 'u') {
            fields::F11_Key_();
        }

        if (pressed_key == 9) {
            capture::Screen_Flic_Capture_();
        }

        fonts::PSTRS_(0, 24, "    ");

        return pressed_key;
    }

} // namespace assert