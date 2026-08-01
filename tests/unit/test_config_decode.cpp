#include "pch.h"

#include <cstring>

#include "config/cfg_parser.h"
#include "config/config_decode.h"
#include "config/config_runtime.h"
#include "doctest/doctest.h"

namespace {
    bool Parse_Assignment_(const char* source, config::s_cfg_statement* statement) {
        config::s_cfg_document document;
        config::s_cfg_diagnostic diagnostic;
        if (!config::Parse_Cfg("decode.cfg", source, std::strlen(source),
                               &document, &diagnostic) || document.statements.size() != 1) {
            return false;
        }
        *statement = std::move(document.statements.front());
        return true;
    }

    bool Decode_(const char* source, config::s_config_write_set* write_set,
                 config::s_cfg_diagnostic* diagnostic) {
        config::s_cfg_statement statement;
        if (!Parse_Assignment_(source, &statement)) {
            return false;
        }
        return config::Decode_Assignment(&statement, write_set, diagnostic);
    }

    void Add_Sentinel_(config::s_config_write_set* write_set) {
        config::s_config_write write;
        write.variable_id = 1;
        write.canonical_path = "sentinel";
        write_set->writes.push_back(std::move(write));
    }
}

DOCTEST_TEST_SUITE("configuration assignment decode") {

DOCTEST_TEST_CASE("stable names resolve to incremental runtime ids") {
    config::t_variable_id music_id = 99;
    DOCTEST_REQUIRE(config::Find_Variable(
        "music_volume", std::strlen("music_volume"), &music_id));
    DOCTEST_CHECK(music_id == 1);
    const config::s_variable_descriptor* music = config::Get_Variable(music_id);
    DOCTEST_REQUIRE(music != nullptr);
    DOCTEST_CHECK(music->type == config::CONFIG_VALUE_INT8);
    DOCTEST_CHECK(music->min == 0);
    DOCTEST_CHECK(music->max == 100);
    DOCTEST_CHECK(std::strcmp(config::Get_Variable_Name(music_id), "music_volume") == 0);
    DOCTEST_CHECK_FALSE(config::Find_Variable("music", 5, &music_id));
    DOCTEST_CHECK_FALSE(config::Find_Variable("MUSIC_VOLUME", 12, &music_id));
    DOCTEST_CHECK_FALSE(config::Find_Variable(nullptr, 0, &music_id));
    DOCTEST_CHECK(config::Get_Variable(config::variable_count) == nullptr);
}

DOCTEST_TEST_CASE("stable path writes resolve for replay in the current build") {
    config::s_config_write write;
    write.variable_id = 99;
    write.canonical_path = "sound_effects_volume";
    write.value.type = config::CONFIG_VALUE_INT8;
    write.value.integer_value = 35;
    write.provenance.source = "savegame";
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Resolve_Write_Path(&write, &diagnostic));
    DOCTEST_CHECK(write.variable_id == 3);

    write.canonical_path = "weapons.foo.bar.1.baz";
    DOCTEST_CHECK_FALSE(config::Resolve_Write_Path(&write, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "unknown configuration variable 'weapons.foo.bar.1.baz'");
    DOCTEST_CHECK(diagnostic.location.source == "savegame");
}

DOCTEST_TEST_CASE("bool and integer assignments produce typed atomic writes") {
    struct s_integer_case {
        const char* source;
        config::t_variable_id variable_id;
        int64_t expected;
    };
    const s_integer_case integers[] = {
        {"music_volume = 75;", 1, 75},
        {"music_volume = 0x32;", 1, 50},
        {"sound_effects_volume = 0b110010;", 3, 50},
        {"sound_effects_volume = 25;", 3, 25},
    };

    for (const s_integer_case& test_case : integers) {
        config::s_config_write_set write_set;
        config::s_cfg_diagnostic diagnostic;
        DOCTEST_REQUIRE(Decode_(test_case.source, &write_set, &diagnostic));
        DOCTEST_REQUIRE(write_set.writes.size() == 1);
        DOCTEST_CHECK(write_set.writes[0].variable_id == test_case.variable_id);
        DOCTEST_CHECK(write_set.writes[0].value.type == config::CONFIG_VALUE_INT8);
        DOCTEST_CHECK(write_set.writes[0].value.integer_value == test_case.expected);
        DOCTEST_CHECK(write_set.writes[0].provenance.source == "decode.cfg");
        DOCTEST_CHECK(write_set.writes[0].provenance.column > 1);
    }

    config::s_config_write_set enabled;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Decode_("music_enabled = true;", &enabled, &diagnostic));
    DOCTEST_REQUIRE(enabled.writes.size() == 1);
    DOCTEST_CHECK(enabled.writes[0].value.type == config::CONFIG_VALUE_BOOL);
    DOCTEST_CHECK(enabled.writes[0].value.bool_value);
    DOCTEST_CHECK(enabled.writes[0].canonical_path == "music_enabled");

    config::s_config_write_set disabled;
    DOCTEST_REQUIRE(Decode_("sound_effects_enabled = false;", &disabled, &diagnostic));
    DOCTEST_REQUIRE(disabled.writes.size() == 1);
    DOCTEST_CHECK_FALSE(disabled.writes[0].value.bool_value);
}

