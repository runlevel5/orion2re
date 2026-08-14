#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtdrw1.h"
#include "game/cmbtmov1.h"
#include "game/combat1.h"
#include "game/combinit.h"

namespace {
    s_combat_data combat_data[2];
    s_colony colonies[1];
    s_missile missiles[300];
    s_planet_data planets[1];
    s_player players[MAX_PLAYERS];
    bool has_phase_shifter;
    bool has_sub_space_teleporter;
    bool has_energy_absorber;
    bool has_damper_field;
    bool has_hard_shields;
    bool planet_has_defenses;
    bool target_is_legal;
    bool sub_space_teleporter_enabled;
    int16_t teleporter_used;
    int16_t ship_center_offset_x;
    int16_t ship_center_offset_y;
    bool font_style_selected;
    s_colors selected_font_colors;
    char weapon_name[] = "Weapon";
    char weapon_name_plural[] = "Weapons";
}

namespace TEST_TRACTOR_POWER {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(players, 0, sizeof(players));

        MOX::_combat_total_ship_count = 2;

        s_combat_data* target = &MOX::_combat_data[0];
        target->owner = 10;
        target->size_class = SHIP_SIZE_LARGE;
        target->ftl_type = DRIVE_NUCLEAR_DRIVE;
        target->combat_speed_base = 10;
        target->drive_current_hits = 10;
        target->drive_max_hits = 10;
        target->black_hole_source_idx = UINT8_MAX;
        target->stasis_source_idx = UINT8_MAX;

        s_combat_data* tractor = &MOX::_combat_data[1];
        tractor->special_cooldown = 0;
        tractor->stasis_source_idx = UINT8_MAX;
        tractor->weapons[0].weapon_id = WEAPON_TRACTOR_BEAM;
        tractor->weapons[0].count = 3;

        const uint8_t defaults[SHIP_SIZE_COUNT] = {1, 2, 3, 4, 5, 6};
        memcpy(COMBAT1::_tractors_to_stop, defaults, sizeof(defaults));
    }
}

namespace TEST_EXPECTED_DAMAGE {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        memset(TECHDATA::_shields, 0, sizeof(TECHDATA::_shields));
        memset(MOX::_ordnance_bonus, 0, sizeof(MOX::_ordnance_bonus));
        memset(MOX::_fighter_beam_type, 0, sizeof(MOX::_fighter_beam_type));
        memset(MOX::_fighter_bomb_type, 0, sizeof(MOX::_fighter_bomb_type));
        has_energy_absorber = false;
        has_damper_field = false;
        has_hard_shields = false;
        planet_has_defenses = false;
        target_is_legal = false;
        MOX::_in_nebula_flag = 0;
        TECHDATA::_hard_shields_damage_reduction = 3;
        CMBTFIRE::_fighters_use_point_defense = 0;
        CMBTFIRE::_planet_non_enveloping = 0;
        CMBTMIS::missile_defense = {0, 50, 30};
    }
}

namespace TEST_MISSILE_BEARING {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(MOX::_combat_grid, 0, sizeof(MOX::_combat_grid));

        MOX::_cur_ship = 0;
        MOX::_speedx2_flag = 0;
        MOX::_net_flag = 0;
        MOX::_combat_grid[0][0] = 2000;

        MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
        MOX::_missile[0].source_unit_idx = 1;
        MOX::_missile[0].target_unit_idx = 0;
        MOX::_missile[0].x = 100;
        MOX::_missile[0].y = 50;
        MOX::_missile[0].travel_dist = 60;
        MOX::_missile[0].facing_dir = 3;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_colony* _colony = colonies;
    s_missile* _missile = missiles;
    s_planet_data* _planet = planets;
    s_player* _player = players;
    int16_t _cmbt_colony_id;
    int16_t _combat_total_ship_count;
    int16_t _cur_ship;
    int16_t _net_flag;
    int16_t _speedx2_flag;
    int16_t _auto_combat;
    int16_t _take_control_flag;
    int16_t _combat_defender_player;
    int16_t _in_nebula_flag;
    int16_t _ordnance_bonus[15];
    int16_t _combat_grid[COMBAT_GRID_WIDTH][COMBAT_GRID_HEIGHT];
    int16_t _fighter_beam_type[9];
    int16_t _fighter_bomb_type[9];
}

