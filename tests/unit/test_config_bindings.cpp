#include "pch.h"

#include "game/build_queue.h"

#include <cstring>

#include "config/cfg_parser.h"
#include "config/config_decode.h"
#include "config/config_runtime.h"
#include "doctest/doctest.h"
#include "game/boarding.h"
#include "game/cmbtfire.h"
#include "game/combinit.h"
#include "game/cmbtmis.h"
#include "game/combat.h"
#include "game/combat1.h"
#include "game/design_config.h"
#include "game/ericnet.h"
#include "game/events.h"
#include "game/invasion.h"
#include "game/mapgen.h"
#include "game/ship_config.h"
#include "game/shipstak.h"

namespace {
    bool Append_Decoded_(const char* source, config::s_config_write_set* write_set,
                         config::s_cfg_diagnostic* diagnostic) {
        config::s_cfg_document document;
        if (!config::Parse_Cfg("bindings.cfg", source, std::strlen(source),
                               &document, diagnostic)) {
            return false;
        }
        for (const config::s_cfg_statement& statement : document.statements) {
            config::s_config_write_set statement_writes;
            if (!config::Decode_Assignment(&statement, &statement_writes, diagnostic)) {
                return false;
            }
            for (config::s_config_write& write : statement_writes.writes) {
                write_set->writes.push_back(std::move(write));
            }
        }
        return true;
    }

    bool States_Equal_(const config::s_config_state& left,
                       const config::s_config_state& right) {
        if (left.values.size() != right.values.size()) {
            return false;
        }
        for (size_t index = 0; index < left.values.size(); ++index) {
            if (left.values[index].type != right.values[index].type ||
                left.values[index].bool_value != right.values[index].bool_value ||
                left.values[index].integer_value != right.values[index].integer_value ||
                left.values[index].string_value != right.values[index].string_value ||
                left.values[index].bitmap_value != right.values[index].bitmap_value ||
                left.values[index].ratio_numerator != right.values[index].ratio_numerator ||
                left.values[index].ratio_denominator != right.values[index].ratio_denominator) {
                return false;
            }
        }
        return true;
    }

    config::s_config_write Invalid_Write_(config::t_variable_id variable_id,
                                          config::e_value_type type,
                                          int64_t integer_value) {
        config::s_config_write write;
        write.variable_id = variable_id;
        write.value.type = type;
        write.value.integer_value = integer_value;
        write.provenance.source = "forged.cfg";
        write.provenance.line = 7;
        write.provenance.column = 9;
        return write;
    }
}

DOCTEST_TEST_CASE("build queue defaults match the eleven 1.50 hotkey lists") {
    config::s_config_state defaults;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&defaults));
    DOCTEST_REQUIRE(config::Commit_State(&defaults));

    DOCTEST_CHECK(BUILD_QUEUE::presets[0][0] == BUILD_QUEUE::INSTRUCTION_CLEAR_CONTINUOUS);
    DOCTEST_CHECK(BUILD_QUEUE::presets[0][1] == BUILD_QUEUE::INSTRUCTION_MERGE);
    DOCTEST_CHECK(BUILD_QUEUE::presets[0][2] ==
                  BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_ALIEN_CONTROL_CENTER);
    DOCTEST_CHECK(BUILD_QUEUE::presets[1][0] == BUILD_QUEUE::INSTRUCTION_PREPEND);
    DOCTEST_CHECK(BUILD_QUEUE::presets[1][1] == BUILD_QUEUE::INSTRUCTION_DESIGN1);
    DOCTEST_CHECK(BUILD_QUEUE::presets[7][1] == BUILD_QUEUE::INSTRUCTION_FREIGHTER_FLEET);
    DOCTEST_CHECK(BUILD_QUEUE::presets[8][1] == BUILD_QUEUE::INSTRUCTION_SPY);
    DOCTEST_CHECK(BUILD_QUEUE::presets[9][0] == BUILD_QUEUE::INSTRUCTION_HOUSING);
    DOCTEST_CHECK(BUILD_QUEUE::presets[10][0] == BUILD_QUEUE::INSTRUCTION_CLEAR);
    for (int preset = 0; preset < BUILD_QUEUE::PRESET_COUNT; ++preset) {
        DOCTEST_CHECK(BUILD_QUEUE::presets[preset][BUILD_QUEUE::INSTRUCTION_CAPACITY - 1] ==
                      BUILD_QUEUE::INSTRUCTION_END);
        bool found_end = false;
        for (int instruction = 0; instruction < BUILD_QUEUE::INSTRUCTION_CAPACITY;
             ++instruction) {
            found_end = found_end ||
                BUILD_QUEUE::presets[preset][instruction] == BUILD_QUEUE::INSTRUCTION_END;
        }
        DOCTEST_CHECK(found_end);
    }
}

