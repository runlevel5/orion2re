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

DOCTEST_TEST_SUITE("configuration schema") {

DOCTEST_TEST_CASE("primitive schema preserves declaration order and normalizes CP850 defaults") {
    config::tool::s_schema schema;
    std::string diagnostic;

    DOCTEST_REQUIRE(Load_("valid_primitives.json5", &schema, &diagnostic));
    DOCTEST_REQUIRE(schema.parameters.size() == 3);
    DOCTEST_CHECK(schema.parameters[0].path == "enabled");
    DOCTEST_CHECK(schema.parameters[1].path == "signed_value");
    DOCTEST_CHECK(schema.parameters[2].path == "captain_name");
    DOCTEST_CHECK(schema.parameters[0].schema_id != schema.parameters[1].schema_id);
    DOCTEST_CHECK(schema.parameters[1].constraints.min == -10);
    DOCTEST_CHECK(schema.parameters[1].constraints.max == 10);
    DOCTEST_CHECK(schema.parameters[2].default_string == "Grüße");
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

    for (const char* name : {"config_schema.h", "config_schema.cpp"}) {
        std::ifstream first_file(first / name, std::ios::binary);
        std::ifstream second_file(second / name, std::ios::binary);
        const std::string first_content((std::istreambuf_iterator<char>(first_file)),
                                        std::istreambuf_iterator<char>());
        const std::string second_content((std::istreambuf_iterator<char>(second_file)),
                                         std::istreambuf_iterator<char>());
        DOCTEST_CHECK(first_content == second_content);
    }

    std::filesystem::remove_all(root);
}

}