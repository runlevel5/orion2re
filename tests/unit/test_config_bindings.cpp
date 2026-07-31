#include "pch.h"

#include <cstring>

#include "config/cfg_parser.h"
#include "config/config_decode.h"
#include "config/generated/config_bindings.h"
#include "config/generated/config_schema.h"
#include "doctest/doctest.h"

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

    bool States_Equal_(const config::generated::s_config_state& left,
                       const config::generated::s_config_state& right) {
        return left.music_enabled == right.music_enabled &&
               left.music_volume == right.music_volume &&
               left.sound_effects_enabled == right.sound_effects_enabled &&
               left.sound_effects_volume == right.sound_effects_volume;
    }

    config::s_config_write Invalid_Write_(config::generated::e_parameter_id parameter_id,
                                          config::generated::e_value_type type,
                                          int64_t integer_value) {
        config::s_config_write write;
        write.parameter_id = parameter_id;
        write.value.type = type;
        write.value.integer_value = integer_value;
        write.provenance.source = "forged.cfg";
        write.provenance.line = 7;
        write.provenance.column = 9;
        return write;
    }
}

DOCTEST_TEST_SUITE("generated configuration bindings") {

DOCTEST_TEST_CASE("logical defaults initialize typed candidate state") {
    config::generated::s_config_state state;
    state.music_enabled = false;
    state.music_volume = 0;
    state.sound_effects_enabled = false;
    state.sound_effects_volume = 0;

    config::generated::Initialize_Defaults(&state);

    DOCTEST_CHECK(state.music_enabled);
    DOCTEST_CHECK(state.music_volume == 50);
    DOCTEST_CHECK(state.sound_effects_enabled);
    DOCTEST_CHECK(state.sound_effects_volume == 50);
}

DOCTEST_TEST_CASE("native state conversion normalizes legacy byte flags") {
    MOX::_settings.music_on = 7;
    MOX::_settings.music_level = 42;
    MOX::_settings.sound_fx_on = 0;
    MOX::_settings.sound_fx_level = 63;

    config::generated::s_config_state state;
    config::generated::Read_Native_State(&state);

    DOCTEST_CHECK(state.music_enabled);
    DOCTEST_CHECK(state.music_volume == 42);
    DOCTEST_CHECK_FALSE(state.sound_effects_enabled);
    DOCTEST_CHECK(state.sound_effects_volume == 63);
}

DOCTEST_TEST_CASE("write sets apply to temporary state before explicit commit") {
    MOX::_settings.music_on = 1;
    MOX::_settings.music_level = 50;
    MOX::_settings.sound_fx_on = 1;
    MOX::_settings.sound_fx_level = 50;

    config::s_config_write_set write_set;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_(
        "music_enabled = false; music_volume = 75; sound_effects_volume = 25;",
        &write_set, &diagnostic));

    config::generated::s_config_state base_state;
    config::generated::Read_Native_State(&base_state);
    config::generated::s_config_state candidate_state;
    config::generated::Initialize_Defaults(&candidate_state);
    DOCTEST_REQUIRE(config::generated::Apply_Write_Set(
        &write_set, &base_state, &candidate_state, &diagnostic));

    DOCTEST_CHECK_FALSE(candidate_state.music_enabled);
    DOCTEST_CHECK(candidate_state.music_volume == 75);
    DOCTEST_CHECK(candidate_state.sound_effects_enabled);
    DOCTEST_CHECK(candidate_state.sound_effects_volume == 25);
    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 50);

    config::generated::Commit_State(&candidate_state);
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
        "music_volume = 10; music_volume = 90; sound_effects_enabled = true;",
        &write_set, &diagnostic));
    DOCTEST_REQUIRE(config::generated::Apply_Transaction(&write_set, &diagnostic));

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

    config::generated::s_config_state base_state;
    config::generated::Read_Native_State(&base_state);
    config::generated::s_config_state candidate_state;
    candidate_state.music_enabled = false;
    candidate_state.music_volume = 1;
    candidate_state.sound_effects_enabled = false;
    candidate_state.sound_effects_volume = 2;
    const config::generated::s_config_state original_candidate = candidate_state;

    config::s_config_write_set write_set;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Append_Decoded_("music_volume = 80;", &write_set, &diagnostic));
    write_set.writes.push_back(Invalid_Write_(
        config::generated::CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME,
        config::generated::CONFIG_VALUE_INT8, 101));

    DOCTEST_CHECK_FALSE(config::generated::Apply_Write_Set(
        &write_set, &base_state, &candidate_state, &diagnostic));
    DOCTEST_CHECK(States_Equal_(candidate_state, original_candidate));
    DOCTEST_CHECK(diagnostic.message ==
                  "typed write range error for sound_effects_volume");
    DOCTEST_CHECK(diagnostic.location.source == "forged.cfg");

    DOCTEST_CHECK_FALSE(config::generated::Apply_Transaction(&write_set, &diagnostic));
    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 40);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 1);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 60);

    write_set.writes.back() = Invalid_Write_(
        config::generated::CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME,
        config::generated::CONFIG_VALUE_BOOL, 0);
    DOCTEST_CHECK_FALSE(config::generated::Apply_Transaction(&write_set, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "typed write mismatch for sound_effects_volume");
    DOCTEST_CHECK(MOX::_settings.music_level == 40);
}

}