DOCTEST_TEST_CASE("experimental autobuild settings bind and validate queue defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BUILD_QUEUE::autobuild_settings.enabled == 0);
    DOCTEST_CHECK(BUILD_QUEUE::autobuild_settings.default_preset == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "experiment autobuild enabled = true; experiment autobuild default = 10;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BUILD_QUEUE::autobuild_settings.enabled == 1);
    DOCTEST_CHECK(BUILD_QUEUE::autobuild_settings.default_preset == 10);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "experiment autobuild default = 11;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("fighter point-defense setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(CMBTFIRE::_fighters_use_point_defense == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat fighters_use_point_defense = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(CMBTFIRE::_fighters_use_point_defense == 1);
}

DOCTEST_TEST_CASE("civilian armor preserves classic defaults and binds by armor name") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    const uint16_t expected[ARMOR_COUNT] = {
        100, 100, 100, 100, 100, 100, 100,
    };
    DOCTEST_CHECK(std::memcmp(CMBTFIRE::_civilian_armor, expected,
                              sizeof(expected)) == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat civilian_armor no_armor = 90;"
        "combat civilian_armor tritanium = 120;"
        "combat civilian_armor xentronium = 200;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(CMBTFIRE::_civilian_armor[ARMOR_NO_ARMOR] == 90);
    DOCTEST_CHECK(CMBTFIRE::_civilian_armor[ARMOR_TRITANIUM_ARMOR] == 120);
    DOCTEST_CHECK(CMBTFIRE::_civilian_armor[ARMOR_XENTRONIUM_ARMOR] == 200);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "combat civilian_armor titanium = 0;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("research retention settings preserve classic defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COLCALC::_keep_research_on_breakthrough == 0);
    DOCTEST_CHECK(COLCALC::_keep_research_on_finds == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "research keep_research_on_breakthrough = true;"
        "research keep_research_on_finds = true;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COLCALC::_keep_research_on_breakthrough == 1);
    DOCTEST_CHECK(COLCALC::_keep_research_on_finds == 1);
}

DOCTEST_TEST_CASE("ground Commando settings preserve classic defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(invasion::_ground_commando_defender_x5 == 0);
    DOCTEST_CHECK(invasion::_ground_commando_attacker_x2 == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "leaders ground_commando_defender_x5 = true;"
        "leaders ground_commando_attacker_x2 = true;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(invasion::_ground_commando_defender_x5 == 1);
    DOCTEST_CHECK(invasion::_ground_commando_attacker_x2 == 1);
}

DOCTEST_TEST_CASE("Housing population assignment setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COLCALC::_housing_creates_no_farmers == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "colony housing_creates_no_farmers = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COLCALC::_housing_creates_no_farmers == 1);
}

DOCTEST_TEST_CASE("Food Replicator costs preserve 1.50 defaults and validate their range") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COLCALC::_replicator_food_pp == 2);
    DOCTEST_CHECK(COLCALC::_replicator_food_bc == 1);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "colony replicator_food_pp = 0;"
        "colony replicator_food_bc = 127;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COLCALC::_replicator_food_pp == 0);
    DOCTEST_CHECK(COLCALC::_replicator_food_bc == 127);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "colony replicator_food_pp = 128;", &writes, &diagnostic));
    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "colony replicator_food_bc = 128;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("event delays preserve 1.50 defaults and bind by event name") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    const uint16_t expected[EVENT_RANDOM_COUNT] = {
        0, 0, 200, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        100, 200, 300, 150, 250, 200, 0, 0, 0, 0,
    };
    DOCTEST_CHECK(std::memcmp(EVENTS::event_delay, expected, sizeof(expected)) == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "events event_delay comet = 150;"
        "events event_delay space_amoeba = 75;"
        "events event_delay mineral_depletion = 50;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(EVENTS::event_delay[EVENT_COMET] == 150);
    DOCTEST_CHECK(EVENTS::event_delay[EVENT_SPACE_AMOEBA] == 75);
    DOCTEST_CHECK(EVENTS::event_delay[EVENT_MINERAL_DEPLETION] == 50);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "events event_delay comet = 65536;", &writes, &diagnostic));
    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "events event_delay overworking = 50;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("satellite capture setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_satellite_capture_enabled == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat satellite_capture = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_satellite_capture_enabled == 1);
}

DOCTEST_TEST_CASE("Crystal fleet retention preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_crystal_keep == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat crystal_keep = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_crystal_keep == 1);
}

