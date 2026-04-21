#pragma once

namespace reduce {
    inline s_palette_entry (&_reduced_palette_list)[512] =
        *reinterpret_cast<s_palette_entry(*)[512]>(std::uintptr_t{0x005865D8});

    inline int16_t& _ncubes =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00587FE0});

    inline uint16_t& _active_cube_count =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00586FD8});

    inline int16_t (&_HistPtr)[512] =
        *reinterpret_cast<int16_t(*)[512]>(std::uintptr_t{0x00586FE0});

    inline s_palette_cube (&_cube_list)[256] =
        *reinterpret_cast<s_palette_cube(*)[256]>(std::uintptr_t{0x005873E0});

    inline uint8_t (&_color_weight)[512] =
        *reinterpret_cast<uint8_t(*)[512]>(std::uintptr_t{0x00586DD8});

    void __cdecl Clear_Reduced_Palette_Groups_();

    void __cdecl Add_Reduced_Palette_Group_(s_animation_header* anim);

    void __cdecl Initial_Cube_(s_palette_entry* pal, int16_t num_colors);

    void __cdecl Shrink_Cube_(s_palette_cube* cube, s_palette_entry* hist);

    int16_t __cdecl Find_Next_Lowest_Cube_Level_();

    int16_t __cdecl Find_Longest_Axis_(int16_t cube_idx);

    void __cdecl Sort_Cube_Points_(s_palette_entry* palette, int16_t* indices, int16_t num_indices, int16_t component);

    void __cdecl Split_Cube_(s_palette_entry* entries, int16_t cube_index, int16_t color_channel);

    void __cdecl Reduce_Palette_(s_palette_entry* palette, int16_t start_index, int16_t num_colors_src, int16_t num_colors_dst);

    void __cdecl Compress_Reduced_Palette_Groups_(int16_t start_index, int16_t count);
}