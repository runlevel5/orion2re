#include "pch.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include "doctest/doctest.h"
#include "generate_cpp.h"
#include "generate_docs.h"
#include "json5_reader.h"
#include "schema_model.h"

namespace {
    std::string Fixture_Path_(const char* name) {
        return (std::filesystem::path(CONFIG_SCHEMA_TEST_DATA_DIR) / name).string();
    }

    bool Load_(const char* name, config::tool::s_schema* schema, std::string* diagnostic) {
        const std::string path = Fixture_Path_(name);
        return config::tool::Load_And_Validate_Schema(path.c_str(), schema, diagnostic);
    }

    const config::tool::s_json_value* Member_(
        const config::tool::s_json_value* object, const char* name) {
        if (object == nullptr || object->kind != config::tool::JSON_OBJECT) {
            return nullptr;
        }
        for (size_t index = 0; index < object->member_names.size(); ++index) {
            if (object->member_names[index] == name) {
                return &object->values[index];
            }
        }
        return nullptr;
    }

    int64_t Integer_(const config::tool::s_json_value* value) {
        if (value->kind == config::tool::JSON_OBJECT) {
            value = Member_(value, "value");
        }
        if (value->kind == config::tool::JSON_SIGNED) {
            return value->signed_value;
        }
        return static_cast<int64_t>(value->unsigned_value);
    }

    bool Internal_Links_Resolve_(const std::string& html) {
        std::vector<std::string> ids;
        size_t offset = 0;
        while ((offset = html.find("id=\"", offset)) != std::string::npos) {
            offset += 4;
            const size_t end = html.find('"', offset);
            ids.push_back(html.substr(offset, end - offset));
            offset = end;
        }
        offset = 0;
        while ((offset = html.find("href=\"#", offset)) != std::string::npos) {
            offset += 7;
            const size_t end = html.find('"', offset);
            const std::string target = html.substr(offset, end - offset);
            if (std::find(ids.begin(), ids.end(), target) == ids.end()) {
                return false;
            }
            offset = end;
        }
        return true;
    }

    size_t Count_(const std::string& text, const std::string& needle) {
        size_t count = 0;
        size_t offset = 0;
        while ((offset = text.find(needle, offset)) != std::string::npos) {
            ++count;
            offset += needle.size();
        }
        return count;
    }
}

