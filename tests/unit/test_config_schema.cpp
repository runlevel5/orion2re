#include "pch.h"

#include <filesystem>
#include <fstream>

#include "doctest/doctest.h"
#include "generate_cpp.h"
#include "schema_model.h"

namespace {
    std::string Fixture_Path_(const char* name) {
        return (std::filesystem::path(CONFIG_SCHEMA_TEST_DATA_DIR) / name).string();
    }

    bool Load_(const char* name, config::tool::s_schema* schema, std::string* diagnostic) {
        const std::string path = Fixture_Path_(name);
        return config::tool::Load_And_Validate_Schema(path.c_str(), schema, diagnostic);
    }
}

DOCTEST_TEST_CASE("aggregate schema normalizes traversal leaves defaults and native targets") {
    config::tool::s_schema schema;
    std::string diagnostic;

    DOCTEST_REQUIRE(Load_("valid_aggregates.json5", &schema, &diagnostic));
    DOCTEST_REQUIRE(schema.parameters.size() == 11);
    DOCTEST_CHECK(schema.parameters[0].path == "guardian.appearance");
    DOCTEST_CHECK(schema.parameters[1].path == "guardian.weapon.1.type");
    DOCTEST_CHECK(schema.parameters[1].scalar_kind == config::tool::SCALAR_ENUM);
    DOCTEST_CHECK(schema.parameters[1].default_integer == 11);
    DOCTEST_CHECK(schema.parameters[2].scalar_kind == config::tool::SCALAR_MASK);
    DOCTEST_CHECK(schema.parameters[2].default_integer == 15);
    DOCTEST_CHECK(schema.parameters[4].native_targets.size() == 2);
    DOCTEST_CHECK(schema.parameters[4].native_targets[0] ==
                  "fixture.weapons[0].amount");
    DOCTEST_CHECK(schema.parameters[4].native_targets[1] ==
                  "fixture.weapons[0].current_amount");
    DOCTEST_CHECK(schema.parameters[9].default_string == "Grüße");
    DOCTEST_CHECK(schema.parameters[10].default_ratio_numerator == 1);
    DOCTEST_CHECK(schema.parameters[10].default_ratio_denominator == 3);

    DOCTEST_REQUIRE(schema.nodes.size() == 15);
    DOCTEST_CHECK(schema.nodes[0].kind == config::tool::NODE_RECORD);
    DOCTEST_CHECK(schema.nodes[0].first_parameter == 0);
    DOCTEST_CHECK(schema.nodes[0].last_parameter == 10);
    DOCTEST_CHECK(schema.nodes[2].kind == config::tool::NODE_ARRAY);
    DOCTEST_CHECK(schema.nodes[2].first_parameter == 1);
    DOCTEST_CHECK(schema.nodes[2].last_parameter == 8);
    DOCTEST_REQUIRE(schema.nodes[2].index_aliases.size() == 2);
    DOCTEST_CHECK(schema.nodes[2].index_aliases[0].name == "primary");
    DOCTEST_CHECK(schema.nodes[2].index_aliases[0].value == 1);
    DOCTEST_CHECK(schema.nodes[2].index_aliases[1].name == "secondary");
    DOCTEST_CHECK(schema.nodes[2].index_aliases[1].value == 2);
}

DOCTEST_TEST_SUITE("configuration schema") {

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
    DOCTEST_CHECK(first_content.find("Get_Default_Value") != std::string::npos);
    DOCTEST_CHECK(first_content.find("Set_Value") != std::string::npos);
    DOCTEST_CHECK(first_content.find("Get_Current_Value") != std::string::npos);
    DOCTEST_CHECK(first_content.find("decltype") == std::string::npos);
    DOCTEST_CHECK(first_content.find("static_assert") == std::string::npos);
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
        "{ 2, \"primary\", 1 }") != std::string::npos);
    DOCTEST_CHECK(generated.find(
        "{ 2, \"secondary\", 2 }") != std::string::npos);
    DOCTEST_CHECK(generated.find("guardian.weapon.primary") == std::string::npos);
    DOCTEST_CHECK(generated.find("guardian.weapon.secondary") == std::string::npos);

    std::filesystem::remove_all(root);
}

}