DOCTEST_TEST_CASE("Amoeba regeneration settings preserve classic defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_amoeba_regeneration == 0);
    DOCTEST_CHECK(COMBINIT::repair_rate.cyber == 10);
    DOCTEST_CHECK(COMBINIT::repair_rate.aru == 20);
    DOCTEST_CHECK(COMBINIT::repair_rate.regen == 20);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat amoeba_regeneration = true;"
        "combat repair_rate cyber = 7;"
        "combat repair_rate aru = 11;"
        "combat repair_rate regen = 13;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_amoeba_regeneration == 1);
    DOCTEST_CHECK(COMBINIT::repair_rate.cyber == 7);
    DOCTEST_CHECK(COMBINIT::repair_rate.aru == 11);
    DOCTEST_CHECK(COMBINIT::repair_rate.regen == 13);
}

DOCTEST_TEST_CASE("combat re-engagement buildings bind by building name") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    const uint8_t expected[] = {
        BUILDING_ARTEMIS_SYSTEM_NET,
        BUILDING_BATTLESTATION,
        BUILDING_MISSILE_BASE,
        BUILDING_GROUND_BATTERIES,
        BUILDING_STAR_BASE,
        BUILDING_STAR_FORTRESS,
        BUILDING_STELLAR_CONVERTER,
        BUILDING_FIGHTER_GARRISON,
        BUILDING_NO_BUILDING,
    };
    DOCTEST_CHECK(std::memcmp(ERICNET::_combat_reengage_buildings, expected,
                              sizeof(expected)) == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat reengage_buildings = radiation_shield none;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(ERICNET::_combat_reengage_buildings[0] == BUILDING_RADIATION_SHIELD);
    DOCTEST_CHECK(ERICNET::_combat_reengage_buildings[1] == BUILDING_NO_BUILDING);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "combat reengage_buildings = unknown_building none;",
        &writes, &diagnostic));

    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
}