DOCTEST_TEST_CASE("aggregate schema normalizes traversal leaves defaults and native targets") {
    config::tool::s_schema schema;
    std::string diagnostic;

    DOCTEST_REQUIRE(Load_("valid_aggregates.json5", &schema, &diagnostic));
    DOCTEST_REQUIRE(schema.parameters.size() == 12);
    DOCTEST_CHECK(schema.parameters[0].path == "ships.guardian.appearance");
    DOCTEST_CHECK(schema.parameters[1].path == "ships.guardian.weapon.1.type");
    DOCTEST_CHECK(schema.parameters[1].scalar_kind == config::tool::SCALAR_ENUM);
    DOCTEST_CHECK(schema.parameters[1].default_integer == 11);
    DOCTEST_REQUIRE(schema.parameters[1].values.size() == 3);
    DOCTEST_CHECK(schema.parameters[1].values[0].description.empty());
    DOCTEST_CHECK(schema.parameters[1].values[1].description ==
                  "A kinetic cannon that fires dense projectiles.");
    DOCTEST_CHECK(schema.parameters[2].scalar_kind == config::tool::SCALAR_MASK);
    DOCTEST_CHECK(schema.parameters[2].default_integer == 15);
    DOCTEST_REQUIRE(schema.parameters[2].flags.size() == 4);
    DOCTEST_CHECK(schema.parameters[2].flags[0].description ==
                  "The weapon can fire toward the front of the ship.");
    DOCTEST_CHECK(schema.parameters[4].native_targets.size() == 2);
    DOCTEST_CHECK(schema.parameters[4].native_targets[0] ==
                  "fixture.weapons[0].amount");
    DOCTEST_CHECK(schema.parameters[4].native_targets[1] ==
                  "fixture.weapons[0].current_amount");
    DOCTEST_CHECK(schema.parameters[9].scalar_kind == config::tool::SCALAR_BITMAP);
    DOCTEST_CHECK(schema.parameters[9].bitmap_bit_count == 40);
    DOCTEST_REQUIRE(schema.parameters[9].default_bitmap.size() == 5);
    DOCTEST_CHECK(schema.parameters[9].default_bitmap[0] == 0x08);
    DOCTEST_CHECK(schema.parameters[9].default_bitmap[4] == 0x80);
    DOCTEST_CHECK(schema.parameters[9].native_targets[0] ==
                  "fixture.special_device_flags");
    DOCTEST_CHECK(schema.parameters[10].default_string == "Grüße");
    DOCTEST_CHECK(schema.parameters[11].default_ratio_numerator == 1);
    DOCTEST_CHECK(schema.parameters[11].default_ratio_denominator == 3);

    DOCTEST_REQUIRE(schema.nodes.size() == 17);
    DOCTEST_CHECK(schema.nodes[0].kind == config::tool::NODE_NAMESPACE);
    DOCTEST_CHECK(schema.nodes[0].first_parameter == 0);
    DOCTEST_CHECK(schema.nodes[0].last_parameter == 11);
    DOCTEST_CHECK(schema.nodes[3].kind == config::tool::NODE_ARRAY);
    DOCTEST_CHECK(schema.nodes[3].first_parameter == 1);
    DOCTEST_CHECK(schema.nodes[3].last_parameter == 8);
    DOCTEST_REQUIRE(schema.nodes[3].index_aliases.size() == 2);
    DOCTEST_CHECK(schema.nodes[3].index_aliases[0].name == "primary");
    DOCTEST_CHECK(schema.nodes[3].index_aliases[0].value == 1);
    DOCTEST_CHECK(schema.nodes[3].index_aliases[1].name == "secondary");
    DOCTEST_CHECK(schema.nodes[3].index_aliases[1].value == 2);

    DOCTEST_REQUIRE(schema.type_declarations.size() == 6);
    DOCTEST_CHECK(schema.type_declarations[1].metadata.title == "Weapon");
    DOCTEST_REQUIRE(schema.record_declarations.size() == 1);
    DOCTEST_CHECK(schema.record_declarations[0].fields[0].metadata.title == "Weapon");
    DOCTEST_REQUIRE(schema.parameter_declarations.size() == 1);
    DOCTEST_CHECK(schema.parameter_declarations[0].name_space == "ships");
    DOCTEST_CHECK(schema.parameter_declarations[0].type.node_kind == config::tool::NODE_RECORD);
    DOCTEST_REQUIRE(schema.parameter_declarations[0].type.fields.size() == 5);
    DOCTEST_CHECK(schema.parameter_declarations[0].type.fields[1].type->node_kind ==
                  config::tool::NODE_ARRAY);
    DOCTEST_CHECK(schema.parameter_declarations[0].type.fields[1].type->array_count == 2);
    DOCTEST_CHECK(schema.parameter_declarations[0].type.fields[1].type->array_index_type ==
                  "weapon_slot_index");
    DOCTEST_CHECK(schema.record_declarations[0].used_by.size() == 1);
    DOCTEST_CHECK(schema.type_declarations[1].used_by.size() == 1);
}

