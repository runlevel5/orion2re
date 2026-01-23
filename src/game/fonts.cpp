#include "deps.h"

namespace fonts {
    void Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index) {
        // Load the font palette resource block
        void* loaded_palette = farload::Far_Reload_(_font_name, static_cast<int32_t>(palette_id) + 1, _palette_block);

        // Calculate the range to copy. Colors are assumed to be 4-byte entries.
        uint32_t* src = reinterpret_cast<uint32_t*>(loaded_palette) + start_index;
        uint32_t* dst = &video::_current_palette[start_index];
        uint32_t byte_count = static_cast<uint32_t>(end_index - start_index + 1) * sizeof(uint32_t);

        struct_::Copy_Structure_(dst, src, byte_count);

        // The mouse palette is offset by 1024 bytes (256 colors * 4 bytes) into the loaded buffer.
        video::_mouse_palette = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(loaded_palette) + 0x400);

        // Signal that the specified color range has been modified.
        palette::Set_Palette_Changes_(start_index, end_index);

        // Internal state update.
        FUN_00526f80();
    }
}