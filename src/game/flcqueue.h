#pragma once
#include <cstdint>

namespace FLCQUEUE {
    extern int16_t _n_queue_entries;

    extern s_flic_queue_entry _flic_queue[100];

    void __cdecl Allocate_Flic_Queue_And_Pointers_();

    void __cdecl Free_Flic_Queue_And_Pointers_();

    void __cdecl Draw_Flic_Queue_();
    void __cdecl Draw_Queue_Entry_(int16_t entry_idx);

    void __cdecl Squeze_Out_Queue_Entry_(int16_t index);

    void __cdecl Add_Flic_To_End_Of_Queue_(int16_t x, int16_t y, s_animation_header* animation, uint8_t play_mode, uint8_t use_clipping);

    void __cdecl Add_Explosion_Anim_(int16_t x, int16_t y);
}