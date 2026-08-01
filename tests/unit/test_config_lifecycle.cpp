#include "pch.h"

#include <cstring>

#include "config/cfg_parser.h"
#include "config/config_lifecycle.h"
#include "doctest/doctest.h"

namespace {
    bool Parse_(const char* source, config::s_cfg_document* document,
                config::s_cfg_diagnostic* diagnostic) {
        return config::Parse_Cfg("lifecycle.cfg", source, std::strlen(source),
                                 document, diagnostic);
    }

    bool Apply_(const char* source, const char* package_id,
                config::s_effective_config* effective,
                config::s_cfg_diagnostic* diagnostic) {
        config::s_cfg_document document;
        return Parse_(source, &document, diagnostic) &&
               config::Apply_Config_Document(&document, package_id, effective, diagnostic);
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
                left.values[index].string_value != right.values[index].string_value) {
                return false;
            }
        }
        return true;
    }
}

DOCTEST_TEST_SUITE("configuration lifecycle") {

DOCTEST_TEST_CASE("effective state initializes from defaults or current native values") {
    config::s_effective_config defaults;
    config::Initialize_Effective_Config(&defaults);
    DOCTEST_CHECK(defaults.state.values[0].bool_value);
    DOCTEST_CHECK(defaults.state.values[1].integer_value == 50);
    DOCTEST_CHECK(defaults.state.values[2].bool_value);
    DOCTEST_CHECK(defaults.state.values[3].integer_value == 50);
    DOCTEST_CHECK(defaults.provenance.size() == config::variable_count);
    for (const config::s_value_provenance& provenance : defaults.provenance) {
        DOCTEST_CHECK_FALSE(provenance.has_override);
    }

    MOX::_settings.music_on = 0;
    MOX::_settings.music_level = 34;
    MOX::_settings.sound_fx_on = 1;
    MOX::_settings.sound_fx_level = 67;
    config::s_effective_config native;
    config::Read_Effective_Native_Config(&native);
    DOCTEST_CHECK_FALSE(native.state.values[0].bool_value);
    DOCTEST_CHECK(native.state.values[1].integer_value == 34);
    DOCTEST_CHECK(native.state.values[2].bool_value);
    DOCTEST_CHECK(native.state.values[3].integer_value == 67);
}

DOCTEST_TEST_CASE("complete documents commit state and per-value provenance atomically") {
    MOX::_settings.music_on = 1;
    MOX::_settings.music_level = 50;
    MOX::_settings.sound_fx_on = 1;
    MOX::_settings.sound_fx_level = 50;
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(Apply_(
        "music_enabled = false;\n"
        "music_volume = 75;\n"
        "sound_effects_volume = 25;\n",
        "base_game", &effective, &diagnostic));

    DOCTEST_CHECK_FALSE(effective.state.values[0].bool_value);
    DOCTEST_CHECK(effective.state.values[1].integer_value == 75);
    DOCTEST_CHECK(effective.state.values[3].integer_value == 25);
    DOCTEST_CHECK(MOX::_settings.music_on == 0);
    DOCTEST_CHECK(MOX::_settings.music_level == 75);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 25);

    config::t_variable_id music_index = 0;
    DOCTEST_REQUIRE(config::Find_Variable(
        "music_volume", std::strlen("music_volume"), &music_index));
    DOCTEST_CHECK(effective.provenance[music_index].has_override);
    DOCTEST_CHECK(effective.provenance[music_index].package_id == "base_game");
    DOCTEST_CHECK(effective.provenance[music_index].location.source == "lifecycle.cfg");
    DOCTEST_CHECK(effective.provenance[music_index].location.line == 2);
    DOCTEST_CHECK(effective.provenance[music_index].statement == 2);
    DOCTEST_REQUIRE(effective.prior_values[music_index].size() == 1);
    DOCTEST_CHECK(effective.prior_values[music_index][0].value.integer_value == 50);
    DOCTEST_CHECK_FALSE(effective.prior_values[music_index][0].provenance.has_override);

    DOCTEST_REQUIRE(Apply_("music_volume = 90;", "balance_mod",
                           &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[music_index].integer_value == 90);
    DOCTEST_CHECK(effective.provenance[music_index].package_id == "balance_mod");
    DOCTEST_CHECK(effective.provenance[music_index].statement == 1);
    DOCTEST_REQUIRE(effective.prior_values[music_index].size() == 2);
    DOCTEST_CHECK(effective.prior_values[music_index][1].value.integer_value == 75);
    DOCTEST_CHECK(effective.prior_values[music_index][1].provenance.package_id == "base_game");
}

DOCTEST_TEST_CASE("document failure rolls back earlier writes provenance and native state") {
    MOX::_settings.music_on = 1;
    MOX::_settings.music_level = 40;
    MOX::_settings.sound_fx_on = 1;
    MOX::_settings.sound_fx_level = 60;
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Apply_("music_enabled = false;", "first",
                           &effective, &diagnostic));
    const config::s_config_state prior_state = effective.state;
    const std::vector<config::s_value_provenance> prior_provenance = effective.provenance;
    const std::vector<std::vector<config::s_prior_value>> prior_values =
        effective.prior_values;

    DOCTEST_CHECK_FALSE(Apply_(
        "music_volume = 80;\n"
        "sound_effects_volume = 101;\n",
        "broken", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "invalid value '101' for 'sound_effects_volume'; expected int8 in range 0..100");
    DOCTEST_CHECK(States_Equal_(effective.state, prior_state));
    DOCTEST_CHECK(effective.provenance.size() == prior_provenance.size());
    DOCTEST_CHECK(effective.prior_values.size() == prior_values.size());
    for (size_t index = 0; index < prior_provenance.size(); ++index) {
        DOCTEST_CHECK(effective.provenance[index].has_override ==
                      prior_provenance[index].has_override);
        DOCTEST_CHECK(effective.provenance[index].package_id ==
                      prior_provenance[index].package_id);
        DOCTEST_CHECK(effective.prior_values[index].size() == prior_values[index].size());
    }
    DOCTEST_CHECK(MOX::_settings.music_on == 0);
    DOCTEST_CHECK(MOX::_settings.music_level == 40);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 60);
}

