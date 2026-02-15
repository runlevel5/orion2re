#include "deps.h"

namespace COLBCACH {
    s_animation_header* __cdecl Cache_Load_Bldg_(int16_t x, int16_t y, int16_t bldg_type) {
        char filename[28];
        int type_index = (int)bldg_type - 1;
        int file_number = type_index / 10;
        int frame_index = type_index % 10;

        char* format = ESTRINGS::E_Strings_(ESTR_BLDGDLBX1);
        _sprintf(filename, format, file_number);

        void* cache_seg = MOX::_global_cache_seg;
        
        int16_t base_frame = COLONY::Bldg_Coords_To_Effective_Frame_(x, y);

        int effective_frame = (int)base_frame + (frame_index * 36);

        return (s_animation_header*)Buffer_Reload_(filename, effective_frame, cache_seg);
    }
}