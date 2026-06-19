#include "pch.h"

namespace init {
    namespace {
        constexpr size_t kIoBufferSize = 0x2000;
    }

    s_animation_header* _io_buffer;

    void __cdecl Create_IO_Buffer_() {
        _io_buffer = (s_animation_header*)malloc(kIoBufferSize);
        memset(_io_buffer, 0, kIoBufferSize);
    }

    int8_t __cdecl Init_Drivers_(uint32_t unused, char* font_name) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: Init_Drivers start font='%s'", font_name != nullptr ? font_name : "<null>");

        init::Create_IO_Buffer_();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: io buffer created");

        video::Init_Video_Drivers_();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: video drivers initialized");

        remap::Sort_Palette_List_();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: palette list sorted");

        sound::Init_Sound_();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: sound initialized");

        if (*font_name == '\0') {
            sysfont::Load_System_Font_();
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: system font loaded");
        } else {
            fonts::Load_Font_File_(font_name);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: font file loaded '%s'", font_name);
        }

        file_ani::Reset_File_Animation_Buffer_();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: file animation buffer reset");

        // Clear both video pages to black (0)
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);

        platform::Toggle_Pages_Stub_();

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init: Init_Drivers complete");
        return 1;
    }
}