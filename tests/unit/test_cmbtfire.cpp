#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfire.h"

namespace TEST_STASIS {
    void Reset_();
    int16_t Last_Cleanup_Source_();
    int16_t Last_Missile_Hits_Owner_();
    int16_t Last_Retargeted_Missile_();
}

namespace TEST_CRYSTAL_FLEET {
    void Reset_();
    int16_t Cleanup_Count_();
    int16_t Cleanup_Source_(int16_t index);
}

namespace TEST_BEAM_MODIFIERS {
    void Reset_();
    void Set_Has_HEF_(bool has_hef);
}

DOCTEST_TEST_CASE("destroyed Crystal releases captures and invalidates effects") {
    TEST_CRYSTAL_FLEET::Reset_();

    MOX::_combat_data[1].owner = 11;
    MOX::_combat_data[1].previous_owner = 11;

    MOX::_combat_data[2].owner = 11;
    MOX::_combat_data[2].previous_owner = 0;
    MOX::_combat_data[2].ship_image_seg =
        reinterpret_cast<s_animation_header*>(1);

    CMBTFIRE::Destroy_Ship_(1, 0, 0, 1);

    DOCTEST_CHECK(MOX::_combat_data[2].owner == 0);
    DOCTEST_CHECK(MOX::_combat_data[2].ship_image_seg == nullptr);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Count_() == 2);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Source_(0) == 1);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Source_(1) == 2);
}

DOCTEST_TEST_CASE("placing a ship in stasis cuts its continuing effects") {
    TEST_STASIS::Reset_();
    MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
    MOX::_missile[0].source_unit_idx = 0;
    MOX::_missile[0].target_unit_idx = 1;

    CMBTFIRE::Place_Ship_In_Stasis_(1, 0);

    DOCTEST_CHECK(MOX::_combat_data[1].stasis_source_idx == 0);
    DOCTEST_CHECK(TEST_STASIS::Last_Retargeted_Missile_() == 0);
    DOCTEST_CHECK(TEST_STASIS::Last_Cleanup_Source_() == 1);
}

DOCTEST_TEST_CASE("missile damage keeps launch armor after source ship capture") {
    TEST_STASIS::Reset_();
    MOX::_combat_data[1].owner = 2;
    MOX::_missile[0].type = WEAPON_INTERCEPTOR;
    MOX::_missile[0].owner = 1;
    MOX::_missile[0].source_unit_idx = 1;

    int16_t damage = 1;
    CMBTFIRE::Apply_Damage_To_Missile_(0, &damage, 0, 0);

    DOCTEST_CHECK(TEST_STASIS::Last_Missile_Hits_Owner_() == 1);
}

DOCTEST_TEST_CASE("heavy fighter missile targeting follows scramble setting") {
    TEST_STASIS::Reset_();
    TECHDATA::_weapons[WEAPON_HEAVY_FIGHTER].type = WEAPON_TYPE_FIGHTER;
    TECHDATA::_weapons[WEAPON_INTERCEPTOR].type = WEAPON_TYPE_FIGHTER;

    CMBTFIRE::_heavy_fighter_can_scramble = 0;
    DOCTEST_CHECK(
        CMBTFIRE::Weapon_Can_Fire_At_Missiles_(
            WEAPON_HEAVY_FIGHTER, 0) == 0);
    DOCTEST_CHECK(
        CMBTFIRE::Weapon_Can_Fire_At_Missiles_(
            WEAPON_INTERCEPTOR, 0) == 1);

    CMBTFIRE::_heavy_fighter_can_scramble = 1;
    DOCTEST_CHECK(
        CMBTFIRE::Weapon_Can_Fire_At_Missiles_(
            WEAPON_HEAVY_FIGHTER, 0) == 1);

    CMBTFIRE::_heavy_fighter_can_scramble = 0;
}

