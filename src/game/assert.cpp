#include "pch.h"

namespace assert {

    int __cdecl hgetch_() {
        fonts::PSTRS_(0, 24, reinterpret_cast<char*>(std::uintptr_t{0x00558490}));

        int key = __getch();

        if ((key == 27 || key == 'Q' || key == 'q') && MOX::_global_var != 'u') {
            fields::F11_Key_();
        }

        if (key == 9) {
            capture::Screen_Flic_Capture_();
        }

        fonts::PSTRS_(0, 24, reinterpret_cast<char*>(std::uintptr_t{0x00558488}));

        return key;
    }

} // namespace assert