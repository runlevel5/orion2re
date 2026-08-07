#include "pch.h"

#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

#include "config/config_loader.h"
#include "doctest/doctest.h"
#include "game/vfs.h"
#include "miniz.h"

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

        void Write_Zip(const fs::path& relative_path,
                       const std::vector<std::pair<std::string, std::string>>& entries) const {
            const fs::path full_path = root_path_ / relative_path;
            fs::create_directories(full_path.parent_path());
            mz_zip_archive archive{};
            mz_zip_zero_struct(&archive);
            DOCTEST_REQUIRE(mz_zip_writer_init_file(
                &archive, full_path.string().c_str(), 0) == MZ_TRUE);
            for (const auto& entry : entries) {
                DOCTEST_REQUIRE(mz_zip_writer_add_mem(
                    &archive, entry.first.c_str(), entry.second.data(),
                    entry.second.size(), MZ_BEST_COMPRESSION) == MZ_TRUE);
            }
            DOCTEST_REQUIRE(mz_zip_writer_finalize_archive(&archive) == MZ_TRUE);
            DOCTEST_REQUIRE(mz_zip_writer_end(&archive) == MZ_TRUE);
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

    std::string Manifest_(const std::string& id, const std::string& extra = {}) {
        return "{\n"
               "  id: \"" + id + "\",\n"
               "  name: \"" + id + "\",\n"
               "  version: \"1.0.0\",\n"
               "  requires: { engine: \">=1.60.0 <2.0.0\" },\n"
               "  config: \"config/main.cfg\",\n" +
               extra +
               "}\n";
    }

    std::string Read_Vfs_Text_(const char* path) {
        s_vfs_file* file = vfs::fopen(path, "rb");
        DOCTEST_REQUIRE(file != nullptr);
        const int32_t size = vfs::fsize(file);
        DOCTEST_REQUIRE(size >= 0);
        std::string result(static_cast<size_t>(size), '\0');
        if (!result.empty()) {
            DOCTEST_REQUIRE(vfs::fread(result.data(), 1, result.size(), file) ==
                            result.size());
        }
        DOCTEST_REQUIRE(vfs::fclose(file) == 0);
        return result;
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

DOCTEST_TEST_CASE("missing enabled package is an error and rolls back configuration") {
    s_config_test_directory files;
    files.Write("main.cfg",
        "music_volume = 80;\n"
        "enable balance_mod;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "package not found: 'balance_mod'");
    DOCTEST_CHECK(effective.state.values[1].integer_value == 50);
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
}

DOCTEST_TEST_CASE("first enable wins configuration and VFS resource precedence") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable first;\nenable second;\n");
    files.Write(fs::path("data") / "mods" / "first" / "mod.json5",
                Manifest_("first"));
    files.Write(fs::path("data") / "mods" / "first" / "config" / "main.cfg",
                "music_volume = 81;\n");
    files.Write(fs::path("data") / "mods" / "first" / "data" / "priority.dat",
                "first");
    files.Write(fs::path("data") / "mods" / "second" / "mod.json5",
                Manifest_("second"));
    files.Write(fs::path("data") / "mods" / "second" / "config" / "main.cfg",
                "music_volume = 62;\n");
    files.Write(fs::path("data") / "mods" / "second" / "data" / "priority.dat",
                "second");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 81);
    DOCTEST_CHECK(MOX::_settings.music_level == 81);
    DOCTEST_CHECK(Read_Vfs_Text_("priority.dat") == "first");
    DOCTEST_CHECK(effective.provenance[1].package_id == "first");
}

DOCTEST_TEST_CASE("supported community patch package ID can be enabled") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable ver150;\n");
    files.Write(fs::path("data") / "mods" / "ver150" / "mod.json5",
                Manifest_("ver150"));
    files.Write(fs::path("data") / "mods" / "ver150" / "config" / "main.cfg",
                "music_volume = 76;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 76);
    DOCTEST_CHECK(effective.provenance[1].package_id == "ver150");
}

DOCTEST_TEST_CASE("direct dependency is loaded once below its package") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable parent;\n");
    files.Write(fs::path("data") / "mods" / "parent" / "mod.json5",
                Manifest_("parent",
                    "  dependencies: [{ id: \"shared\", version: \">=1.0.0\" }],\n"));
    files.Write(fs::path("data") / "mods" / "parent" / "config" / "main.cfg",
                "music_volume = 77;\n");
    files.Write(fs::path("data") / "mods" / "shared" / "mod.json5",
                Manifest_("shared"));
    files.Write(fs::path("data") / "mods" / "shared" / "config" / "main.cfg",
                "music_volume = 55;\nsound_effects_volume = 42;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 77);
    DOCTEST_CHECK(effective.state.values[3].integer_value == 42);
    DOCTEST_CHECK(effective.provenance[1].package_id == "parent");
    DOCTEST_CHECK(effective.provenance[3].package_id == "shared");
}

DOCTEST_TEST_CASE("later explicit enable outranks dependencies of an earlier package") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable parent;\nenable explicit_mod;\n");
    files.Write(fs::path("data") / "mods" / "parent" / "mod.json5",
                Manifest_("parent",
                    "  dependencies: [{ id: \"dependency\", version: \"1.0.0\" }],\n"));
    files.Write(fs::path("data") / "mods" / "parent" / "config" / "main.cfg",
                "music_enabled = false;\n");
    files.Write(fs::path("data") / "mods" / "dependency" / "mod.json5",
                Manifest_("dependency"));
    files.Write(fs::path("data") / "mods" / "dependency" / "config" / "main.cfg",
                "music_volume = 51;\n");
    files.Write(fs::path("data") / "mods" / "dependency" / "data" / "priority.dat",
                "dependency");
    files.Write(fs::path("data") / "mods" / "explicit_mod" / "mod.json5",
                Manifest_("explicit_mod"));
    files.Write(fs::path("data") / "mods" / "explicit_mod" / "config" / "main.cfg",
                "music_volume = 69;\n");
    files.Write(fs::path("data") / "mods" / "explicit_mod" / "data" / "priority.dat",
                "explicit");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 69);
    DOCTEST_CHECK(effective.provenance[1].package_id == "explicit_mod");
    DOCTEST_CHECK(Read_Vfs_Text_("priority.dat") == "explicit");
}

