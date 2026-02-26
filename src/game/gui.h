#pragma once
#include <cstdint>

namespace gui {

// Suggested name: GUI_Set_Font
inline void __cdecl GUI_Set_Font(int font_id) {
    using Fn = void(__cdecl*)(int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004183d0});
    fn(font_id);
}

// Suggested name: GUI_Get_Line_Height
inline int __cdecl GUI_Get_Line_Height() {
    using Fn = int(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004185d0});
    return fn();
}

// Suggested name: GUI_Get_Line_Count
inline int __cdecl GUI_Get_Line_Count(const char* str, int width) {
    using Fn = int(__cdecl*)(const char*, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00418650});
    return fn(str, width);
}

// Suggested name: GUI_Get_Line_Offset
inline int __cdecl GUI_Get_Line_Offset(const char* str, int width, int line_index) {
    using Fn = int(__cdecl*)(const char*, int, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004186c0});
    return fn(str, width, line_index);
}

// Suggested name: GUI_Get_Line_Length
inline int __cdecl GUI_Get_Line_Length(const char* str, int width, int line_index) {
    using Fn = int(__cdecl*)(const char*, int, int);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00418730});
    return fn(str, width, line_index);
}

char** __cdecl GUI_Prepare_Text(const char* text, int font_id, int width, int* out_height, int* out_line_count);

}