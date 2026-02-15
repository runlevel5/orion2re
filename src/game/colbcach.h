#pragma once
#include <cstdint>

struct s_animation_header;

namespace COLBCACH {
    s_animation_header* __cdecl Cache_Load_Bldg_(int16_t x, int16_t y, int16_t bldg_type);
}