DOCTEST_TEST_CASE("nested enable resolves a package and preserves parent priority") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable parent;\n");
    files.Write(fs::path("data") / "mods" / "parent" / "mod.json5",
                Manifest_("parent"));
    files.Write(fs::path("data") / "mods" / "parent" / "config" / "main.cfg",
                "enable nested;\nmusic_volume = 79;\n");
    files.Write(fs::path("data") / "mods" / "nested" / "mod.json5",
                Manifest_("nested"));
    files.Write(fs::path("data") / "mods" / "nested" / "config" / "main.cfg",
                "music_volume = 58;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 79);
}

DOCTEST_TEST_CASE("ZIP package manifest config and data use projected virtual paths") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable zipped;\n");
    files.Write_Zip(fs::path("data") / "mods" / "zipped.zip", {
        {"mod.json5", Manifest_("zipped")},
        {"config/main.cfg", "music_volume = 73;\n"},
        {"data/zipped.dat", "zip-resource"},
    });
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(effective.state.values[1].integer_value == 73);
    DOCTEST_CHECK(Read_Vfs_Text_("zipped.dat") == "zip-resource");
}

DOCTEST_TEST_CASE("incompatible engine manifest fails with semantic version diagnostic") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable future;\n");
    files.Write(fs::path("data") / "mods" / "future" / "mod.json5",
        "{ id: \"future\", name: \"Future\", version: \"1.0.0\",\n"
        "  requires: { engine: \">=2.0.0\" }, config: \"config/main.cfg\" }\n");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "package requires engine '>=2.0.0' but this engine is 1.60.0");
    DOCTEST_CHECK(diagnostic.location.source == "mods/future/mod.json5");
}

DOCTEST_TEST_CASE("dependency cycle fails without committing configuration or resources") {
    s_config_test_directory files;
    files.Write("main.cfg", "music_volume = 88;\nenable alpha;\n");
    files.Write(fs::path("data") / "mods" / "alpha" / "mod.json5",
                Manifest_("alpha",
                    "  dependencies: [{ id: \"beta\", version: \"1.0.0\" }],\n"));
    files.Write(fs::path("data") / "mods" / "alpha" / "config" / "main.cfg", "");
    files.Write(fs::path("data") / "mods" / "alpha" / "data" / "failed.dat", "bad");
    files.Write(fs::path("data") / "mods" / "beta" / "mod.json5",
                Manifest_("beta",
                    "  dependencies: [{ id: \"alpha\", version: \"1.0.0\" }],\n"));
    files.Write(fs::path("data") / "mods" / "beta" / "config" / "main.cfg", "");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "mod dependency cycle: alpha -> beta -> alpha");
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
    DOCTEST_CHECK(vfs::File_Exists("failed.dat") == 0);
}

