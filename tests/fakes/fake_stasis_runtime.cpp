#include "pch.h"

#include "game/cmbtmis.h"
#include "game/cmbtspec.h"
#include "game/combat1.h"

namespace {
    s_combat_data combat_data[4];
    s_missile missiles[300];
    int16_t cleanup_sources[4];
    int16_t cleanup_count;
    int16_t last_missile_hits_owner;
    int16_t last_retargeted_missile;
    bool has_high_energy_focus;
}

namespace TEST_STASIS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(cleanup_sources, 0xff, sizeof(cleanup_sources));
        cleanup_count = 0;
        last_missile_hits_owner = -1;
        last_retargeted_missile = -1;
    }

    int16_t Last_Cleanup_Source_() {
        return cleanup_count > 0 ? cleanup_sources[cleanup_count - 1] : -1;
    }

    int16_t Last_Missile_Hits_Owner_() {
        return last_missile_hits_owner;
    }

    int16_t Last_Retargeted_Missile_() {
        return last_retargeted_missile;
    }
}

namespace TEST_WEAPON_RANGE {
    void Reset_() {
        TEST_STASIS::Reset_();
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        CMBTFIRE::_defensive_range_bonus = 0;
    }
}

namespace TEST_CRYSTAL_FLEET {
    void Reset_() {
        TEST_STASIS::Reset_();
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 11;
        MOX::_combat_total_ship_count = 3;
        MOX::_apply_damage_to_planet = 0;
        MOX::_super_fast_flag = 0;
    }

    int16_t Cleanup_Count_() {
        return cleanup_count;
    }

    int16_t Cleanup_Source_(int16_t index) {
        return cleanup_sources[index];
    }
}

namespace TEST_BEAM_MODIFIERS {
    void Reset_() {
        has_high_energy_focus = false;
        memset(MOX::_ranged_to_hit_penalty, 0,
               sizeof(MOX::_ranged_to_hit_penalty));
        memset(MOX::_ranged_damage_penalty, 0,
               sizeof(MOX::_ranged_damage_penalty));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        CMBTFIRE::_mount_dmg[CMBTFIRE::HIGH_ENERGY_FOCUS_ABSENT] =
            { 50, 100, 150 };
        CMBTFIRE::_mount_dmg[CMBTFIRE::HIGH_ENERGY_FOCUS_PRESENT] =
            { 100, 150, 200 };
    }

    void Set_Has_HEF_(bool has_hef) {
        has_high_energy_focus = has_hef;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_missile* _missile = missiles;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    int16_t _combat_total_ship_count;
    uint16_t _apply_damage_to_planet;
    int16_t _super_fast_flag;
    int16_t _ranged_to_hit_penalty[9];
    int16_t _ranged_damage_penalty[9];
}

namespace TECHDATA {
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(
        int16_t, int16_t special_id) {
        return special_id == SPECIAL_HIGH_ENERGY_FOCUS &&
            has_high_energy_focus;
    }
}

namespace CMBTMIS {
    int16_t __cdecl Get_Missile_Hits_(int16_t, int16_t owner, uint16_t) {
        last_missile_hits_owner = owner;
        return 4;
    }

    void __cdecl Retarget_Missile_(int16_t missile_idx) {
        last_retargeted_missile = missile_idx;
    }

    void __cdecl Kill_Missile_(int16_t) {
    }
}

namespace COMBAT1 {
    void __cdecl Remove_Continuing_Effect_Specials_(int16_t source_idx) {
        cleanup_sources[cleanup_count++] = source_idx;
    }
}

namespace CMBTAI {
    int16_t __cdecl Range_To_Ship_(int16_t, int16_t) {
        return 9;
    }

    int16_t __cdecl Range_To_Missile_(int16_t, int16_t) {
        return 9;
    }
}

namespace CMBTSPEC {
    void __cdecl Destroy_Ship_FX_(int16_t, int16_t, int16_t, int16_t) {
    }
}