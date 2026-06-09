#include "pch.h"

namespace exitm {
    void Exit_With_Message(const char* message) {
        fields::Clear_Fields_();

        platform::_need_quit = 1;

        video::Clear_Display_And_Release_Palette_();

        MessageBoxA(platform::g_main_window, message, "Microprose", MB_OK);

        platform::Prepare_Exit_();

        platform::Destroy_Main_Window_();
        exit(1);
    }

    void __cdecl Exit_With_message2_(const char* message) {
        Exit_With_Message(message);
    }
}