DOCTEST_TEST_CASE("stop terminates the current document before later statements") {
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(Apply_(
        "music_volume = 70; stop; music_volume = 99;",
        "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 70);
    DOCTEST_CHECK(MOX::_settings.music_level == 70);
}

DOCTEST_TEST_CASE("include and enable remain loader operations and roll back the document") {
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(Apply_(
        "music_volume = 70; include extra.cfg;",
        "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "include execution requires the configuration loader");
    DOCTEST_CHECK(effective.state.values[1].integer_value == 50);

    DOCTEST_CHECK_FALSE(Apply_(
        "music_volume = 70; enable balance_mod;",
        "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "enable execution requires the mod loader");
    DOCTEST_CHECK(effective.state.values[1].integer_value == 50);
}

DOCTEST_TEST_CASE("canonical extraction follows schema order and round trips") {
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Apply_(
        "sound_effects_volume = 12; music_enabled = false; music_volume = 88;",
        "balance_mod", &effective, &diagnostic));

    std::string canonical;
    DOCTEST_REQUIRE(config::Extract_Canonical_Config(
        &effective, false, &canonical, &diagnostic));
    DOCTEST_CHECK(canonical ==
        "music_enabled = false;\n"
        "music_volume = 88;\n"
        "sound_effects_enabled = true;\n"
        "sound_effects_volume = 12;\n");

    config::s_effective_config round_trip;
    config::Initialize_Effective_Config(&round_trip);
    DOCTEST_REQUIRE(Apply_(canonical.c_str(), "extracted",
                           &round_trip, &diagnostic));
    DOCTEST_CHECK(States_Equal_(round_trip.state, effective.state));

    std::string with_provenance;
    DOCTEST_REQUIRE(config::Extract_Canonical_Config(
        &effective, true, &with_provenance, &diagnostic));
    DOCTEST_CHECK(with_provenance.find(
        "# from balance_mod: lifecycle.cfg:1\nmusic_enabled = false;") !=
        std::string::npos);
}

}