DOCTEST_TEST_CASE("resolution and decode failures retain no partial writes") {
    struct s_case {
        const char* source;
        const char* expected_message;
        uint32_t column;
    };
    const s_case cases[] = {
        {"unknown_setting = 1;", "unknown configuration variable 'unknown_setting'", 1},
        {"music_volume extra = 50;",
         "unknown configuration variable 'music_volume.extra'", 1},
        {"music_volume = 101;",
         "invalid value '101' for 'music_volume'; expected int8 in range 0..100", 16},
        {"music_volume = -1;",
         "invalid value '-1' for 'music_volume'; expected int8 in range 0..100", 16},
        {"music_volume = true;",
         "invalid value 'true' for 'music_volume'; expected int8", 16},
        {"music_enabled = 1;",
         "invalid value '1' for 'music_enabled'; expected bool", 17},
        {"music_enabled = TRUE;",
         "invalid value 'TRUE' for 'music_enabled'; expected bool", 17},
        {"music_volume = -;",
         "an assignment containing only skips has no effect", 16},
        {"music_volume = 25 50;",
         "value 2 exceeds scalar parameter 'music_volume'", 19},
    };

    for (const s_case& test_case : cases) {
        config::s_config_write_set write_set;
        config::s_cfg_diagnostic diagnostic;
        Add_Sentinel_(&write_set);
        DOCTEST_CHECK_FALSE(Decode_(test_case.source, &write_set, &diagnostic));
        DOCTEST_CHECK_MESSAGE(diagnostic.message == test_case.expected_message,
                              test_case.source, ": ", diagnostic.message);
        DOCTEST_CHECK(diagnostic.location.source == "decode.cfg");
        DOCTEST_CHECK(diagnostic.location.line == 1);
        DOCTEST_CHECK(diagnostic.location.column == test_case.column);
        DOCTEST_CHECK(write_set.writes.empty());
    }
}

DOCTEST_TEST_CASE("non-assignment statements are rejected atomically") {
    config::s_cfg_document document;
    config::s_cfg_diagnostic parse_diagnostic;
    DOCTEST_REQUIRE(config::Parse_Cfg("decode.cfg", "stop;", 5,
                                     &document, &parse_diagnostic));
    DOCTEST_REQUIRE(document.statements.size() == 1);

    config::s_config_write_set write_set;
    Add_Sentinel_(&write_set);
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_CHECK_FALSE(config::Decode_Assignment(&document.statements[0],
                                                  &write_set, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "expected an assignment statement");
    DOCTEST_CHECK(write_set.writes.empty());
}

}