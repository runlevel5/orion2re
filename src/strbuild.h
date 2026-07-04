#pragma once

#include <cstdint>

namespace STRBUILD {
    struct s_builder {
        char* start;
        char* cur;
        char* end;
    };

    s_builder Init(char* buffer, uint32_t buffer_size);

    void Copy(s_builder* builder, const char* source);

    void Cat(s_builder* builder, const char* source);

    void Sprintf(s_builder* builder, const char* format, ...);
}