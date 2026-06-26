#include "pch.h"

namespace exitm {
    void __cdecl Exit_With_Message(const char* message) {
        platform::Request_Exit_(message);
        fields::Clear_Fields_();
        video::Clear_Display_And_Release_Palette_();
        platform::Check_Exit_();
    }

    void __cdecl Exit_With_message2_(const char* message) {
        Exit_With_Message(message);
    }
}