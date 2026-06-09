#include "pch.h"

#include <SDL3/SDL_main.h>

static int SDLCALL Orion2re_SDL_Main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    char* command_line = GetCommandLineA();
    char* lp_cmd_line = command_line;

    if (lp_cmd_line != nullptr && *lp_cmd_line != '\0') {
        if (*lp_cmd_line == '"') {
            ++lp_cmd_line;
            while (*lp_cmd_line != '\0' && *lp_cmd_line != '"') {
                ++lp_cmd_line;
            }
            if (*lp_cmd_line == '"') {
                ++lp_cmd_line;
            }
        } else {
            while (*lp_cmd_line != '\0' && *lp_cmd_line > ' ') {
                ++lp_cmd_line;
            }
        }

        while (*lp_cmd_line != '\0' && *lp_cmd_line <= ' ') {
            ++lp_cmd_line;
        }
    }

    return platform::WinMain_Proxy(GetModuleHandleA(nullptr), nullptr, lp_cmd_line, SW_SHOWDEFAULT);
}

namespace {
    void Initialize_Process_State() {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
        uint32_t version_info = GetVersion();
#ifdef _MSC_VER
#pragma warning(pop)
#endif

        platform::g_os_ver = version_info;
        platform::g_win_minor = (version_info >> 8) & 0xFF;
        platform::g_win_major = version_info & 0xFF;
        platform::g_win_ver = (platform::g_win_major << 8) | platform::g_win_minor;
        platform::g_os_ver >>= 16;

        platform::g_command_line = GetCommandLineA();
        platform::g_env_strings = nullptr;
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nShowCmd;

    Initialize_Process_State();

    if (platform::g_command_line == nullptr) {
        return -1;
    }

    return SDL_RunApp(0, nullptr, Orion2re_SDL_Main, nullptr);
}