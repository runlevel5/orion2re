#include "deps.h"

namespace mouse {
    void Set_Mouse_List_(uint8_t* mouse_data, uint16_t count) {
        _current_mouse_list_count = count;
        _current_mouse_list = mouse_data;

        Check_Mouse_Shape_();

        // Send a custom message (WM_USER + 2) to the main window
        SendMessageA(platform::g_main_window, 0x402, 0, 0);
    }
}