namespace TECHDATA {
    s_tech_shield_data _shields[SHIELD_COUNT];
    s_tech_weapon_data _weapons[WEAPON_COUNT];
    s_tech_weapon_mod_data _weapon_modifications[WEAPON_MOD_COUNT];
    uint8_t _hard_shields_damage_reduction;
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t, int16_t special_idx) {
        if (special_idx == SPECIAL_ENERGY_ABSORBER) {
            return has_energy_absorber;
        }
        if (special_idx == SPECIAL_DAMPER_FIELD) {
            return has_damper_field;
        }
        if (special_idx == SPECIAL_HARD_SHIELDS) {
            return has_hard_shields;
        }
        if (special_idx == SPECIAL_PHASE_SHIFTER) {
            return has_phase_shifter;
        }
        if (special_idx == SPECIAL_SUB_SPACE_TELEPORTER) {
            return has_sub_space_teleporter;
        }
        return 0;
    }

    void __cdecl Ship_Center_Offsets_(int16_t, int16_t* out_x, int16_t* out_y) {
        *out_x = ship_center_offset_x;
        *out_y = ship_center_offset_y;
    }
}

namespace CMBTMIS {
    int16_t __cdecl Get_Missile_Hits_(int16_t, int16_t, uint16_t) {
        return 1;
    }

    int16_t __cdecl Missile_Dcv_(int16_t, int16_t) {
        return 0;
    }

    char __cdecl Move_Missile_(int16_t missile_idx, int16_t, int16_t, int16_t) {
        s_missile* missile = &MOX::_missile[missile_idx];
        missile->x -= 2;
        missile->y -= 1;
        missile->travel_dist -= 2;
        missile->facing_dir = 9;
        return 0;
    }
}

namespace CMBTFIR2 {
    int16_t __cdecl Used_Teleporter_(int16_t) {
        return teleporter_used;
    }
}

namespace CMBTFIRE {
    s_mount_damage _mount_dmg[HIGH_ENERGY_FOCUS_PRESENCE_COUNT] = {
        {50, 100, 150},
        {50, 150, 200},
    };

    int32_t Energy_Absorber_Blocked_Damage_(int32_t damage) {
        return damage / 4;
    }

    int32_t Damper_Field_Remaining_Damage_(int32_t damage) {
        return damage / 4;
    }

    int16_t __cdecl Does_Combat_Planet_Have_Defenses_() {
        return planet_has_defenses;
    }

    bool Enveloping_Applies_To_Target_(int16_t) {
        return true;
    }

    void __cdecl Get_Beam_Weapon_Modifiers_(
        int16_t,
        int16_t,
        uint16_t,
        int16_t,
        int16_t* hit_bonus_out,
        int16_t* damage_bonus_out
    ) {
        *hit_bonus_out = 0;
        *damage_bonus_out = 100;
    }

    int8_t __cdecl Special_Is_Turned_On_(int16_t, int16_t) {
        return sub_space_teleporter_enabled;
    }

    int16_t __cdecl Weapon_In_Range_(int16_t, int16_t, int16_t, int16_t, int16_t) {
        return 0;
    }

    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t, uint16_t, int16_t, int16_t) {
        return target_is_legal;
    }
}

namespace INITSHIP {
    int16_t __cdecl Best_Ship_Shield_(int16_t) {
        return SHIELD_NO_SHIELD;
    }
}

