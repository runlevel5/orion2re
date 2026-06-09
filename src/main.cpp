#include "pch.h"

#include <SDL3/SDL_main.h>

int main(int argc, char** argv) {
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

    return result;
}