#pragma once

namespace dos {
    inline int32_t DIR_(const char* filename, char* buffer) {
        using Fn = int32_t(*)(const char*, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D360 });
        return fn(filename, buffer);
    }
}