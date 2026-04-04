#include "pch.h"

namespace palette {
    void __cdecl Set_Palette_Changes_(int32_t start_index, int32_t end_index) {
        int32_t start = start_index;
        int32_t end = end_index;

        if (start <= end) {
            // Pointer to the first palette status entry to modify
            s_palette_entry* entry = &video::_current_palette[start];
            
            // Calculate number of entries to mark (inclusive)
            int32_t count = (end - start) + 1;
            
            do {
                // Set the changed flag (first byte of the 4-byte Palette_Entry)
                entry->changed = 1;
                
                // Move to the next 4-byte entry
                entry++;
                count--;
            } while (count != 0);
        }
    }

    void __cdecl Clear_Palette_Changes_(int32_t start, int32_t end) {
        if (start <= end) {
            // Calculate how many entries to clear
            int count = (int)end - (int)start + 1;
            s_palette_entry* entry = &video::_current_palette[start];

            do {
                // Clear only the 'changed' status byte (offset 0 in the 4-byte record)
                entry->changed = 0;
                entry++;
                count--;
            } while (count != 0);
        }
    }
}