namespace GEO {
    int32_t __cdecl Actual_Expected_Damage_(
        int32_t min_damage,
        int32_t,
        int32_t,
        int32_t,
        int32_t points_blocked
    ) {
        return min_damage - points_blocked;
    }
}

namespace graphics {
    void __cdecl Interlaced_Fill_(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t) {
    }
}

namespace fonts {
    void __cdecl Set_Font_Style_Shadow_Down_(int16_t, const s_colors* colors) {
        font_style_selected = colors != nullptr;
        if (colors != nullptr) {
            selected_font_colors = *colors;
        }
    }

    void __cdecl Set_Outline_Color_(uint16_t, uint16_t) {
    }

    void __cdecl Set_Font_Spacing_(int16_t) {
    }

    void __cdecl Print_Integer_Right_(int16_t, int16_t, int32_t) {
    }

    void __cdecl Print_(int16_t, int16_t, const char*) {
    }
}

namespace KEN {
    void __cdecl Ken_Get_Text_Message_(int32_t, char* buffer, int16_t buffer_size) {
        if (buffer_size > 0) {
            buffer[0] = '\0';
        }
    }
}

extern "C" size_t SDL_strlcpy(char* destination, const char* source, size_t destination_size) {
    const size_t source_length = strlen(source);
    if (destination_size > 0) {
        const size_t copy_length =
            source_length < destination_size - 1 ? source_length : destination_size - 1;
        memcpy(destination, source, copy_length);
        destination[copy_length] = '\0';
    }
    return source_length;
}

extern "C" size_t SDL_strlcat(char* destination, const char* source, size_t destination_size) {
    size_t destination_length = 0;
    while (destination_length < destination_size &&
           destination[destination_length] != '\0') {
        destination_length++;
    }

    const size_t source_length = strlen(source);
    if (destination_length < destination_size) {
        const size_t available = destination_size - destination_length - 1;
        const size_t copy_length = source_length < available ? source_length : available;
        memcpy(destination + destination_length, source, copy_length);
        destination[destination_length + copy_length] = '\0';
    }

    return destination_length + source_length;
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void*, uint16_t) {
        return 0;
    }

    void __cdecl Clear_Structure_(void* dest, uint32_t size) {
        memset(dest, 0, size);
    }
}

namespace angle {
    int16_t __cdecl Get_Angle_(int32_t, int32_t) {
        return 0;
    }
}

namespace special {
    int16_t __cdecl Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        const int16_t dx = abs(x2 - x1);
        const int16_t dy = abs(y2 - y1);
        return dx > dy ? dx + dy / 2 : dy + dx / 2;
    }

    void __cdecl Absolute_Interpolate_Line_(
        int16_t* x_ptr,
        int16_t* y_ptr,
        int16_t,
        int16_t,
        int16_t x2,
        int16_t y2,
        int16_t) {
        *x_ptr = x2;
        *y_ptr = y2;
    }
}

DOCTEST_TEST_CASE("disabled spent weapons remain red") {
    memset(combat_data, 0, sizeof(combat_data));
    memset(players, 0, sizeof(players));
    memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));

    MOX::_cur_ship = 0;
    MOX::_auto_combat = 0;
    MOX::_take_control_flag = 0;
    MOX::_combat_data[0].owner = 0;
    MOX::_player[0].objectives = PLAYER_OBJECTIVE_HUMAN;

    s_combat_weapon_slot* slot = &MOX::_combat_data[0].weapons[0];
    slot->weapon_id = WEAPON_LASER_CANNON;
    slot->count = 1;
    slot->arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    slot->active = -1;
    slot->shots_left = 0;
    slot->status = 0;

    TECHDATA::_weapons[slot->weapon_id].name = weapon_name;
    TECHDATA::_weapons[slot->weapon_id].name_plural = weapon_name_plural;
    TECHDATA::_weapons[slot->weapon_id].type = WEAPON_TYPE_BEAM;

    font_style_selected = false;
    memset(&selected_font_colors, 0, sizeof(selected_font_colors));

    CMBTDRW1::Draw_Weapon_Status_Display_(-1, -1);

    const uint8_t expected_colors[8] = {
        0x48, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a,
    };
    DOCTEST_REQUIRE(font_style_selected);
    for (int i = 0; i < 8; ++i) {
        DOCTEST_CHECK(selected_font_colors.colors[i] == expected_colors[i]);
    }
}

