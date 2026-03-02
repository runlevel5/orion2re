#include "pch.h"

namespace CMBTFIR2 {
    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        if (special_skills & 0x40) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        if (special_skills & 0x80) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;

        if ((MOX::_leaders[leader_idx].special_skills & 0x4000) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((bonus + 1) * 5);
        }

        if ((MOX::_leaders[leader_idx].special_skills & 0x8000) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((bonus * 3 + 3) * 5) / 2);
        }

        return bonus;
    }
}