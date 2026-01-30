#include "deps.h"

namespace exitm {
    void Exit_With_Message(const char* message) {
        FUN_00521080();

        DAT_00578e68 = 1;

        FUN_0052d290();

        // Show terminal error message to user
        //MessageBoxA(platform::g_main_window, message, "Microprose", MB_OK);

        FUN_00533630();

        DestroyWindow(platform::g_main_window);
        __exit(1);
    }
}