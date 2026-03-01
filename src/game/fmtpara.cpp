#include "deps.h"

namespace FMTPARA {
    void __cdecl Initialize_Line_Start_() {
        _line_p->line_y_offset = 0;
        _line_p->space_count = 0;
        _line_p->char_count = 0;
        _line_p->prev_x = 0;

        _break_p->char_count = 0;
        _break_p->space_count = 0;
    }

    void __cdecl Reset_Max_Min_Values_() {
        _para_global.max_x = 0;
        _para_global.max_y = 0;
        _para_global.min_x = _para_p->x1;
        _para_global.min_y = _para_p->y;
    }

    void __cdecl Set_Para_Y2_() {
        _para_p->y2_limit = 480 - _style_p->font_height_px;
    }

    void __cdecl Reset_Font_LF_() {
        _style_p->font_height_px = fonts::_font_header.font_heights[_style_p->font_id];
        _para_p->line_advance_y = fonts::_font_header.leading_extra[_style_p->font_id] + _style_p->font_height_px;
    }

    void __cdecl Reset_Font_Spacing_Width_() {
        _style_p->space_width = (int16_t)(int8_t)fonts::_font_header.font_glyph_widths[_style_p->font_id][32];
    }

    bool __cdecl Set_Line_Limits_() {
        int16_t excl_left_x = 0;
        int16_t left_limit = _para_p->x1 + _para_p->left_indent;
        int16_t right_limit = _para_p->x2 - _para_p->right_indent;

        Initialize_Line_Start_();

        int16_t excl_right_x = right_limit;

        if (_para_p->paragraph_break != 0) {
            left_limit += _para_p->first_line_indent;
        }

        if (paragrph::_exclusion_count > 0) {
            int16_t font_height = _style_p->font_height_px;
            if (font_height > 0) {
                // Stabilize line window by iterating exclusions
                while (right_limit > left_limit) {
                    for (int16_t i = 0; i < (int16_t)paragrph::_exclusion_count; ++i) {
                        int16_t py = _para_p->para_y;

                        // Check if paragraph Y coordinate is affected by this exclusion zone
                        if (py <= paragrph::_exclusion_area_y2[i] && py <= paragrph::_exclusion_area_y1[i]) {
                            int16_t x2_ex = paragrph::_exclusion_area_x2[i];
                            int16_t x1_ex = paragrph::_exclusion_area_x1[i];

                            // Check if current line window overlaps with exclusion zone edges
                            if ((left_limit < x2_ex && right_limit > x2_ex) || 
                                (left_limit < x1_ex && right_limit > x1_ex)) {
                                
                                if (left_limit < x2_ex) {
                                    if (right_limit > x2_ex) {
                                        right_limit = x2_ex;
                                    } else if (excl_right_x > x2_ex) {
                                        excl_right_x = x2_ex;
                                    }
                                } else if (right_limit <= x2_ex && excl_right_x > x2_ex) {
                                    excl_right_x = x2_ex;
                                }

                                if (x1_ex > left_limit) {
                                    if (right_limit > x1_ex) {
                                        left_limit = x1_ex;
                                    } else if (right_limit <= x1_ex && excl_right_x > x1_ex) {
                                        excl_left_x = x1_ex;
                                    }
                                } else if (right_limit <= x1_ex && excl_right_x > x1_ex) {
                                    excl_left_x = x1_ex;
                                }
                            } else {
                                // Check if window is completely occluded by the exclusion zone
                                if (left_limit >= x2_ex && right_limit <= x1_ex) {
                                    right_limit = left_limit;
                                }
                            }
                        }

                        if (right_limit <= left_limit) {
                            break;
                        }
                    }

                    // Stabilization loop: re-check all exclusions if limits were modified.
                    // Note: This relies on limits eventually becoming invalid or stable.
                    if (font_height > 0) {
                        continue; // BUG: potential infinite loop
                    }
                    break;
                }
            }
        }

        if (left_limit < right_limit) {
            _line_p->left_limit_x = left_limit;
            _line_p->x = _line_p->left_limit_x;
            _line_p->x_candidate = _line_p->x;
            _line_p->right_limit_x = right_limit;
            _line_p->exclusion_left_x = excl_left_x;
            _line_p->exclusion_right_x = excl_right_x;

            _para_p->line_start_x = _line_p->left_limit_x;
            _para_p->line_right_limit_x = _line_p->right_limit_x;
            return true;
        }

        // Failure state: zero out exclusion data and set invalid right limit
        _line_p->exclusion_left_x = 0;
        _line_p->right_limit_x = _line_p->left_limit_x;
        _line_p->x = _line_p->left_limit_x;
        _line_p->x_candidate = _line_p->x;

        return false;
    }

    bool __cdecl Legalize_Paragraph_Y_() {
        do {
            if (Set_Line_Limits_()) {
                return true;
            }
            _para_p->para_y++;
        } while (_para_p->para_y <= _para_p->y2_limit);

        return false;
    }

