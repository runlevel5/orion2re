#include "pch.h"
#include "beams.h"
#include "animate.h"
#include "palette.h"
#include "firepts.h"
#include "farload.h"
#include "colony.h"
#include "mox.h"

namespace BEAMS {
    s_animation_header* __cdecl Reflection_Field_Anims_(int32_t p1, int32_t p2) {
        // The index calculation is performed in 16-bit registers in disassembly
        int16_t entry_num = (int16_t)p2 * 5 + (int16_t)p1 + 89;

        return (s_animation_header*)Buffer_Reload_(
            COLONY::_BEAMS_LBX,
            static_cast<int32_t>(entry_num),
            MOX::_global_cache_seg
        );
    }

    void __cdecl Load_Reflection_Field_Hit_(uint8_t should_load) {
        if (should_load != 0) {
            s_animation_header* anim = BEAMS::Reflection_Field_Anims_(0, 0);
            animate::Draw_Palette_(anim);
            palette::Refresh_Palette_();
        }
    }

    void __cdecl Load_Combat_Beam_Stuff_(uint8_t flag) {
        BEAMS::Init_Beams_();
        FIREPTS::Load_FP_Data_From_LBX_();
        BEAMS::Load_Ship_Burst_();
        BEAMS::Load_Ship_Explosion_();
        BEAMS::Load_Shield_Hit_();
        BEAMS::Load_Reflection_Field_Hit_(flag);

        BEAMS::_mauler_ball_seg = static_cast<s_animation_header*>(
            farload::Far_Reload_Next_(COLONY::_BEAMS_LBX, 0x58, MOX::_screen_seg)
        );
    }
}