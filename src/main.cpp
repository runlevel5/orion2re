#include "pch.h"

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
    Initialize_Process_State();

    if (platform::g_command_line == nullptr) {
        return -1;
    }

    return platform::WinMain_Proxy(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}