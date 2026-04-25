#include "pch.h"

namespace fields {
    void __cdecl Assign_Update_Function_(MOX2::action_func_t fn) {
        _update_function = fn;
    }

    void __cdecl Assign_Auto_Function_(MOX2::action_func_t fn, int32_t delay) {
        _auto_active_flag = 1;
        _function = fn;

        if (delay > 0 && delay < 10) {
            _auto_function_delay = (uint16_t)delay;
        } else {
            _auto_function_delay = 2;
        }
    }

    void __cdecl Deactivate_Auto_Function_() {
        _auto_active_flag = 0;
    }

    void __cdecl Activate_Auto_Function_() {
        _auto_active_flag = 1;
    }

    int16_t __cdecl Auto_Input_() {
        return _auto_input_variable;
    }

    void __cdecl Set_Help_List_(void* help_data, int16_t count) {
        _help_list_count = count;
        _help_struct_pointer = help_data;
        _help_list_active = 1;
    }

    void __cdecl Disable_Draw_All_Fields_() {
        _draw_fields_flag = 0;
    }

    void __cdecl Enable_Draw_All_Fields_() {
        _draw_fields_flag = 1;
    }

    void __cdecl Disable_Cancel_() {
        _mouse_cancel_disabled = 1;
    }

    void __cdecl Deactivate_Help_List_() {
        _help_list_active = 0;
        _help_struct_pointer = nullptr;
        _help_list_count = 0;
    }

    void __cdecl Invoke_Auto_Function_() {
        if (_auto_active_flag != 0) {
            if (_function != nullptr) {
                _function();
            }
        }
    }

    void __cdecl Invoke_Saved_Auto_Function_() {
        _function2();
    }

    void __cdecl Save_Auto_Function_() {
        _function2 = _function;
    }

    void __cdecl Restore_Auto_Function_() {
        _auto_active_flag = 1;
        _function = _function2;
    }

    void __cdecl Set_Input_Delay_(uint16_t delay) {
        _input_delay = delay;
        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }

    void __cdecl Set_Field_Outline_Color_(uint8_t color) {
        _field_outline_color = color;
    }

    void __cdecl Clear_Mouse_Buffer_() {
        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }

    int16_t __cdecl Get_Input_() {
        if (_input_delay > 0) {
            _input_delay--;
            return 0;
        }

        if (_input_delay < 0) {
            _input_delay = 0;
        }

        if (_fields_count < 2) {
            return 0;
        }

        int16_t input_result = Interpret_Mouse_Input_();
        video::Set_Page_Off_();

        if (input_result != 0) {
            int16_t field_idx = (input_result < 0) ? -input_result : input_result;
            sound::Play_Sound_(_fields[field_idx].sound);
        }

        return input_result;
    }

    void __cdecl Clear_Fields_() {
        _fields_count = 1;
        _input_delay = 0;
        _down_mouse_button = -1;
        _auto_input_variable = 0;
        _input_field_active = 0;
        _active_input_field_number = -1;
        _multi_hotkey_active_field = 0;

        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }

    void __cdecl Clear_Fields_Above_(int16_t param_1) {
        _down_mouse_button = -1;
        _fields_count = param_1 + 1;
        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }

    int16_t __cdecl Get_Field_Counter_() {
        return _fields_count;
    }

    void __cdecl Enable_Mouse_Pressed_Field_Update_() {
        _dynamic_field_refresh = 1;
        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }

    int16_t __cdecl Add_Hot_Key_(const char* hotkey_str) {
        if (_fields_count >= _max_fields_count) {
            char error_msg[80];
            const uint32_t* src = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields);
            uint32_t* dst = reinterpret_cast<uint32_t*>(error_msg);

            for (int i = 0; i < 11; ++i) {
                dst[i] = src[i];
            }
            for (int i = 11; i < 20; ++i) {
                dst[i] = 0;
            }

            exitm::Exit_With_Message(error_msg);
        }

        s_field& field = _fields[_fields_count];

        field.x = 5000;
        field.y = 5000;
        field.x_end = 5000;
        field.y_end = 5000;
        field.field_type = FIELD_TYPE_HIDDEN;

        char hk = *hotkey_str;
        if (hk > 0x60 && hk < 0x7B) {
            hk -= 0x20;
        }
        field.hotkey = hk;

