#pragma once

#include <cstdint>

namespace file_ani {
    inline uint8_t*& _file_animation_buffer =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{ 0x0057E0AC });

    inline bool& _file_animation_open =
        *reinterpret_cast<bool*>(std::uintptr_t{ 0x0055B7F0 });

    inline uint32_t& _file_animation_length =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0057E094 });

    inline uint32_t& _file_animation_end =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0057E098 });

    inline FILE*& _file_animation_fptr =
        *reinterpret_cast<FILE**>(std::uintptr_t{ 0x0057E0D4 });

    inline uint32_t*& _file_lbx_header =
        *reinterpret_cast<uint32_t**>(std::uintptr_t{ 0x0057E0E4 });

    inline uint32_t& _file_animation_start =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0057E0F4 });

    inline uint8_t*& _file_animation_header =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{ 0x0057E080 });

    inline const char* s_File_Animation_Frame_Too_Large = reinterpret_cast<const char*>(0x0055B804);
    inline const char* s_Refresh_Mask_cant_be_drawn = reinterpret_cast<const char*>(0x0055B10C);

    inline const char* s_Differential_Error_868 = reinterpret_cast<const char*>(0x0055B868);
    inline const char* s_Differential_Error_898 = reinterpret_cast<const char*>(0x0055B898);
    inline const char* s_Differential_Error_8C8 = reinterpret_cast<const char*>(0x0055B8C8);

    void __cdecl Set_File_Animation_Buffer_(void* buffer);

    inline void __cdecl Reset_File_Animation_Buffer_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00531780})();
    }

    void __cdecl Open_File_Animation_(const char* filename, int16_t entry_index);

    void __cdecl Draw_File_Animation_();

    void __cdecl Draw_File_Animation_Stencil_();

    int16_t __cdecl Get_File_Animation_Frame_();

    int16_t __cdecl Get_File_Animation_Total_Frames_();
}