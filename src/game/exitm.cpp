#include "pch.h"

namespace exitm {
    void Exit_With_Message(const char* message) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "fatal exit: %s", message != nullptr ? message : "<null>");

        fields::Clear_Fields_();

        video::Clear_Display_And_Release_Palette_();

        Show_Message_Box_(platform::g_main_window, message, "Microprose");

        platform::Request_Exit_(message);
        platform::Check_Exit_();
    }

    void __cdecl Exit_With_message2_(const char* message) {
        Exit_With_Message(message);
    }
}