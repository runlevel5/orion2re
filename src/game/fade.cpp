#include "pch.h"

namespace fade {
    void __cdecl Blinds_Fade_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t step) {
        if (step < 1) {
            return;
        }

        video::Save_Refresh_();
        video::Set_Refresh_Differential_No_Full_();

        // redundant check from disassembly (0x00533793)
        if (step > 0) {
            for (int16_t i = 0; i < step; ++i) {
                video::Clear_Scan_List_();

                for (int16_t y = y1 + i; y <= y2; y += step) {
                    setvesa::Add_Square_To_Scan_List_(x1, y, x2, y);
                }

                platform::Toggle_Pages_Stub_();
            }
        }

        video::Restore_Refresh_();
    }
}