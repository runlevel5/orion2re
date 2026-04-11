#include "pch.h"

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text) {
        TEXTBOX::Do_Text_Box_(nullptr, (char*)text, 0);
    }

    void __cdecl Help_(const char* message) {
        Message_Box_(message);
    }
}