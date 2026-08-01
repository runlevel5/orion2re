#include "pch.h"

#include <cstring>

#include "config/cfg_parser.h"
#include "config/config_decode.h"
#include "config/config_runtime.h"
#include "doctest/doctest.h"

namespace {
    const config::s_symbol_descriptor WEAPON_VALUES[] = {
        {"none", 0},
        {"plasma_cannon", 11},
        {"mass_driver", 4},
    };
    const config::s_symbol_descriptor ARC_VALUES[] = {
        {"none", 0},
        {"all", 15},
    };
    const config::s_symbol_descriptor ARC_FLAGS[] = {
        {"forward", 1},
        {"aft", 2},
        {"left", 4},
        {"right", 8},
    };
    const config::s_symbol_descriptor MOD_VALUES[] = {
        {"none", 0},
    };
    const config::s_symbol_descriptor MOD_FLAGS[] = {
        {"heavy", 2},
        {"eccm", 0x200},
    };
    const config::s_symbol_descriptor SPECIAL_VALUES[] = {
        {"none", 0},
        {"augmented_engines", 1},
        {"battle_scanner", 5},
        {"damper_field", 7},
    };
    const config::s_array_index_alias_descriptor ARRAY_INDEX_ALIASES[] = {
        {21, "plasma_cannon", 11},
    };

    config::s_variable_descriptor Variable_(const char* name,
                                            config::e_value_type type) {
        config::s_variable_descriptor variable = {};
        variable.name = name;
        variable.title = name;
        variable.description = "fixture";
        variable.remarks = "";
        variable.units = "";
        variable.group = "fixture";
        variable.type = type;
        variable.scalar_kind = config::CONFIG_SCALAR_PRIMITIVE;
        variable.ratio_component_type = config::CONFIG_VALUE_INT16;
        return variable;
    }

