#pragma once
#include <cstdint>

namespace ESTRINGS {
    extern char* _strings[812];
    extern const char s_0_0055110c[4];
    extern const char s_1_00551110[4];

    extern char _string_data[21000];

    extern const char s_estrings_lbx[16];
    extern const char s_estrGERM_lbx[16];
    extern const char s_estrFREN_lbx[16];
    extern const char s_estrSPAN_lbx[16];
    extern const char s_estrITAL_lbx[16];
    extern const char s_estrPOLI_lbx[16];

    extern const char s_maintext_lbx[16];
    extern const char s_maingerm_lbx[16];
    extern const char s_mainfren_lbx[16];
    extern const char s_mainspan_lbx[16];
    extern const char s_mainital_lbx[16];
    extern const char s_mainpoli_lbx[16];

    extern const char s_you_blew_the_array_eric[24];

    void __cdecl Load_H_And_Pro_File_Strings_(void);

    void __cdecl Load_E_Strings_();

    char* __cdecl E_Strings_(int16_t string_id);
}