    bool __cdecl Process_Command_(char param_1) {
        if (param_1 < 0x17) {
            if (param_1 > 0xd) {
                return false;
            }

            switch (param_1) {
                case '\0':
                    _insert_case = 0;
                    if (!Previous_Nesting_Level_()) {
                        Complete_Line_();
                        return true;
                    }
                    break;
                case '\a':
                    Process_Function_();
                    if (*_para_p->str == '\0') {
                        return true;
                    }
                    break;
                case '\b':
                    Record_Break_Position_('\b');
                    return false;
                case '\t':
                    Record_Break_Position_('\t');
                    Paragraph_HT_();
                    return false;
                case '\n':
                    _para_p->paragraph_break = 1;
                    // fall through
                case '\r':
                    Complete_Line_();
                    _char_p = _first_char_p;
                    // fall through
                case '\v':
                    if (!Vertical_Move_Line_Advance_()) {
                        Quit_Paragraph_();
                        return false;
                    }
                    break;
                case '\f':
                    Complete_Line_();
                    _char_p = _first_char_p;
                    if (!Vertical_Move_Paragraph_Advance_()) {
                        Quit_Paragraph_();
                        return false;
                    }
                    break;
                default:
                    break;
            }
        } else {
            uint8_t cmd = param_1 - 0x17;
            if (cmd > 8) {
                return false;
            }

            switch (param_1) {
                case 0x17:
                    Set_Value_();
                    // fall through
                case 0x1F:
                    return Quit_Context_();
                case 0x18:
                    Insert_Item_();
                    _para_p->str--;
                    return false;
                case 0x19:
                    Test_Item_();
                    return false;
                case 0x1A:
                    Set_Justification_();
                    return false;
                case 0x1B:
                    Set_Current_Colors_();
                    return false;
                case 0x1C:
                    Set_Current_Style_();
                    return false;
                case 0x1D:
                    Reset_Parameters_();
                    return false;
                default:
                    break;
            }
        }
        return false;
    }


    void __cdecl Process_Function_(void) {
        int32_t val;
        bool flag;
        bool sort_tabs = false;

        if (*_para_p->str == '\a') {
            _para_p->str++;
        }

        _para_p->y_origin = 480 - _style_p->font_height_px;

        while (true) {
            char func_char = (char)_toupper(*_para_p->str);
            if (!strchr(_legal_functions, func_char)) {
                break;
            }

            if (func_char == 'T') {
                _tabs[0] = 0;
            }

            sort_tabs = false;
            int16_t arg_index = 0;

            do {
                arg_index++;
                _para_p->str++;

                int16_t arg_ret = Get_Function_Argument_(&val, &flag);

                switch (func_char) {
                case '=': // 0x3d
                    func_char = FN_Set_Variable_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'C': // 0x43
                    func_char = FN_User_Call_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'F': // 0x46
                case 'R': // 0x52
                    func_char = FN_Set_Font_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'H': // 0x48
                    func_char = FN_Set_H_Spacing_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'I': // 0x49
                    func_char = FN_Set_Indent_(func_char, val, arg_index, arg_ret, flag);
                    Set_Line_Limits_();
                    break;
                case 'M': // 0x4d
                    func_char = FN_Set_Margins_(func_char, val, arg_index, arg_ret, flag);
                    Set_Line_Limits_();
                    break;
                case 'O': // 0x4f
                    func_char = FN_Set_Origin_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'P': // 0x50
                    func_char = FN_Set_FF_Size_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'S': // 0x53
                    func_char = FN_Set_Space_Size_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'T': // 0x54
                    sort_tabs = true;
                    func_char = FN_Set_Tabs_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'V': // 0x56
                    func_char = FN_Set_V_Spacing_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'X': // 0x58
                    func_char = FN_Set_X_Pos_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case 'Y': // 0x59
                    func_char = FN_Set_Y_Pos_(func_char, val, arg_index, arg_ret, flag);
                    break;
                case '^': // 0x5e
                    func_char = FN_Superscript_(func_char, val, arg_index, arg_ret, flag);
                    break;
                default:
                    break;
                }

                _para_p->str = Skip_Spaces_(_para_p->str);

            } while (*_para_p->str == ',');

            if (func_char == 'O') {
                _para_p->str = _Print_Formatted_Paragraph_(
                    _para_p->x1,
                    _para_p->y,
                    _para_p->width,
                    _para_p->str,
                    _para_p->justify_mode,
                    _para_p->seg,
                    _para_p->do_render
                );
            }
        }

        while (*_para_p->str != '\a' && *_para_p->str != '.' && *_para_p->str != '\0') {
            _para_p->str++;
        }

        if (sort_tabs) {
            Sort_Tabs_();
        }
    }