DOCTEST_TEST_CASE("conflicting packages fail atomically") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable alpha;\nenable beta;\n");
    files.Write(fs::path("data") / "mods" / "alpha" / "mod.json5",
                Manifest_("alpha", "  conflicts: [\"beta\"],\n"));
    files.Write(fs::path("data") / "mods" / "alpha" / "config" / "main.cfg", "");
    files.Write(fs::path("data") / "mods" / "beta" / "mod.json5",
                Manifest_("beta"));
    files.Write(fs::path("data") / "mods" / "beta" / "config" / "main.cfg", "");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "package 'alpha' conflicts with active package 'beta'");
}

DOCTEST_TEST_CASE("optional missing dependency is skipped but an installed one is validated") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable parent;\n");
    files.Write(fs::path("data") / "mods" / "parent" / "mod.json5",
                Manifest_("parent",
                    "  optional_dependencies: [{ id: \"optional\", version: \">=2.0.0\" }],\n"));
    files.Write(fs::path("data") / "mods" / "parent" / "config" / "main.cfg",
                "music_volume = 64;\n");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(MOX::_settings.music_level == 64);
}

DOCTEST_TEST_CASE("dependency version mismatch is fatal") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable parent;\n");
    files.Write(fs::path("data") / "mods" / "parent" / "mod.json5",
                Manifest_("parent",
                    "  dependencies: [{ id: \"old\", version: \">=2.0.0\" }],\n"));
    files.Write(fs::path("data") / "mods" / "parent" / "config" / "main.cfg", "");
    files.Write(fs::path("data") / "mods" / "old" / "mod.json5", Manifest_("old"));
    files.Write(fs::path("data") / "mods" / "old" / "config" / "main.cfg", "");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "package 'old' version 1.0.0 does not satisfy '>=2.0.0'");
}

DOCTEST_TEST_CASE("malformed duplicate and mismatched manifests report distinct errors") {
    s_config_test_directory files;
    files.Write("malformed.cfg", "enable malformed;\n");
    files.Write("duplicate.cfg", "enable duplicate;\n");
    files.Write("mismatch.cfg", "enable mismatch;\n");
    files.Write("capabilities.cfg", "enable obsolete;\n");
    files.Write(fs::path("data") / "mods" / "malformed" / "mod.json5", "{ id:");
    files.Write(fs::path("data") / "mods" / "duplicate" / "mod.json5",
        "{ id: \"duplicate\", id: \"duplicate\", name: \"Duplicate\",\n"
        "  version: \"1.0.0\", requires: { engine: \"1.60.0\" },\n"
        "  config: \"config/main.cfg\" }\n");
    files.Write(fs::path("data") / "mods" / "mismatch" / "mod.json5",
        "{ id: \"other\", name: \"Other\", version: \"1.0.0\",\n"
        "  requires: { engine: \"1.60.0\" }, config: \"config/main.cfg\" }\n");
    files.Write(fs::path("data") / "mods" / "obsolete" / "mod.json5",
        "{ id: \"obsolete\", name: \"Obsolete\", version: \"1.0.0\",\n"
        "  requires: { engine: \"1.60.0\" }, config: \"config/main.cfg\",\n"
        "  capabilities: [\"configuration\"] }\n");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "malformed.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message.find("invalid mod manifest:") == 0);
    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "duplicate.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "duplicate manifest property 'id'");
    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "mismatch.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "manifest ID 'other' does not match package 'mismatch'");
    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "capabilities.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "unknown manifest property 'capabilities'");
}

