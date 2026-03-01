#include "deps.h"
#include <cstdlib>

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

    void __cdecl Reset_Font_Spacing_() {
        int8_t font_id = _style_p->font_id;

        _line_p->font_spacing = fonts::_font_header.font_spacing[font_id];

        _style_p->char_widths = (int8_t*)fonts::_font_header.font_glyph_widths[font_id];

        Reset_Font_Spacing_Width_();
    }

    uint8_t __cdecl Get_Current_Color_Set_() {
        int normal_match_count = 0;
        int highlight_match_count = 0;

        for (int i = 0; i < 8; ++i) {
            uint8_t selected_color = fonts::_font_header.selected_colors[i];

            if (fonts::_font_header.normal_colors[i] == selected_color) {
                normal_match_count++;
            } else if (fonts::_font_header.highlight_colors[i] == selected_color) {
                highlight_match_count++;
            }
        }

        if (normal_match_count == 8) {
            return 0x00;
        }

        if (highlight_match_count == 8) {
            return 0x20;
        }

        return 0x40;
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

    void __cdecl Paragraph_HT_() {
        for (int16_t i = 0; i < _tabs[0]; ++i) {
            if (_tabs[i + 1] > _line_p->x) {
                _line_p->x = _tabs[i + 1];
                return;
            }
        }
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
            char cmd = toupper(*_para_p->str);
            if (strchr(_legal_functions, cmd) == NULL) {
                break;
            }

            if (cmd == 'T') {
                _tabs[0] = 0;
            }

            char c = cmd;

            int16_t arg_index = 0;

            do {
                arg_index++;
                _para_p->str++;

                bool arg_ret = Get_Function_Argument_(&val, &flag);

                switch (c) {
                case '=': // 0x3d
                    c = FN_Set_Variable_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'C': // 0x43
                    c = FN_User_Call_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'F': // 0x46
                case 'R': // 0x52
                    c = FN_Set_Font_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'H': // 0x48
                    c = FN_Set_H_Spacing_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'I': // 0x49
                    c = FN_Set_Indent_(c, val, arg_index, arg_ret, flag);
                    Set_Line_Limits_();
                    break;
                case 'M': // 0x4d
                    c = FN_Set_Margins_(c, val, arg_index, arg_ret, flag);
                    Set_Line_Limits_();
                    break;
                case 'O': // 0x4f
                    c = FN_Set_Origin_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'P': // 0x50
                    c = FN_Set_FF_Size_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'S': // 0x53
                    c = FN_Set_Space_Size_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'T': // 0x54
                    sort_tabs = true;
                    c = FN_Set_Tabs_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'V': // 0x56
                    c = FN_Set_V_Spacing_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'X': // 0x58
                    c = FN_Set_X_Pos_(c, val, arg_index, arg_ret, flag);
                    break;
                case 'Y': // 0x59
                    c = FN_Set_Y_Pos_(c, val, arg_index, arg_ret, flag);
                    break;
                case '^': // 0x5e
                    c = FN_Superscript_(c, val, arg_index, arg_ret, flag);
                    break;
                default:
                    break;
                }

                _para_p->str = Skip_Spaces_(_para_p->str);

            } while (*_para_p->str == ',');

            if (cmd == 'O') {
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

    int8_t __cdecl Vertical_Move_(int16_t delta) {
        int16_t original_y = _para_p->para_y;
        _para_p->para_y = original_y + delta;

        bool success = FMTPARA::Set_Line_Limits_();

        // If move failed, try to find a valid position by incrementing para_y
        while (!success && _para_p->para_y <= _para_p->y2_limit) {
            _para_p->para_y++;
            success = FMTPARA::Set_Line_Limits_();
        }

        if (_para_p->para_y <= _para_p->y2_limit) {
            return 1;
        }

        // Restore original position if no valid position was found within limits
        _para_p->para_y = original_y;
        return 0;
    }

    int8_t __cdecl Vertical_Move_Line_Advance_() {
        return FMTPARA::Vertical_Move_(FMTPARA::_para_p->line_advance_y);
    }

    void __cdecl Quit_Paragraph_() {
        if (_para_p->nest_depth != 0) {
            _para_p->str = _para_p->nest_stack[0];
            _para_p->nest_depth = 0;
        }

        while (*_para_p->str != '\0') {
            _para_p->str++;
        }

        if (*_para_p->str == '\0') {
            _para_p->str--;
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

    bool __cdecl Vertical_Move_Paragraph_Advance_() {
        _para_p->paragraph_break = 1;
        return Vertical_Move_(_para_p->para_advance_y) != 0;
    }

    int16_t __cdecl Get_Last_Formatted_Paragraph_Max_Width_() {
        return (int16_t)(_para_global.max_x - _para_global.min_x + 1);
    }

    int16_t __cdecl Get_Formatted_Paragraph_Max_Height_(int16_t width, char* text) {
        _Print_Formatted_Paragraph_(0, 0, width, text, 0, nullptr, false);
        return (int16_t)((_para_global.max_y - _para_global.min_y) + 1);
    }

    char* __cdecl _Print_Formatted_Paragraph_(short x, short y, short width, char* str, short justify, s_animation_header* seg, bool do_render) {
        s_fmtpara_para para;
        short* unused_tabs;
        s_fmtpara_global* unused_para_global;
        s_fmtpara_line linep[2];
        s_fmtpara_style stylep;
        s_fmtpara_break breakp;
        s_fmtpara_char endp[150];

        if (width >= 10 && width <= 640 &&
            x >= 0 && x < 640 &&
            y >= 0 && y < 480 &&
            str != nullptr) {

            _para_p = &para;
            _line_p = linep;
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
            para.line = linep;

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

    char* __cdecl Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, char* str, int16_t param_5) {
        return _Print_Formatted_Paragraph_(x, y, width, str, param_5, nullptr, true);
    }

    void __cdecl Add_Character_To_Line_(char ch) {
        _char_p->ch = ch;
        _char_p->y = _para_p->para_y;
        _char_p->x_advance = _line_p->x - _line_p->prev_x;
        _char_p->fg_and_attr = _style_p->fg_color | _style_p->color_attr;
        _char_p->font_id = _style_p->font_id;
        _char_p->bg_color = _style_p->bg_color;

        if (_style_p->highlight != 0) {
            _char_p->font_id |= 0x80;
        }

        _char_p++;

        if (ch == ' ' /* 0x20 */) {
            Record_Break_Position_(' ' /* 0x20 */);
            _line_p->space_count++;
        }

        _line_p->char_count++;
    }

    void __cdecl Record_Break_Position_(char char_code) {
        if (_line_p->line_started == 0 && char_code != '\0') {
            _break_p->break_valid = true;

            int16_t hyphen_x = 0;
            if (char_code == '\b') {
                hyphen_x = (int16_t)_style_p->char_widths['-'] + _line_p->x;
                if (hyphen_x > _line_p->right_limit_x) {
                    return;
                }
            }

            _break_p->break_str_next = _para_p->str + 1;
            _break_p->break_x = _line_p->x;
            _break_p->break_x_candidate = _line_p->x_candidate;
            _break_p->insert_case = _insert_case;
            _break_p->line_y_offset = _line_p->line_y_offset;
            _break_p->font_spacing = _line_p->font_spacing;
            _break_p->nest_depth = _para_p->nest_depth;
            _break_p->char_count = _line_p->char_count;
            _break_p->space_count = _line_p->space_count;

            // Copy next line configuration into break point state
            _break_p->x = _line_p[1].x;
            _break_p->left_limit_x = _line_p[1].left_limit_x;
            _break_p->right_limit_x = _line_p[1].right_limit_x;
            _break_p->exclusion_left_x = _line_p[1].exclusion_left_x;
            _break_p->exclusion_right_x = _line_p[1].exclusion_right_x;
            _break_p->x_candidate = _line_p[1].x_candidate;
            _break_p->line_char_count = (int8_t)_line_p[1].char_count;

            _break_p->break_is_backspace = false;
            if (char_code == '\b') {
                _break_p->break_is_backspace = true;
                _break_p->break_x = hyphen_x;
                _break_p->break_x_candidate = hyphen_x;
                _break_p->char_count++;
            } else if (char_code == ' ') {
                _break_p->break_x_candidate -= (_style_p->char_widths[' '] + _line_p->font_spacing);
            }
        } else {
            _break_p->break_valid = false;
        }
    }

    bool __cdecl Previous_Nesting_Level_() {
        if (_para_p->nest_depth > 0) {
            _para_p->nest_depth--;
            _para_p->str = _para_p->nest_stack[_para_p->nest_depth];
            _para_p->str--;
            return true;
        }
        return false;
    }

    void __cdecl Set_Value_() {
        int16_t rel_x = _line_p->x - _para_p->x1;
        char cmd = _para_p->str[1];

        switch (cmd) {
            case 'I':
                _para_p->first_line_indent = rel_x - _para_p->left_indent;
                break;

            case 'L':
                _para_p->left_indent = rel_x;
                break;

            case 'R':
                _para_p->right_indent = _para_p->width - rel_x;
                break;

            case 'T':
                if (_tabs[0] < 10) {
                    _tabs[_tabs[0] + 1] = _line_p->x;
                    _tabs[0]++;
                    Sort_Tabs_();
                }
                break;

            case 'i':
                _para_p->first_line_indent = 0;
                break;

            case 'l':
                _para_p->left_indent = 0;
                break;

            case 'm':
                _para_p->left_indent = 0;
                // Fallthrough
            case 'r':
                _para_p->right_indent = 0;
                break;

            case 't':
                _tabs[0] = 0;
                break;

            default:
                break;
        }
    }

    bool __cdecl Quit_Context_() {
        char current_char = *_para_p->str;
        _para_p->str++;

        if (current_char == 'L' /* 0x4c */) {
            if (_para_global.cond_flag) {
                Previous_Nesting_Level_();
            }
            // Logic falls through to return false
        } else if (current_char == 'P' /* 0x50 */) {
            Quit_Paragraph_();
            return true;
        }

        return false;
    }

    void __cdecl Set_Justification_() {
        _para_p->str++;
        int8_t new_mode = *_para_p->str - '0';

        if (_line_p->char_count > 0) {
            int16_t saved_right_limit_x = _line_p->right_limit_x;
            _line_p->right_limit_x = _line_p->x;

            Complete_Line_();

            _line_p->right_limit_x = saved_right_limit_x;
            _para_p->line_start_x = _line_p->x;

            Initialize_Line_Start_();

            _char_p = _first_char_p;
            _line_p->line_done = false;
        }

        _para_p->justify_mode = new_mode;
    }

    void __cdecl Complete_Line_() {
        Complete_Line_Data_();

        s_fmtpara_para* para = _para_p;
        int8_t mode = para->justify_mode;

        // If justifying fully, check if this is the end of a block/paragraph.
        // Special character codes: 13 (CR), 10 (LF), 12 (FF).
        if (mode == 3 /* JUSTIFY_FULL */) {
            char current_char = *para->str;
            if (current_char == '\r' || current_char == '\n' || current_char == '\f' || 
               (para->nest_depth == 0 && current_char == '\0')) {
                
                // Do not fully justify the last line of a paragraph.
                Justify_Line_(0 /* JUSTIFY_LEFT */);
                Print_Line_();
                return;
            }
        }

        Justify_Line_(mode);
        Print_Line_();
    }

    void __cdecl Test_Item_() {
        _para_global.cond_flag = false;
        _para_p->str++;
        char* current_str = _para_p->str;
        char c = *current_str;
        uint32_t val1 = Get_Next_Item_Value_();

        switch (c) {
            case '0':
                _para_global.cond_flag = (val1 != 0);
                break;

            case '1':
                _para_global.cond_flag = _para_global.use_plural;
                _para_p->str = current_str;
                break;

            case '2':
                _para_global.cond_flag = !_para_global.cond_flag;
                _para_p->str = current_str;
                break;

            case '3': {
                _para_p->str = current_str;
                uint32_t cond_val = (uint32_t)_para_global.cond_flag;
                uint16_t arg;
                if (Get_Insert_Argument_Next_(&arg)) {
                    _para_global.cond_flag = (bool)Set_Item_Value_(cond_val, arg);
                }
                break;
            }

            case '4': {
                uint16_t arg;
                if (Get_Insert_Argument_Next_(&arg)) {
                    _para_global.cond_flag = (bool)Set_Item_Value_(val1, arg);
                }
                break;
            }

            case '5': {
                uint32_t val2 = Get_Next_Item_Value_();
                _para_global.cond_flag = (val2 == val1);
                break;
            }

            case '6': {
                uint32_t val2 = Get_Next_Item_Value_();
                _para_global.cond_flag = (val2 != val1);
                break;
            }

            case '7': {
                uint32_t val2 = Get_Next_Item_Value_();
                _para_global.cond_flag = ((int32_t)val2 > (int32_t)val1);
                break;
            }

            case '8': {
                uint32_t val2 = Get_Next_Item_Value_();
                _para_global.cond_flag = ((int32_t)val2 < (int32_t)val1);
                break;
            }

            case '9': {
                uint32_t val2 = Get_Next_Item_Value_();
                _para_global.cond_flag = ((int32_t)val2 >= (int32_t)val1);
                break;
            }

            case 'A': {
                uint32_t val2 = Get_Next_Item_Value_();
                _para_global.cond_flag = ((int32_t)val2 <= (int32_t)val1);
                break;
            }

            default:
                break;
        }
    }

    void __cdecl Set_Current_Colors_() {
        _para_p->str++;
        _style_p->color_attr = (uint8_t)(*_para_p->str << 5);
    }

    void __cdecl Set_Current_Style_() {
        uint16_t arg;
        _para_p->str++;
        uint8_t ch = (uint8_t)*_para_p->str;

        if (ch == '6' /* 0x36 */) {
            if (Get_Insert_Argument_Next_(&arg)) {
                if (arg <= 7) {
                    _style_p->bg_color = (uint8_t)arg;
                }
            }
        } else {
            if ((uint16_t)(ch - '0' /* 0x30 */) <= 5) {
                _style_p->fg_color = ch;
            }
        }
    }

    bool __cdecl Get_Function_Argument_(int32_t* val, bool* flag) {
        int8_t is_blank_arg = 1;

        _para_p->str = Skip_Spaces_(_para_p->str);

        if (*_para_p->str == '+' || *_para_p->str == '-') {
            *flag = true;
        } else {
            *flag = false;
        }

        *val = _atoi(_para_p->str);

        // Advance current pointer past all characters that are legal in an argument
        while (strchr(_legal_fn_arg_chars, *_para_p->str) != nullptr) {
            if (*_para_p->str != ' ') {
                is_blank_arg = 0;
            }
            _para_p->str++;
        }

        return (int16_t)is_blank_arg;
    }

    char __cdecl FN_Set_Variable_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (arg_index == 1 /* Case 1: Initialize index */) {
            if (arg_ret == 0) {
                _var_index = (uint16_t)abs(val);
            }
            return func_char;
        }

        if (arg_index == 2 /* Case 2: Update item value */) {
            if (_var_index < 0x10) {
                int32_t value = val;
                if (flag) {
                    value += Get_Item_Value_(_var_index);
                }
                Set_Item_Value_(value, _var_index);
            }
        }

        return 0;
    }

    char __cdecl FN_Set_Font_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (flag) {
            val += _style_p->font_id;
        }

        if (val >= 0 && val < 6) {
            uint8_t old_font_base = _font_base_offsets[_style_p->font_id];

            _style_p->font_id = static_cast<int8_t>(val);

            if (func_char == 'F' /* 0x46 */) {
                _style_p->highlight = 0;
            } else {
                _style_p->highlight = 1;
            }

            Reset_Font_Spacing_();
            Reset_Font_LF_();
            Set_Para_Y2_();

            int diff = static_cast<int>(_font_base_offsets[_style_p->font_id]) - static_cast<int>(old_font_base);

            if (_line_p->line_y_offset < static_cast<int16_t>(diff)) {
                _line_p->line_y_offset = static_cast<int16_t>(diff);
            }

            int adjusted_para_y = static_cast<int>(_para_p->para_y) - diff;
            if (adjusted_para_y >= 0) {
                _para_p->para_y = static_cast<int16_t>(adjusted_para_y);
                return 0;
            } else {
                _line_p->line_y_offset = _para_p->para_y;
                _para_p->para_y = 0;
            }
        }

        return 0;
    }
    char __cdecl FN_Set_H_Spacing_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (arg_ret == 0) {
            int32_t spacing = val;

            if (flag) {
                spacing += (int32_t)_line_p->font_spacing;
            }

            // Check if horizontal spacing is within valid bounds [0, 64)
            if (spacing >= 0 && spacing < 0x40) {
                _line_p->font_spacing = (int16_t)spacing;
            }
        }

        return 'S';
    }

    char __cdecl FN_Set_Indent_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        int32_t new_indent;

        if (flag) {
            // Relative indentation adjustment
            new_indent = val + _para_p->first_line_indent;
        } else {
            // Absolute indentation adjustment
            new_indent = val;
        }

        // Calculate total left position (left indent + the new indentation value)
        int32_t total_left_pos = (int32_t)_para_p->left_indent + new_indent;

        // Ensure the paragraph block still fits within the defined width
        if ((int32_t)_para_p->right_indent + total_left_pos < (int32_t)_para_p->width) {
            if (total_left_pos >= 0) {
                // If indentation results in a valid positive coordinate, apply it
                _para_p->first_line_indent = (int16_t)new_indent;
            } else {
                // If indentation would move text outside of bounds, clamp to left_indent
                _para_p->first_line_indent = _para_p->left_indent;
            }
        }

        return 0;
    }

    char __cdecl FN_Set_Margins_(char status, int margin_val, int16_t side, bool skip_logic, bool apply_delta) {
        if (skip_logic == 0) {
            if (side == 1 /* Left side */) {
                if (apply_delta != 0) {
                    margin_val += _para_p->left_indent;
                }

                if (margin_val < (int)_para_p->width) {
                    if (margin_val < 0) {
                        _para_p->left_indent = 0;
                    } else {
                        _para_p->left_indent = (int16_t)margin_val;
                    }

                    // Ensure that the left boundary (indent + x1) does not overlap the right boundary (x2 + right_indent)
                    if ((int)_para_p->left_indent + (int)_para_p->x1 >= (int)_para_p->x2 + (int)_para_p->right_indent) {
                        _para_p->right_indent = 0;
                    }
                }
            } else { // Right side
                if (apply_delta != 0) {
                    margin_val += _para_p->right_indent;
                }

                if (margin_val >= 0) {
                    // Only apply if the right indent doesn't exceed the available width based on current left indent
                    if (margin_val < (int)_para_p->width - (int)_para_p->left_indent) {
                        _para_p->right_indent = (int16_t)margin_val;
                    }
                }
            }
        }

        if (side == 1) {
            return status;
        }

        return 0x49 /* 'I' */;
    }

    char __cdecl FN_Set_FF_Size_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (arg_ret != 0) {
            _para_p->para_advance_y = _para_p->line_advance_y;
        } else {
            int32_t value;
            if (flag) {
                value = val + (int32_t)_para_p->para_advance_y;
            } else {
                value = val;
            }

            int32_t absolute_value = value;
            if (absolute_value < 0) {
                absolute_value = -absolute_value;
            }

            if (absolute_value < 64 /* 0x40 */) {
                _para_p->para_advance_y = (int16_t)value;
            }
        }

        return 0;
    }

    char __cdecl FN_Set_Space_Size_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (arg_ret != 0) {
            _style_p->space_width = _line_p->font_spacing;
        } else {
            int32_t new_width = val;

            if (flag) {
                new_width += _style_p->space_width;
            }

            if (new_width >= 0 && new_width < 0x40 /* 64 */) {
                _style_p->space_width = (int16_t)new_width;
            }
        }

        return 0;
    }

    char __cdecl FN_Set_Tabs_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (_tabs[0] < 10) {
            short base_x;
            if (flag) {
                base_x = _line_p->x;
            } else {
                base_x = _para_p->x1;
            }

            int32_t calculated_x = val + base_x;

            if (_para_p->x1 <= calculated_x && calculated_x <= _para_p->x2 && arg_ret == 0) {
                short current_count = _tabs[0];
                _tabs[current_count + 1] = static_cast<short>(calculated_x);
                _tabs[0]++;
            }

            return func_char;
        }

        return 0;
    }

    char __cdecl FN_Set_Origin_(char param_1, int32_t param_2, int16_t param_3, bool param_4, bool param_5) {
        if (param_4 == 0 /* false */) {
            if (param_3 == 1) {
                if (param_5 != 0 /* true */) {
                    param_2 += (int32_t)_para_p->x1;
                }
                // Bounds check: 0 <= x < 640
                if (param_2 >= 0 && param_2 < 640) {
                    _para_p->x1 = (int16_t)param_2;
                    return param_1;
                }
            }
            else if (param_3 == 2) {
                if (param_5 != 0 /* true */) {
                    param_2 += (int32_t)_para_p->y;
                }
                // Bounds check: 0 <= y < 480
                if (param_2 >= 0 && param_2 < 480) {
                    _para_p->y = (int16_t)param_2;
                    return param_1;
                }
            }
            else if (param_3 == 3) {
                if (param_5 != 0 /* true */) {
                    param_2 += (int32_t)_para_p->width;
                }
                // Bounds check: 0 < width <= 640
                if (param_2 > 0 && param_2 <= 640) {
                    _para_p->width = (int16_t)param_2;
                }
            }
        }
        return param_1;
    }

    char __cdecl FN_Set_V_Spacing_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        if (arg_ret == 0) {
            int32_t v_spacing = val;

            if (flag) {
                v_spacing += _para_p->line_advance_y;
            }

            int32_t abs_v_spacing = v_spacing;
            if (abs_v_spacing < 0) {
                abs_v_spacing = -abs_v_spacing;
            }

            if (abs_v_spacing < 0x40) {
                _para_p->line_advance_y = (int16_t)v_spacing;
            }
        }

        return 'P';
    }

    char __cdecl FN_Set_X_Pos_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        short base_x;

        if (flag) {
            base_x = _line_p->x;
        } else {
            base_x = _para_p->x1;
        }

        int new_x = val + base_x;

        if (new_x >= _para_p->x1 && new_x <= _para_p->x2) {
            _line_p->x = static_cast<short>(new_x);
        }

        return 'Y';
    }

    char __cdecl FN_Set_Y_Pos_(char func_char, int32_t val, int16_t arg_index, bool arg_ret, bool flag) {
        short start_y;

        // Determine if we are calculating relative to current paragraph Y or the absolute Y limit
        if (flag) {
            start_y = _para_p->para_y;
        } else {
            start_y = _para_p->y;
        }

        int final_y = val + (int)start_y;

        // Clamp the new paragraph Y position between 0 and the vertical origin/limit
        if (final_y > (int)_para_p->y_origin) {
            final_y = (int)_para_p->y_origin;
        } else if (final_y < 0) {
            final_y = 0;
        }

        _para_p->para_y = (short)final_y;

        // Calculate discrepancy between the base y and the clamped paragraph y
        int diff = (int)_para_p->y - (int)_para_p->para_y;

        if (diff > 0) {
            // Update line vertical offset if current offset is smaller than the discrepancy
            if ((int)_line_p->line_y_offset < diff) {
                _line_p->line_y_offset = (short)diff;
            }
        }

        return 0;
    }

    char* __cdecl Skip_Spaces_(char* str) {
        char* current = str;
        char c = *str;

        while (true) {
            if (c != ' ') {
                if (*current != '\0') {
                    return current;
                }
                return str;
            }

            if (*current == '\0') {
                return str;
            }

            c = current[1];
            current++;
        }
    }

    /**
     * @address 00435a70
     */
    void __cdecl Sort_Tabs_() {
        if (_tabs[0] > 1) {
            _qsort(&_tabs[1], (size_t)_tabs[0], sizeof(int16_t), _Comp_Tabs_);
        }
    }
}