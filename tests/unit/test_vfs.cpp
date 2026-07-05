#include "pch.h"

#include "doctest/doctest.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "game/vfs.h"

#include "miniz.h"

namespace test_vfs_detail {
    namespace fs = std::filesystem;

    class Scoped_Vfs_Test_Directory {
      public:
        Scoped_Vfs_Test_Directory() {
            const auto unique_id = std::chrono::steady_clock::now().time_since_epoch().count();
            root_path_ = fs::temp_directory_path() / ("orion2re-vfs-" + std::to_string(unique_id));
            fs::create_directories(root_path_);
        }

        ~Scoped_Vfs_Test_Directory() {
            vfs::Shutdown();
            std::error_code error;
            fs::remove_all(root_path_, error);
        }

        [[nodiscard]] const fs::path& root_path() const {
            return root_path_;
        }

        void Write_File(const fs::path& relative_path, const std::string& contents) const {
            const fs::path full_path = root_path_ / relative_path;
            fs::create_directories(full_path.parent_path());

            std::ofstream output(full_path, std::ios::binary);
            DOCTEST_REQUIRE(output.good());
            output.write(contents.data(), (std::streamsize)contents.size());
            DOCTEST_REQUIRE(output.good());
        }

        void Write_Zip_File(const fs::path& relative_path, const std::vector<std::pair<std::string, std::string>>& entries) const {
            const fs::path full_path = root_path_ / relative_path;
            mz_zip_archive archive{};

            fs::create_directories(full_path.parent_path());

            mz_zip_zero_struct(&archive);
            DOCTEST_REQUIRE(mz_zip_writer_init_file(&archive, full_path.string().c_str(), 0) == MZ_TRUE);

            for (const auto& entry : entries) {
                DOCTEST_REQUIRE(mz_zip_writer_add_mem(
                    &archive,
                    entry.first.c_str(),
                    entry.second.data(),
                    entry.second.size(),
                    MZ_BEST_COMPRESSION) == MZ_TRUE);
            }

            DOCTEST_REQUIRE(mz_zip_writer_finalize_archive(&archive) == MZ_TRUE);
            DOCTEST_REQUIRE(mz_zip_writer_end(&archive) == MZ_TRUE);
        }

        void Write_Lbx_File(const fs::path& relative_path, const std::vector<std::string>& entries) const {
            const fs::path full_path = root_path_ / relative_path;
            s_lbx_disk_header header{};
            uint32_t offsets[510] = {};
            uint32_t next_offset = sizeof(header) + sizeof(offsets);

            fs::create_directories(full_path.parent_path());

            header.entry_count = (uint16_t)entries.size();
            header.magic = 0xFEAD;
            header.header_data = 0;
            offsets[0] = next_offset;

            for (size_t i = 0; i < entries.size(); ++i) {
                next_offset += (uint32_t)entries[i].size();
                offsets[i + 1] = next_offset;
            }

            std::ofstream output(full_path, std::ios::binary);
            DOCTEST_REQUIRE(output.good());
            output.write((const char*)&header, sizeof(header));
            output.write((const char*)offsets, sizeof(offsets));
            for (const std::string& entry : entries) {
                output.write(entry.data(), (std::streamsize)entry.size());
            }
            DOCTEST_REQUIRE(output.good());
        }

      private:
        fs::path root_path_;
    };
}