    const config::s_runtime_schema* Aggregate_Schema_() {
        static config::s_variable_descriptor variables[17];
        static bool initialized = false;
        if (!initialized) {
            const char* names[] = {
                "guardian.appearance",
                "guardian.weapon.1.type", "guardian.weapon.1.arc",
                "guardian.weapon.1.mods", "guardian.weapon.1.ammo",
                "guardian.weapon.1.amount", "guardian.weapon.2.type",
                "guardian.weapon.2.arc", "guardian.weapon.2.mods",
                "guardian.weapon.2.ammo", "guardian.weapon.2.amount",
                "guardian.special.1", "guardian.special.2", "guardian.special.3",
                "guardian.label", "guardian.reduction",
                "weapon.11.available_mods",
            };
            const config::e_value_type types[] = {
                config::CONFIG_VALUE_UINT8,
                config::CONFIG_VALUE_UINT8, config::CONFIG_VALUE_UINT8,
                config::CONFIG_VALUE_UINT16, config::CONFIG_VALUE_UINT8,
                config::CONFIG_VALUE_UINT8, config::CONFIG_VALUE_UINT8,
                config::CONFIG_VALUE_UINT8, config::CONFIG_VALUE_UINT16,
                config::CONFIG_VALUE_UINT8, config::CONFIG_VALUE_UINT8,
                config::CONFIG_VALUE_UINT8, config::CONFIG_VALUE_UINT8,
                config::CONFIG_VALUE_UINT8, config::CONFIG_VALUE_STRING,
                config::CONFIG_VALUE_RATIO,
                config::CONFIG_VALUE_UINT16,
            };
            for (size_t index = 0; index < 17; ++index) {
                variables[index] = Variable_(names[index], types[index]);
            }
            for (size_t index : {size_t(1), size_t(6)}) {
                variables[index].scalar_kind = config::CONFIG_SCALAR_ENUM;
                variables[index].values = WEAPON_VALUES;
                variables[index].value_count = 3;
                variables[index].allow_numeric = true;
                variables[index].reject_unknown_numeric = true;
            }
            for (size_t index : {size_t(2), size_t(7)}) {
                variables[index].scalar_kind = config::CONFIG_SCALAR_MASK;
                variables[index].values = ARC_VALUES;
                variables[index].value_count = 2;
                variables[index].flags = ARC_FLAGS;
                variables[index].flag_count = 4;
                variables[index].allow_numeric = true;
                variables[index].reject_unknown_numeric = true;
                variables[index].known_bits = 15;
            }
            for (size_t index : {size_t(3), size_t(8)}) {
                variables[index].scalar_kind = config::CONFIG_SCALAR_MASK;
                variables[index].values = MOD_VALUES;
                variables[index].value_count = 1;
                variables[index].flags = MOD_FLAGS;
                variables[index].flag_count = 2;
                variables[index].allow_numeric = true;
                variables[index].reject_unknown_numeric = true;
                variables[index].known_bits = 0x202;
            }
            for (size_t index = 11; index <= 13; ++index) {
                variables[index].scalar_kind = config::CONFIG_SCALAR_ENUM;
                variables[index].values = SPECIAL_VALUES;
                variables[index].value_count = 4;
                variables[index].allow_numeric = true;
                variables[index].reject_unknown_numeric = true;
            }
            variables[14].has_min_length = true;
            variables[14].has_max_length = true;
            variables[14].min_length = 1;
            variables[14].max_length = 8;
            variables[15].scalar_kind = config::CONFIG_SCALAR_RATIO;
            initialized = true;
        }

        static const config::s_schema_node_descriptor nodes[] = {
            {"guardian", config::CONFIG_NODE_RECORD, config::CONFIG_NO_NODE, 0, 0, 15, config::CONFIG_NO_VARIABLE},
            {"appearance", config::CONFIG_NODE_SCALAR, 0, 0, 0, 0, 0},
            {"weapon", config::CONFIG_NODE_ARRAY, 0, 0, 1, 10, config::CONFIG_NO_VARIABLE},
            {"", config::CONFIG_NODE_RECORD, 2, 1, 1, 5, config::CONFIG_NO_VARIABLE},
            {"type", config::CONFIG_NODE_SCALAR, 3, 0, 1, 1, 1},
            {"arc", config::CONFIG_NODE_SCALAR, 3, 0, 2, 2, 2},
            {"mods", config::CONFIG_NODE_SCALAR, 3, 0, 3, 3, 3},
            {"ammo", config::CONFIG_NODE_SCALAR, 3, 0, 4, 4, 4},
            {"amount", config::CONFIG_NODE_SCALAR, 3, 0, 5, 5, 5},
            {"", config::CONFIG_NODE_RECORD, 2, 2, 6, 10, config::CONFIG_NO_VARIABLE},
            {"type", config::CONFIG_NODE_SCALAR, 9, 0, 6, 6, 6},
            {"arc", config::CONFIG_NODE_SCALAR, 9, 0, 7, 7, 7},
            {"mods", config::CONFIG_NODE_SCALAR, 9, 0, 8, 8, 8},
            {"ammo", config::CONFIG_NODE_SCALAR, 9, 0, 9, 9, 9},
            {"amount", config::CONFIG_NODE_SCALAR, 9, 0, 10, 10, 10},
            {"special", config::CONFIG_NODE_ARRAY, 0, 0, 11, 13, config::CONFIG_NO_VARIABLE},
            {"", config::CONFIG_NODE_SCALAR, 15, 1, 11, 11, 11},
            {"", config::CONFIG_NODE_SCALAR, 15, 2, 12, 12, 12},
            {"", config::CONFIG_NODE_SCALAR, 15, 3, 13, 13, 13},
            {"label", config::CONFIG_NODE_SCALAR, 0, 0, 14, 14, 14},
            {"reduction", config::CONFIG_NODE_SCALAR, 0, 0, 15, 15, 15},
            {"weapon", config::CONFIG_NODE_ARRAY, config::CONFIG_NO_NODE, 0, 16, 16, config::CONFIG_NO_VARIABLE},
            {"", config::CONFIG_NODE_RECORD, 21, 11, 16, 16, config::CONFIG_NO_VARIABLE},
            {"available_mods", config::CONFIG_NODE_SCALAR, 22, 0, 16, 16, 16},
        };
        static const config::s_runtime_schema schema = {
            nodes, sizeof(nodes) / sizeof(nodes[0]), variables, 17,
            ARRAY_INDEX_ALIASES,
            sizeof(ARRAY_INDEX_ALIASES) / sizeof(ARRAY_INDEX_ALIASES[0]),
        };
        return &schema;
    }

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

