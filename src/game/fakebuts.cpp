#include "pch.h"

namespace FAKEBUTS {
    void __cdecl Clear_Fake_Buttons_() {
        memset(_fake_buttons, 0, sizeof(_fake_buttons));
        _n_fake_buttons = 0;
    }
}