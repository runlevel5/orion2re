#pragma once

struct s_animation_header;

namespace FMTPARA {
    inline char* const _legal_functions = reinterpret_cast<char*>(0x00556288);
    inline char* const _legal_fn_arg_chars = reinterpret_cast<char*>(0x00556298);
    inline uint8_t* _font_base_offsets = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005562C8});
    inline short (&_tabs)[11] = *reinterpret_cast<short(*)[11]>(std::uintptr_t{0x005562B0});
    inline s_fmtpara_style*& _style_p = *reinterpret_cast<s_fmtpara_style**>(std::uintptr_t{0x005650C4});
    inline s_fmtpara_char*& _char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x005650C8});
    inline s_fmtpara_char*& _end_char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x005650CC});
    inline s_fmtpara_para*& _para_p = *reinterpret_cast<s_fmtpara_para**>(std::uintptr_t{0x005650D4});
    inline int16_t& _insert_case = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005650E8});
    inline s_fmtpara_line*& _line_p = *reinterpret_cast<s_fmtpara_line**>(std::uintptr_t{0x005650F0});
    inline s_fmtpara_break*& _break_p = *reinterpret_cast<s_fmtpara_break**>(std::uintptr_t{0x005650EC});
    inline s_fmtpara_char*& _first_char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x00565118});
    inline uint16_t& _var_index = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0056511C});
    inline s_fmtpara_global& _para_global = *reinterpret_cast<s_fmtpara_global*>(std::uintptr_t{0x005A7B10});

    void __cdecl Set_Para_Y2_();
    void __cdecl Reset_Max_Min_Values_();
    void __cdecl Reset_Font_LF_();
    void __cdecl Reset_Font_Spacing_Width_();

    uint8_t __cdecl Get_Current_Color_Set_();

    void __cdecl Reset_Font_Spacing_();

    int16_t __cdecl Get_Formatted_Paragraph_Max_Height_(int16_t width, char* text);

    int16_t __cdecl Get_Last_Formatted_Paragraph_Max_Width_();

    char* __cdecl Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, char* str, int16_t param_5);

    void __cdecl Initialize_Line_Start_();

    bool __cdecl Set_Line_Limits_();

    bool __cdecl Legalize_Paragraph_Y_();

    void __cdecl Record_Break_Position_(char char_code);

    void __cdecl Add_Character_To_Line_(char ch);

    inline bool __cdecl Return_To_Last_Break_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004334A0});
        return fn();
    }

    bool __cdecl Previous_Nesting_Level_();

    inline void __cdecl Complete_Line_Data_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433ad0});
        fn();
    }

    inline void __cdecl Justify_Line_(int8_t mode) {
        using Fn = void(__cdecl*)(int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00435290});
        fn(mode);
    }

    inline void __cdecl Print_Line_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00435440});
        fn();
    }

    void __cdecl Complete_Line_();

    void __cdecl Process_Function_();

    void __cdecl Paragraph_HT_();

    int8_t __cdecl Vertical_Move_(int16_t delta);

    int8_t __cdecl Vertical_Move_Line_Advance_();

    void __cdecl Quit_Paragraph_();

    bool __cdecl Vertical_Move_Paragraph_Advance_();

    void __cdecl Set_Value_();

    bool __cdecl Quit_Context_();

    inline void __cdecl Insert_Item_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004342E0});
        fn();
    }

    void __cdecl Test_Item_();

    inline uint32_t __cdecl Get_Next_Item_Value_() {
        using Fn = uint32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00434190});
        return fn();
    }

    inline int32_t __cdecl Get_Item_Value_(uint16_t index) {
        using Fn = int32_t(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004341C0});
        return fn(index);
    }

    inline bool __cdecl Get_Insert_Argument_Next_(uint16_t* arg) {
        using Fn = bool(__cdecl*)(uint16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004346E0});
        return fn(arg);
    }

    inline uint32_t __cdecl Set_Item_Value_(uint32_t value, uint16_t index) {
        using Fn = uint32_t(__cdecl*)(uint32_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00434240});
        return fn(value, index);
    }

    void __cdecl Set_Justification_();

    void __cdecl Set_Current_Colors_();

    void __cdecl Set_Current_Style_();

    inline void __cdecl Reset_Parameters_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433D30});
        fn();
    }

    bool __cdecl Get_Function_Argument_(int32_t* val, bool* flag);

    char __cdecl FN_Set_Variable_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    inline char __cdecl FN_User_Call_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435210 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    char __cdecl FN_Set_Font_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_H_Spacing_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_Indent_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_Margins_(char status, int32_t margin_val, int16_t side, bool skip_logic, bool flag);

    char __cdecl FN_Set_Origin_(char param_1, int32_t param_2, int16_t param_3, bool param_4, bool param_5);

    char __cdecl FN_Set_FF_Size_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_Space_Size_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_Tabs_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_V_Spacing_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_X_Pos_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char __cdecl FN_Set_Y_Pos_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    inline char __cdecl FN_Superscript_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434ca0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    char* __cdecl Skip_Spaces_(char* str);

    inline int __cdecl _Comp_Tabs_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435AA0 });
        return fn(a, b);
    }

    void __cdecl Sort_Tabs_();

    bool __cdecl Process_Command_(char param_1);

    void __cdecl Process_Paragraph_Text_();

    char* __cdecl _Print_Formatted_Paragraph_(short x, short y, short width, char* str, short justify, s_animation_header* seg, bool do_render);
}