DOCTEST_TEST_SUITE("vfs cpp") {

DOCTEST_TEST_CASE("vfs requires initialization before assets can be opened") {
    vfs::Shutdown();

    DOCTEST_CHECK(vfs::File_Exists("sample.dat") == 0);
    DOCTEST_CHECK(vfs::fopen("sample.dat", "rb") == nullptr);
}

DOCTEST_TEST_CASE("vfs raw filesystem backend opens and reads files from the configured base path") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File("assets.bin", "abcdef");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_CHECK(vfs::File_Exists("assets.bin") == 1);

    s_vfs_file* file = vfs::fopen("assets.bin", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 8> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 3, file) == 3);
    DOCTEST_CHECK(std::string(buffer.data(), 3) == "abc");
    DOCTEST_CHECK(vfs::ftell(file) == 3);

    DOCTEST_CHECK(vfs::fseek(file, -2, SEEK_END) == 0);
    DOCTEST_CHECK(vfs::ftell(file) == 4);
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 2, file) == 2);
    DOCTEST_CHECK(std::string(buffer.data(), 2) == "ef");

    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs gives data overrides precedence over raw files") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File("voices.dat", "raw");
    test_dir.Write_File(test_vfs_detail::fs::path("data") / "overrides" / "voices.dat", "override");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen("voices.dat", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 8, file) == 8);
    DOCTEST_CHECK(std::string(buffer.data(), 8) == "override");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs normalizes relative asset paths across nested directories") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File(test_vfs_detail::fs::path("nested") / "sample.bin", "12345");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_CHECK(vfs::File_Exists("nested\\sample.bin") == 1);

    s_vfs_file* file = vfs::fopen("./nested/sample.bin", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 8> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 2, 3, file) == 2);
    DOCTEST_CHECK(std::string(buffer.data(), 4) == "1234");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs resolves raw file names case-insensitively") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File("FONTS.LBX", "font-data");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_CHECK(vfs::File_Exists("fonts.lbx") == 1);

    s_vfs_file* file = vfs::fopen("fonts.lbx", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 8, file) == 8);
    DOCTEST_CHECK(std::string(buffer.data(), 8) == "font-dat");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs can fall back to a secondary raw asset path without affecting local override lookup") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File(test_vfs_detail::fs::path("cdroot") / "diplomat.lbx", "cd-asset");
    test_dir.Write_File(test_vfs_detail::fs::path("data") / "overrides" / "diplomat.lbx", "override");

    const std::string base_path = test_dir.root_path().string();
    const std::string fallback_path = (test_dir.root_path() / "cdroot").string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_REQUIRE(vfs::Set_Raw_Fallback_Path(fallback_path.c_str()) == 1);

    char resolved_path[1024] = {};
    DOCTEST_CHECK(vfs::Resolve_Read_Path_No_Zip("diplomat.lbx", resolved_path, sizeof(resolved_path)) == 1);

    s_vfs_file* file = vfs::fopen("diplomat.lbx", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 8, file) == 8);
    DOCTEST_CHECK(std::string(buffer.data(), 8) == "override");
    DOCTEST_CHECK(vfs::fclose(file) == 0);

    test_dir.Write_File("raw-only.dat", "local-raw");
    DOCTEST_CHECK(vfs::File_Exists("raw-only.dat") == 1);
}

