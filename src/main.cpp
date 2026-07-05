#include "pch.h"

#include <SDL3/SDL_main.h>

#if defined(ORION2RE_ASAN_BUILD)
extern "C" const char* __asan_default_options() {
    return "log_path=logs/asan";
}

static void Build_Path_(char* output, size_t output_size, const char* base_path, const char* relative_path) {
    if (base_path != nullptr && base_path[0] != '\0') {
        snprintf(output, output_size, "%s%s", base_path, relative_path);
    } else {
        snprintf(output, output_size, "%s", relative_path);
    }
}

static void Set_Environment_Variable_If_Unset_(const char* name, const char* value) {
    if (SDL_getenv(name) != nullptr) {
        return;
    }

    SDL_setenv_unsafe(name, value, 0);
}

static void Configure_Asan_Runtime_() {
    const char* base_path = SDL_GetBasePath();
    char logs_path[1024];

    Build_Path_(logs_path, sizeof(logs_path), base_path, "logs");
    SDL_CreateDirectory(logs_path);

#if defined(_WIN32)
    char dumps_path[1024];
    char dump_file_path[1024];

    Build_Path_(dumps_path, sizeof(dumps_path), base_path, "dumps");
    SDL_CreateDirectory(dumps_path);
    Build_Path_(dump_file_path, sizeof(dump_file_path), base_path, "dumps\\orion2re.dmp");
    Set_Environment_Variable_If_Unset_("ASAN_SAVE_DUMPS", dump_file_path);
#endif
}
#endif

int main(int argc, char** argv) {
#if defined(ORION2RE_ASAN_BUILD)
    Configure_Asan_Runtime_();
#endif

    char* current_directory = SDL_GetCurrentDirectory();
    const char* base_path = SDL_GetBasePath();
    const char* vfs_base_path = current_directory != nullptr ? current_directory : base_path;

    if (vfs::Init(vfs_base_path) == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "main: vfs::Init failed for base path '%s'", vfs_base_path != nullptr ? vfs_base_path : "");
        if (current_directory != nullptr) {
            SDL_free(current_directory);
        }
        return -1;
    }

    size_t command_line_length = 0;
    for (int i = 1; i < argc; ++i) {
        command_line_length += strlen(argv[i]);
        if (i + 1 < argc) {
            ++command_line_length;
        }
    }

    char* command_line = nullptr;
    if (command_line_length != 0) {
        command_line = (char*)malloc(command_line_length + 1);
        if (command_line == nullptr) {
            return -1;
        }

        size_t write_offset = 0;
        for (int i = 1; i < argc; ++i) {
            const size_t arg_length = strlen(argv[i]);
            memcpy(command_line + write_offset, argv[i], arg_length);
            write_offset += arg_length;

            if (i + 1 < argc) {
                command_line[write_offset++] = ' ';
            }
        }
        command_line[write_offset] = '\0';
    }

    const int result = platform::Run_Main_Loop(command_line);
    if (command_line != nullptr) {
        free(command_line);
    }

    vfs::Shutdown();
    if (current_directory != nullptr) {
        SDL_free(current_directory);
    }

    return result;
}