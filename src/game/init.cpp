#include "pch.h"

namespace init {
    int8_t __cdecl Init_Drivers_(uint32_t param_1, char* font_name) {
        init::Create_IO_Buffer_();
        video::Init_Video_Drivers_();
        remap::Sort_Palette_List_();
        key::Init_Keyboard_();
        fields::Init_Mouse_Keyboard_();

        if (*font_name == '\0') {
            sysfont::Load_System_Font_();
        } else {
            fonts::Load_Font_File_(font_name);
        }

        file_ani::Reset_File_Animation_Buffer_();

        // Clear both video pages to black (0)
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);

        platform::Toggle_Pages_Stub_();

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);

        return 1;
    }
}