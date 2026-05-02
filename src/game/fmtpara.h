#pragma once

struct s_animation_header;

namespace FMTPARA {
    using user_fn = void(*)(s_fmtpara_para*, uint32_t, int32_t);

    inline char* const _legal_functions = reinterpret_cast<char*>(0x00556288);
    inline char* const _legal_fn_arg_chars = reinterpret_cast<char*>(0x00556298);
    inline uint8_t* _font_base_offsets = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005562C8});
    inline int8_t (&_reset_parameters_lookup)[16] = *reinterpret_cast<int8_t(*)[16]>(std::uintptr_t{0x005562D0});
    inline short (&_tabs)[11] = *reinterpret_cast<short(*)[11]>(std::uintptr_t{0x005562B0});
    inline uint8_t (&_saved_superscript)[13] = *reinterpret_cast<uint8_t (*)[13]>(std::uintptr_t{0x005650D8});
    inline s_fmtpara_style*& _style_p = *reinterpret_cast<s_fmtpara_style**>(std::uintptr_t{0x005650C4});
    inline s_fmtpara_char*& _char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x005650C8});
    inline s_fmtpara_char*& _end_char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x005650CC});
    inline uint8_t& _last_fg_and_attr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005650D0});
    inline s_fmtpara_para*& _para_p = *reinterpret_cast<s_fmtpara_para**>(std::uintptr_t{0x005650D4});
    inline int16_t& _insert_case = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005650E8});
    inline s_fmtpara_line*& _line_p = *reinterpret_cast<s_fmtpara_line**>(std::uintptr_t{0x005650F0});
    inline s_fmtpara_break*& _break_p = *reinterpret_cast<s_fmtpara_break**>(std::uintptr_t{0x005650EC});
    inline s_fmtpara_char*& _first_char_p = *reinterpret_cast<s_fmtpara_char**>(std::uintptr_t{0x00565118});
    inline char (&justify_spread_flags)[24] = *reinterpret_cast<char (*)[24]>(std::uintptr_t{0x005562E0});
    inline uint16_t& _var_index = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0056511C});
    inline uint8_t& _last_font_id = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565120});
    inline s_fmtpara_global& _para_global = *reinterpret_cast<s_fmtpara_global*>(std::uintptr_t{0x005A7B10});
    inline uint32_t (&_variable)[16] =
        *reinterpret_cast<uint32_t(*)[16]>(std::uintptr_t{0x00565070});
    inline char* _tmp_str = reinterpret_cast<char*>(std::uintptr_t{0x005650b0});
    inline char* _tmp_str2 = reinterpret_cast<char*>(std::uintptr_t{0x005650f8});
    inline char** _ordinal_ending = reinterpret_cast<char**>(std::uintptr_t{0x00556220});
    inline char** _ordinal_word = reinterpret_cast<char**>(std::uintptr_t{0x00556248});
    inline const char* _ordinal_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00556278});
    inline uint8_t* _tabs_3_plus_1 = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005562B7});
    inline const char* s_s_00556218 = reinterpret_cast<const char*>(std::uintptr_t{0x00556218});
    inline const char* s_es_0055621c = reinterpret_cast<const char*>(std::uintptr_t{0x0055621C});
    inline uint32_t& _user_fn_arg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005650C0 });

    inline void **_para_global_item_ptr =
        reinterpret_cast<void **>(std::uintptr_t{0x005A7B18}); // _para_global.item_ptr[0]
    inline int8_t *_para_global_item_type =
        reinterpret_cast<int8_t *>(std::uintptr_t{0x005A7B58}); // _para_global.item_type[0]

    void __cdecl Init_();

    void __cdecl Set_Para_Y2_();
    void __cdecl Reset_Max_Min_Values_();
    void __cdecl Reset_Font_LF_();
    void __cdecl Reset_Font_Spacing_Width_();

    uint8_t __cdecl Get_Current_Color_Set_();

    void __cdecl Reset_Font_Spacing_();

    void __cdecl Initialize_Formatted_Paragraph_System_();

    void __cdecl Reset_A_Paragraph_Variable_(uint16_t index);

    void __cdecl Pick_Font_Color_(uint8_t fg_and_attr);

    void __cdecl Pick_Font_Number_(uint8_t font_id);

    void __cdecl Update_Max_Min_Values_();

    void __cdecl Set_Max_Min_X_(short current_x, s_fmtpara_char* char_info);

    void __cdecl Set_Max_Min_Y_(s_fmtpara_char* char_p);

    int16_t __cdecl Get_Formatted_Paragraph_Max_Height_(int16_t width, char* text);

    int16_t __cdecl Get_Formatted_Paragraph_Max_Width_(int16_t width, char* text, int16_t justify);

    int16_t __cdecl Get_Last_Formatted_Paragraph_Max_Width_();

    char* __cdecl Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, char* str, int16_t param_5);

    void __cdecl Initialize_Line_Start_();

    bool __cdecl Set_Line_Limits_();

    bool __cdecl Legalize_Paragraph_Y_();

    void __cdecl Record_Break_Position_(char char_code);

    void __cdecl Add_Character_To_Line_(char ch);

    bool __cdecl Return_To_Last_Break_();

    bool __cdecl Previous_Nesting_Level_();

    void __cdecl Complete_Line_Data_();

    void __cdecl Justify_Line_(int8_t mode);

    void __cdecl _Justify_Line_Fully_(int16_t extra_space);

    void __cdecl Print_Line_();

    void __cdecl Complete_Line_();

    void __cdecl Process_Function_();

    void __cdecl Paragraph_HT_();

    int8_t __cdecl Vertical_Move_(int16_t delta);

    int8_t __cdecl Vertical_Move_Line_Advance_();

    void __cdecl Quit_Paragraph_();

    bool __cdecl Vertical_Move_Paragraph_Advance_();

    void __cdecl Set_Value_();

    bool __cdecl Quit_Context_();

    void __cdecl Insert_Item_();

    void __cdecl Skip_Arg_Insert_A_String_(uint16_t arg, bool flag);

    void __cdecl Insert_A_String_(char* new_str);

    uint32_t __cdecl Get_Item_Value_Set_Plural_(short item_idx);

    void __cdecl Insert_String_(const char* str);

    void __cdecl Insert_Next_Argument_String_(bool flag);

    void __cdecl Test_Item_();

    uint32_t __cdecl Get_Next_Item_Value_();

    uint32_t __cdecl Get_Item_Value_(uint16_t index);

    bool __cdecl Get_Insert_Argument_Next_(uint16_t* value);

    bool __cdecl Set_Item_Value_(uint16_t index, uint32_t value);

    void __cdecl Set_Justification_();

    void __cdecl Set_Current_Colors_();

    void __cdecl Set_Current_Style_();

    void __cdecl Reset_Parameters_();

    bool __cdecl Get_Function_Argument_(int32_t* val, bool* flag);

    char __cdecl FN_Set_Variable_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    int8_t __cdecl FN_User_Call_(int8_t func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

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

    char __cdecl FN_Superscript_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag);

    char* __cdecl Skip_Spaces_(char* str);

    int __cdecl _Comp_Tabs_(const void* a, const void* b);

    void __cdecl Sort_Tabs_();

    bool __cdecl Process_Command_(char param_1);

    void __cdecl Process_Paragraph_Text_();

    char* __cdecl _Print_Formatted_Paragraph_(short x, short y, short width, char* str, short justify, s_animation_header* seg, bool do_render);

    void __cdecl Set_Paragraph_Sint_Pointer_(uint16_t id, void* ptr);

    void __cdecl Set_Paragraph_Integer_Pointer_(uint16_t index, void* ptr);

    void __cdecl Set_Paragraph_Variable_Pointer_(uint16_t item_index, void *value_ptr, int8_t value_type);

    void __cdecl Print_Formatted_Paragraph_To_Bitmap_(short param_1, short param_2, short param_3, char* param_4, short param_5, s_animation_header* param_6);
}