        int16_t result_index = _fields_count;
        _fields_count++;
        return result_index;
    }

    void __cdecl Quick_Call_Auto_Function_() {
        if (_auto_active_flag == 0) {
            platform::Toggle_Pages_Stub_();
            return;
        }

        timer::Mark_Time_();
        video::Set_Page_Off_();
        Invoke_Auto_Function_();
        platform::Toggle_Pages_Stub_();
        timer::Release_Time_(1);

        if (_update_function != nullptr) {
            _update_function();
        }
    }

    void __cdecl Add_Button_Info_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int16_t sound) {
        s_field* field = &_fields[_fields_count];

        field->x = x;
        field->y = y;
        field->detail.button.help = const_cast<char*>(help);

        field->x_end = field->x + animate::Get_Width_(pic) - 1;
        field->y_end = field->y + animate::Get_Height_(pic) - 1;

        field->font_style = fonts::Get_Current_Font_Style_();
        field->font_color = fonts::Get_Current_Font_Color_();

        field->sound = (int16_t)(int8_t)sound;
        field->detail.button.pic = pic;
        field->detail.button.full_store_flag = animate::Get_Full_Store_Flag_(pic);

        char key = *hotkey;
        if (key > '`' && key < '{') {
            key -= 0x20;
        }
        field->hotkey = key;
    }

    int16_t __cdecl Add_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int16_t sound) {
        char error_msg[80];

        const uint32_t* src = reinterpret_cast<const uint32_t*>("You exceeded the max number of fields (150)");
        uint32_t* dest = reinterpret_cast<uint32_t*>(error_msg);
        for (int i = 0; i < 11; ++i) {
            dest[i] = src[i];
        }
        for (int i = 11; i < 20; ++i) {
            dest[i] = 0;
        }

        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(error_msg);
        }

        s_field* field = &_fields[_fields_count];

        field->x = x;
        field->y = y;
        field->x_end = x_end;
        field->y_end = y_end;
        field->field_type = FIELD_TYPE_HIDDEN;
        field->sound = sound;
        field->hotkey = *hotkey;

        if (field->hotkey > '`' && field->hotkey < '{') {
            field->hotkey -= 0x20;
        }

        int16_t added_index = _fields_count;
        _fields_count++;

        return added_index;
    }

    int16_t __cdecl Add_Multi_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* value, int16_t count, const char* hotkey, int16_t sound) {
        char error_msg[80];

        const uint32_t* src = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields);
        uint32_t* dest = reinterpret_cast<uint32_t*>(error_msg);
        for (int i = 0; i < 11; ++i) {
            dest[i] = src[i];
        }
        for (int i = 11; i < 20; ++i) {
            dest[i] = 0;
        }

        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(error_msg);
        }

        Add_Button_Info_(x, y, help, pic, hotkey, sound);

        s_field* field = &_fields[_fields_count];

        field->field_type = FIELD_TYPE_MULTI_BUTTON;
        field->detail.multibtn.value = value;
        field->detail.multibtn.count = count;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int16_t sound) {
        char error_msg[80];

        const uint32_t* src = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields);
        uint32_t* dest = reinterpret_cast<uint32_t*>(error_msg);
        for (int i = 0; i < 11; ++i) {
            dest[i] = src[i];
        }
        for (int i = 11; i < 20; ++i) {
            dest[i] = 0;
        }

        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(error_msg);
        }

        Add_Button_Info_(x, y, help, pic, hotkey, sound);

        _fields[_fields_count].field_type = FIELD_TYPE_BUTTON;

        int16_t current_id = _fields_count;
        _fields_count++;

        return current_id;
    }

    int16_t __cdecl Add_Radio_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* variable, const char* hotkey, int16_t sound) {
        char error_msg[80];

        const uint32_t* src = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields);
        uint32_t* dest = reinterpret_cast<uint32_t*>(error_msg);
        for (int i = 0; i < 11; ++i) {
            dest[i] = src[i];
        }
        for (int i = 11; i < 20; ++i) {
            dest[i] = 0;
        }

        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(error_msg);
        }

        Add_Button_Info_(x, y, help, pic, hotkey, sound);

        int16_t current_field = _fields_count;
        _fields[current_field].field_type = FIELD_TYPE_RADIO;
        _fields[current_field].detail.radio.selection = variable;

        _fields_count++;

        return current_field;
    }

    int16_t __cdecl Add_Clipped_Button_Field_(int16_t x, int16_t y, char* help, s_animation_header* pic, void* highlight_color, char hotkey) {
        char error_msg[80];
        // The original code manually copies the error message into a local buffer.
        for (int i = 0; i < 11; ++i) {
            reinterpret_cast<uint32_t*>(error_msg)[i] = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields)[i];
        }
        for (int i = 0; i < 9; ++i) {
            reinterpret_cast<uint32_t*>(error_msg + 44)[i] = 0;
        }

        if (animate::_clipping_flag == 1) {
            int16_t width = animate::Get_Width_(pic);
            int16_t height = animate::Get_Height_(pic);

            if (x < video::_screen_window_x1 || 
                (x + width) > video::_screen_window_x2 || 
                y < video::_screen_window_y1 || 
                (y + height) > video::_screen_window_y2) 
            {
                return -1000;
            }
        }

        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(error_msg);
        }

        Add_Button_Info_(x, y, help, pic, (const char*)highlight_color, (int16_t)hotkey);

        _fields[_fields_count].field_type = FIELD_TYPE_BUTTON;
        
        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Irregular_Button_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* help, s_animation_header* pic, int16_t help_start, int16_t help_end, const char* hotkey_ptr, int16_t sound) {
        if (_fields_count > _max_fields_count) {
            char error_buffer[80];
            _sprintf(error_buffer, s_Maximum_number_of_button_fields_e_0055b0b0, (int)_max_fields_count);
            exitm::Exit_With_Message(error_buffer);
        }

        s_field& field = _fields[_fields_count];

        field.x = x;
        field.y = y;
        field.x_end = x_end;
        field.y_end = y_end;
        field.detail.button.help = const_cast<char*>(help);
        field.detail.button.pic = pic;
        field.detail.button.help_start = help_start;
        field.detail.button.help_end = help_end;
        field.detail.button.full_store_flag = animate::Get_Full_Store_Flag_(pic);
        
        field.font_style = fonts::Get_Current_Font_Style_();
        field.font_color = fonts::Get_Current_Font_Color_();
        field.sound = sound;

        char hk = *hotkey_ptr;
        if (hk > '`' && hk < '{') {
            hk -= 0x20;
        }
        field.hotkey = hk;
        
        field.field_type = FIELD_TYPE_IRREGULAR_INPUT;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t step_x, int16_t step_y, int16_t cols, int16_t rows, short* grid_x, short* grid_y, char sound) {
        if (_fields_count >= _max_fields_count) {
            char error_msg[80];
            const uint32_t* src = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields);
            uint32_t* dst = reinterpret_cast<uint32_t*>(error_msg);

            for (int i = 0; i < 11; ++i) {
                dst[i] = src[i];
            }
            for (int i = 11; i < 20; ++i) {
                dst[i] = 0;
            }

            exitm::Exit_With_Message(error_msg);
        }

        s_field& field = _fields[_fields_count];

        field.x = x;
        field.y = y;
        field.x_end = x + (cols * step_x) - 1;
        field.y_end = y + (rows * step_y) - 1;
        field.sound = static_cast<int16_t>(sound);
        field.field_type = FIELD_TYPE_GRID;

        field.detail.grid.step_x = step_x;
        field.detail.grid.step_y = step_y;
        field.detail.grid.grid_x_ptr = grid_x;
        field.detail.grid.grid_y_ptr = grid_y;

        field.hotkey = '\0';

        if (field.hotkey > '`' && field.hotkey < '{') {
            field.hotkey -= 32;
        }

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Continuous_String_Input_Field_(int16_t x, int16_t y, int16_t width, int16_t height, char* help, int16_t max_len, int16_t help_end, int32_t buffer_ptr, int16_t sound, int16_t param_10) {
        if (_fields_count >= _max_fields_count) {
            char error_buffer[80];
            const uint32_t* src = reinterpret_cast<const uint32_t*>(s_You_exceeded_the_max_number_of_fields);
            uint32_t* dst = reinterpret_cast<uint32_t*>(error_buffer);

            for (int i = 0; i < 11; ++i) {
                dst[i] = src[i];
            }
            for (int i = 11; i < 20; ++i) {
                dst[i] = 0;
            }

            exitm::Exit_With_Message(error_buffer);
        }

        s_colors* normal_src = fonts::Get_Current_Font_Color_();
        s_colors* highlight_src = fonts::Get_Current_Font_Color_();

        for (int i = 0; i < 8; ++i) {
            _list_normal_color.colors[i] = normal_src->colors[i];
            _list_highlight_colors.colors[i] = highlight_src->colors[i];
        }

        s_field* field = &_fields[_fields_count];

        field->x = x;
        field->y = y;
        field->x_end = x + width;
        field->y_end = y + height;
        field->field_type = FIELD_TYPE_CONTINUOUS_INPUT;
        field->font_style = fonts::Get_Current_Font_Style_();
        field->font_color = &_list_normal_color;

        field->detail.cont_input.highlight_color = &_list_highlight_colors;
        field->detail.cont_input.help = help;
        field->detail.cont_input.field5_0xc = buffer_ptr;
        field->detail.cont_input.help_end = help_end;
        field->detail.cont_input.full_store_flag = 0;

        if (max_len > 256) {
            max_len = 256;
        }
        field->detail.cont_input.field13_0x1c = max_len;
        field->detail.cont_input.field16_0x20 = param_10;

        field->hotkey = '\0';
        field->sound = (int16_t)(int8_t)sound;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Scroll_Field_(int16_t x, int16_t y, int16_t min_val, int16_t max_val, int16_t pos, int16_t step, int16_t width, int16_t height, short* context_ptr, const char* hotkey_ptr, char sound) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(s_You_exceeded_the_max_number_of_fields);
        }

        s_field& field = _fields[_fields_count];

        field.x = x;
        field.y = y;
        field.x_end = field.x + width;
        field.y_end = field.y + height;

        field.detail.scroll.field4_0x8 = pos;
        field.detail.scroll.field7_0xc = step;
        field.detail.scroll.field11_0x14 = min_val;
        field.detail.scroll.field14_0x18 = max_val;
        field.sound = static_cast<int16_t>(sound);

        if (height < width) {
            field.detail.scroll.field17_0x1c = 0;
        } else {
            field.detail.scroll.field17_0x1c = 1;
        }

        field.field_type = FIELD_TYPE_SCROLL;
        field.detail.scroll.value = context_ptr;

        char hk = *hotkey_ptr;
        if (hk > 0x60 && hk < 0x7B) {
            hk -= 0x20;
        }
        field.hotkey = hk;

        int16_t index = _fields_count;
        _fields_count++;
        return index;
    }

    int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* help_text) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(s_You_exceeded_the_max_number_of_fields);
        }

        // Access the current field structure (size 0x37 / 55 bytes)
        s_field& field = _fields[_fields_count];

        field.x = -1;
        field.y = -1;
        field.x_end = -1;
        field.y_end = -1;
        field.hotkey = '\0';
        field.field_type = FIELD_TYPE_MULTI_HOTKEY;

        field.detail.button.help = const_cast<char*>(help_text);
        field.detail.button.help_start = 0;

        // Calculate length of help string manually as seen in the assembly (SCASB)
        uint16_t length = 0;
        if (help_text != nullptr) {
            while (help_text[length] != '\0') {
                length++;
            }
        }
        field.detail.button.help_end = (int16_t)length;

        _multi_hotkey_active_field = 1;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Scan_Field_() {
        int16_t mouse_x = static_cast<int16_t>(mouse::Pointer_X_());
        int16_t mouse_y = static_cast<int16_t>(mouse::Pointer_Y_());
        mouse::Check_Mouse_Shape_();
        _pointer_offset = mouse::Get_Pointer_Offset_();

        if (_fields_count > 1) {
            int32_t adj_x = static_cast<int32_t>(mouse_x) + _pointer_offset;
            int32_t adj_y = static_cast<int32_t>(mouse_y) + _pointer_offset;

            for (int16_t i = 1; i < _fields_count; ++i) {
                s_field* field = &_fields[i];

                if (field->x <= adj_x && adj_x <= field->x_end &&
                    field->y <= adj_y && adj_y <= field->y_end) {
                    return i;
                }
            }
        }

        return 0;
    }

    int16_t __cdecl Add_Locked_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* selection, const char* hotkey, int16_t sound) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message(s_You_exceeded_the_max_number_of_fields);
        }

        Add_Button_Info_(x, y, help, pic, hotkey, sound);

        s_field& current_field = _fields[_fields_count];
        current_field.field_type = FIELD_TYPE_LOCKED;
        current_field.detail.radio.selection = selection;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    void __cdecl Release_Fields_() {
        _fields_count = _old_fields_count;
        _input_delay = _old_input_delay;
        _down_mouse_button = _old_down_mouse_button;

        // Disassembly uses a 32-bit move here, despite the symbol being a short.
        *reinterpret_cast<int32_t*>(&_max_fields_count) = 0xfa;

        _auto_input_variable = _old_auto_input_variable;
        _input_field_active = _old_input_field_active;
        _fields = _old_fields;
        _active_input_field_number = _old_active_input_field_number;
    }

    int16_t __cdecl Add_String_List_Field_(int16_t x, int16_t y, int16_t width, char* help, int16_t param_5, char** strings, int16_t help_start, int16_t param_8, int16_t* value, int32_t param_10, s_animation_header* pic, int32_t param_12, char* hotkey_str, char* param_14) {
        int16_t n = _fields_count;

        _fields[n].x = x;
        _fields[n].y = y - 1;
        _fields[n].font_style = fonts::Get_Current_Font_Style_();
        _fields[n].font_color = &_list_normal_color;
        _fields[n].detail.strings.highlight_color = &_list_highlight_colors;
        _fields[n].detail.strings.special_color = &_list_special_colors;
        _fields[n].x_end = x + width;
        _fields[n].y_end = y + fonts::Get_Font_Height_() + 1;
        _fields[n].detail.strings.help_start = help_start;
        _fields[n].hotkey = *hotkey_str;
        _fields[n].detail.strings.field1_0x2 = param_5;

        if (_fields[n].hotkey > '`' && _fields[n].hotkey < '{') {
            _fields[n].hotkey -= 0x20;
        }

        _fields[n].detail.strings.help = help;
        _fields[n].detail.strings.strings = strings;
        _fields[n].detail.strings.field0_0x0 = param_8;
        _fields[n].detail.strings.value = value;
        _fields[n].detail.strings.field8_0x18 = param_10;
        _fields[n].detail.strings.pic = pic;
        _fields[n].field_type = FIELD_TYPE_10;

        _fields_count++;

        return _fields_count - 1;
    }

    void __cdecl Enable_Mouse_Wait_() {
        _mouse_auto_exit = 0;
    }

    void __cdecl Disable_Mouse_Wait_() {
        _mouse_auto_exit = 1;
    }

    void __cdecl Timed_Wait_For_Input_(int timeout) {
        bool has_input = false;

        if (timeout == 0) {
            do {
                if (key::Keyboard_Status_()) {
                    has_input = true;
                }
                if (mouse::Mouse_Buffer_() == 1) {
                    has_input = true;
                }
                Quick_Call_Auto_Function_();
            } while (!has_input);
        } else {
            int ticks = 0;
            do {
                if (key::Keyboard_Status_()) {
                    has_input = true;
                }
                if (mouse::Mouse_Buffer_() == 1) {
                    has_input = true;
                }

                ticks++;
                Quick_Call_Auto_Function_();

                if (ticks == timeout) {
                    has_input = true;
                }
            } while (!has_input);
        }
    }

    void __cdecl Draw_All_Fields_() {
        _draw_override_flag = 1;
        Draw_Visible_Fields_();
        _draw_override_flag = 0;
    }

    void __cdecl Mark_Fields_() {
        _old_fields_count = _fields_count;
        _old_input_delay = _input_delay;
        _old_down_mouse_button = _down_mouse_button;
        _old_auto_input_variable = _auto_input_variable;
        _old_input_field_active = _input_field_active;
        _max_fields_count -= _fields_count;
        _old_active_input_field_number = _active_input_field_number;

        _fields += _fields_count;

        Clear_Fields_();
    }

    void __cdecl Clipped_Dot_(int16_t x, int16_t y, uint8_t color) {
        if (x >= video::_screen_window_x1 && x <= video::_screen_window_x2) {
            if (y >= video::_screen_window_y1 && y <= video::_screen_window_y2) {
                graphics::Dot_(x, y, color);
            }
        }
    }

    void __cdecl Exit_List_Field_() {
        _exit_the_list_field = 1;
    }

    int16_t __cdecl Add_Clipped_Circle_Field_(int16_t center_x, int16_t center_y, int16_t radius, char* hotkey, int8_t sound) {
        char msg[80] = "You exceeded the max number of fields (150)";

        int32_t max_x = center_x + radius;
        int32_t min_x = center_x - radius;
        int32_t min_y = center_y - radius;
        int32_t max_y = center_y + radius;

        if (_max_fields_count <= _fields_count) {
            exitm::Exit_With_Message(msg);
        }

        if (max_x < video::_screen_window_x1 ||
            video::_screen_window_x2 < min_x ||
            max_y < video::_screen_window_y1 ||
            video::_screen_window_y2 < min_y) {
            return -10000;
        }

        if (video::_screen_window_x1 > min_x) {
            _fields[_fields_count].x = video::_screen_window_x1;
        } else {
            _fields[_fields_count].x = static_cast<int16_t>(min_x);
        }

        if (video::_screen_window_x2 < max_x) {
            _fields[_fields_count].x_end = video::_screen_window_x2;
        } else {
            _fields[_fields_count].x_end = static_cast<int16_t>(max_x);
        }

        if (video::_screen_window_y1 > min_y) {
            _fields[_fields_count].y = video::_screen_window_y1;
        } else {
            _fields[_fields_count].y = static_cast<int16_t>(min_y);
        }

        if (video::_screen_window_y2 < max_y) {
            _fields[_fields_count].y_end = video::_screen_window_y2;
        } else {
            _fields[_fields_count].y_end = static_cast<int16_t>(max_y);
        }

        _fields[_fields_count].detail.circle.center_x = center_x;
        _fields[_fields_count].detail.circle.center_y = center_y;
        _fields[_fields_count].detail.circle.radius_squared = radius * radius;
        _fields[_fields_count].field_type = 14;
        _fields[_fields_count].sound = sound;
        _fields[_fields_count].hotkey = *hotkey;

        if (_fields[_fields_count].hotkey >= 'a' && _fields[_fields_count].hotkey <= 'z') {
            _fields[_fields_count].hotkey -= 0x20;
        }

        int16_t result = _fields_count;
        _fields_count++;

        return result;
    }

    void __cdecl Disable_Draw_Field_(int16_t field_idx) {
        switch (_fields[field_idx].field_type) {
            case 0:
                _fields[field_idx].field_type = -1001;
                break;
            case 1:
                _fields[field_idx].field_type = -1;
                break;
            case 2:
                _fields[field_idx].field_type = -2;
                break;
            case 3:
                _fields[field_idx].field_type = -3;
                break;
            default:
                break;
        }
    }

    void __cdecl Enable_Draw_Field_(int16_t field_idx) {
        int16_t field_type = _fields[field_idx].field_type;

        if (field_type > -3) {
            if (field_type == -2) {
                _fields[field_idx].field_type = 2;
            } else if (field_type == -1) {
                _fields[field_idx].field_type = 1;
            }
        } else if (field_type == -3) {
            _fields[field_idx].field_type = 3;
        } else if (field_type == -1001) {
            _fields[field_idx].field_type = 0;
        }
    }

    int16_t __cdecl Get_List_Field_(
        int16_t x, int16_t y, int16_t width, char* help, char* list_ptr, int32_t item_size,
        int16_t* selected_index, uint8_t* item_flags, int16_t param_9, int16_t* param_10,
        int32_t param_11, s_animation_header* pic, int32_t param_13, char* param_14)
    {
        _list_field_on = 1;
        Set_Input_Delay_(1);
        Clear_Fields_();
        _exit_the_list_field = 0;

        int16_t font_height = fonts::Get_Font_Height_();
        int16_t font_spacing = fonts::Get_Font_Vertical_Spacing_();

        int32_t local_14 = -1;
        int16_t local_c = 0;
        int32_t item_count = 0;
        int32_t current_y = y;
        int32_t y_step = font_height + font_spacing;

        bool done_parsing = false;
        do {
            if (*list_ptr == '\0') {
                done_parsing = true;
            } else {
                if (local_c == 0) {
                    if (item_flags != nullptr && item_flags[item_count] == 0) {
                        // Keep searching for the first unflagged item
                    } else {
                        local_c = 1;
                        local_14 = item_count;
                    }
                }

                current_y += y_step;

                int16_t flags = 1;
                if (item_flags != nullptr) {
                    flags = (int16_t)item_flags[item_count];
                }

                Add_String_List_Field_(
                    x, static_cast<int16_t>(current_y), width, list_ptr, flags,
                    (char**)selected_index, static_cast<int16_t>(item_count + 1),
                    (int16_t)(uintptr_t)param_9, (int16_t*)param_10, param_11, pic, param_13,
                    s__00551100, param_14
                );

                item_count++;
            }
            list_ptr += item_size;
        } while (!done_parsing);

        int16_t font_style = fonts::Get_Current_Font_Style_();
        s_colors* font_color = fonts::Get_Current_Font_Color_();
        s_colors* highlight_color = fonts::Get_Current_Highlight_Color_();
        s_colors* special_color = fonts::Get_Current_Special_Color_();

        fonts::Set_Font_Style_(font_style, font_color);

        for (int i = 0; i < 8; ++i) {
            _list_normal_color.colors[i] = font_color->colors[i];
            _list_special_colors.colors[i] = special_color->colors[i];
            _list_highlight_colors.colors[i] = highlight_color->colors[i];
        }

        int32_t local_4 = 0;
        int16_t help_field_id = Add_String_List_Field_(
            x, y, width, help, 1, (char**)&local_4, 1,
            0, nullptr, 0, nullptr, 0, s__00551100, DAT_0055afc0
        );

        int8_t sel = *selected_index;
        if (sel < 0 || sel >= item_count || sel < local_14) {
            if (local_14 < 0 || local_14 >= item_count) {
                *selected_index = -1;
            } else {
                *selected_index = static_cast<int8_t>(_fields[local_14 + 1].detail.button.help_start);
            }
        }

        bool bVar2 = false;
        int16_t input = 0;
        do {
            timer::Mark_Time_();
            input = Get_Input_();

            if (input != 0) {
                bVar2 = true;
                if (input > 0 && item_flags != nullptr && item_flags[input - 1] == 0) {
                    bVar2 = false;
                } else if (input == help_field_id) {
                    bVar2 = false;
                }
            }

            if (bVar2) {
                break;
            }

            if (_exit_the_list_field == 1) {
                bVar2 = true;
            }

            video::Set_Page_Off_();
            Invoke_Auto_Function_();

            if (_list_field_scan_flag != 0) {
                int16_t scan = Scan_Input_();
                if (scan > 0) {
                    *selected_index = static_cast<int8_t>(_fields[scan].detail.button.help_start);
                }
            }

            palette::Refresh_Palette_();
            platform::Toggle_Pages_Stub_();
            timer::Release_Time_(_auto_function_delay);

        } while (!bVar2);

        Clear_Fields_();
        _list_field_on = 0;
        _list_field_scan_flag = 1;
        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();

        if (input < 0) {
            return -1;
        }
        return input - 1;
    }

    int16_t __cdecl Add_Clipped_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int16_t sound) {
        if (_fields_count >= _max_fields_count) {
            char error_msg[80] = "You exceeded the max number of fields (150)";
            exitm::Exit_With_Message(error_msg);
        }

        if (x_end < video::_screen_window_x1 || x > video::_screen_window_x2 ||
            y_end < video::_screen_window_y1 || y > video::_screen_window_y2) {
            return -10000;
        }

        if (x < video::_screen_window_x1) {
            x = video::_screen_window_x1;
        }

        if (x_end > video::_screen_window_x2) {
            x_end = video::_screen_window_x2;
        }

        if (y < video::_screen_window_y1) {
            y = video::_screen_window_y1;
        }

        if (y_end > video::_screen_window_y2) {
            y_end = video::_screen_window_y2;
        }

        _fields[_fields_count].x = x;
        _fields[_fields_count].x_end = x_end;
        _fields[_fields_count].y = y;
        _fields[_fields_count].y_end = y_end;
        _fields[_fields_count].field_type = FIELD_TYPE_HIDDEN;
        _fields[_fields_count].sound = (int16_t)(int8_t)sound;
        _fields[_fields_count].hotkey = *hotkey;

        char c = _fields[_fields_count].hotkey;
        if (c >= 'a' && c <= 'z') {
            _fields[_fields_count].hotkey = c - 0x20;
        }

        int16_t result = _fields_count;
        _fields_count++;

        return result;
    }
}