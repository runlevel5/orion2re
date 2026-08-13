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
    bool has_transporters;
    int16_t ship_center_offsets[4];
    int16_t destroy_fx_type;
    int16_t explosion_damage_source;
    int16_t spherical_effect_type;
    int16_t spherical_damage;
}

namespace TEST_STASIS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(cleanup_sources, 0xff, sizeof(cleanup_sources));
        cleanup_count = 0;
        last_missile_hits_owner = -1;
        last_retargeted_missile = -1;
        has_transporters = false;
        for (int16_t& offset : ship_center_offsets) {
            offset = 10;
        }
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

namespace TEST_ENGINE_EXPLOSION {
    void Reset_() {
        TEST_STASIS::Reset_();
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 1;
        MOX::_combat_total_ship_count = 2;
        MOX::_apply_damage_to_planet = 1;
        MOX::_super_fast_flag = 0;
        CMBTFIRE::_broken_engine_explodes = 1;
        destroy_fx_type = -1;
        explosion_damage_source = -1;
        spherical_effect_type = -1;
        spherical_damage = -1;
    }

    int16_t Destroy_FX_Type_() {
        return destroy_fx_type;
    }

    int16_t Explosion_Damage_Source_() {
        return explosion_damage_source;
    }

    int16_t Spherical_Effect_Type_() {
        return spherical_effect_type;
    }

    int16_t Spherical_Damage_() {
        return spherical_damage;
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

namespace TEST_BOMBING_RANGE {
    void Reset_() {
        TEST_STASIS::Reset_();
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        BOARDING::board_range = {3, 12};
    }

    void Set_Has_Transporters_(bool value) {
        has_transporters = value;
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
        if (special_id == SPECIAL_TRANSPORTERS) {
            return has_transporters;
        }
        return special_id == SPECIAL_HIGH_ENERGY_FOCUS &&
            has_high_energy_focus;
    }

    void __cdecl Ship_Center_Offsets_(
        int16_t ship_idx,
        int16_t* out_x,
        int16_t* out_y
    ) {
        *out_x = ship_center_offsets[ship_idx];
        *out_y = ship_center_offsets[ship_idx];
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

namespace CMBTFIR2 {
    int16_t __cdecl Damage_From_Ship_Exploding_(int16_t ship_idx) {
        explosion_damage_source = ship_idx;
        return 42;
    }
}

namespace CMBTSPEC {
    void __cdecl Destroy_Ship_FX_(
        int16_t ship_idx, int16_t, int16_t, int16_t explosion_type) {
        destroy_fx_type = explosion_type;
        MOX::_combat_data[ship_idx].unit_status = 5;
    }

    void __cdecl Do_Spherical_SFX_(
        int16_t, int16_t, int16_t, int16_t effect_type, int16_t damage) {
        spherical_effect_type = effect_type;
        spherical_damage = damage;
    }
}