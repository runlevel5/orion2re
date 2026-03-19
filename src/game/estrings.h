#pragma once
#include <cstdint>

namespace ESTRINGS {
    inline char* (&_strings)[812] =
        *reinterpret_cast<char* (*)[812]>(std::uintptr_t{0x0056B788});
    inline const char*& s_0_0055110c = *reinterpret_cast<const char**>(std::uintptr_t{0x0055110C}); // " 0"
    inline const char*& s_1_00551110 = *reinterpret_cast<const char**>(std::uintptr_t{0x00551110}); // " 1"

    inline char (&_string_data)[21000] = *reinterpret_cast<char (*)[21000]>(std::uintptr_t{0x00566580});

    inline const char* s_estrings_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558358});
    inline const char* s_estrGERM_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558348});
    inline const char* s_estrFREN_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558338});
    inline const char* s_estrSPAN_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558328});
    inline const char* s_estrITAL_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558318});
    inline const char* s_estrPOLI_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558308});

    inline const char* s_maintext_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x005582d0});
    inline const char* s_maingerm_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x005582e0});
    inline const char* s_mainfren_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x005582c0});
    inline const char* s_mainspan_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x005582b0});
    inline const char* s_mainital_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x005582a0});
    inline const char* s_mainpoli_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558290});

    inline const char* s_you_blew_the_array_eric = reinterpret_cast<const char*>(std::uintptr_t{0x005582f0});

    inline void __cdecl Load_H_And_Pro_File_Strings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a02f0});
        fn();
    }

    void __cdecl Load_E_Strings_();

    char* __cdecl E_Strings_(int16_t index);
}