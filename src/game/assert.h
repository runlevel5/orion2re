#pragma once

#include <cstdint>

namespace assert {
    /**
     * Reads a keypress, handles the debug hotkeys, and returns the pressed key code.
     */
    int32_t __cdecl hgetch_();
}