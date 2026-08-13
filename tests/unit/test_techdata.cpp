#include "pch.h"

#include "doctest/doctest.h"
#include "game/techdata.h"

namespace {
    struct s_expected_strategic_damage {
        int16_t weapon_id;
        uint8_t minimum;
        uint8_t maximum;
    };

    constexpr s_expected_strategic_damage kExpectedDamage[] = {
        { WEAPON_GAUSS_CANNON, 18, 18 },
        { WEAPON_PARTICLE_BEAM, 10, 30 },
        { WEAPON_GRAVITON_BEAM, 3, 15 },
        { WEAPON_DISRUPTER, 40, 40 },
        { WEAPON_DEATH_RAY, 50, 100 },
        { WEAPON_PLASMA_CANNON, 6, 30 },
        { WEAPON_PLASMA_TORPEDO, 120, 120 },
        { WEAPON_MAULER_DEVICE, 100, 100 },
        { WEAPON_HEAVY_FIGHTER, 8, 32 },
        { WEAPON_BOMBER, 5, 20 },
        { WEAPON_PLASMA_WEB, 5, 25 },
        { WEAPON_DRAGON_BREATH, 250, 250 },
        { WEAPON_CRYSTAL_RAY, 40, 80 },
        { WEAPON_PLASMA_BREATH, 60, 60 },
        { WEAPON_PLASMA_FLUX, 10, 40 },
        { WEAPON_CAUSTIC_SLIME, 25, 50 },
    };
}

DOCTEST_TEST_CASE("strategic weapon damage uses corrected estimates") {
    for (const s_expected_strategic_damage& expected : kExpectedDamage) {
        const s_tech_weapon_data& weapon =
            TECHDATA::_weapons[expected.weapon_id];
        DOCTEST_CHECK(weapon.strat_min_damage == expected.minimum);
        DOCTEST_CHECK(weapon.strat_max_damage == expected.maximum);
    }
}