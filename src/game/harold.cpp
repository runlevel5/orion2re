#include "deps.h"

namespace HAROLD {
    void __cdecl Get_Font_File_(char* buffer) {
        const char* src;

        switch (MOX::_settings.language) {
        case 1: // German
            src = "fontsg.lbx";
            break;
        case 2: // French
            src = "fontsf.lbx";
            break;
        case 3: // Spanish
            src = "fontss.lbx";
            break;
        case 4: // Italian
            src = "fontsi.lbx";
            break;
        default: // English or Other
            src = "fonts.lbx";
            break;
        }

        _memcpy(buffer, src, 10);
    }
}