DOCTEST_TEST_CASE("tactical planets use size-specific draw offsets") {
    struct s_planet_draw_offset_case {
        int8_t size;
        int16_t x;
        int16_t y;
    };

    const s_planet_draw_offset_case cases[] = {
        {PLANET_SIZE_TINY, 2, 2},
        {PLANET_SIZE_SMALL, 0, 1},
        {PLANET_SIZE_MEDIUM, 7, 7},
        {PLANET_SIZE_LARGE, 4, 4},
        {PLANET_SIZE_HUGE, 6, 6},
    };

    MOX::_cmbt_colony_id = 0;
    MOX::_colony[0].planet = 0;

    for (const s_planet_draw_offset_case test_case : cases) {
        MOX::_planet[0].size = test_case.size;

        int16_t x = -1;
        int16_t y = -1;
        CMBTDRW1::Get_Draw_Ship_Offsets_(0, &x, &y);

        DOCTEST_CHECK(x == test_case.x);
        DOCTEST_CHECK(y == test_case.y);
    }

    MOX::_cmbt_colony_id = -1;
    int16_t x = -1;
    int16_t y = -1;
    CMBTDRW1::Get_Draw_Ship_Offsets_(0, &x, &y);

    DOCTEST_CHECK(x == 7);
    DOCTEST_CHECK(y == 7);
}

DOCTEST_TEST_CASE("interactive speed loss preserves movement already spent") {
    int16_t reserve = 0;

    int16_t movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        10, 4, 10, 7, &reserve);

    DOCTEST_CHECK(movement == 1);
    DOCTEST_CHECK(reserve == 6);

    movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        4, 10, 10, movement, &reserve);

    DOCTEST_CHECK(movement == 7);
    DOCTEST_CHECK(reserve == 0);
}

DOCTEST_TEST_CASE("interactive immobilization never makes movement negative") {
    int16_t reserve = 0;

    int16_t movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        10, 0, 10, 3, &reserve);

    DOCTEST_CHECK(movement == 0);
    DOCTEST_CHECK(reserve == 3);

    movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        0, 10, 10, movement, &reserve);

    DOCTEST_CHECK(movement == 3);
    DOCTEST_CHECK(reserve == 0);
}

DOCTEST_TEST_CASE("tractor speed uses the configured hull threshold") {
    TEST_TRACTOR_POWER::Reset_();

    DOCTEST_CHECK(COMBAT1::Calc_Current_Speed_(0) == 0);

    COMBAT1::_tractors_to_stop[SHIP_SIZE_LARGE] = 4;
    DOCTEST_CHECK(COMBAT1::Calc_Current_Speed_(0) == 3);
}

DOCTEST_TEST_CASE("Trans-Dimensional combat speed uses the configured bonus") {
    TEST_TRACTOR_POWER::Reset_();
    MOX::_combat_total_ship_count = 1;
    MOX::_combat_data[0].owner = 0;
    MOX::_player[0].traits[TRAIT_TRANS_DIMENSIONAL] = 1;
    COMBAT1::_td_combat_speed_bonus = 7;

    DOCTEST_CHECK(COMBAT1::Calc_Current_Speed_(0) == 17);

    COMBAT1::_td_combat_speed_bonus = 4;
}