DOCTEST_TEST_CASE("vfs resolves raw assets from the secondary fallback path when no local asset exists") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File(test_vfs_detail::fs::path("cdroot") / "simtex.lbx", "cinematic");

    const std::string base_path = test_dir.root_path().string();
    const std::string fallback_path = (test_dir.root_path() / "cdroot").string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_REQUIRE(vfs::Set_Raw_Fallback_Path(fallback_path.c_str()) == 1);

    char resolved_path[1024] = {};
    DOCTEST_CHECK(vfs::Resolve_Read_Path_No_Zip("simtex.lbx", resolved_path, sizeof(resolved_path)) == 1);
    DOCTEST_CHECK(vfs::File_Exists("simtex.lbx") == 1);

    s_vfs_file* file = vfs::fopen("simtex.lbx", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 9, file) == 9);
    DOCTEST_CHECK(std::string(buffer.data(), 9) == "cinematic");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs rejects write capable modes and out of range seeks") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File("sample.dat", "hello");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    DOCTEST_CHECK(vfs::fopen("sample.dat", "wb") == nullptr);
    DOCTEST_CHECK(vfs::fopen("sample.dat", "ab") == nullptr);
    DOCTEST_CHECK(vfs::fopen("sample.dat", "r+") == nullptr);
    DOCTEST_CHECK(vfs::fopen("sample.dat", "rb+") == nullptr);

    s_vfs_file* file = vfs::fopen("sample.dat", "rb");
    DOCTEST_REQUIRE(file != nullptr);
    DOCTEST_CHECK(vfs::fseek(file, -1, SEEK_SET) == -1);
    DOCTEST_CHECK(vfs::fseek(file, 6, SEEK_SET) == -1);
    DOCTEST_CHECK(vfs::ftell(file) == 0);
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs opens zip-backed assets and supports backward seek by recreating the stream") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-assets.zip", {
        {"music/theme.dat", "zip-theme"},
    });

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_CHECK(vfs::File_Exists("music/theme.dat") == 1);

    s_vfs_file* file = vfs::fopen("music/theme.dat", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 4, file) == 4);
    DOCTEST_CHECK(std::string(buffer.data(), 4) == "zip-");
    DOCTEST_CHECK(vfs::fseek(file, 0, SEEK_SET) == 0);
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 9, file) == 9);
    DOCTEST_CHECK(std::string(buffer.data(), 9) == "zip-theme");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs prefers lexicographically earlier zip files over later zips and raw files") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_File("intro.dat", "raw-intro");
    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "20-later.zip", {
        {"intro.dat", "later"},
    });
    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-earlier.zip", {
        {"intro.dat", "earlier"},
    });

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen("intro.dat", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 7, file) == 7);
    DOCTEST_CHECK(std::string(buffer.data(), 7) == "earlier");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs loose overrides still win over zip-backed assets") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-assets.zip", {
        {"voices/alert.dat", "zip-alert"},
    });
    test_dir.Write_File(test_vfs_detail::fs::path("data") / "overrides" / "voices" / "alert.dat", "loose-alert");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen("voices/alert.dat", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 11, file) == 11);
    DOCTEST_CHECK(std::string(buffer.data(), 11) == "loose-alert");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs resolves zip entry paths case-insensitively") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-assets.zip", {
        {"MUSIC/THEME.DAT", "zip-theme"},
    });

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);
    DOCTEST_CHECK(vfs::File_Exists("music/theme.dat") == 1);

    s_vfs_file* file = vfs::fopen("music/theme.dat", "rb");
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 9, file) == 9);
    DOCTEST_CHECK(std::string(buffer.data(), 9) == "zip-theme");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs fopen_lbx returns an entry-relative stream from a raw lbx") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Lbx_File("TEST.LBX", {"zero", "entry-one"});

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen_lbx("TEST.LBX", 1);
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 5, file) == 5);
    DOCTEST_CHECK(std::string(buffer.data(), 5) == "entry");
    DOCTEST_CHECK(vfs::ftell(file) == 5);
    DOCTEST_CHECK(vfs::fseek(file, 0, SEEK_SET) == 0);
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 9, file) == 9);
    DOCTEST_CHECK(std::string(buffer.data(), 9) == "entry-one");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs fopen_lbx prefers per-entry loose override over whole lbx replacement from zip") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Lbx_File("TEST.LBX", {"raw-zero", "raw-one"});

    test_dir.Write_Lbx_File("WHOLEZIP.LBX", {"whole-zero", "whole-one"});
    const test_vfs_detail::fs::path whole_zip_lbx_path = test_dir.root_path() / "WHOLEZIP.LBX";
    std::ifstream whole_zip_lbx(whole_zip_lbx_path, std::ios::binary);
    std::string whole_zip_lbx_contents((std::istreambuf_iterator<char>(whole_zip_lbx)), std::istreambuf_iterator<char>());
    DOCTEST_REQUIRE(!whole_zip_lbx_contents.empty());

    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-whole.zip", {
        {"TEST.LBX", whole_zip_lbx_contents},
    });
    test_dir.Write_File(test_vfs_detail::fs::path("data") / "overrides" / "TEST.LBX" / "001", "entry-override");

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen_lbx("TEST.LBX", 1);
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 14, file) == 14);
    DOCTEST_CHECK(std::string(buffer.data(), 14) == "entry-override");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs fopen_lbx falls back to whole lbx replacement from zip") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Lbx_File("TEST.LBX", {"raw-zero", "raw-one"});
    test_dir.Write_Lbx_File("ZIPTEST.LBX", {"zip-zero", "zip-one"});

    const test_vfs_detail::fs::path zip_lbx_path = test_dir.root_path() / "ZIPTEST.LBX";
    std::ifstream zip_lbx(zip_lbx_path, std::ios::binary);
    std::string zip_lbx_contents((std::istreambuf_iterator<char>(zip_lbx)), std::istreambuf_iterator<char>());
    DOCTEST_REQUIRE(!zip_lbx_contents.empty());

    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-assets.zip", {
        {"TEST.LBX", zip_lbx_contents},
    });

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen_lbx("TEST.LBX", 0);
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 16> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 8, file) == 8);
    DOCTEST_CHECK(std::string(buffer.data(), 8) == "zip-zero");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

DOCTEST_TEST_CASE("vfs fopen_lbx prefers zip per-entry override over whole lbx zip entry") {
    test_vfs_detail::Scoped_Vfs_Test_Directory test_dir;
    test_dir.Write_Lbx_File("ZIPTEST2.LBX", {"whole-zero", "whole-one"});

    const test_vfs_detail::fs::path zip_lbx_path = test_dir.root_path() / "ZIPTEST2.LBX";
    std::ifstream zip_lbx(zip_lbx_path, std::ios::binary);
    std::string zip_lbx_contents((std::istreambuf_iterator<char>(zip_lbx)), std::istreambuf_iterator<char>());
    DOCTEST_REQUIRE(!zip_lbx_contents.empty());

    test_dir.Write_Zip_File(test_vfs_detail::fs::path("data") / "10-assets.zip", {
        {"TEST2.LBX", zip_lbx_contents},
        {"TEST2.LBX/001", "zip-entry-override"},
    });

    const std::string base_path = test_dir.root_path().string();
    DOCTEST_REQUIRE(vfs::Init(base_path.c_str()) == 1);

    s_vfs_file* file = vfs::fopen_lbx("TEST2.LBX", 1);
    DOCTEST_REQUIRE(file != nullptr);

    std::array<char, 32> buffer{};
    DOCTEST_CHECK(vfs::fread(buffer.data(), 1, 18, file) == 18);
    DOCTEST_CHECK(std::string(buffer.data(), 18) == "zip-entry-override");
    DOCTEST_CHECK(vfs::fclose(file) == 0);
}

}