DOCTEST_TEST_CASE("planet non-enveloping setting only suppresses planetary multiplier") {
    CMBTFIRE::_planet_non_enveloping = 0;
    DOCTEST_CHECK(CMBTFIRE::Enveloping_Applies_To_Target_(0));
    DOCTEST_CHECK(CMBTFIRE::Enveloping_Applies_To_Target_(1));

    CMBTFIRE::_planet_non_enveloping = 1;
    DOCTEST_CHECK_FALSE(CMBTFIRE::Enveloping_Applies_To_Target_(0));
    DOCTEST_CHECK(CMBTFIRE::Enveloping_Applies_To_Target_(1));

    CMBTFIRE::_planet_non_enveloping = 0;
}

DOCTEST_TEST_CASE("absorber and damper percentage calculations preserve classic behavior") {
    CMBTFIRE::_absorber_damage_reduction = 25;
    CMBTFIRE::_damper_field_ratio = 25;
    DOCTEST_CHECK(CMBTFIRE::Energy_Absorber_Blocked_Damage_(103) == 25);
    DOCTEST_CHECK(CMBTFIRE::Damper_Field_Remaining_Damage_(103) == 25);

    CMBTFIRE::_absorber_damage_reduction = 40;
    CMBTFIRE::_damper_field_ratio = 60;
    DOCTEST_CHECK(CMBTFIRE::Energy_Absorber_Blocked_Damage_(103) == 41);
    DOCTEST_CHECK(CMBTFIRE::Damper_Field_Remaining_Damage_(103) == 61);

    CMBTFIRE::_absorber_damage_reduction = 25;
    CMBTFIRE::_damper_field_ratio = 25;
}

DOCTEST_TEST_CASE("beam mount damage defaults match classic HEF combinations") {
    TEST_BEAM_MODIFIERS::Reset_();

    struct s_case {
        uint16_t flags;
        bool has_hef;
        int16_t expected_hit_bonus;
        int16_t expected_damage;
    };
    const s_case cases[] = {
        { WEAPON_MOD_FLAG_POINT_DEFENSE, false, 25, 50 },
        { 0, false, 0, 100 },
        { WEAPON_MOD_FLAG_HEAVY_MOUNT, false, 0, 150 },
        { WEAPON_MOD_FLAG_POINT_DEFENSE, true, 25, 100 },
        { 0, true, 0, 150 },
        { WEAPON_MOD_FLAG_HEAVY_MOUNT, true, 0, 200 },
    };

    for (const s_case& value : cases) {
        TEST_BEAM_MODIFIERS::Set_Has_HEF_(value.has_hef);
        int16_t hit_bonus = 0;
        int16_t damage = 0;
        CMBTFIRE::Get_Beam_Weapon_Modifiers_(
            0, 0, value.flags, 0, &hit_bonus, &damage);
        DOCTEST_CHECK(hit_bonus == value.expected_hit_bonus);
        DOCTEST_CHECK(damage == value.expected_damage);
    }
}

DOCTEST_TEST_CASE("beam modifiers use configured mount damage") {
    TEST_BEAM_MODIFIERS::Reset_();
    CMBTFIRE::_mount_dmg[CMBTFIRE::HIGH_ENERGY_FOCUS_PRESENT].heavy = 175;
    TEST_BEAM_MODIFIERS::Set_Has_HEF_(true);

    int16_t hit_bonus = 0;
    int16_t damage = 0;
    CMBTFIRE::Get_Beam_Weapon_Modifiers_(
        0, 0, WEAPON_MOD_FLAG_HEAVY_MOUNT, 0,
        &hit_bonus, &damage);

    DOCTEST_CHECK(hit_bonus == 0);
    DOCTEST_CHECK(damage == 175);
}

DOCTEST_TEST_CASE("configured beam mount damage clamps negative modifiers") {
    TEST_BEAM_MODIFIERS::Reset_();
    CMBTFIRE::_mount_dmg[CMBTFIRE::HIGH_ENERGY_FOCUS_ABSENT].pd = 20;
    MOX::_ranged_damage_penalty[0] = -30;

    int16_t hit_bonus = 0;
    int16_t damage = 0;
    CMBTFIRE::Get_Beam_Weapon_Modifiers_(
        0, 0, WEAPON_MOD_FLAG_POINT_DEFENSE, 0,
        &hit_bonus, &damage);

    DOCTEST_CHECK(hit_bonus == 25);
    DOCTEST_CHECK(damage == 0);
}