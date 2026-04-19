#include "pch.h"

namespace LOADER {
    void __cdecl Load_Pictures_() {
        Load_Main_Screen_Pictures_();
        RUSS::Mox_Update_();
        COLONY::Load_Colony_Anims_();
        RUSS::Mox_Update_();
        TECHINIT::Init_Tech_();
        RUSS::Mox_Update_();

        void* block = MOX::_global_screen_seg;
        int32_t remaining_space = allocate::Get_Remaining_Block_Space_(block);

        // Allocate block and leave space for the animation header
        s_animation_header* anim_seg = (s_animation_header*)allocate::Allocate_Next_Block_(block, remaining_space - 12 /* sizeof(s_animation_header) */);
        MOX::_screen_seg = anim_seg;

        // Initialize the start of the frame offset data area
        memset(anim_seg->frame_offsets, 0, 20);
    }
}