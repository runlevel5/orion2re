#include "pch.h"

#include <cstring>

#include "config/cfg_parser.h"
#include "config/config_decode.h"
#include "config/generated/config_schema.h"
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
        write.parameter_id = config::generated::CONFIG_PARAMETER_MUSIC_VOLUME;
        write.canonical_path = "sentinel";
        write_set->writes.push_back(std::move(write));
    }
}

DOCTEST_TEST_SUITE("configuration assignment decode") {

DOCTEST_TEST_CASE("generated lookup resolves exact canonical paths") {
    const config::generated::s_parameter_descriptor* music =
        config::generated::Find_Parameter("music_volume", std::strlen("music_volume"));
    DOCTEST_REQUIRE(music != nullptr);
    DOCTEST_CHECK(music->id == config::generated::CONFIG_PARAMETER_MUSIC_VOLUME);
    DOCTEST_CHECK(music->type == config::generated::CONFIG_VALUE_INT8);
    DOCTEST_CHECK(music->min == 0);
    DOCTEST_CHECK(music->max == 100);
    DOCTEST_CHECK(config::generated::Find_Parameter("music", 5) == nullptr);
    DOCTEST_CHECK(config::generated::Find_Parameter("MUSIC_VOLUME", 12) == nullptr);
    DOCTEST_CHECK(config::generated::Find_Parameter(nullptr, 0) == nullptr);
}

DOCTEST_TEST_CASE("bool and integer assignments produce typed atomic writes") {
    struct s_integer_case {
        const char* source;
        config::generated::e_parameter_id parameter_id;
        int64_t expected;
    };
    const s_integer_case integers[] = {
        {"music_volume = 75;", config::generated::CONFIG_PARAMETER_MUSIC_VOLUME, 75},
        {"music_volume = 0x32;", config::generated::CONFIG_PARAMETER_MUSIC_VOLUME, 50},
        {"sound_effects_volume = 0b110010;",
         config::generated::CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME, 50},
        {"sound_effects_volume = 25;",
         config::generated::CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME, 25},
    };

    for (const s_integer_case& test_case : integers) {
        config::s_config_write_set write_set;
        config::s_cfg_diagnostic diagnostic;
        DOCTEST_REQUIRE(Decode_(test_case.source, &write_set, &diagnostic));
        DOCTEST_REQUIRE(write_set.writes.size() == 1);
        DOCTEST_CHECK(write_set.writes[0].parameter_id == test_case.parameter_id);
        DOCTEST_CHECK(write_set.writes[0].value.type == config::generated::CONFIG_VALUE_INT8);
        DOCTEST_CHECK(write_set.writes[0].value.integer_value == test_case.expected);
        DOCTEST_CHECK(write_set.writes[0].provenance.source == "decode.cfg");
        DOCTEST_CHECK(write_set.writes[0].provenance.column > 1);
    }

    config::s_config_write_set enabled;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Decode_("music_enabled = true;", &enabled, &diagnostic));
    DOCTEST_REQUIRE(enabled.writes.size() == 1);
    DOCTEST_CHECK(enabled.writes[0].value.type == config::generated::CONFIG_VALUE_BOOL);
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
        {"unknown_setting = 1;", "unknown configuration parameter 'unknown_setting'", 1},
        {"music_volume extra = 50;",
         "parameter 'music_volume' is a scalar and cannot be traversed", 14},
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