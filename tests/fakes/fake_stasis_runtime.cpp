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
    int16_t missile_hits;
    uint32_t random_value = 100;
    int16_t beam_target_range = 9;
}

namespace TEST_STASIS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(cleanup_sources, 0xff, sizeof(cleanup_sources));
        cleanup_count = 0;
        last_missile_hits_owner = -1;
        last_retargeted_missile = -1;
        missile_hits = 4;
        random_value = 100;
        beam_target_range = 9;
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
        CMBTFIRE::weapon_range = {
            8, 5, 5, 5, 4, 5, 5, 1, 5,
        };
        CMBTFIRE::spherical_range_to_ship = {
            6, 4, 6, 6,
        };
        CMBTFIRE::spherical_range_to_missile = {
            122, 96, 122, 135,
        };
    }

    void Set_Center_Offset_(int16_t ship_idx, int16_t offset) {
        ship_center_offsets[ship_idx] = offset;
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

namespace TEST_FAIR_BEAMS {
    void Reset_() {
        TEST_STASIS::Reset_();
        TEST_BEAM_MODIFIERS::Reset_();
        random_value = 20;
        beam_target_range = 8;
        missile_hits = 20;
        CMBTFIRE::_simplified_beam_formula = 0;
        MOX::_ordnance_bonus[0] = 100;
        MOX::_combat_data[0].owner = 0;
        MOX::_combat_data[0].tac_val_attack = 100;
        MOX::_combat_data[1].owner = 0;
        MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
        MOX::_missile[0].source_unit_idx = 1;
        MOX::_missile[0].quantity = 1;
        TECHDATA::_weapons[WEAPON_LASER_CANNON].min_damage = 20;
        TECHDATA::_weapons[WEAPON_LASER_CANNON].max_damage = 20;
        MOX::_ranged_to_hit_penalty[8] = -85;
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

namespace TEST_FIGHTER_ORDNANCE {
    void Reset_() {
        TEST_STASIS::Reset_();
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        memset(MOX::_ordnance_bonus, 0, sizeof(MOX::_ordnance_bonus));
        memset(MOX::_fighter_beam_type, 0,
               sizeof(MOX::_fighter_beam_type));
        missile_hits = 20;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_missile* _missile = missiles;
    s_colony* _colony;
    s_player* _player;
    int16_t _ordnance_bonus[15];
    int16_t _fighter_beam_type[9];
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    int16_t _combat_total_ship_count;
    int16_t _cmbt_colony_id;
    int16_t _in_nebula_flag;
    int16_t _bio_weapon_flag;
    uint16_t _apply_damage_to_planet;
    int16_t _super_fast_flag;
    int16_t _ranged_to_hit_penalty[9];
    int16_t _ranged_damage_penalty[9];
    s_combat_bomb_result _combat_bomb_result;
}

namespace TECHDATA {
    s_tech_weapon_data _weapons[WEAPON_COUNT];
    s_tech_armor_data _armor[ARMOR_COUNT];
    s_techdata_planetary_shield _planetary_shields[PLANETARY_SHIELD_COUNT];
    s_tech_shield_data _shields[SHIELD_COUNT];
    uint8_t _hard_shields_damage_reduction;
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
        return missile_hits;
    }

    int16_t __cdecl Is_Missile_Target_Alive_(int16_t missile_idx) {
        const s_missile* target =
            &missiles[missiles[missile_idx].target_unit_idx];
        return target->type > 0 && target->quantity > 0;
    }

    int16_t __cdecl Fighter_Ocv_(int16_t) {
        return 0;
    }

    int16_t __cdecl Missile_Dcv_(int16_t, int16_t) {
        return 0;
    }

    void __cdecl Retarget_Missile_(int16_t missile_idx) {
        last_retargeted_missile = missile_idx;
    }

    void __cdecl Kill_Missile_(int16_t) {
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return random_value;
    }
}

namespace COMBAT1 {
    void __cdecl Remove_Continuing_Effect_Specials_(int16_t source_idx) {
        cleanup_sources[cleanup_count++] = source_idx;
    }
}

namespace CMBTDRW1 {
    int16_t __cdecl Relative_Bearing_(int16_t, int16_t) {
        return 0;
    }
}

namespace CMBTAI {
    int16_t __cdecl Range_To_Ship_(int16_t, int16_t) {
        return beam_target_range;
    }

    int16_t __cdecl Range_To_Missile_(int16_t, int16_t) {
        return beam_target_range;
    }
}

namespace CMBTFIR2 {
    int16_t __cdecl Damage_From_Ship_Exploding_(int16_t ship_idx) {
        explosion_damage_source = ship_idx;
        return 42;
    }

    void __cdecl Kill_Crew_(int16_t, int16_t) {
    }

    void __cdecl Select_Internal_(
        int16_t, int16_t* hit_type, int16_t*, int16_t, int16_t,
        int16_t, int16_t
    ) {
        *hit_type = -1;
    }

    void __cdecl Apply_Internal_Damage_(
        int16_t, int16_t*, int16_t, int16_t, int16_t
    ) {
    }
}

namespace INITSHIP {
    int16_t __cdecl Best_Armor_(int16_t) {
        return 0;
    }
}

namespace BOMB {
    void __cdecl Apply_Bomb_Hits_(s_combat_bomb_result*) {
    }
}

namespace ERICNET {
    void __cdecl Remove_Building_(int16_t, int16_t) {
    }
}

namespace struct_ {
    void __cdecl Clear_Structure_(void* dest, uint32_t size) {
        memset(dest, 0, size);
    }

    void __cdecl Clear_Bit_Field_(void*, uint16_t) {
    }
}

namespace strings {
    void __cdecl Delete_Structure_(int32_t, void*, uint32_t, uint32_t) {
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