    void __cdecl Process_Paragraph_Text_() {
        _char_p = _first_char_p;
        bool stop_processing = false;

        while (true) {
            _line_p->line_done = false;
            _line_p->line_started = 1;
            _line_p->prev_x = 0;
            _line_p->x_candidate = _line_p->x;
            Record_Break_Position_(0);

            while (!_line_p->line_done && !stop_processing) {
                char c = *_para_p->str;

                if (c >= 0x20) {
                    uint32_t type = isalpha(c);

                    if (type != 0 && _insert_case != 0) {
                        if (_insert_case == 1 || _insert_case == 3) {
                            c = _toupper(c);
                        } else if (_insert_case == 2) {
                            c = _tolower(c);
                        }
                    }

                    int16_t char_x_end;
                    if (c == ' ') {
                        char_x_end = _style_p->space_width + _line_p->x;
                    } else {
                        char_x_end = (int16_t)_style_p->char_widths[c] + _line_p->x;
                    }
                    char_x_end--; // x_candidate calculation

                    if (_line_p->char_count < 150 && (char_x_end <= _line_p->right_limit_x || _line_p->line_started != 0)) {
                        _line_p->x_candidate = char_x_end;
                        Add_Character_To_Line_(c);
                        _line_p->prev_x = _line_p->x;
                        _line_p->x = _line_p->font_spacing + char_x_end + 1;
                        _line_p->line_started = 0;

                        if (_insert_case == 3) {
                            uint32_t alpha_check = isalpha(c);
                            if (alpha_check != 0) {
                                _insert_case = 0;
                            }
                        }
                    } else {
                        // Word wrap or line break processing
                        if (c != ' ' && c != '\t') {
                            if (Return_To_Last_Break_() && _break_p->break_is_backspace) {
                                _char_p->ch = '-';
                            }
                            _para_p->str--;
                        }

                        if (_line_p->x < _line_p->exclusion_left_x) {
                            _line_p->x = _line_p->exclusion_left_x;
                            _line_p->x_candidate = _line_p->x;
                            _line_p->left_limit_x = _line_p->exclusion_left_x;
                            _line_p->right_limit_x = _line_p->exclusion_right_x;
                            _line_p->exclusion_left_x = 0;
                        } else {
                            c = 0x0D; // Carriage Return to signal end of line to Process_Command_
                        }
                    }
                }

                if (c < 0x20) {
                    stop_processing = Process_Command_(c);
                }

                _para_p->str++;
            }

            if (stop_processing) {
                setvesa::Add_Square_To_Scan_List_(_para_global.min_x, _para_global.min_y, _para_global.max_x, _para_global.max_y);
                return;
            }
        }
    }

    char* __cdecl _Print_Formatted_Paragraph_(short x, short y, short width, char* str, short justify, s_animation_header* seg, bool do_render) {
        s_fmtpara_para para;
        short* unused_tabs;
        s_fmtpara_global* unused_para_global;
        s_fmtpara_line linep;
        s_fmtpara_style stylep;
        s_fmtpara_break breakp;
        s_fmtpara_char endp[150];

        if (width >= 10 && width <= 640 &&
            x >= 0 && x < 640 &&
            y >= 0 && y < 480 &&
            str != nullptr) {

            _para_p = &para;
            _line_p = &linep;
            _style_p = &stylep;
            _break_p = &breakp;
            _first_char_p = endp;
            _end_char_p = endp;

            para.para_y = y;
            para.x1 = x;
            para.y = y;
            para.width = width;
            para.x2 = x + width - 1;
            para.str = str;
            para.seg = seg;
            para.do_render = do_render;
            para.first_line_indent = 0;
            para.right_indent = 0;
            para.left_indent = 0;
            para.justify_mode = static_cast<int8_t>(justify);
            para.paragraph_break = 1;
            para.nest_depth = 0;
            para.nest_stack[9] = &linep;

            unused_tabs = _tabs;
            unused_para_global = &_para_global;

            para.y_origin = 479;

            stylep.font_id = static_cast<int8_t>(fonts::_current_font_style);
            stylep.highlight = 0;
            stylep.fg_color = static_cast<uint8_t>(fonts::_font_header.font_style);

            _style_p->color_attr = Get_Current_Color_Set_();

            Reset_Font_LF_();
            Reset_Font_Spacing_();

            para.para_advance_y = para.line_advance_y;

            Set_Para_Y2_();

            if (Legalize_Paragraph_Y_()) {
                Reset_Max_Min_Values_();

                int16_t saved_current_font_style = fonts::_current_font_style;
                s_font saved_font;
                memcpy(&saved_font, &fonts::_font_header, sizeof(s_font));

                Process_Paragraph_Text_();

                fonts::_current_font_style = saved_current_font_style;
                memcpy(&fonts::_font_header, &saved_font, sizeof(s_font));

                return para.str;
            } else {
                return nullptr;
            }
        }
        return str;
    }
}