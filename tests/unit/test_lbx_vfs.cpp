#include "pch.h"

#include "doctest/doctest.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "game/lbx.h"
#include "game/vfs.h"

#include "miniz.h"

namespace sound {
    extern uint8_t _close_stream;
    extern uint8_t _musicActive;
}

namespace test_lbx_vfs_detail {
    namespace fs = std::filesystem;

    class Scoped_Test_Directory {
      public:
        Scoped_Test_Directory() {
            const auto unique_id = std::chrono::steady_clock::now().time_since_epoch().count();
            root_path_ = fs::temp_directory_path() / ("orion2re-lbx-" + std::to_string(unique_id));
            fs::create_directories(root_path_);
        }

        ~Scoped_Test_Directory() {
            vfs::Shutdown();
            std::error_code error;
            fs::remove_all(root_path_, error);
        }

        [[nodiscard]] const fs::path& root_path() const {
            return root_path_;
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

        void Write_Zip_File(const fs::path& relative_path, const std::vector<std::pair<std::string, std::string>>& entries) const {
            const fs::path full_path = root_path_ / relative_path;
            mz_zip_archive archive{};

            fs::create_directories(full_path.parent_path());
            mz_zip_zero_struct(&archive);
            DOCTEST_REQUIRE(mz_zip_writer_init_file(&archive, full_path.string().c_str(), 0) == MZ_TRUE);

            for (const auto& entry : entries) {
                DOCTEST_REQUIRE(mz_zip_writer_add_mem(&archive, entry.first.c_str(), entry.second.data(), entry.second.size(), MZ_BEST_COMPRESSION) == MZ_TRUE);
            }

            DOCTEST_REQUIRE(mz_zip_writer_finalize_archive(&archive) == MZ_TRUE);
            DOCTEST_REQUIRE(mz_zip_writer_end(&archive) == MZ_TRUE);
        }

        std::string Read_File_Contents(const fs::path& relative_path) const {
            std::ifstream input(root_path_ / relative_path, std::ios::binary);
            return std::string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        }

      private:
        fs::path root_path_;
    };
}

DOCTEST_TEST_SUITE("lbx vfs") {

DOCTEST_TEST_CASE("lbx init and read record use case-insensitive raw vfs lookup") {
    test_lbx_vfs_detail::Scoped_Test_Directory test_dir;
    test_dir.Write_Lbx_File("FONTS.LBX", {"zero", "entry-one"});

    DOCTEST_REQUIRE(vfs::Init(test_dir.root_path().string().c_str()) == 1);

    s_lbx_stream_header header{};
    lbx::LBX_init_(&header, "fonts.lbx");

    DOCTEST_REQUIRE(header.file != nullptr);
    DOCTEST_CHECK(header.entry_count == 2);
    DOCTEST_CHECK(lbx::LBX_getRecordLength_(&header, 1) == 9);

    std::array<char, 16> buffer{};
    lbx::LBX_readRecord_(&header, 1, buffer.data());
    DOCTEST_CHECK(std::string(buffer.data(), 9) == "entry-one");

    lbx::LBX_uninit_(&header);
    DOCTEST_CHECK(header.file == nullptr);
}

DOCTEST_TEST_CASE("lbx init stream can open whole lbx replacement from zip") {
    test_lbx_vfs_detail::Scoped_Test_Directory test_dir;
    test_dir.Write_Lbx_File("STREAMSRC.LBX", {"zip-zero", "zip-one"});
    const std::string zip_lbx = test_dir.Read_File_Contents("STREAMSRC.LBX");
    test_dir.Write_Zip_File(test_lbx_vfs_detail::fs::path("data") / "10-stream.zip", {
        {"stream.lbx", zip_lbx},
    });

    DOCTEST_REQUIRE(vfs::Init(test_dir.root_path().string().c_str()) == 1);

    sound::_musicActive = 1;
    sound::_close_stream = 1;

    s_wave_stream_header stream{};
    lbx::LBX_init_stream_(&stream, "STREAM.LBX");

    DOCTEST_REQUIRE(stream.file != nullptr);
    DOCTEST_CHECK(stream.entry_count == 2);
    DOCTEST_CHECK(stream.entries[0].size == 8);
    DOCTEST_CHECK(sound::_musicActive == 1);
}

}