#include "deps.h"

namespace MAPGEN {
    uint8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index) {
        uint8_t weights[7];

        for (int32_t i = 0; i < 7; ++i) {
            weights[i] = MOX::_star_class_table[i * 3 + star_type_index];
        }

        return RUSS::Get_Weighted_Choice_Char_(weights, 7);
    }
}