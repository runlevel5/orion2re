#include "pch.h"

namespace FLCQUEUE {
    int16_t _n_queue_entries;

    s_flic_queue_entry _flic_queue[100];

    void __cdecl Allocate_Flic_Queue_And_Pointers_() {
        _n_queue_entries = 0;
    }

    void __cdecl Free_Flic_Queue_And_Pointers_() {
        _n_queue_entries = 0;
    }

    void __cdecl Draw_Flic_Queue_() {
        for (int16_t i = 0; i < _n_queue_entries; ++i) {
            Draw_Queue_Entry_(i);
        }
    }

    void __cdecl Squeze_Out_Queue_Entry_(int16_t index) {
        strings::Delete_Structure_(index, _flic_queue, sizeof(s_flic_queue_entry), 100);
        _n_queue_entries--;
    }

    void __cdecl Draw_Queue_Entry_(int16_t queue_idx) {
        s_flic_queue_entry* entry = &_flic_queue[queue_idx];

        animate::Set_Animation_Frame_(entry->animation, entry->current_frame);

        if (entry->use_clipping != 0) {
            graphics::Set_Window_(0, 0, 0x27F, 0x1DF);
            video::Clip_On_();
            animate::Draw_(entry->x, entry->y, entry->animation);
            video::Clip_Off_();
        } else {
            animate::Draw_(entry->x, entry->y, entry->animation);
        }

        entry->current_frame++;

        if (entry->play_mode == 0) {
            if (animate::Get_Animation_Frame_Count_(entry->animation) <= entry->current_frame) {
                entry->current_frame = 0;
            }
        } else if (entry->play_mode == 1) {
            if (animate::Get_Animation_Frame_Count_(entry->animation) <= entry->current_frame) {
                Squeze_Out_Queue_Entry_(queue_idx);
            }
        }
    }

    void __cdecl Add_Flic_To_End_Of_Queue_(int16_t x, int16_t y, s_animation_header* animation, uint8_t play_mode, uint8_t use_clipping) {
        if (_n_queue_entries < 100) {
            int16_t index = _n_queue_entries;
            _flic_queue[index].x = x;
            _flic_queue[index].y = y;
            _flic_queue[index].animation = animation;
            _flic_queue[index].play_mode = play_mode;
            _flic_queue[index].use_clipping = use_clipping;
            _n_queue_entries++;
        }
    }

    void __cdecl Add_Explosion_Anim_(int16_t x, int16_t y) {
        Add_Flic_To_End_Of_Queue_(x + 15, y + 15, COLGCBT::_explosion_anim, 1, 1);
    }
}