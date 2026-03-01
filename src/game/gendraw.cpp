#include "pch.h"

namespace GENDRAW {
    void __cdecl Message_Box_(char* text) {
        TEXTBOX::Do_Text_Box_(nullptr, text, 0);
    }

    void __cdecl Help_(char* message) {
        Message_Box_(message);
    }
}