DOCTEST_TEST_CASE("ship range exposes tactical squares without changing legacy range") {
    memset(combat_data, 0, sizeof(combat_data));
    MOX::_combat_data[0].x = 10;
    MOX::_combat_data[0].y = 10;
    MOX::_combat_data[1].x = 16;
    MOX::_combat_data[1].y = 10;

    DOCTEST_CHECK(CMBTAI::Range_To_Ship_Squares_(0, 1) == 6);
    DOCTEST_CHECK(CMBTAI::Range_To_Ship_(0, 1) == 2);
}

DOCTEST_TEST_CASE("ship to point range measures pixels before scaling") {
    memset(combat_data, 0, sizeof(combat_data));
    ship_center_offset_x = 0;
    ship_center_offset_y = 0;

    DOCTEST_CHECK(CMBTAI::Range_To_XY_(0, 2, 19) == 1);

    ship_center_offset_x = 20;
    DOCTEST_CHECK(CMBTAI::Range_To_XY_(0, 20, 0) == 0);
    DOCTEST_CHECK(CMBTAI::Range_To_XY_From_Corner_(0, 20, 0) == 1);

    ship_center_offset_x = 0;
}

DOCTEST_TEST_CASE("round teleporter excludes square corners") {
    memset(combat_data, 0, sizeof(combat_data));
    MOX::_combat_data[0].x = 20;
    MOX::_combat_data[0].y = 20;
    CMBTMOV1::_sub_space_teleporter_range = 20;

    CMBTMOV1::_round_teleporter = 0;
    DOCTEST_CHECK(CMBTMOV1::Teleporter_Destination_In_Range_(
        0, 40 * 20, 40 * 20));

    CMBTMOV1::_round_teleporter = 1;
    DOCTEST_CHECK_FALSE(CMBTMOV1::Teleporter_Destination_In_Range_(
        0, 40 * 20, 40 * 20));
    DOCTEST_CHECK(CMBTMOV1::Teleporter_Destination_In_Range_(
        0, 40 * 20, 20 * 20));
}

DOCTEST_TEST_CASE("Sub-Space Teleporter range is independent of ship speed") {
    memset(combat_data, 0, sizeof(combat_data));
    for (int x = 0; x < COMBAT_GRID_WIDTH; ++x) {
        for (int y = 0; y < COMBAT_GRID_HEIGHT; ++y) {
            MOX::_combat_grid[x][y] = -1;
        }
    }
    MOX::_combat_data[0].x = 40;
    MOX::_combat_data[0].y = 34;
    has_sub_space_teleporter = true;
    has_phase_shifter = false;
    sub_space_teleporter_enabled = true;
    teleporter_used = 0;
    CMBTMOV1::_round_teleporter = 0;
    CMBTMOV1::_sub_space_teleporter_range = 5;

    COMBAT1::Set_Legal_Moves_(0, 30, 0);

    DOCTEST_CHECK(MOX::_combat_grid[45][34] == 2000);
    DOCTEST_CHECK(MOX::_combat_grid[46][34] == -1);

    has_sub_space_teleporter = false;
    has_phase_shifter = true;
    for (int x = 0; x < COMBAT_GRID_WIDTH; ++x) {
        for (int y = 0; y < COMBAT_GRID_HEIGHT; ++y) {
            MOX::_combat_grid[x][y] = -1;
        }
    }

    COMBAT1::Set_Legal_Moves_(0, 7, 0);

    DOCTEST_CHECK(MOX::_combat_grid[47][34] == 2000);
    DOCTEST_CHECK(MOX::_combat_grid[48][34] == -1);

    has_sub_space_teleporter = true;
    for (int x = 0; x < COMBAT_GRID_WIDTH; ++x) {
        for (int y = 0; y < COMBAT_GRID_HEIGHT; ++y) {
            MOX::_combat_grid[x][y] = -1;
        }
    }

    COMBAT1::Set_Legal_Moves_(0, 7, 0);

    DOCTEST_CHECK(MOX::_combat_grid[47][34] == 2000);
    DOCTEST_CHECK(MOX::_combat_grid[48][34] == -1);

    has_sub_space_teleporter = false;
    has_phase_shifter = false;
}

