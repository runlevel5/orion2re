#include "deps.h"

namespace HAROLD {
    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx) {
        uint8_t count = 0;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            // Check if slot contains a valid planet (-1 is empty/gas giant in some contexts)
            if (planet_idx != -1) {
                // The assembly specifically checks if planet_type is 0x03
                if (MOX::_planet[planet_idx].planet_type == 3) {
                    count++;
                }
            }
        }

        return count;
    }

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

    /**
     * Checks if a star system has a habitable spectral class (O, B, A, F, G, or K).
     * Spectral classes are indexed such that habitable ones are < 6.
     */
    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx) {
        // Star data structure size is 113 bytes.
        return MOX::_star[star_idx].spectral_class < 6;
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