DOCTEST_TEST_CASE("Warlord combat settings preserve classic defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_satellite_warlord_bonus == 0);
    DOCTEST_CHECK(COMBAT::_strategic_xp_bonus == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat satellite_warlord_bonus = true;"
        "combat strategic_xp_bonus = true;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_satellite_warlord_bonus == 1);
    DOCTEST_CHECK(COMBAT::_strategic_xp_bonus == 1);
}

DOCTEST_TEST_CASE("planetary Damper Field setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_planet_damper_enabled == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat planet_damper = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_planet_damper_enabled == 1);
}

DOCTEST_TEST_CASE("satellite capture state applies once to the matching colony") {
    COMBINIT::s_satellite_capture_state state;
    COMBINIT::Record_Captured_Satellite_State_(
        &state, 4, BUILDING_BATTLESTATION);

    DOCTEST_CHECK(COMBINIT::Take_Captured_Satellite_State_(&state, 5) == -1);
    DOCTEST_CHECK(COMBINIT::Take_Captured_Satellite_State_(&state, 4) ==
                  BUILDING_BATTLESTATION);
    DOCTEST_CHECK(COMBINIT::Take_Captured_Satellite_State_(&state, 4) == -1);
}

DOCTEST_TEST_CASE("satellite capture requires an attacker-owned prize") {
    COMBINIT::_satellite_capture_enabled = 0;
    DOCTEST_CHECK_FALSE(COMBINIT::Satellite_Was_Captured_By_Attacker_(1, 1, 1));

    COMBINIT::_satellite_capture_enabled = 1;
    DOCTEST_CHECK(COMBINIT::Satellite_Was_Captured_By_Attacker_(1, 1, 1));
    DOCTEST_CHECK_FALSE(COMBINIT::Satellite_Was_Captured_By_Attacker_(0, 1, 1));
    DOCTEST_CHECK_FALSE(COMBINIT::Satellite_Was_Captured_By_Attacker_(1, 0, 1));

    COMBINIT::_satellite_capture_enabled = 0;
}

DOCTEST_TEST_CASE("black-hole boarding setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BOARDING::_board_in_black_hole == 1);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat board_in_black_hole = false;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BOARDING::_board_in_black_hole == 0);
}

DOCTEST_TEST_CASE("telepathic multiple capture setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BOARDING::_telepathic_multiple_capture == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat telepathic_multiple_capture = true;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BOARDING::_telepathic_multiple_capture == 1);
}

DOCTEST_TEST_CASE("excess marine capture setting preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BOARDING::_kill_excess_marines_after_capture == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat kill_excess_marines_after_capture = true;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(BOARDING::_kill_excess_marines_after_capture == 1);
}

DOCTEST_TEST_CASE("interactive combat stat settings preserve classic defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBAT1::_interactive_combat_stats == 0);
    DOCTEST_CHECK(COMBAT1::_continuous_bhg == 0);
    DOCTEST_CHECK(COMBAT1::_non_ics_tractor_and_bhg_mode == 0);
    DOCTEST_CHECK(COMBAT1::_recloak_next_turn == 0);
    DOCTEST_CHECK(COMBAT1::_rephase_cloak_next_turn == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat interactive_combat_stats = true;"
        "combat continuous_bhg = true;"
        "combat non_ics_tractor_and_bhg_mode = true;"
        "combat recloak_next_turn = true;"
        "combat rephase_cloak_next_turn = true;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBAT1::_interactive_combat_stats == 1);
    DOCTEST_CHECK(COMBAT1::_continuous_bhg == 1);
    DOCTEST_CHECK(COMBAT1::_non_ics_tractor_and_bhg_mode == 1);
    DOCTEST_CHECK(COMBAT1::_recloak_next_turn == 1);
    DOCTEST_CHECK(COMBAT1::_rephase_cloak_next_turn == 1);
}

DOCTEST_TEST_CASE("Phasing Cloak duration preserves the 1.50 default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_phasing_cloak_effective_turns == 10);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat_bonus phasing_cloak_effective_turns = 12;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(COMBINIT::_phasing_cloak_effective_turns == 12);
}

DOCTEST_TEST_CASE("combat namespace settings resolve bidirectionally and commit defaults") {
    const char* paths[] = {
        "combat_bonus.phasing_cloak_effective_turns",
        "combat.recloak_next_turn",
        "combat.rephase_cloak_next_turn",
        "combat.lightning_field_ratio",
        "combat.lightning_field_mode",
    };
    for (const char* expected : paths) {
        config::t_variable_id id = config::CONFIG_NO_VARIABLE;
        DOCTEST_REQUIRE(config::Find_Variable(expected, std::strlen(expected), &id));
        std::string resolved;
        DOCTEST_REQUIRE(config::Get_Variable_Name(id, &resolved));
        DOCTEST_CHECK(resolved == expected);
        DOCTEST_CHECK(config::Get_Variable_Type(id) != nullptr);
    }

    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(CMBTMIS::_lightning_field_ratio == 50);
    DOCTEST_CHECK(CMBTMIS::_lightning_field_mode == CMBTMIS::LIGHTNING_FIELD_MODE_CLASSIC);
}

DOCTEST_TEST_CASE("missile defense settings preserve classic defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(CMBTMIS::missile_defense.cloaking_device_missile_defense == 0);
    DOCTEST_CHECK(CMBTMIS::missile_defense.cloaking_device_missile_miss_percent == 50);
    DOCTEST_CHECK(CMBTMIS::missile_defense.displacement_device_missiles_miss_percent == 30);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "combat missile_defense cloaking_device_missile_defense = 60;"
        "combat missile_defense cloaking_device_missile_miss_percent = 45;"
        "combat missile_defense displacement_device_missiles_miss_percent = 35;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(CMBTMIS::missile_defense.cloaking_device_missile_defense == 60);
    DOCTEST_CHECK(CMBTMIS::missile_defense.cloaking_device_missile_miss_percent == 45);
    DOCTEST_CHECK(CMBTMIS::missile_defense.displacement_device_missiles_miss_percent == 35);
}

DOCTEST_TEST_CASE("alternative refit formula preserves the classic default") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(DESIGN::_alternative_refit == 0);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "design alternative_refit = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(DESIGN::_alternative_refit == 1);
}

DOCTEST_TEST_CASE("miniaturization curves and categories preserve 1.50 defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    const uint16_t expected_cost[DESIGN::MINIATURIZATION_LEVEL_COUNT] = {
        100, 75, 55, 40, 30, 25,
    };
    const uint16_t expected_space[DESIGN::MINIATURIZATION_CATEGORY_COUNT]
                                     [DESIGN::MINIATURIZATION_LEVEL_COUNT] = {
        { 1000, 800, 650, 500, 350, 250 },
        { 1000, 800, 700, 600, 500, 400 },
        { 1000, 1000, 1000, 1000, 1000, 1000 },
    };
    DOCTEST_CHECK(std::memcmp(DESIGN::miniaturization.cost, expected_cost,
                              sizeof(expected_cost)) == 0);
    DOCTEST_CHECK(std::memcmp(DESIGN::miniaturization.space, expected_space,
                              sizeof(expected_space)) == 0);

    int category_counts[DESIGN::MINIATURIZATION_CATEGORY_COUNT] = {};
    for (int tech = 0; tech < TECH_APP_COUNT; ++tech) {
        DOCTEST_REQUIRE(DESIGN::miniaturization_categories[tech] <
                        DESIGN::MINIATURIZATION_CATEGORY_COUNT);
        category_counts[DESIGN::miniaturization_categories[tech]]++;
    }
    DOCTEST_CHECK(category_counts[DESIGN::MINIATURIZATION_CATEGORY_GROUP_1] == 154);
    DOCTEST_CHECK(category_counts[DESIGN::MINIATURIZATION_CATEGORY_GROUP_2] == 49);
    DOCTEST_CHECK(category_counts[DESIGN::MINIATURIZATION_CATEGORY_GROUP_3] == 9);
    DOCTEST_CHECK(DESIGN::miniaturization_categories[TECH_APP_ANTI_MATTER_TORPEDOES] ==
                  DESIGN::MINIATURIZATION_CATEGORY_GROUP_2);
    DOCTEST_CHECK(DESIGN::miniaturization_categories[TECH_APP_REINFORCED_HULL] ==
                  DESIGN::MINIATURIZATION_CATEGORY_GROUP_3);
    DOCTEST_CHECK(DESIGN::Miniaturization_Cost_Multiplier_(2) == 55);
    DOCTEST_CHECK(DESIGN::Miniaturization_Cost_Multiplier_(6) == 25);
    DOCTEST_CHECK(DESIGN::Miniaturization_Space_Multiplier_(2,
                      DESIGN::MINIATURIZATION_CATEGORY_GROUP_1) == 650);
    DOCTEST_CHECK(DESIGN::Miniaturization_Space_Multiplier_(2,
                      DESIGN::MINIATURIZATION_CATEGORY_GROUP_2) == 700);
    DOCTEST_CHECK(DESIGN::Miniaturization_Space_Multiplier_(5,
                      DESIGN::MINIATURIZATION_CATEGORY_GROUP_3) == 1000);
}

