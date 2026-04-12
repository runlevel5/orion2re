#include "pch.h"

namespace motrans {

uint32_t __cdecl Modem_Status_() {
    if (_mtp_receiving != 0) {
        return 6;
    }
    return 4;
}

}