DOCTEST_TEST_CASE("destroyed cloak devices update combat cloak state") {
    int8_t cloak_state = 1;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 0, false, false, false);
    DOCTEST_CHECK(cloak_state == 2);

    cloak_state = 4;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, true, false);
    DOCTEST_CHECK(cloak_state == 1);

    cloak_state = 6;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, true, false);
    DOCTEST_CHECK(cloak_state == 2);

    cloak_state = 7;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, true, false);
    DOCTEST_CHECK(cloak_state == 3);

    cloak_state = 4;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, false, false);
    DOCTEST_CHECK(cloak_state == 6);
}

DOCTEST_TEST_CASE("combat damage storage uses the percentage of lost hit points") {
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(14, 10) == 28);
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(9, 6) == 33);
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(20, 13) == 35);
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(0, 0) == 0);
}

DOCTEST_TEST_CASE("combat damage storage reconstructs surviving fighter units") {
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(7, 3) == 3);
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(6, 3) == 2);
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(0, 3) == 0);
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(7, 0) == 0);
}

DOCTEST_TEST_CASE("simulated rotation restores missile bearing") {
    TEST_MISSILE_BEARING::Reset_();

    int16_t target_facing = 1;
    CMBTMOV1::Rotate_Ship_Until_Impact_(0, &target_facing);

    DOCTEST_CHECK(MOX::_missile[0].x == 100);
    DOCTEST_CHECK(MOX::_missile[0].y == 50);
    DOCTEST_CHECK(MOX::_missile[0].travel_dist == 60);
    DOCTEST_CHECK(MOX::_missile[0].facing_dir == 3);
}

DOCTEST_TEST_CASE("missile threat uses the requested missile") {
    TEST_EXPECTED_DAMAGE::Reset_();
    MOX::_ordnance_bonus[0] = 100;
    MOX::_combat_data[1].shield_current_hits = 1;
    TECHDATA::_shields[0].shield_class = 10;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 5;
    TECHDATA::_weapons[WEAPON_MERCULITE_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_MERCULITE_MISSILE].max_damage = 20;
    MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
    MOX::_missile[0].owner = 0;
    MOX::_missile[1].type = WEAPON_MERCULITE_MISSILE;
    MOX::_missile[1].owner = 0;

    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 1) == 1);
}

DOCTEST_TEST_CASE("overloaded launched missile can penetrate shields") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 10;
    TECHDATA::_shields[0].shield_class = 12;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;
    MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
    MOX::_missile[0].owner = 0;
    MOX::_missile[0].specials = WEAPON_MOD_FLAG_OVERLOADED;
    MOX::_ordnance_bonus[0] = 100;
    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 1);
}

DOCTEST_TEST_CASE("Hard Shields stop inherent Shield Piercing in expected damage") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].natural_mods =
        WEAPON_NATURAL_FLAG_IGNORES_SHIELDS;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 12;
    TECHDATA::_shields[0].shield_class = 10;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;
    MOX::_ordnance_bonus[10] = 100;
    has_hard_shields = true;

    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        1, WEAPON_NUCLEAR_MISSILE, 1, 0, 10) == 0);

    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].natural_mods = 0;
    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        1,
        WEAPON_NUCLEAR_MISSILE,
        1,
        WEAPON_MOD_FLAG_SHIELD_PIERCING,
        10) == 0);
}

DOCTEST_TEST_CASE("only the three standard torpedoes are unshootable") {
    TEST_EXPECTED_DAMAGE::Reset_();
    const int16_t unshootable_torpedoes[] = {
        WEAPON_ANTI_MATTER_TORP,
        WEAPON_PROTON_TORPEDO,
        WEAPON_PLASMA_TORPEDO,
    };
    MOX::_ordnance_bonus[0] = 100;

    for (const int16_t weapon_id : unshootable_torpedoes) {
        TECHDATA::_weapons[weapon_id].type = WEAPON_TYPE_TORPEDO;
        TECHDATA::_weapons[weapon_id].max_damage = 300;
        MOX::_missile[0].type = weapon_id;
        MOX::_missile[0].owner = 0;

        DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 0);
    }
}

