#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtmov1.h"
#include "game/combat1.h"
#include "game/combinit.h"

namespace {
    s_combat_data combat_data[2];
    s_missile missiles[300];
    s_player players[MAX_PLAYERS];
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
    s_missile* _missile = missiles;
    s_player* _player = players;
    int16_t _combat_total_ship_count;
    int16_t _cur_ship;
    int16_t _net_flag;
    int16_t _speedx2_flag;
    int16_t _combat_grid[COMBAT_GRID_WIDTH][COMBAT_GRID_HEIGHT];
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t, int16_t) {
        return 0;
    }

    void __cdecl Ship_Center_Offsets_(int16_t, int16_t* out_x, int16_t* out_y) {
        *out_x = 0;
        *out_y = 0;
    }
}

namespace CMBTMIS {
    char __cdecl Move_Missile_(int16_t missile_idx, int16_t, int16_t, int16_t) {
        s_missile* missile = &MOX::_missile[missile_idx];
        missile->x -= 2;
        missile->y -= 1;
        missile->travel_dist -= 2;
        missile->facing_dir = 9;
        return 0;
    }
}

namespace CMBTAI {
    int16_t __cdecl Will_Missile_Do_Damage_(int16_t, int16_t) {
        return 0;
    }
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