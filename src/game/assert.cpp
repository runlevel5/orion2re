#include "pch.h"

namespace assert {

    char s__00558488[8] = "    ";
    char s__00558490[8] = "*";

    int __cdecl hgetch_() {
        fonts::PSTRS_(0, 24, s__00558490);

        int key = _getch();

        if ((key == 27 || key == 'Q' || key == 'q') && MOX::_global_var != 'u') {
            fields::F11_Key_();
        }

        if (key == 9) {
            capture::Screen_Flic_Capture_();
        }

        fonts::PSTRS_(0, 24, s__00558488);

        return key;
    }

} // namespace assert