#include "pch.h"

#include <chrono>
#include <filesystem>
#include <fstream>

#include "config/config_loader.h"
#include "doctest/doctest.h"
#include "game/vfs.h"

namespace {
    namespace fs = std::filesystem;

    class s_config_test_directory {
      public:
        s_config_test_directory() {
            const auto unique_id = std::chrono::steady_clock::now().time_since_epoch().count();
            root_path_ = fs::temp_directory_path() /
                         ("orion2re-config-loader-" + std::to_string(unique_id));
            fs::create_directories(root_path_);
        }

        ~s_config_test_directory() {
            vfs::Shutdown();
            std::error_code error;
            fs::remove_all(root_path_, error);
        }

        void Initialize_Vfs() const {
            const std::string root = root_path_.string();
            DOCTEST_REQUIRE(vfs::Init(root.c_str()) == 1);
        }

        void Write(const fs::path& relative_path, const std::string& contents) const {
            const fs::path full_path = root_path_ / relative_path;
            fs::create_directories(full_path.parent_path());
            std::ofstream output(full_path, std::ios::binary);
            DOCTEST_REQUIRE(output.good());
            output.write(contents.data(), static_cast<std::streamsize>(contents.size()));
            DOCTEST_REQUIRE(output.good());
        }

      private:
        fs::path root_path_;
    };

    void Set_Native_Defaults_() {
        MOX::_settings.music_on = 1;
        MOX::_settings.music_level = 50;
        MOX::_settings.sound_fx_on = 1;
        MOX::_settings.sound_fx_level = 50;
    }
}

DOCTEST_TEST_CASE("cycle detection follows case-insensitive VFS path identity") {
    s_config_test_directory files;
    files.Write("Config/A.cfg", "include a.CFG;\n");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "CONFIG/A.CFG", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "include cycle: config/a.cfg -> config/a.cfg");
    DOCTEST_CHECK(diagnostic.location.source == "config/a.cfg");
}

