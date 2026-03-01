#pragma once

namespace FMTPARA {
    inline char* const _legal_functions = reinterpret_cast<char*>(0x00556288);
    inline short (&_tabs)[11] = *reinterpret_cast<short(*)[11]>(std::uintptr_t{0x005562B0});
    inline s_fmtpara_style*& _style_p = *reinterpret_cast<s_fmtpara_style**>(std::uintptr_t{0x005650C4});
    inline s_fmtpara_char*& _char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x005650C8});
    inline s_fmtpara_char*& _end_char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x005650CC});
    inline s_fmtpara_para*& _para_p = *reinterpret_cast<s_fmtpara_para**>(std::uintptr_t{0x005650D4});
    inline int16_t& _insert_case = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005650E8});
    inline s_fmtpara_line*& _line_p = *reinterpret_cast<s_fmtpara_line**>(std::uintptr_t{0x005650F0});
    inline s_fmtpara_break*& _break_p = *reinterpret_cast<s_fmtpara_break**>(std::uintptr_t{0x005650EC});
    inline s_fmtpara_char*& _first_char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x00565118});
    inline s_fmtpara_global& _para_global = *reinterpret_cast<s_fmtpara_global*>(std::uintptr_t{0x005A7B10});

    void __cdecl Set_Para_Y2_();
    void __cdecl Reset_Max_Min_Values_();
    void __cdecl Reset_Font_LF_();
    void __cdecl Reset_Font_Spacing_Width_();

    inline uint8_t __cdecl Get_Current_Color_Set_() {
        using Fn = uint8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00435AC0});
        return fn();
    }

    inline void __cdecl Reset_Font_Spacing_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433C90});
        fn();
    }

    inline int16_t __cdecl Get_Formatted_Paragraph_Max_Height_(int16_t max_width, char* text) {
        using Fn = int16_t(__cdecl*)(int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432e70});
        return fn(max_width, text);
    }

    inline int16_t __cdecl Get_Last_Formatted_Paragraph_Max_Width_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432ee0});
        return fn();
    }

    inline void __cdecl Print_Formatted_Paragraph_(short x, short y, short width, char* buffer, short unknown) {
        using Fn = void(__cdecl*)(short, short, short, char*, short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432dd0});
        fn(x, y, width, buffer, unknown);
    }

    void __cdecl Initialize_Line_Start_();

    bool __cdecl Set_Line_Limits_();

    bool __cdecl Legalize_Paragraph_Y_();

    inline void __cdecl Record_Break_Position_(int16_t some_val) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433600});
        fn(some_val);
    }

    inline void __cdecl Add_Character_To_Line_(uint8_t c) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004333E0});
        fn(c);
    }

    inline bool __cdecl Return_To_Last_Break_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004334A0});
        return fn();
    }

    inline bool __cdecl Previous_Nesting_Level_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004347C0});
        return fn();
    }

    inline void __cdecl Complete_Line_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433A70});
        fn();
    }

    void __cdecl Process_Function_();

    inline void __cdecl Paragraph_HT_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433BA0});
        fn();
    }

    inline bool __cdecl Vertical_Move_Line_Advance_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433C10});
        return fn();
    }

    inline void __cdecl Quit_Paragraph_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433990});
        fn();
    }

    inline bool __cdecl Vertical_Move_Paragraph_Advance_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433BF0});
        return fn();
    }

    inline void __cdecl Set_Value_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00434060});
        fn();
    }

    inline bool __cdecl Quit_Context_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433950});
        return fn();
    }

    inline void __cdecl Insert_Item_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004342E0});
        fn();
    }

    inline void __cdecl Test_Item_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433EC0});
        fn();
    }

    inline void __cdecl Set_Justification_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004339F0});
        fn();
    }

    inline void __cdecl Set_Current_Colors_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433E40});
        fn();
    }

    inline void __cdecl Set_Current_Style_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433E60});
        fn();
    }

    inline void __cdecl Reset_Parameters_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00433D30});
        fn();
    }

    inline int16_t __cdecl Get_Function_Argument_(int32_t* val, bool* flag) {
        using Fn = int16_t(__cdecl*)(int32_t*, bool*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434b20 });
        return fn(val, flag);
    }

    inline char __cdecl FN_Set_Variable_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434d40 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_User_Call_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435210 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Font_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434bd0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_H_Spacing_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434ee0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Indent_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004350e0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Margins_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435010 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Origin_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435140 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_FF_Size_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434fb0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Space_Size_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434f20 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Tabs_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434db0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_V_Spacing_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434f70 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_X_Pos_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434e20 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Set_Y_Pos_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434e70 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char __cdecl FN_Superscript_(char func_char, int32_t val, int16_t arg_index, int16_t arg_ret, bool flag) {
        using Fn = char(__cdecl*)(char, int32_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00434ca0 });
        return fn(func_char, val, arg_index, arg_ret, flag);
    }

    inline char* __cdecl Skip_Spaces_(char* str) {
        using Fn = char* (__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435d80 });
        return fn(str);
    }

    inline void __cdecl Sort_Tabs_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00435a70 });
        fn();
    }

    bool __cdecl Process_Command_(char param_1);

    void __cdecl Process_Paragraph_Text_();

    char* __cdecl _Print_Formatted_Paragraph_(short x, short y, short width, char* str, short justify, s_animation_header* seg, bool do_render);
}