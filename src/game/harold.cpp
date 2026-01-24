#include "deps.h"

namespace HAROLD {
    void __cdecl Get_Officer_Data_LBX_Name_(char* out) {
        const char* filename;

        switch (MOX::_settings.language) {
        case 1:  // German
            filename = s_HERODATG_LBX;
            break;
        case 2:  // French
            filename = s_HERODATF_LBX;
            break;
        case 3:  // Spanish
            filename = s_HERODATS_LBX;
            break;
        case 4:  // Italian
            filename = s_HERODATI_LBX;
            break;
        default: // English / Standard
            filename = s_HERODATA_LBX;
            break;
        }

        for (int32_t i = 0; i < 13; ++i) {
            out[i] = filename[i];
        }
    }

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