DOCTEST_TEST_CASE("miniaturization curves and categories bind by normalized names") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "design miniaturization cost = 100 75 60 40 30 25;"
        "design miniaturization space group1 = 1000 800 675 500 350 250;"
        "design miniaturization space group3 = 1000 1000 900 800 700 600;"
        "design tech_mini anti_matter_torpedo = group1;"
        "design tech_mini reinforced_hull = group2;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(DESIGN::Miniaturization_Cost_Multiplier_(2) == 60);
    DOCTEST_CHECK(DESIGN::Miniaturization_Space_Multiplier_(2,
                      DESIGN::MINIATURIZATION_CATEGORY_GROUP_1) == 675);
    DOCTEST_CHECK(DESIGN::Miniaturization_Space_Multiplier_(5,
                      DESIGN::MINIATURIZATION_CATEGORY_GROUP_3) == 600);
    DOCTEST_CHECK(DESIGN::miniaturization_categories[TECH_APP_ANTI_MATTER_TORPEDOES] ==
                  DESIGN::MINIATURIZATION_CATEGORY_GROUP_1);
    DOCTEST_CHECK(DESIGN::miniaturization_categories[TECH_APP_REINFORCED_HULL] ==
                  DESIGN::MINIATURIZATION_CATEGORY_GROUP_2);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "design tech_mini reinforced_hull = group4;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("x3 missile racks preserve classic defaults and bind autodesign behavior") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    const int16_t classic[DESIGN::MISSILE_RACK_COUNT] = { 2, 5, 10, 15, 20 };
    for (int16_t i = 0; i < DESIGN::MISSILE_RACK_COUNT; ++i) {
        DOCTEST_CHECK(DESIGN::Missile_Rack_Quantity_(i) == classic[i]);
    }
    DOCTEST_CHECK_FALSE(DESIGN::Should_Autodesign_Use_X3_Rack_(5, 3, 3));

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "design missile_x3_shot = true;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    const int16_t x3[DESIGN::MISSILE_RACK_COUNT] = { 2, 3, 5, 10, 20 };
    for (int16_t i = 0; i < DESIGN::MISSILE_RACK_COUNT; ++i) {
        DOCTEST_CHECK(DESIGN::Missile_Rack_Quantity_(i) == x3[i]);
    }
    DOCTEST_CHECK(DESIGN::Missile_Rack_Size_(3) == 20);
    DOCTEST_CHECK(DESIGN::Missile_Rack_Size_(5) == 30);
    DOCTEST_CHECK(DESIGN::Missile_Rack_Size_(15) == 0);
    DOCTEST_CHECK(DESIGN::Should_Autodesign_Use_X3_Rack_(5, 3, 3));
    DOCTEST_CHECK(DESIGN::Should_Autodesign_Use_X3_Rack_(5, 2, 3));
    DOCTEST_CHECK_FALSE(DESIGN::Should_Autodesign_Use_X3_Rack_(5, 4, 3));
}

