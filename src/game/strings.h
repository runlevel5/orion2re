#pragma once

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b);

    void __cdecl Swap_(int32_t* ptr1, int32_t* ptr2);

    void __cdecl Swap_Char_(uint8_t* a, uint8_t* b);

    extern const char s__00551100[1];
    extern const char s_plus_00551118[2];
    extern const char s_minus_0055111c[2];
    extern const char s_space_00551120[2];
    extern const char s_B_005560e4[2];
    extern const char s_S_005569cc[2];
    extern const char s_O_00556afc[2];
    extern const char s_A_00557ed4[2];
    extern const char s_M_00557ed8[2];
    extern const char s_fmt_plus_d_00557ed0[4];
    extern const char s_fmt_plus_15bc_00558cd4[6];
    extern const char s_fmt_plus_10bc_00558cdc[6];
    extern const char s_fmt_plus_d_percent_00558ce4[6];
    extern const char s_fmt_minus_d_bc_00558cec[6];
    extern const char s_fmt_minus_d_percent_00558cf4[6];
    extern const char s_fmt_plus_d_percent_short_00558cfc[5];
    extern const char s_ld_00558280[4]; // "%ld"
    extern const char s_0_0055110c[3];
    extern const char s_1_00551110[3];
    extern const char s__0055ac34[2]; /* "\t" */
    extern const char s_t_T0_5_124__0055ac38[13];

    void Load_Strings();

    void __cdecl String_To_Lower_(char* str);

    void __cdecl String_To_Upper_(char* str);

    void __cdecl Trim_(char* str);

    void __cdecl Delete_Structure_(int32_t index, void* array_ptr, uint32_t element_size, uint32_t count);
}