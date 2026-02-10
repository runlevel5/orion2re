#include "deps.h"

namespace fields {
    void __cdecl Assign_Update_Function_(void (*function_ptr)()) {
        _update_function = function_ptr;
    }

    void __cdecl Assign_Auto_Function_(void (*function_ptr)(), int16_t delay) {
        _auto_active_flag = 1;
        _function = function_ptr;

        if (delay > 0 && delay < 10) {
            _auto_function_delay = delay;
        } else {
            _auto_function_delay = 2;
        }
    }

    void __cdecl Deactivate_Auto_Function_() {
        _auto_active_flag = 0;
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
                auto fn = reinterpret_cast<void(__cdecl*)()>(_function);
                fn();
            }
        }
    }

    void __cdecl Set_Input_Delay_(uint16_t delay) {
        _input_delay = delay;
        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }

    void __cdecl Set_Field_Outline_Color_(uint16_t color) {
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
}