DOCTEST_TEST_SUITE("configuration schema") {

DOCTEST_TEST_CASE("JSON reader replaces reused output instead of appending to it") {
    config::tool::s_json_value reused;
    config::tool::s_json_value expected;
    std::string diagnostic;
    const std::string primitives = Fixture_Path_("valid_primitives.json5");
    const std::string aggregates = Fixture_Path_("valid_aggregates.json5");

    DOCTEST_REQUIRE(config::tool::Read_Json5_File(
        primitives.c_str(), &reused, &diagnostic));
    DOCTEST_REQUIRE(config::tool::Read_Json5_File(
        aggregates.c_str(), &reused, &diagnostic));
    DOCTEST_REQUIRE(config::tool::Read_Json5_File(
        aggregates.c_str(), &expected, &diagnostic));

    DOCTEST_CHECK(reused.kind == expected.kind);
    DOCTEST_CHECK(reused.member_names == expected.member_names);
    DOCTEST_CHECK(reused.values.size() == expected.values.size());
}

DOCTEST_TEST_CASE("main schema game-domain types are documented and match native values") {
    config::tool::s_schema schema;
    std::string diagnostic;
    DOCTEST_REQUIRE(config::tool::Load_And_Validate_Schema(
        CONFIG_MAIN_SCHEMA_PATH, &schema, &diagnostic));

    config::tool::s_json_value root;
    DOCTEST_REQUIRE(config::tool::Read_Json5_File(
        CONFIG_MAIN_SCHEMA_PATH, &root, &diagnostic));
    const config::tool::s_json_value* types = Member_(&root, "types");
    DOCTEST_REQUIRE(types != nullptr);
    DOCTEST_REQUIRE(types->member_names.size() == 29);

    for (const config::tool::s_json_value& type : types->values) {
        for (const char* field : {"title", "description", "remarks"}) {
            const config::tool::s_json_value* documentation = Member_(&type, field);
            DOCTEST_REQUIRE(documentation != nullptr);
            DOCTEST_CHECK(documentation->kind == config::tool::JSON_STRING);
            DOCTEST_CHECK_FALSE(documentation->string_value.empty());
        }
    }

    struct s_expected_symbol {
        const char* type;
        const char* collection;
        const char* symbol;
        int64_t value;
    };
    const s_expected_symbol symbols[] = {
        {"game_difficulty", "values", "tutor", GAME_DIFFICULTY_TUTOR},
        {"game_difficulty", "values", "impossible", GAME_DIFFICULTY_IMPOSSIBLE},
        {"galaxy_age", "values", "old", GALAXY_AGE_OLD},
        {"galaxy_size", "values", "maximum", GALAXY_SIZE_MAXIMUM},
        {"government", "values", "feudal", GOVERNMENT_FEUDAL},
        {"government", "values", "galactic_unification", GOVERNMENT_GALACTIC_UNIFICATION},
        {"stock_race_index", "values", "alkari", STOCK_RACE_ALKARI},
        {"stock_race_index", "values", "trilarian", STOCK_RACE_TRILARIAN},
        {"crew_quality", "values", "ultra_elite", CREW_QUALITY_ULTRA_ELITE},
        {"leader_type", "values", "colony", LEADER_TYPE_COLONY},
        {"strategic_special_category", "values", "beam_missile_damage", SPECIAL_CATEGORY_BEAM_MISSILE_DAMAGE},
        {"technology_application_type", "values", "reserved_6", TECH_APPLICATION_TYPE_RESERVED_6},
        {"technology_application_type", "values", "special", TECH_APPLICATION_TYPE_SPECIAL},
        {"planet_environment_group", "values", "lifeless", PLANET_GROUP_LIFELESS},
        {"map_generation_satellite", "values", "companion_star", MAPGEN_SATELLITE_COMPANION_STAR},
        {"ai_special_choice_item_kind", "values", "weapon", AI_SPECIAL_ITEM_WEAPON},
        {"ai_ship_design_theme", "values", "human", AI_SHIP_DESIGN_THEME_HUMAN},
        {"ai_ship_special_theme", "values", "human", AI_SHIP_SPECIAL_THEME_HUMAN},
        {"weapon_modification_id", "values", "no_mods", WEAPON_MOD_NO_MODS},
        {"weapon_modification_id", "values", "overloaded", WEAPON_MOD_OVERLOADED},
        {"available_weapon_modification_mask", "flags", "reserved_0", WEAPON_MOD_FLAG_RESERVED_0},
        {"available_weapon_modification_mask", "flags", "ovr", WEAPON_MOD_FLAG_OVERLOADED},
        {"natural_weapon_property_mask", "flags", "sh", WEAPON_NATURAL_FLAG_ANTI_SHIELD},
        {"natural_weapon_property_mask", "flags", "ai2", WEAPON_NATURAL_FLAG_AI_SPATIAL_COMPRESSOR},
        {"weapon_firing_arc", "values", "all_sectors", WEAPON_FIRING_ARC_ALL_SECTORS},
        {"weapon_firing_arc", "values", "design_360", WEAPON_FIRING_ARC_360},
    };

    for (const s_expected_symbol& expected : symbols) {
        const config::tool::s_json_value* type = Member_(types, expected.type);
        const config::tool::s_json_value* collection = Member_(type, expected.collection);
        const config::tool::s_json_value* symbol = Member_(collection, expected.symbol);
        DOCTEST_REQUIRE_MESSAGE(symbol != nullptr, expected.type, ".", expected.symbol);
        DOCTEST_CHECK(Integer_(symbol) == expected.value);
    }

    const char* described_symbols[][2] = {
        {"technology_application_type", "special"},
        {"planet_environment_group", "hostile"},
        {"ai_special_choice_item_kind", "special"},
        {"ai_ship_design_theme", "special"},
        {"weapon_firing_arc", "all_sectors"},
        {"weapon_firing_arc", "design_360"},
    };
    for (const auto& expected : described_symbols) {
        const config::tool::s_json_value* type = Member_(types, expected[0]);
        const config::tool::s_json_value* symbol = Member_(Member_(type, "values"), expected[1]);
        const config::tool::s_json_value* description = Member_(symbol, "description");
        DOCTEST_REQUIRE_MESSAGE(description != nullptr, expected[0], ".", expected[1]);
        DOCTEST_CHECK_FALSE(description->string_value.empty());
    }

    struct s_expected_extent {
        const char* type;
        const char* collection;
        size_t count;
    };
    const s_expected_extent extents[] = {
        {"game_difficulty", "values", GAME_DIFFICULTY_COUNT},
        {"galaxy_age", "values", GALAXY_AGE_COUNT},
        {"galaxy_size", "values", GALAXY_SIZE_COUNT},
        {"government", "values", GOVERNMENT_COUNT},
        {"stock_race_index", "values", STOCK_RACE_COUNT},
        {"crew_quality", "values", CREW_QUALITY_COUNT},
        {"leader_type", "values", LEADER_TYPE_COUNT},
        {"strategic_special_category", "values", SPECIAL_CATEGORY_COUNT},
        {"technology_application_type", "values", TECH_APPLICATION_TYPE_COUNT},
        {"planet_environment_group", "values", PLANET_GROUP_COUNT},
        {"map_generation_satellite", "values", MAPGEN_SATELLITE_COUNT},
        {"weapon_modification_id", "values", WEAPON_MOD_COUNT},
        {"available_weapon_modification_mask", "flags", WEAPON_MOD_COUNT},
    };
    for (const s_expected_extent& expected : extents) {
        const config::tool::s_json_value* type = Member_(types, expected.type);
        const config::tool::s_json_value* collection = Member_(type, expected.collection);
        DOCTEST_REQUIRE_MESSAGE(collection != nullptr, expected.type);
        DOCTEST_CHECK(collection->member_names.size() == expected.count);
    }
}

DOCTEST_TEST_CASE("primitive schema preserves declaration order and normalizes CP850 defaults") {
    config::tool::s_schema schema;
    std::string diagnostic;

    DOCTEST_REQUIRE(Load_("valid_primitives.json5", &schema, &diagnostic));
    DOCTEST_REQUIRE(schema.parameters.size() == 3);
    DOCTEST_CHECK(schema.parameters[0].path == "enabled");
    DOCTEST_CHECK(schema.parameters[1].path == "signed_value");
    DOCTEST_CHECK(schema.parameters[2].path == "captain_name");
    DOCTEST_CHECK(schema.parameters[1].constraints.min == -10);
    DOCTEST_CHECK(schema.parameters[1].constraints.max == 10);
    DOCTEST_CHECK(schema.parameters[2].default_string == "Grüße");
    DOCTEST_REQUIRE(schema.nodes.size() == 3);
    DOCTEST_CHECK(schema.nodes[0].parameter == 0);
    DOCTEST_CHECK(schema.nodes[1].parameter == 1);
    DOCTEST_CHECK(schema.nodes[2].parameter == 2);
    const std::string encoded = config::tool::Encode_Game_String(schema.parameters[2].default_string);
    DOCTEST_REQUIRE(encoded.size() == 5);
    DOCTEST_CHECK(static_cast<uint8_t>(encoded[2]) == 0x81);
    DOCTEST_CHECK(static_cast<uint8_t>(encoded[3]) == 0xE1);
}

DOCTEST_TEST_CASE("interleaved namespace members receive contiguous depth-first runtime IDs") {
    config::tool::s_schema schema;
    std::string diagnostic;

    DOCTEST_REQUIRE(Load_("valid_interleaved_namespaces.json5", &schema, &diagnostic));
    DOCTEST_REQUIRE(schema.parameters.size() == 3);
    DOCTEST_CHECK(schema.parameters[0].path == "combat.first");
    DOCTEST_CHECK(schema.parameters[1].path == "combat.second");
    DOCTEST_CHECK(schema.parameters[2].path == "standalone");
    DOCTEST_REQUIRE(schema.nodes.size() == 4);
    DOCTEST_CHECK(schema.nodes[0].kind == config::tool::NODE_NAMESPACE);
    DOCTEST_CHECK(schema.nodes[0].first_parameter == 0);
    DOCTEST_CHECK(schema.nodes[0].last_parameter == 1);
    DOCTEST_CHECK(schema.nodes[1].parameter == 0);
    DOCTEST_CHECK(schema.nodes[2].parent == UINT32_MAX);
    DOCTEST_CHECK(schema.nodes[2].first_parameter == 2);
    DOCTEST_CHECK(schema.nodes[2].last_parameter == 2);
    DOCTEST_CHECK(schema.nodes[3].parent == 0);
    DOCTEST_CHECK(schema.nodes[3].parameter == 1);
    DOCTEST_CHECK(config::tool::Validate_Runtime_Layout(&schema, &diagnostic));

    const std::filesystem::path output = std::filesystem::temp_directory_path() /
        "orion2re-config-interleaved-namespace-test";
    std::filesystem::remove_all(output);
    DOCTEST_REQUIRE(config::tool::Generate_Cpp(
        &schema, output.string().c_str(), &diagnostic));
    std::ifstream generated_file(output / "config.cpp", std::ios::binary);
    const std::string generated((std::istreambuf_iterator<char>(generated_file)),
                                std::istreambuf_iterator<char>());
    DOCTEST_CHECK(generated.find(
        "{ \"combat\", CONFIG_NODE_NAMESPACE, CONFIG_NO_NODE, 0, 0, 1") !=
        std::string::npos);
    DOCTEST_CHECK(generated.find(
        "{ \"standalone\", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 2, 2, 2") !=
        std::string::npos);
    std::filesystem::remove_all(output);
}

DOCTEST_TEST_CASE("C++ generation rejects invalid runtime variable ranges") {
    config::tool::s_schema schema;
    std::string diagnostic;
    DOCTEST_REQUIRE(Load_("valid_interleaved_namespaces.json5", &schema, &diagnostic));
    schema.nodes[0].last_parameter = 2;

    DOCTEST_CHECK_FALSE(config::tool::Validate_Runtime_Layout(&schema, &diagnostic));
    DOCTEST_CHECK(diagnostic.find("aggregate range does not match its descendants") !=
                  std::string::npos);
    const std::filesystem::path output = std::filesystem::temp_directory_path() /
        "orion2re-config-invalid-layout-test";
    std::filesystem::remove_all(output);
    DOCTEST_CHECK_FALSE(config::tool::Generate_Cpp(
        &schema, output.string().c_str(), &diagnostic));
    DOCTEST_CHECK_FALSE(std::filesystem::exists(output / "config.cpp"));
    std::filesystem::remove_all(output);
}

DOCTEST_TEST_CASE("schema diagnostics reject malformed and unsupported declarations") {
    struct s_case {
        const char* fixture;
        const char* expected;
    };
    const s_case cases[] = {
        {"duplicate_key.json5", "duplicate object key 'title'"},
        {"unknown_property.json5", "unknown property 'format'"},
        {"invalid_default.json5", "integer violates type or field constraints"},
        {"invalid_unicode.json5", "not representable in CP850"},
        {"invalid_aggregate_default.json5", "record default is missing field 'right'"},
        {"invalid_recursive_record.json5", "recursive aggregate value type"},
        {"invalid_mask.json5", "flags must be single positive bits"},
        {"invalid_bitmap.json5", "expected a bit index within the bitmap"},
        {"invalid_namespace.json5", "namespace conflicts with a top-level parameter"},
        {"invalid_unused_type.json5", "composite contains unknown bits"},
        {"invalid_public_type_documentation.json5", ".title: expected a non-empty string"},
        {"invalid_ratio_binding.json5", "ratio leaves require numerator and denominator targets"},
        {"invalid_array_index_type.json5", "index_type: expected an enum type"},
        {"invalid_array_index_range.json5",
         "index_type.outside: value is outside the array's external index range"},
    };

    for (const s_case& test_case : cases) {
        config::tool::s_schema schema;
        std::string diagnostic;
        DOCTEST_CHECK_FALSE(Load_(test_case.fixture, &schema, &diagnostic));
        DOCTEST_CHECK_MESSAGE(diagnostic.find(test_case.expected) != std::string::npos,
                              test_case.fixture, ": ", diagnostic);
    }
}

DOCTEST_TEST_CASE("C++ generation is byte-for-byte deterministic") {
    config::tool::s_schema schema;
    std::string diagnostic;
    DOCTEST_REQUIRE(Load_("valid_primitives.json5", &schema, &diagnostic));

    const std::filesystem::path root = std::filesystem::temp_directory_path() /
                                       "orion2re-config-schema-test";
    const std::filesystem::path first = root / "first";
    const std::filesystem::path second = root / "second";
    std::filesystem::remove_all(root);

    DOCTEST_REQUIRE(config::tool::Generate_Cpp(&schema, first.string().c_str(), &diagnostic));
    DOCTEST_REQUIRE(config::tool::Generate_Cpp(&schema, second.string().c_str(), &diagnostic));

    std::ifstream first_file(first / "config.cpp", std::ios::binary);
    std::ifstream second_file(second / "config.cpp", std::ios::binary);
    const std::string first_content((std::istreambuf_iterator<char>(first_file)),
                                    std::istreambuf_iterator<char>());
    const std::string second_content((std::istreambuf_iterator<char>(second_file)),
                                     std::istreambuf_iterator<char>());
    DOCTEST_CHECK(first_content == second_content);
    DOCTEST_CHECK(first_content.find("const s_variable_descriptor variables[]") ==
                  std::string::npos);
    DOCTEST_CHECK(first_content.find("bool Get_Default_Value(") != std::string::npos);
    DOCTEST_CHECK(first_content.find("bool Resolve_Variable(") != std::string::npos);
    DOCTEST_CHECK(first_content.find("switch (variable_id)") != std::string::npos);
    DOCTEST_CHECK(first_content.find("&generated_type_0") != std::string::npos);
    DOCTEST_CHECK(first_content.find("decltype") == std::string::npos);
    DOCTEST_CHECK(first_content.find(
        "static_assert(CONFIG_MAX_VARIABLE_TARGETS >= 1);") != std::string::npos);
    DOCTEST_CHECK(first_content.find("CONFIG_PARAMETER_") == std::string::npos);

    std::filesystem::remove_all(root);
}

DOCTEST_TEST_CASE("C++ generation emits array aliases only for path resolution") {
    config::tool::s_schema schema;
    std::string diagnostic;
    DOCTEST_REQUIRE(Load_("valid_aggregates.json5", &schema, &diagnostic));

    const std::filesystem::path root = std::filesystem::temp_directory_path() /
                                       "orion2re-config-alias-test";
    std::filesystem::remove_all(root);
    DOCTEST_REQUIRE(config::tool::Generate_Cpp(
        &schema, root.string().c_str(), &diagnostic));

    std::ifstream generated_file(root / "config.cpp", std::ios::binary);
    const std::string generated((std::istreambuf_iterator<char>(generated_file)),
                                std::istreambuf_iterator<char>());
    DOCTEST_CHECK(generated.find(
        "{ 3, \"primary\", 1 }") != std::string::npos);
    DOCTEST_CHECK(generated.find(
        "{ 3, \"secondary\", 2 }") != std::string::npos);
    DOCTEST_CHECK(generated.find("guardian.weapon.primary") == std::string::npos);
    DOCTEST_CHECK(generated.find("guardian.weapon.secondary") == std::string::npos);
    DOCTEST_CHECK(generated.find(
        "static_assert(sizeof(fixture.special_device_flags) == 5);") !=
        std::string::npos);
    DOCTEST_CHECK(generated.find(
        "{ (void*)&fixture.special_device_flags, "
        "sizeof(fixture.special_device_flags) }") != std::string::npos);
    DOCTEST_CHECK(Count_(generated,
        "static const s_symbol_descriptor generated_values_") == 2);
    DOCTEST_CHECK(Count_(generated,
        "static const s_symbol_descriptor generated_flags_") == 1);
    DOCTEST_CHECK(Count_(generated,
        "static const s_bitmap_symbol_descriptor generated_bitmap_values_") == 1);
    DOCTEST_CHECK(Count_(generated,
        "static const s_bitmap_symbol_descriptor generated_bitmap_flags_") == 1);
    DOCTEST_CHECK(Count_(generated, "generated_values_0") == 2);
    DOCTEST_CHECK(Count_(generated, "generated_type_1") >= 3);
    DOCTEST_CHECK(generated.find(
        "Number of weapons installed in this slot.") == std::string::npos);
    DOCTEST_CHECK(generated.find("Aggregate schema fixture.") == std::string::npos);
    DOCTEST_CHECK(generated.find("s_group_descriptor") == std::string::npos);
    DOCTEST_CHECK(generated.find("variable_1_values") == std::string::npos);
    DOCTEST_CHECK(generated.find("variable_6_values") == std::string::npos);

    std::filesystem::remove_all(root);
}

DOCTEST_TEST_CASE("HTML documentation is deterministic searchable linked and author-facing") {
    config::tool::s_schema schema;
    std::string diagnostic;
    DOCTEST_REQUIRE(Load_("valid_aggregates.json5", &schema, &diagnostic));

    const std::filesystem::path root = std::filesystem::temp_directory_path() /
                                       "orion2re-config-docs-test";
    const std::filesystem::path first = root / "first.html";
    const std::filesystem::path second = root / "second.html";
    std::filesystem::remove_all(root);
    DOCTEST_REQUIRE(config::tool::Generate_Documentation(
        &schema, first.string().c_str(), &diagnostic));
    DOCTEST_REQUIRE(config::tool::Generate_Documentation(
        &schema, second.string().c_str(), &diagnostic));

    std::ifstream first_file(first, std::ios::binary);
    std::ifstream second_file(second, std::ios::binary);
    const std::string generated((std::istreambuf_iterator<char>(first_file)),
                                std::istreambuf_iterator<char>());
    const std::string regenerated((std::istreambuf_iterator<char>(second_file)),
                                  std::istreambuf_iterator<char>());
    DOCTEST_CHECK(generated == regenerated);
    DOCTEST_CHECK(generated.starts_with("<!doctype html>"));
    DOCTEST_CHECK(generated.find("<style>") != std::string::npos);
    DOCTEST_CHECK(generated.find("fonts.googleapis.com/css?family=Open+Sans") !=
                  std::string::npos);
    DOCTEST_CHECK(generated.find("font-family:\"Noto Serif\"") != std::string::npos);
    DOCTEST_CHECK(generated.find("color:#ba3925") != std::string::npos);
    DOCTEST_CHECK(generated.find("color:#2156a5") != std::string::npos);
    DOCTEST_CHECK(generated.find("background:#f7f7f8") != std::string::npos);
    DOCTEST_CHECK(generated.find("window.CONFIG_SEARCH_INDEX") != std::string::npos);
    DOCTEST_CHECK(generated.find("Ctrl+K") == std::string::npos);
    DOCTEST_CHECK(generated.find("id=\"parameter-guardian\"") != std::string::npos);
    DOCTEST_CHECK(generated.find("id=\"type-weapon_id\"") != std::string::npos);
    DOCTEST_CHECK(generated.find("id=\"record-weapon_slot\"") != std::string::npos);
    DOCTEST_CHECK(generated.find("href=\"#type-weapon_id\"") != std::string::npos);
    DOCTEST_CHECK(generated.find("href=\"#record-weapon_slot\"") != std::string::npos);
    DOCTEST_CHECK(generated.find("Used by") != std::string::npos);
    DOCTEST_CHECK(generated.find(
        "A kinetic cannon that fires dense projectiles.") !=
        std::string::npos);
    DOCTEST_CHECK(generated.find(
        "The weapon can fire toward the front of the ship.") !=
        std::string::npos);
    DOCTEST_CHECK(generated.find("Type in the file") != std::string::npos);
    DOCTEST_CHECK(generated.find("<code>ships guardian</code>") != std::string::npos);
    DOCTEST_CHECK(generated.find("Name to type") == std::string::npos);
    DOCTEST_CHECK(generated.find("See the containing setting") == std::string::npos);
    DOCTEST_CHECK(generated.find("<div class=\"field-list\">") != std::string::npos);
    DOCTEST_CHECK(generated.find("Built-in default configuration") != std::string::npos);
    DOCTEST_CHECK(generated.find("ships guardian appearance = 2;") != std::string::npos);
    DOCTEST_CHECK(generated.find(
        "ships guardian weapon =\n"
        "    plasma_cannon all 0 2\n"
        "    mass_driver forward,aft 0 10;") !=
        std::string::npos);
    DOCTEST_CHECK(generated.find(
        "ships guardian weapon primary type = plasma_cannon;") == std::string::npos);
    DOCTEST_CHECK(generated.find("ships guardian weapon 1 type") == std::string::npos);
    DOCTEST_CHECK(generated.find(
        "ships guardian specials = self_repairing;") != std::string::npos);
    DOCTEST_CHECK(generated.find(
        "ships guardian label = &quot;Grüße&quot;;") != std::string::npos);
    DOCTEST_CHECK(generated.find("ships guardian reduction = 1/3;") != std::string::npos);
    DOCTEST_CHECK(Count_(generated, "ships guardian weapon =") == 1);
    DOCTEST_CHECK(generated.find("Configuration path") == std::string::npos);
    DOCTEST_CHECK(generated.find("<dt>Storage</dt>") == std::string::npos);
    DOCTEST_CHECK(generated.find("uint8") == std::string::npos);
    DOCTEST_CHECK(generated.find("uint16") == std::string::npos);
    DOCTEST_CHECK(generated.find("int8") == std::string::npos);
    DOCTEST_CHECK(generated.find("int16") == std::string::npos);
    DOCTEST_CHECK(generated.find("Implementation details") == std::string::npos);
    DOCTEST_CHECK(generated.find("fixture.weapons") == std::string::npos);
    DOCTEST_CHECK(generated.find("guardian.weapon.1.type") == std::string::npos);
    DOCTEST_CHECK(generated.find("guardian.weapon.2.type") == std::string::npos);
    DOCTEST_CHECK(generated.find("guardian label = &quot;Sentinel&quot;;") !=
                  std::string::npos);
    DOCTEST_CHECK(Internal_Links_Resolve_(generated));

    std::filesystem::remove_all(root);
}

DOCTEST_TEST_CASE("HTML documentation escapes metadata and handles empty public sections") {
    config::tool::s_schema schema;
    std::string diagnostic;
    DOCTEST_REQUIRE(Load_("valid_primitives.json5", &schema, &diagnostic));
    schema.groups[0].metadata.description = "Local <script>alert(1)</script> & settings.";
    schema.parameter_declarations[0].metadata.title = "Enabled <control>";
    schema.parameter_declarations[0].metadata.examples = {
        "enabled = \"<unsafe>&\";",
    };

    const std::filesystem::path root = std::filesystem::temp_directory_path() /
                                       "orion2re-config-docs-escape-test";
    const std::filesystem::path output = root / "config-reference.html";
    std::filesystem::remove_all(root);
    DOCTEST_REQUIRE(config::tool::Generate_Documentation(
        &schema, output.string().c_str(), &diagnostic));

    std::ifstream generated_file(output, std::ios::binary);
    const std::string generated((std::istreambuf_iterator<char>(generated_file)),
                                std::istreambuf_iterator<char>());
    DOCTEST_CHECK(generated.find("Local &lt;script&gt;alert(1)&lt;/script&gt; &amp; settings.") !=
                  std::string::npos);
    DOCTEST_CHECK(generated.find("Enabled &lt;control&gt;") != std::string::npos);
    DOCTEST_CHECK(generated.find("enabled = &quot;&lt;unsafe&gt;&amp;&quot;;") !=
                  std::string::npos);
    DOCTEST_CHECK(generated.find("Local <script>alert(1)</script>") == std::string::npos);
    DOCTEST_CHECK(generated.find("\\u003cscript\\u003ealert(1)\\u003c/script\\u003e") !=
                  std::string::npos);
    DOCTEST_CHECK(generated.find("There are no additional allowed-value lists.") !=
                  std::string::npos);
    DOCTEST_CHECK(generated.find("There are no additional setting layouts.") !=
                  std::string::npos);
    DOCTEST_CHECK(Internal_Links_Resolve_(generated));

    std::filesystem::remove_all(root);
}

}