DOCTEST_TEST_CASE("Dragon Breath remains a shootable torpedo threat") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_DRAGON_BREATH].type = WEAPON_TYPE_TORPEDO;
    TECHDATA::_weapons[WEAPON_DRAGON_BREATH].max_damage = 300;
    MOX::_missile[0].type = WEAPON_DRAGON_BREATH;
    MOX::_missile[0].owner = 0;
    MOX::_ordnance_bonus[0] = 100;

    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 1);
}

DOCTEST_TEST_CASE("fighter threat includes the classic extra maximum damage point") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_INTERCEPTOR].type = WEAPON_TYPE_FIGHTER;
    TECHDATA::_weapons[WEAPON_LASER_CANNON].type = WEAPON_TYPE_BEAM;
    TECHDATA::_weapons[WEAPON_LASER_CANNON].max_damage = 10;
    TECHDATA::_shields[0].shield_class = 10;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;
    MOX::_fighter_beam_type[0] = WEAPON_LASER_CANNON;
    MOX::_missile[0].type = WEAPON_INTERCEPTOR;
    MOX::_missile[0].owner = 0;
    MOX::_ordnance_bonus[0] = 100;

    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 1);
}

DOCTEST_TEST_CASE("fighter expected damage retains the 1.50 absorber exception") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_BOMBER].type = WEAPON_TYPE_FIGHTER;
    TECHDATA::_weapons[WEAPON_NUCLEAR_BOMB].max_damage = 20;
    MOX::_fighter_bomb_type[0] = WEAPON_NUCLEAR_BOMB;
    MOX::_ordnance_bonus[0] = 100;
    has_energy_absorber = true;

    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        1, WEAPON_BOMBER, 1, 0, 0) == 40);
}

DOCTEST_TEST_CASE("fighter expected damage retains the 1.50 Shield Piercing exception") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_BOMBER].type = WEAPON_TYPE_FIGHTER;
    TECHDATA::_weapons[WEAPON_NUCLEAR_BOMB].max_damage = 5;
    TECHDATA::_shields[0].shield_class = 10;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;
    MOX::_fighter_bomb_type[0] = WEAPON_NUCLEAR_BOMB;
    MOX::_ordnance_bonus[0] = 100;

    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        1,
        WEAPON_BOMBER,
        1,
        WEAPON_MOD_FLAG_SHIELD_PIERCING,
        0) == 0);
}

DOCTEST_TEST_CASE("1.50 expected damage keeps Enveloping against planets") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 10;
    MOX::_ordnance_bonus[10] = 100;
    CMBTFIRE::_planet_non_enveloping = 1;

    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        0,
        WEAPON_NUCLEAR_MISSILE,
        1,
        WEAPON_MOD_FLAG_ENVELOPING,
        10) == 40);
}

DOCTEST_TEST_CASE("1.50 cloak prediction retains the configured miss percentage") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 100;
    MOX::_combat_data[1].special_status_flag = 1;
    MOX::_ordnance_bonus[10] = 100;
    CMBTMIS::missile_defense.cloaking_device_missile_miss_percent = 30;

    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        1, WEAPON_NUCLEAR_MISSILE, 1, 0, 10) == 30);
}

