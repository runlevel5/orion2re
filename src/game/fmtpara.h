#pragma once

struct s_animation_header;

namespace FMTPARA {
    using user_fn = void(*)(s_fmtpara_para*, uint32_t, int32_t);

    extern const char _legal_functions[];
    extern const char _legal_fn_arg_chars[];
    extern const uint8_t _legal_variable_types[6];
    extern uint8_t _font_base_offsets[6];
    extern int8_t _reset_parameters_lookup[16];
    extern int16_t _tabs[11];
    extern s_fmtpara_style _saved_superscript;
    extern s_fmtpara_style* _style_p;
    extern s_fmtpara_char* _char_p;
    extern s_fmtpara_char* _end_char_p;
    extern uint8_t _last_fg_and_attr;
    extern s_fmtpara_para* _para_p;
    extern int16_t _insert_case;
    extern s_fmtpara_line* _line_p;
    extern s_fmtpara_break* _break_p;
    extern s_fmtpara_char* _first_char_p;
    extern char justify_spread_flags[24];
    extern uint16_t _var_index;
    extern uint8_t _last_font_id;
    extern s_fmtpara_global _para_global;
    extern uint32_t _variable[16];
    extern char _tmp_str[16];
    extern char _tmp_str2[32];
    extern const char* const _ordinal_ending[10];
    extern const char* const _ordinal_word[11];
    extern uint32_t _user_fn_arg;

    void __cdecl Init_();

    void __cdecl Set_Para_Y2_();
    void __cdecl Reset_Max_Min_Values_();
    void __cdecl Reset_Font_LF_();
    void __cdecl Reset_Font_Spacing_Width_();

    uint8_t __cdecl Get_Current_Color_Set_();

    void __cdecl Reset_Font_Spacing_();

    /** Initializes the paragraph variable table to its default internal storage. */
    void __cdecl Initialize_Formatted_Paragraph_System_();

    /** Resets one paragraph variable slot to the internal backing array.
     * @param index Variable slot to reset.
     */
    void __cdecl Reset_A_Paragraph_Variable_(uint16_t index);

    void __cdecl Pick_Font_Color_(uint8_t fg_and_attr);

    void __cdecl Pick_Font_Number_(uint8_t font_id);

    void __cdecl Update_Max_Min_Values_();

    void __cdecl Set_Max_Min_X_(short current_x, s_fmtpara_char* char_info);

    void __cdecl Set_Max_Min_Y_(s_fmtpara_char* char_p);

    /** Measures the rendered height of a formatted paragraph without drawing it.
     * @param width Paragraph width in pixels.
     * @param text Source formatted string.
     */
    int16_t __cdecl Get_Formatted_Paragraph_Max_Height_(int16_t width, const char* text);

    /** Measures the rendered width of a formatted paragraph without drawing it.
     * @param width Paragraph width in pixels.
     * @param text Source formatted string.
     * @param justify Justification mode.
     */
    int16_t __cdecl Get_Formatted_Paragraph_Max_Width_(int16_t width, const char* text, int16_t justify);

    int16_t __cdecl Get_Last_Formatted_Paragraph_Max_Width_();

    /** Renders a formatted paragraph and returns the next unread source position.
     * @param x Left edge in pixels.
     * @param y Top edge in pixels.
     * @param width Paragraph width in pixels.
     * @param str Source formatted string.
     * @param justify Justification mode.
     */
    const char* __cdecl Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, const char* str, int16_t justify);

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

    void __cdecl Insert_A_String_(const char* new_str);

    uint32_t __cdecl Get_Item_Value_Set_Plural_(int16_t item_index);

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

    const char* __cdecl Skip_Spaces_(const char* str);

    int __cdecl _Comp_Tabs_(const void* a, const void* b);

    void __cdecl Sort_Tabs_();

    bool __cdecl Process_Command_(char command_char);

    void __cdecl Process_Paragraph_Text_();

    /** Lays out or renders a formatted paragraph and returns the next unread source position.
     * @param x Left edge in pixels.
     * @param y Top edge in pixels.
     * @param width Paragraph width in pixels.
     * @param str Source formatted string.
     * @param justify Justification mode.
     * @param seg Destination bitmap, or nullptr to draw to the active screen.
     * @param do_render False to measure only, true to render glyphs.
     */
    const char* __cdecl _Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, const char* str, int16_t justify, s_animation_header* seg, bool do_render);

    /** Binds a signed 16-bit paragraph variable slot to external storage.
     * @param index Variable slot index.
     * @param value_ptr Pointer to the bound value.
     */
    void __cdecl Set_Paragraph_Sint_Pointer_(uint16_t index, void* value_ptr);

    /** Binds a 32-bit paragraph variable slot to external storage.
     * @param index Variable slot index.
     * @param value_ptr Pointer to the bound value.
     */
    void __cdecl Set_Paragraph_Integer_Pointer_(uint16_t index, void* value_ptr);

    /** Binds a paragraph variable slot to caller-provided storage and a tagged value type.
     * @param item_index Variable slot index.
     * @param value_ptr Pointer to the bound value.
     * @param value_type Tagged storage type used by Get_Item_Value_ and Set_Item_Value_.
     */
    void __cdecl Set_Paragraph_Variable_Pointer_(uint16_t item_index, void *value_ptr, int8_t value_type);

    /** Renders a formatted paragraph into a destination bitmap.
     * @param x Left edge in pixels.
     * @param y Top edge in pixels.
     * @param width Paragraph width in pixels.
     * @param str Source formatted string.
     * @param justify Justification mode.
     * @param seg Destination bitmap.
     */
    void __cdecl Print_Formatted_Paragraph_To_Bitmap_(int16_t x, int16_t y, int16_t width, const char* str, int16_t justify, s_animation_header* seg);
}