DOCTEST_TEST_CASE("load after constraint rejects contradictory explicit priority") {
    s_config_test_directory files;
    files.Write("main.cfg", "enable first;\nenable second;\n");
    files.Write(fs::path("data") / "mods" / "first" / "mod.json5",
                Manifest_("first"));
    files.Write(fs::path("data") / "mods" / "first" / "config" / "main.cfg", "");
    files.Write(fs::path("data") / "mods" / "second" / "mod.json5",
                Manifest_("second", "  load_after: [\"first\"],\n"));
    files.Write(fs::path("data") / "mods" / "second" / "config" / "main.cfg", "");
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "main.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message == "package 'second' must load after 'first'");
}

DOCTEST_TEST_CASE("failed reload preserves previously committed package layers and values") {
    s_config_test_directory files;
    files.Write("valid.cfg", "enable stable;\n");
    files.Write("invalid.cfg", "enable broken;\n");
    files.Write(fs::path("data") / "mods" / "stable" / "mod.json5",
                Manifest_("stable"));
    files.Write(fs::path("data") / "mods" / "stable" / "config" / "main.cfg",
                "music_volume = 74;\n");
    files.Write(fs::path("data") / "mods" / "stable" / "data" / "stable.dat", "stable");
    files.Write(fs::path("data") / "mods" / "broken" / "mod.json5",
                Manifest_("broken"));
    files.Write(fs::path("data") / "mods" / "broken" / "config" / "main.cfg",
                "music_volume = 101;\n");
    files.Write(fs::path("data") / "mods" / "broken" / "data" / "broken.dat", "broken");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "valid.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(Read_Vfs_Text_("stable.dat") == "stable");
    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "invalid.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(MOX::_settings.music_level == 74);
    DOCTEST_CHECK(Read_Vfs_Text_("stable.dat") == "stable");
    DOCTEST_CHECK(vfs::File_Exists("broken.dat") == 0);
}

DOCTEST_TEST_CASE("successful reload rebuilds configuration and package layers from defaults") {
    s_config_test_directory files;
    files.Write("modded.cfg", "enable stable;\n");
    files.Write("plain.cfg", "sound_effects_volume = 31;\n");
    files.Write(fs::path("data") / "mods" / "stable" / "mod.json5",
                Manifest_("stable"));
    files.Write(fs::path("data") / "mods" / "stable" / "config" / "main.cfg",
                "music_volume = 74;\n");
    files.Write(fs::path("data") / "mods" / "stable" / "data" / "stable.dat", "stable");
    files.Initialize_Vfs();
    Set_Native_Defaults_();
    config::s_effective_config effective;
    config::Read_Effective_Native_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_REQUIRE(config::Load_Config_File(
        "modded.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(MOX::_settings.music_level == 74);
    DOCTEST_CHECK(Read_Vfs_Text_("stable.dat") == "stable");

    DOCTEST_REQUIRE(config::Load_Config_File(
        "plain.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(MOX::_settings.music_level == 50);
    DOCTEST_CHECK(MOX::_settings.sound_fx_level == 31);
    DOCTEST_CHECK(vfs::File_Exists("stable.dat") == 0);
    DOCTEST_CHECK_FALSE(effective.provenance[1].has_override);
    DOCTEST_CHECK(effective.prior_values[1].empty());
}

DOCTEST_TEST_CASE("ambiguous package forms and unsafe ZIP paths are fatal") {
    s_config_test_directory files;
    files.Write("ambiguous.cfg", "enable ambiguous;\n");
    files.Write("unsafe.cfg", "enable unsafe;\n");
    files.Write(fs::path("data") / "mods" / "ambiguous" / "mod.json5",
                Manifest_("ambiguous"));
    files.Write_Zip(fs::path("data") / "mods" / "ambiguous.zip",
                    {{"mod.json5", Manifest_("ambiguous")}});
    files.Write_Zip(fs::path("data") / "mods" / "unsafe.zip", {
        {"mod.json5", Manifest_("unsafe")},
        {"../outside.dat", "unsafe"},
    });
    files.Initialize_Vfs();
    config::s_effective_config effective;
    config::Initialize_Effective_Config(&effective);
    config::s_cfg_diagnostic diagnostic;

    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "ambiguous.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "package 'ambiguous' has both directory and ZIP forms");
    DOCTEST_CHECK_FALSE(config::Load_Config_File(
        "unsafe.cfg", "base_game", &effective, &diagnostic));
    DOCTEST_CHECK(diagnostic.message ==
        "unsafe path in package ZIP: '../outside.dat'");
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