DOCTEST_TEST_CASE("maximum galaxy star count binds and validates its storage range") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(MAPGEN::_maximum_star_count == 99);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "map maximum_star_count = 100;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));
    DOCTEST_CHECK(MAPGEN::_maximum_star_count == 100);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "map maximum_star_count = 71;", &writes, &diagnostic));
    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "map maximum_star_count = 1024;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("strategic detection settings preserve and replace 1.50 defaults") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(SHIPSTAK::scan_bonus.battle_scanner == 2);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.scout_lab == 0);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.star_base == 2);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.battlestation == 4);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.star_fortress == 6);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.no_scanner == 2);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.tachyon_scanner == 4);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.neutron_scanner == 6);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.sensors == 9);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.planet == 32000);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.fleet == 32000);
    DOCTEST_CHECK(SHIPSTAK::stealth_bonus.stealthy_ships == 500);
    DOCTEST_CHECK(SHIPSTAK::stealth_bonus.stealth_field == 500);
    DOCTEST_CHECK(SHIPSTAK::stealth_bonus.cloaking_device == 500);
    DOCTEST_CHECK(SHIPSTAK::stealth_bonus.phasing_cloak == 500);
    DOCTEST_CHECK(SHIPSTAK::stealth_size_penalty ==
                  SHIPSTAK::e_stealth_size_penalty::race_tech);

    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "detection scan_bonus battle_scanner = 7;"
        "detection scan_bonus scout_lab = 3;"
        "detection scan_bonus sensors = 12;"
        "detection scan_bonus planet = 30000;"
        "detection stealth_bonus stealthy_ships = 450;"
        "detection stealth_bonus stealth_field = 650;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(SHIPSTAK::scan_bonus.battle_scanner == 7);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.scout_lab == 3);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.sensors == 12);
    DOCTEST_CHECK(SHIPSTAK::scan_bonus.planet == 30000);
    DOCTEST_CHECK(SHIPSTAK::stealth_bonus.stealthy_ships == 450);
    DOCTEST_CHECK(SHIPSTAK::stealth_bonus.stealth_field == 650);

    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "detection scan_bonus battle_scanner = 32768;",
        &writes, &diagnostic));
    writes.writes.clear();
    DOCTEST_CHECK_FALSE(Append_Decoded_(
        "detection scan_bonus sensors = 128;", &writes, &diagnostic));
}

DOCTEST_TEST_CASE("strategic stealth size-penalty modes bind by name") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));

    struct s_mode_case {
        const char* name;
        SHIPSTAK::e_stealth_size_penalty expected;
        bool racial_penalty_applies;
        bool device_penalty_applies;
    };
    const s_mode_case cases[] = {
        {"none", SHIPSTAK::e_stealth_size_penalty::none, true, true},
        {"race", SHIPSTAK::e_stealth_size_penalty::race, false, true},
        {"race_tech", SHIPSTAK::e_stealth_size_penalty::race_tech, false, false},
    };

    for (const s_mode_case& mode : cases) {
        const std::string source =
            std::string("detection stealth_size_penalty = ") + mode.name + ";";
        config::s_config_write_set writes;
        config::s_cfg_diagnostic diagnostic;
        DOCTEST_REQUIRE(Append_Decoded_(source.c_str(), &writes, &diagnostic));
        DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
        DOCTEST_REQUIRE(config::Commit_State(&state));
        DOCTEST_CHECK(SHIPSTAK::stealth_size_penalty == mode.expected);
        DOCTEST_CHECK(SHIPSTAK::Traveling_Size_Penalty_Applies_(true, false) ==
                      mode.racial_penalty_applies);
        DOCTEST_CHECK(SHIPSTAK::Traveling_Size_Penalty_Applies_(false, true) ==
                      mode.device_penalty_applies);
        DOCTEST_CHECK(SHIPSTAK::Traveling_Size_Penalty_Applies_(false, false));
    }
}

DOCTEST_TEST_CASE("shorter build queue assignments replace execution through an earlier end") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(Append_Decoded_(
        "colony build_queue 1 = prepend design1 design2 design3 end;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    writes.writes.clear();
    DOCTEST_REQUIRE(Append_Decoded_(
        "colony build_queue 1 = housing end;", &writes, &diagnostic));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&writes, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(BUILD_QUEUE::presets[1][0] == BUILD_QUEUE::INSTRUCTION_HOUSING);
    DOCTEST_CHECK(BUILD_QUEUE::presets[1][1] == BUILD_QUEUE::INSTRUCTION_END);
    DOCTEST_CHECK(BUILD_QUEUE::presets[1][2] == BUILD_QUEUE::INSTRUCTION_DESIGN2);
}

DOCTEST_TEST_CASE("shared build queue layouts validate every row terminator") {
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&state));
    config::t_variable_id first = config::CONFIG_NO_VARIABLE;
    DOCTEST_REQUIRE(config::Find_Variable(
        "colony.build_queue.10.0", std::strlen("colony.build_queue.10.0"), &first));
    for (int instruction = 0; instruction < BUILD_QUEUE::INSTRUCTION_CAPACITY;
         ++instruction) {
        state.values[first + instruction].integer_value = BUILD_QUEUE::INSTRUCTION_CLEAR;
    }
    DOCTEST_CHECK_FALSE(config::Commit_State(&state));
}

