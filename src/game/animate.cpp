#include "deps.h"

namespace animate {
    void __cdecl Set_Animation_Frame_(void* raw, int32_t frame) {
        // seg[3] (offset 0x6) is likely the total frame count
        int16_t* seg = (int16_t*)raw;
        int32_t num_frames = static_cast<int32_t>(seg[3]);

        if (frame >= num_frames) {
            frame = num_frames - 1;
        }

        // seg[2] (offset 0x4) is likely the current frame index
        seg[2] = static_cast<int16_t>(frame);
    }

    int16_t __cdecl Get_Animation_Frame_Count_(void *raw) {
        // Offset 0x6 in the animation segment header
        int16_t* seg = (int16_t*)raw;
        return seg[3];
    }

    void __cdecl Reset_Animation_Frame_(void* raw) {
        int16_t* seg = (int16_t*)raw;
        seg[2] = 0;
    }
}