DOCTEST_TEST_CASE("1.50 special expected damage retains negative estimates") {
    TEST_EXPECTED_DAMAGE::Reset_();
    target_is_legal = true;
    TECHDATA::_weapons[WEAPON_PLASMA_WEB].type = WEAPON_TYPE_SPECIAL;
    TECHDATA::_weapons[WEAPON_PLASMA_WEB].min_damage = 5;
    TECHDATA::_weapons[WEAPON_PLASMA_WEB].max_damage = 5;
    TECHDATA::_shields[0].shield_class = 10;
    MOX::_combat_data[0].weapons[0].weapon_id = WEAPON_PLASMA_WEB;
    MOX::_combat_data[0].weapons[0].count = 1;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;

    DOCTEST_CHECK(CMBTAI::Expected_Weapon_Damage_(
        0, 0, 1, TARGET_TYPE_SHIP, 0) == -20);
}

DOCTEST_TEST_CASE("nebula disables ordinary shields in missile prediction") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 10;
    TECHDATA::_shields[0].shield_class = 15;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;
    MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
    MOX::_missile[0].owner = 0;
    MOX::_missile[0].specials = 0;
    MOX::_ordnance_bonus[0] = 100;
    MOX::_in_nebula_flag = 1;

    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 1);

    has_hard_shields = true;
    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 0);
}

DOCTEST_TEST_CASE("expected missile damage applies absorber before MIRV") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].type = WEAPON_TYPE_MISSILE;
    TECHDATA::_weapons[WEAPON_NUCLEAR_MISSILE].max_damage = 10;
    MOX::_combat_data[1].shield_current_hits = 0;
    MOX::_combat_data[1].tac_val_evade = 0;
    MOX::_ordnance_bonus[10] = 100;
    has_energy_absorber = true;
    DOCTEST_CHECK(CMBTAI::Expected_Missile_Damage_(
        1,
        WEAPON_NUCLEAR_MISSILE,
        1,
        WEAPON_MOD_FLAG_MIRV,
        10) == 32);
}

DOCTEST_TEST_CASE("heavy fighter threat checks each beam and bomb payload") {
    TEST_EXPECTED_DAMAGE::Reset_();
    TECHDATA::_weapons[WEAPON_HEAVY_FIGHTER].type = WEAPON_TYPE_FIGHTER;
    TECHDATA::_weapons[WEAPON_LASER_CANNON].type = WEAPON_TYPE_BEAM;
    TECHDATA::_weapons[WEAPON_LASER_CANNON].max_damage = 5;
    TECHDATA::_weapons[WEAPON_NUCLEAR_BOMB].type = WEAPON_TYPE_BOMB;
    TECHDATA::_weapons[WEAPON_NUCLEAR_BOMB].max_damage = 20;
    TECHDATA::_shields[0].shield_class = 10;
    MOX::_combat_data[1].shield_type = 0;
    MOX::_combat_data[1].shield_current_hits = 1;
    MOX::_fighter_beam_type[0] = WEAPON_LASER_CANNON;
    MOX::_fighter_bomb_type[0] = WEAPON_NUCLEAR_BOMB;
    MOX::_missile[0].type = WEAPON_HEAVY_FIGHTER;
    MOX::_missile[0].owner = 0;
    MOX::_ordnance_bonus[0] = 100;
    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 1);

    TECHDATA::_weapons[WEAPON_LASER_CANNON].max_damage = 20;
    TECHDATA::_weapons[WEAPON_NUCLEAR_BOMB].max_damage = 5;
    DOCTEST_CHECK(CMBTAI::Will_Missile_Do_Damage_(1, 0) == 1);
}

DOCTEST_TEST_CASE("simulated movement restores missile bearing") {
    TEST_MISSILE_BEARING::Reset_();

    int16_t destination_x = 40;
    int16_t destination_y = 0;
    int16_t destination_facing = 0;
    CMBTMOV1::Test_Move_Until_Impact_(
        0, &destination_x, &destination_y, &destination_facing);

    DOCTEST_CHECK(MOX::_missile[0].x == 100);
    DOCTEST_CHECK(MOX::_missile[0].y == 50);
    DOCTEST_CHECK(MOX::_missile[0].travel_dist == 60);
    DOCTEST_CHECK(MOX::_missile[0].facing_dir == 3);
}