    bool Decode_Aggregate_(const char* source, config::s_config_write_set* write_set,
                           config::s_cfg_diagnostic* diagnostic) {
        config::s_cfg_statement statement;
        return Parse_Assignment_(source, &statement) &&
               config::Decode_Assignment_With_Schema(
                   &statement, Aggregate_Schema_(), write_set, diagnostic);
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

DOCTEST_TEST_CASE("record and array assignments follow declaration order and boundaries") {
    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Decode_Aggregate_(
        "guardian weapon 1 = plasma_cannon all heavy 0 2;", &writes, &diagnostic));
    DOCTEST_REQUIRE(writes.writes.size() == 5);
    DOCTEST_CHECK(writes.writes[0].canonical_path == "guardian.weapon.1.type");
    DOCTEST_CHECK(writes.writes[0].value.integer_value == 11);
    DOCTEST_CHECK(writes.writes[1].value.integer_value == 15);
    DOCTEST_CHECK(writes.writes[2].value.integer_value == 2);
    DOCTEST_CHECK(writes.writes[4].value.integer_value == 2);

    DOCTEST_REQUIRE(Decode_Aggregate_(
        "guardian weapon = plasma_cannon all heavy 0 2 mass_driver forward none 0 10;",
        &writes, &diagnostic));
    DOCTEST_REQUIRE(writes.writes.size() == 10);
    DOCTEST_CHECK(writes.writes[5].canonical_path == "guardian.weapon.2.type");
    DOCTEST_CHECK(writes.writes[5].value.integer_value == 4);
}

DOCTEST_TEST_CASE("enum-backed array aliases resolve to numeric canonical paths") {
    config::s_config_write_set numeric;
    config::s_config_write_set keyed;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Decode_Aggregate_(
        "weapon 11 available_mods = 0x202;", &numeric, &diagnostic));
    DOCTEST_REQUIRE(Decode_Aggregate_(
        "weapon plasma_cannon available_mods = 0x202;", &keyed, &diagnostic));
    DOCTEST_REQUIRE(numeric.writes.size() == 1);
    DOCTEST_REQUIRE(keyed.writes.size() == 1);
    DOCTEST_CHECK(keyed.writes[0].variable_id == numeric.writes[0].variable_id);
    DOCTEST_CHECK(keyed.writes[0].canonical_path == numeric.writes[0].canonical_path);
    DOCTEST_CHECK(keyed.writes[0].canonical_path == "weapon.11.available_mods");
    DOCTEST_CHECK(keyed.writes[0].value.integer_value == numeric.writes[0].value.integer_value);

    DOCTEST_CHECK_FALSE(Decode_Aggregate_(
        "weapon plasma_cannon available_mods = 1 2;", &keyed, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "value 2 exceeds selected configuration object "
                  "'weapon.11.available_mods'");

    DOCTEST_CHECK_FALSE(Decode_Aggregate_(
        "weapon unknown_weapon available_mods = 1;", &keyed, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "unknown configuration variable 'weapon.unknown_weapon'");
}

DOCTEST_TEST_CASE("scalar starts and skips remain within the nearest aggregate") {
    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Decode_Aggregate_(
        "guardian weapon 1 mods = heavy,eccm - 4;", &writes, &diagnostic));
    DOCTEST_REQUIRE(writes.writes.size() == 2);
    DOCTEST_CHECK(writes.writes[0].canonical_path == "guardian.weapon.1.mods");
    DOCTEST_CHECK(writes.writes[0].value.integer_value == 0x202);
    DOCTEST_CHECK(writes.writes[1].canonical_path == "guardian.weapon.1.amount");
    DOCTEST_CHECK(writes.writes[1].value.integer_value == 4);

    DOCTEST_REQUIRE(Decode_Aggregate_(
        "guardian special 2 = battle_scanner damper_field;", &writes, &diagnostic));
    DOCTEST_REQUIRE(writes.writes.size() == 2);
    DOCTEST_CHECK(writes.writes[0].canonical_path == "guardian.special.2");
    DOCTEST_CHECK(writes.writes[1].canonical_path == "guardian.special.3");

    DOCTEST_CHECK_FALSE(Decode_Aggregate_(
        "guardian weapon 1 mods = heavy 0 2 4;", &writes, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "value 4 exceeds selected configuration object 'guardian.weapon.1.mods'");
    DOCTEST_CHECK(writes.writes.empty());
}

DOCTEST_TEST_CASE("semantic scalars decode enums masks strings and normalized ratios") {
    config::s_config_write_set writes;
    config::s_cfg_diagnostic diagnostic;
    DOCTEST_REQUIRE(Decode_Aggregate_(
        "guardian label = \"Grüße\" 2/6;", &writes, &diagnostic));
    DOCTEST_REQUIRE(writes.writes.size() == 2);
    DOCTEST_REQUIRE(writes.writes[0].value.string_value.size() == 5);
    DOCTEST_CHECK(static_cast<uint8_t>(writes.writes[0].value.string_value[2]) == 0x81);
    DOCTEST_CHECK(writes.writes[1].value.ratio_numerator == 1);
    DOCTEST_CHECK(writes.writes[1].value.ratio_denominator == 3);

    DOCTEST_CHECK_FALSE(Decode_Aggregate_(
        "guardian weapon 1 type = 99;", &writes, &diagnostic));
    DOCTEST_CHECK(writes.writes.empty());
    DOCTEST_CHECK_FALSE(Decode_Aggregate_(
        "guardian weapon 1 mods = 0x4000;", &writes, &diagnostic));
    DOCTEST_CHECK(writes.writes.empty());
    DOCTEST_CHECK_FALSE(Decode_Aggregate_(
        "guardian reduction = 1/0;", &writes, &diagnostic));
    DOCTEST_CHECK(writes.writes.empty());
}

}