DOCTEST_TEST_SUITE("generated configuration bindings") {

DOCTEST_TEST_CASE("predefined ship templates have native defaults before configuration") {
    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.picture_num == 7);
    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.ship_weapon[2].type == 20);
    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.ship_weapon[2].specials == 0x4500);
    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.ship_weapon[2].count == 1);
    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.ship_weapon[2].current_count == 1);
    DOCTEST_CHECK((SHIP_CONFIG::guardian_template.special_device_flags[3] & 0x02) != 0);

    DOCTEST_CHECK(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].count == 5);
    DOCTEST_CHECK((SHIP_CONFIG::amoeba_templates[0].special_device_flags[4] & 0x80) != 0);

    DOCTEST_CHECK(SHIP_CONFIG::antaran_templates[GAME_DIFFICULTY_IMPOSSIBLE]
        [SHIP_SIZE_TITAN].ship_weapon[7].count == 12);
    DOCTEST_CHECK((SHIP_CONFIG::antaran_templates[GAME_DIFFICULTY_HARD]
        [SHIP_SIZE_LARGE].special_device_flags[2] & 0x20) != 0);
    DOCTEST_CHECK((SHIP_CONFIG::antaran_templates[GAME_DIFFICULTY_IMPOSSIBLE]
        [SHIP_SIZE_TITAN].special_device_flags[4] & 0x40) != 0);
}

DOCTEST_TEST_CASE("schema defaults restore complete predefined ship templates") {
    const s_ship_design guardian = SHIP_CONFIG::guardian_template;
    const s_ship_design avenger = SHIP_CONFIG::avenger_template;
    s_ship_design amoebas[2];
    s_ship_design crystals[2];
    s_ship_design dragons[2];
    s_ship_design eels[2];
    s_ship_design hydras[2];
    s_ship_design antarans[GAME_DIFFICULTY_COUNT][5];
    std::memcpy(amoebas, SHIP_CONFIG::amoeba_templates, sizeof(amoebas));
    std::memcpy(crystals, SHIP_CONFIG::crystal_templates, sizeof(crystals));
    std::memcpy(dragons, SHIP_CONFIG::dragon_templates, sizeof(dragons));
    std::memcpy(eels, SHIP_CONFIG::eel_templates, sizeof(eels));
    std::memcpy(hydras, SHIP_CONFIG::hydra_templates, sizeof(hydras));
    std::memcpy(antarans, SHIP_CONFIG::antaran_templates, sizeof(antarans));

    config::s_config_state defaults;
    DOCTEST_REQUIRE(config::Initialize_Defaults(&defaults));
    DOCTEST_REQUIRE(config::Commit_State(&defaults));

    DOCTEST_CHECK(std::memcmp(&guardian, &SHIP_CONFIG::guardian_template,
                              sizeof(guardian)) == 0);
    DOCTEST_CHECK(std::memcmp(&avenger, &SHIP_CONFIG::avenger_template,
                              sizeof(avenger)) == 0);
    DOCTEST_CHECK(std::memcmp(amoebas, SHIP_CONFIG::amoeba_templates,
                              sizeof(amoebas)) == 0);
    DOCTEST_CHECK(std::memcmp(crystals, SHIP_CONFIG::crystal_templates,
                              sizeof(crystals)) == 0);
    DOCTEST_CHECK(std::memcmp(dragons, SHIP_CONFIG::dragon_templates,
                              sizeof(dragons)) == 0);
    DOCTEST_CHECK(std::memcmp(eels, SHIP_CONFIG::eel_templates,
                              sizeof(eels)) == 0);
    DOCTEST_CHECK(std::memcmp(hydras, SHIP_CONFIG::hydra_templates,
                              sizeof(hydras)) == 0);
    DOCTEST_CHECK(std::memcmp(antarans, SHIP_CONFIG::antaran_templates,
                              sizeof(antarans)) == 0);

    SHIP_CONFIG::guardian_template.ship_weapon[0].count = 99;
    SHIP_CONFIG::guardian_template.special_device_flags[4] = 0;
    SHIP_CONFIG::antaran_templates[GAME_DIFFICULTY_IMPOSSIBLE]
        [SHIP_SIZE_TITAN].ship_weapon[7].current_count = 0;

    DOCTEST_REQUIRE(config::Initialize_Defaults(&defaults));
    DOCTEST_REQUIRE(config::Commit_State(&defaults));

    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.ship_weapon[0].count == 2);
    DOCTEST_CHECK(SHIP_CONFIG::guardian_template.ship_weapon[0].current_count == 2);
    DOCTEST_CHECK((SHIP_CONFIG::guardian_template.special_device_flags[3] & 0x02) != 0);
    DOCTEST_CHECK(SHIP_CONFIG::antaran_templates[GAME_DIFFICULTY_IMPOSSIBLE]
        [SHIP_SIZE_TITAN].ship_weapon[7].current_count == 12);
}