DOCTEST_TEST_SUITE("configuration include loader") {

DOCTEST_TEST_CASE("nested includes resolve relative paths in statement order") {
    s_config_test_directory files;
    files.Write("config/main.cfg",
        "music_volume = 60;\n"
        "include balance/first.cfg;\n"
        "music_enabled = false;\n");
    files.Write("config/balance/first.cfg",
        "sound_effects_volume = 25;\n"
        "include ../shared.cfg;\n"
        "music_volume = 80;\n");
    files.Write("config/shared.cfg", "music_volume = 70;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "config/main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK_FALSE(effective.state.values[0].bool_value);
    DOCTEST_CHECK(effective.state.values[1].integer_value == 80);
    DOCTEST_CHECK(effective.state.values[3].integer_value == 25);
    DOCTEST_CHECK(MOX::_settings.music_level == 80);

    config::t_variable_id music_index = 0;
    DOCTEST_REQUIRE(config::Find_Variable(
        "music_volume", std::strlen("music_volume"), &music_index));
    DOCTEST_CHECK(effective.provenance[music_index].location.source ==
                  "config/balance/first.cfg");
    DOCTEST_CHECK(effective.provenance[music_index].location.line == 3);
    DOCTEST_CHECK(effective.provenance[music_index].statement == 3);
    DOCTEST_REQUIRE(effective.prior_values[music_index].size() == 3);
    DOCTEST_CHECK(effective.prior_values[music_index][0].value.integer_value == 50);
    DOCTEST_CHECK(effective.prior_values[music_index][1].value.integer_value == 60);
    DOCTEST_CHECK(effective.prior_values[music_index][2].value.integer_value == 70);
}

DOCTEST_TEST_CASE("stop ends only the included file") {
    s_config_test_directory files;
    files.Write("main.cfg",
        "include child.cfg;\n"
        "music_volume = 75;\n");
    files.Write("child.cfg",
        "sound_effects_volume = 30;\n"
        "stop;\n"
        "sound_effects_volume = 99;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 75);
    DOCTEST_CHECK(effective.state.values[3].integer_value == 30);
}

DOCTEST_TEST_CASE("include cycles report the normalized include chain") {
    s_config_test_directory files;
    files.Write("config/a.cfg", "include sub/b.cfg;\n");
    files.Write("config/sub/b.cfg", "include ../a.cfg;\n");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "config/a.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "include cycle: config/a.cfg -> config/sub/b.cfg -> config/a.cfg");
    DOCTEST_CHECK(diagnostic.location.source == "config/sub/b.cfg");
    DOCTEST_CHECK(diagnostic.location.line == 1);
}

DOCTEST_TEST_CASE("missing and malformed includes are distinct and roll back") {
    s_config_test_directory files;
    files.Write("missing-main.cfg",
        "music_volume = 80;\n"
        "include absent.cfg;\n");
    files.Write("malformed-main.cfg",
        "music_volume = 80;\n"
        "include malformed.cfg;\n");
    files.Write("malformed.cfg", "sound_effects_volume = 0x;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "missing-main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
                  "configuration file not found: 'absent.cfg'");
    DOCTEST_CHECK(diagnostic.location.source == "missing-main.cfg");
    DOCTEST_CHECK(effective.state.values[1].integer_value == 50);
    DOCTEST_CHECK(MOX::_settings.music_level == 50);

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "malformed-main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "malformed numeric value '0x'");
    DOCTEST_CHECK(diagnostic.location.source == "malformed.cfg");
    DOCTEST_CHECK(effective.state.values[1].integer_value == 50);
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
}

DOCTEST_TEST_CASE("invalid include paths cannot escape the VFS root") {
    s_config_test_directory files;
    files.Write("config/main.cfg", "include ../../outside.cfg;\n");
    files.Write("absolute.cfg", "include /outside.cfg;\n");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "config/main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "invalid include path '../../outside.cfg'");

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "absolute.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "invalid include path '/outside.cfg'");
}

DOCTEST_TEST_CASE("deferred enable in an include graph is ignored") {
    s_config_test_directory files;
    files.Write("main.cfg",
        "music_volume = 80;\n"
        "enable balance_mod;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 80);
    DOCTEST_CHECK(MOX::_settings.music_level == 80);
}

DOCTEST_TEST_CASE("startup config uses defaults when absent") {
    s_config_test_directory files;
    files.Initialize_Vfs();
    MOX::_settings.music_on = 0;
    MOX::_settings.music_level = 1;
    MOX::_settings.sound_fx_on = 0;
    MOX::_settings.sound_fx_level = 2;
    config::s_effective_config effective;
    config::s_cfg_diagnostic diagnostic;
    bool loaded = true;

    DOCTEST_REQUIRE(config::Load_Startup_Config(
        "orion2re.cfg", "base_game", &effective, &loaded, &diagnostic));
    DOCTEST_CHECK_FALSE(loaded);
    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 1);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 50);
}

DOCTEST_TEST_CASE("invalid startup config preserves native state") {
    s_config_test_directory files;
    files.Write("orion2re.cfg",
        "music_volume = 80;\n"
        "sound_effects_volume = 101;\n");
    files.Initialize_Vfs();
    MOX::_settings.music_on = 0;
    MOX::_settings.music_level = 7;
    MOX::_settings.sound_fx_on = 0;
    MOX::_settings.sound_fx_level = 9;
    config::s_effective_config effective;
    config::s_cfg_diagnostic diagnostic;
    bool loaded = true;

    DOCTEST_CHECK_FALSE(config::Load_Startup_Config(
        "orion2re.cfg", "base_game", &effective, &loaded, &diagnostic));
    DOCTEST_CHECK_FALSE(loaded);
    DOCTEST_CHECK(diagnostic.location.source == "orion2re.cfg");
    DOCTEST_CHECK(diagnostic.location.line == 2);
    DOCTEST_CHECK(MOX::_settings.music_on == 0);
    DOCTEST_CHECK(MOX::_settings.music_level == 7);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 0);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 9);
}

DOCTEST_TEST_CASE("valid startup config commits defaults and overrides together") {
    s_config_test_directory files;
    files.Write("orion2re.cfg", "music_volume = 75;\n");
    files.Initialize_Vfs();
    MOX::_settings.music_on = 0;
    MOX::_settings.music_level = 7;
    MOX::_settings.sound_fx_on = 0;
    MOX::_settings.sound_fx_level = 9;
    config::s_effective_config effective;
    config::s_cfg_diagnostic diagnostic;
    bool loaded = false;

    DOCTEST_REQUIRE(config::Load_Startup_Config(
        "orion2re.cfg", "base_game", &effective, &loaded, &diagnostic));
    DOCTEST_CHECK(loaded);
    DOCTEST_CHECK(MOX::_settings.music_on == 1);
    DOCTEST_CHECK(MOX::_settings.music_level == 75);
    DOCTEST_CHECK(MOX::_settings.sound_fx_on == 1);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 50);
}

}