DOCTEST_TEST_CASE("logical defaults initialize typed candidate state") {
    config::s_config_state state;

    config::Initialize_Defaults(&state);

    DOCTEST_REQUIRE(state.values.size() == config::variable_count);
    DOCTEST_CHECK(state.values[0].bool_value);
    DOCTEST_CHECK(state.values[1].integer_value == 50);
    DOCTEST_CHECK(state.values[2].bool_value);
    DOCTEST_CHECK(state.values[3].integer_value == 50);
}

DOCTEST_TEST_CASE("native state conversion normalizes legacy byte flags") {
    MOX::_settings.music_on = 7;
    MOX::_settings.music_level = 42;
    MOX::_settings.sound_fx_on = 0;
    MOX::_settings.sound_fx_level = 63;

    config::s_config_state state;
    config::Read_Current_State(&state);

    DOCTEST_CHECK(state.values[0].bool_value);
    DOCTEST_CHECK(state.values[1].integer_value == 42);
    DOCTEST_CHECK_FALSE(state.values[2].bool_value);
    DOCTEST_CHECK(state.values[3].integer_value == 63);
}

DOCTEST_TEST_CASE("write sets apply to temporary state before explicit commit") {
    MOX::_settings.music_on = 1;
    MOX::_settings.music_level = 50;
    MOX::_settings.sound_fx_on = 1;
    MOX::_settings.sound_fx_level = 50;

    config::s_config_write_set write_set;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "audio music_enabled = false; audio music_volume = 75;"
        "audio sound_effects_volume = 25;",
        &write_set, &diagnostic));

    config::s_config_state base_state;
    config::Read_Current_State(&base_state);
    config::s_config_state candidate_state = base_state;
    DOCTEST_REQUIRE(config::Apply_Write_Set(&write_set, &candidate_state, &diagnostic));

    DOCTEST_CHECK_FALSE(candidate_state.values[0].bool_value);
    DOCTEST_CHECK(candidate_state.values[1].integer_value == 75);
    DOCTEST_CHECK(candidate_state.values[2].bool_value);
    DOCTEST_CHECK(candidate_state.values[3].integer_value == 25);
    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 50);

    DOCTEST_REQUIRE(config::Commit_State(&candidate_state));
    DOCTEST_CHECK(MOX::_settings.music_on == 0);
    DOCTEST_CHECK(MOX::_settings.music_level == 75);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 1);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 25);
}

DOCTEST_TEST_CASE("transaction commits complete write sets and later writes win") {
    MOX::_settings.music_on = 1;
    MOX::_settings.music_level = 50;
    MOX::_settings.sound_fx_on = 0;
    MOX::_settings.sound_fx_level = 20;

    config::s_config_write_set write_set;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "audio music_volume = 10; audio music_volume = 90;"
        "audio sound_effects_enabled = true;",
        &write_set, &diagnostic));
    config::s_config_state state;
    DOCTEST_REQUIRE(config::Read_Current_State(&state));
    DOCTEST_REQUIRE(config::Apply_Write_Set(&write_set, &state, &diagnostic));
    DOCTEST_REQUIRE(config::Commit_State(&state));

    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 90);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 1);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 20);
}

DOCTEST_TEST_CASE("invalid typed writes roll back candidate and native state") {
    MOX::_settings.music_on = 1;
    MOX::_settings.music_level = 40;
    MOX::_settings.sound_fx_on = 1;
    MOX::_settings.sound_fx_level = 60;

    config::s_config_state base_state;
    config::Read_Current_State(&base_state);
    config::s_config_state candidate_state;
    config::Initialize_Defaults(&candidate_state);
    candidate_state.values[0].bool_value = false;
    candidate_state.values[1].integer_value = 1;
    candidate_state.values[2].bool_value = false;
    candidate_state.values[3].integer_value = 2;
    const config::s_config_state original_candidate = candidate_state;

    config::s_config_write_set write_set;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "audio music_volume = 80;", &write_set, &diagnostic));
    write_set.writes.push_back(Invalid_Write_(
        3, config::CONFIG_VALUE_INT8, 101));

    DOCTEST_CHECK_FALSE(config::Apply_Write_Set(
        &write_set, &candidate_state, &diagnostic));
    DOCTEST_CHECK(States_Equal_(candidate_state, original_candidate));
    DOCTEST_CHECK(diagnostic.message ==
                  "typed write range error for audio.sound_effects_volume");
    DOCTEST_CHECK(diagnostic.location.source == "forged.cfg");

    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 40);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 1);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 60);

    write_set.writes.back() = Invalid_Write_(
        3, config::CONFIG_VALUE_BOOL, 0);
    DOCTEST_CHECK_FALSE(config::Apply_Write_Set(
        &write_set, &candidate_state, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "typed write mismatch for audio.sound_effects_volume");
    DOCTEST_CHECK(MOX::_settings.music_level == 40);
}

}