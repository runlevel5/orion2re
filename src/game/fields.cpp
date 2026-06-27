#include "pch.h"

namespace fields {
    namespace {
        s_field g_fields_storage[MAX_FIELDS_COUNT];
        constexpr std::uintptr_t kInvalidStringMarker = static_cast<std::uintptr_t>(-1);
    }

    MOX2::action_func_t _update_function;
    uint16_t _auto_active_flag;
    uint16_t _list_field_on;
    int16_t _auto_function_delay = 2;
    uint16_t _list_field_scan_flag = 1;
    int16_t _down_mouse_button = -1;
    int16_t _auto_input_variable;
    uint16_t _input_field_active;
    int16_t _active_input_field_number = -1;
    int16_t _cursor_count;
    int16_t _cursor_position;
    int16_t _cursor_on;
    int16_t _help_list_active;
    int16_t _help_list_count;
    int16_t _multi_hotkey_active_field;
    uint16_t _draw_fields_flag = 1;
    char s__00551100[1] = "";
    char* _invalid_field_string_ptr = reinterpret_cast<char*>(kInvalidStringMarker);
    int32_t _max_fields_count = MAX_FIELDS_COUNT;
    uint8_t _exit_the_list_field;
    s_field* _fields = g_fields_storage;
    s_field* _old_fields = g_fields_storage;
    int16_t _pointer_offset;
    uint16_t _old_input_field_active;
    int16_t _old_active_input_field_number;
    int16_t _old_down_mouse_button;
    int16_t _old_fields_count;
    int16_t _old_input_delay;
    int16_t _old_auto_input_variable;
    int16_t _fields_count;
    int16_t _selected_count;
    int16_t mouse_saved_center_y;
    int16_t mouse_saved_center_x;
    uint16_t _pressed_mouse_x_value;
    uint16_t _pressed_mouse_y_value;
    int16_t _pressed_button_flag;
    int16_t _last_button_number;
    int16_t _last_button_x;
    int16_t _last_button_y;
    s_colors _list_normal_color;
    s_colors _list_highlight_colors;
    s_colors _list_special_colors;
    int16_t _input_delay;
    MOX2::action_func_t _function;
    MOX2::action_func_t _function2;
    s_help_box* _help_struct_pointer;
    char _continuous_string[256];
    uint8_t _active_input_field_first;
    uint16_t _field_outline_color = 14;
    uint16_t _help_field_outline_color = 180;
    int16_t _selected_button_text_x_offset = 1;
    int16_t _selected_button_text_y_offset = -1;
    uint16_t _mouse_cancel_disabled;
    uint16_t _dynamic_field_refresh = 1;
    uint16_t _field_box_mode;
    uint16_t _help_box_mode;
    int16_t _mouse_auto_exit;
    uint32_t _draw_override_flag;

    void __cdecl Assign_Update_Function_(MOX2::action_func_t fn) {
        _update_function = fn;
    }

    void __cdecl Assign_Auto_Function_(MOX2::action_func_t fn, int16_t delay) {
        _auto_active_flag = 1;
        _function = fn;

        if (delay > 0 && delay < 10) {
            _auto_function_delay = delay;
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

    void __cdecl Set_Help_List_(s_help_box* help_data, int16_t count) {
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
        platform::Check_Exit_();

        if (_auto_active_flag != 0) {
            _function();
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

    void __cdecl Set_Input_Delay_(int16_t delay) {
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
        platform::Check_Exit_();

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

    void __cdecl Clear_Fields_Above_(int16_t field_idx) {
        _down_mouse_button = -1;
        _fields_count = field_idx + 1;
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

    void __cdecl Disable_Mouse_Pressed_Field_Update_() {
        _dynamic_field_refresh = 0;
    }

    int16_t __cdecl Add_Hot_Key_(const char* hotkey_str) {
        if ((int32_t)_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
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
        platform::Check_Exit_();

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

    void __cdecl Call_Auto_Function_() {
        platform::Check_Exit_();

        if (_auto_active_flag == 0) {
            platform::Toggle_Pages_Stub_();
            return;
        }

        timer::Mark_Time_();
        video::Set_Page_Off_();
        Invoke_Auto_Function_();
        platform::Toggle_Pages_Stub_();
        timer::Release_Time_(_auto_function_delay);

        if (_update_function != nullptr) {
            _update_function();
        }
    }

    int16_t __cdecl Add_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int8_t sound) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        const int16_t field_index = _fields_count;
        s_field* const field = &_fields[field_index];

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

        ++_fields_count;
        return field_index;
    }

    int16_t __cdecl Add_Multi_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* value, int16_t button_value, const char* hotkey, int8_t sound) {
        if ((int32_t)_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        s_field* field = &_fields[_fields_count];

        struct_::Clear_Structure_(field, sizeof(*field));

        field->x = x;
        field->y = y;
        field->x_end = field->x + animate::Get_Width_(pic) - 1;
        field->y_end = field->y + animate::Get_Height_(pic) - 1;
        field->font_style = fonts::Get_Current_Font_Style_();
        field->font_color = fonts::Get_Current_Font_Color_();
        field->sound = sound;
        field->hotkey = *hotkey;
        if (field->hotkey > '`' && field->hotkey < '{') {
            field->hotkey -= 0x20;
        }

        field->field_type = FIELD_TYPE_MULTI_BUTTON;
        s_field_multi_button* multi_button = Field_Data_Multi_Button_(field);
        multi_button->help = help;
        multi_button->pic = pic;
        multi_button->draw_style = animate::Get_Full_Store_Flag_(pic);
        multi_button->selection = value;
        multi_button->button_value = button_value;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int8_t sound) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        s_field* field = &_fields[_fields_count];

        struct_::Clear_Structure_(field, sizeof(*field));

        field->x = x;
        field->y = y;
        field->x_end = field->x + animate::Get_Width_(pic) - 1;
        field->y_end = field->y + animate::Get_Height_(pic) - 1;
        field->field_type = FIELD_TYPE_BUTTON;
        field->font_style = fonts::Get_Current_Font_Style_();
        field->font_color = fonts::Get_Current_Font_Color_();
        field->sound = sound;
        field->hotkey = *hotkey;
        if (field->hotkey > '`' && field->hotkey < '{') {
            field->hotkey -= 0x20;
        }
        s_field_button* button = Field_Data_Button_(field);
        button->help = help;
        button->pic = pic;
        button->draw_style = animate::Get_Full_Store_Flag_(pic);

        int16_t current_id = _fields_count;
        _fields_count++;

        return current_id;
    }

    int16_t __cdecl Add_Radio_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* variable, const char* hotkey, int8_t sound) {
        if ((int32_t)_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        int16_t current_field = _fields_count;
        s_field* field = &_fields[current_field];

        struct_::Clear_Structure_(field, sizeof(*field));

        field->x = x;
        field->y = y;
        field->x_end = field->x + animate::Get_Width_(pic) - 1;
        field->y_end = field->y + animate::Get_Height_(pic) - 1;
        field->font_style = fonts::Get_Current_Font_Style_();
        field->font_color = fonts::Get_Current_Font_Color_();
        field->sound = sound;
        field->hotkey = *hotkey;
        if (field->hotkey > '`' && field->hotkey < '{') {
            field->hotkey -= 0x20;
        }

        _fields[current_field].field_type = FIELD_TYPE_RADIO;
        s_field_radio* radio = Field_Data_Radio_(&_fields[current_field]);
        radio->help = help;
        radio->pic = pic;
        radio->draw_style = animate::Get_Full_Store_Flag_(pic);
        radio->selection = variable;

        _fields_count++;

        return current_field;
    }

    int16_t __cdecl Add_Clipped_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int8_t sound) {
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
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        s_field* field = &_fields[_fields_count];

        struct_::Clear_Structure_(field, sizeof(*field));

        field->x = x;
        field->y = y;
        field->x_end = field->x + animate::Get_Width_(pic) - 1;
        field->y_end = field->y + animate::Get_Height_(pic) - 1;
        field->field_type = FIELD_TYPE_BUTTON;
        field->font_style = fonts::Get_Current_Font_Style_();
        field->font_color = fonts::Get_Current_Font_Color_();
        field->sound = sound;
        field->hotkey = *hotkey;
        if (field->hotkey > '`' && field->hotkey < '{') {
            field->hotkey -= 0x20;
        }
        s_field_button* button = Field_Data_Button_(field);
        button->help = help;
        button->pic = pic;
        button->draw_style = animate::Get_Full_Store_Flag_(pic);
        
        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Irregular_Button_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* help, s_animation_header* pic, int16_t draw_x, int16_t draw_y, const char* hotkey, int8_t sound) {
        char buffer[80];

        if (_max_fields_count < _fields_count) {
            snprintf(buffer, sizeof(buffer), "Maximum number of button fields exceeded (%d)", (int32_t)_max_fields_count);
            exitm::Exit_With_Message(buffer);
        }

        s_field& field = _fields[_fields_count];

        struct_::Clear_Structure_(&field, sizeof(field));

        field.x = x;
        field.y = y;
        field.x_end = x_end;
        field.y_end = y_end;

        field.font_style = fonts::Get_Current_Font_Style_();
        field.font_color = fonts::Get_Current_Font_Color_();

        field.sound = (int16_t)sound;
        field.field_type = FIELD_TYPE_IRREGULAR_BUTTON;
        s_field_irregular_button* irregular_button = Field_Data_Irregular_Button_(&field);
        irregular_button->help = help;
        irregular_button->pic = pic;
        irregular_button->draw_style = animate::Get_Full_Store_Flag_(pic);

        field.hotkey = *hotkey;
        if (field.hotkey >= 'a' && field.hotkey <= 'z') {
            field.hotkey -= 32;
        }

        irregular_button->draw_x = draw_x;
        irregular_button->draw_y = draw_y;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t step_x, int16_t step_y, int16_t cols, int16_t rows, int16_t* grid_x, int16_t* grid_y, int8_t sound) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        s_field& field = _fields[_fields_count];

        field.x = x;
        field.y = y;
        field.x_end = x + (cols * step_x) - 1;
        field.y_end = y + (rows * step_y) - 1;
        field.sound = static_cast<int16_t>(sound);
        field.field_type = FIELD_TYPE_GRID;

        s_field_grid* grid = Field_Data_Grid_(&field);
        grid->step_x = step_x;
        grid->step_y = step_y;
        grid->grid_x_ptr = grid_x;
        grid->grid_y_ptr = grid_y;

        field.hotkey = '\0';

        if (field.hotkey > '`' && field.hotkey < '{') {
            field.hotkey -= 32;
        }

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    int16_t __cdecl Add_Continuous_String_Input_Field_(
        int16_t x, int16_t y, int16_t width, int16_t height, char* value,
        int16_t max_length, int16_t background_color, s_animation_header* pic,
        int8_t sound, int16_t font_shadow_style)
    {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        s_colors* norm_color = fonts::Get_Current_Font_Color_();
        s_colors* high_color = fonts::Get_Current_Font_Color_();

        for (int i = 0; i < 8; ++i) {
            _list_normal_color.colors[i] = norm_color->colors[i];
            _list_highlight_colors.colors[i] = high_color->colors[i];
        }

        int16_t current = _fields_count;

        _fields[current].x = x;
        _fields[current].y = y;
        _fields[current].font_style = fonts::Get_Current_Font_Style_();
        _fields[current].font_color = &_list_normal_color;
        _fields[current].field_type = FIELD_TYPE_CONTINUOUS_INPUT;
        s_field_cont_input* cont_input = Field_Data_Continuous_Input_(&_fields[current]);
        cont_input->highlight_color = &_list_highlight_colors;

        _fields[current].x_end = x + width;
        _fields[current].y_end = y + height;

        if (max_length > (int16_t)sizeof(_continuous_string)) {
            max_length = (int16_t)sizeof(_continuous_string);
        }

        cont_input->max_length = max_length;
        _fields[current].hotkey = 0;
        _fields[current].sound = sound;
        cont_input->buffer = value;
        cont_input->full_store_flag = 0;
        cont_input->pic = pic;
        cont_input->background_color = background_color;
        cont_input->font_shadow_style = font_shadow_style;

        _fields_count++;

        return current;
    }

    int16_t __cdecl Add_Scroll_Field_(int16_t x, int16_t y, int16_t range_min, int16_t range_max,
                                      int16_t return_value, int16_t set_value, int16_t width,
                                      int16_t height, int16_t* value, const char* hotkey_str, int8_t sound) {
        if ((int32_t)_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        _fields[_fields_count].x = x;
        _fields[_fields_count].y = y;
        _fields[_fields_count].x_end = _fields[_fields_count].x + width;
        _fields[_fields_count].y_end = _fields[_fields_count].y + height;
        _fields[_fields_count].sound = sound;
        _fields[_fields_count].field_type = FIELD_TYPE_SCROLL;

        s_field_scroll* scroll = Field_Data_Scroll_(&_fields[_fields_count]);
        scroll->return_value = return_value;
        scroll->set_value = set_value;
        scroll->range_min = range_min;
        scroll->range_max = range_max;

        if (height < width) {
            scroll->is_vertical = 0;
        } else {
            scroll->is_vertical = 1;
        }

        scroll->value = value;
        _fields[_fields_count].hotkey = hotkey_str[0];

        if (_fields[_fields_count].hotkey > '`' && _fields[_fields_count].hotkey < '{') {
            _fields[_fields_count].hotkey -= 0x20;
        }

        int16_t current_count = _fields_count;
        _fields_count++;

        return current_count;
    }

    int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* help_text) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        int16_t current_idx = _fields_count;
        s_field* field = &_fields[current_idx];

        field->x = -1;
        field->y = -1;
        field->x_end = -1;
        field->y_end = -1;
        field->hotkey = '\0';
        field->field_type = FIELD_TYPE_MULTI_HOTKEY;

        s_field_multi_hotkey* multi_hotkey = Field_Data_Multi_Hot_Key_(field);
        multi_hotkey->help = help_text;
        multi_hotkey->help_start = 0;
        multi_hotkey->help_len = (short)strlen(help_text);

        _multi_hotkey_active_field = 1;
        _fields_count++;

        return _fields_count - 1;
    }

    int16_t __cdecl Scan_Input_() {
        platform::Check_Exit_();

        platform::Pump_Game_Input_Queue_();
        mouse::Pointer_X_();
        mouse::Pointer_Y_();
        mouse::Check_Mouse_Shape_();

        _pointer_offset = mouse::Get_Pointer_Offset_();

        int16_t field_id = Scan_Field_();
        s_field* field = &_fields[field_id];

        int16_t type = field->field_type;

        if (type == FIELD_TYPE_SCROLL_RETURN) {
            s_field_scroll* scroll = Field_Data_Scroll_(field);
            *scroll->value = scroll->set_value;
            return scroll->return_value;
        }

        if (type == FIELD_TYPE_STRING_LIST) {
            s_field_string_list* string_list = Field_Data_String_List_(field);
            if (string_list->is_selectable == 0) {
                return 0;
            }
        }

        if (type == FIELD_TYPE_GRID) {
            if (_down_mouse_button == -1 || _fields[_down_mouse_button].field_type != FIELD_TYPE_GRID) {
                s_field_grid* grid = Field_Data_Grid_(field);
                *grid->grid_x_ptr = (int16_t)((mouse::Pointer_X_() - field->x) / grid->step_x);
                *grid->grid_y_ptr = (int16_t)((mouse::Pointer_Y_() - field->y) / grid->step_y);
            }
        }

        return field_id;
    }

    int16_t __cdecl Scan_Field_() {
        platform::Check_Exit_();

        platform::Pump_Game_Input_Queue_();
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

    int16_t __cdecl Add_Locked_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* selection, const char* hotkey, int8_t sound) {
        if ((int32_t)_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
        }

        s_field& current_field = _fields[_fields_count];

        struct_::Clear_Structure_(&current_field, sizeof(current_field));

        current_field.x = x;
        current_field.y = y;
        current_field.x_end = current_field.x + animate::Get_Width_(pic) - 1;
        current_field.y_end = current_field.y + animate::Get_Height_(pic) - 1;
        current_field.field_type = FIELD_TYPE_LOCKED;
        current_field.font_style = fonts::Get_Current_Font_Style_();
        current_field.font_color = fonts::Get_Current_Font_Color_();
        current_field.sound = sound;
        current_field.hotkey = *hotkey;
        if (current_field.hotkey > '`' && current_field.hotkey < '{') {
            current_field.hotkey -= 0x20;
        }
        s_field_locked* locked = Field_Data_Locked_Button_(&current_field);
        locked->help = help;
        locked->pic = pic;
        locked->draw_style = animate::Get_Full_Store_Flag_(pic);
        locked->selection = selection;

        int16_t current_index = _fields_count;
        _fields_count++;

        return current_index;
    }

    void __cdecl Release_Fields_() {
        _fields_count = _old_fields_count;
        _input_delay = _old_input_delay;
        _down_mouse_button = _old_down_mouse_button;

        _max_fields_count = MAX_FIELDS_COUNT;

        _auto_input_variable = _old_auto_input_variable;
        _input_field_active = _old_input_field_active;
        _fields = _old_fields;
        _active_input_field_number = _old_active_input_field_number;
    }

    int16_t __cdecl Add_String_List_Field_(
        int16_t x,
        int16_t y,
        int16_t width,
        const char* help,
        int16_t is_selectable,
        int8_t* value,
        int16_t help_start,
        int16_t gradient_mode,
        uint8_t* gradient_palette,
        int32_t gradient_extra,
        s_animation_header* gradient_pic,
        int32_t unused_param_12,
        const char* hotkey_str,
        const char* unused_param_14
    ) {
        int16_t index = _fields_count;

        _fields[index].x = x;
        _fields[index].y = y - 1;
        _fields[index].x_end = x + width;

        _fields[index].font_style = fonts::Get_Current_Font_Style_();
        _fields[index].font_color = &_list_normal_color;

        _fields[index].field_type = FIELD_TYPE_STRING_LIST;
        s_field_string_list* string_list = Field_Data_String_List_(&_fields[index]);
        string_list->highlight_color = &_list_highlight_colors;
        string_list->gradient_colors = &_list_special_colors;

        _fields[index].y_end = y + fonts::Get_Font_Height_() + 1;
        string_list->help_start = help_start;
        _fields[index].hotkey = *hotkey_str;

        string_list->is_selectable = is_selectable;

        if (_fields[index].hotkey >= 'a' && _fields[index].hotkey <= 'z') {
            _fields[index].hotkey -= 0x20;
        }

        string_list->help = help;
        string_list->value = value;
        string_list->gradient_mode = gradient_mode;
        string_list->gradient_palette = gradient_palette;
        string_list->gradient_extra = gradient_extra;
        string_list->gradient_pic = gradient_pic;
        (void)unused_param_12;
        (void)unused_param_14;

        _fields_count++;
        return _fields_count - 1;
    }

    void __cdecl Enable_Mouse_Wait_() {
        _mouse_auto_exit = 0;
    }

    void __cdecl Disable_Mouse_Wait_() {
        _mouse_auto_exit = 1;
    }

    void __cdecl Timed_Wait_For_Input_(int32_t timeout) {
        bool has_input = false;

        if (timeout == 0) {
            do {
                platform::Check_Exit_();
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
                platform::Check_Exit_();
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

    int16_t __cdecl Add_Clipped_Circle_Field_(int16_t center_x, int16_t center_y, int16_t radius, const char* hotkey, int8_t sound) {
        int32_t max_x = center_x + radius;
        int32_t min_x = center_x - radius;
        int32_t min_y = center_y - radius;
        int32_t max_y = center_y + radius;

        if (_max_fields_count <= _fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
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

        _fields[_fields_count].field_type = FIELD_TYPE_CIRCLE;
        s_field_circle* circle = Field_Data_Circle_(&_fields[_fields_count]);
        circle->center_x = center_x;
        circle->center_y = center_y;
        circle->radius_squared = radius * radius;
        _fields[_fields_count].sound = sound;
        _fields[_fields_count].hotkey = *hotkey;

        if (_fields[_fields_count].hotkey >= 'a' && _fields[_fields_count].hotkey <= 'z') {
            _fields[_fields_count].hotkey -= 0x20;
        }

        int16_t result = _fields_count;
        _fields_count++;

        return result;
    }

    int16_t __cdecl Interpret_Mouse_Input_() {
        platform::Check_Exit_();

        int16_t field_idx = 0;
        _down_mouse_button = -1;
        _auto_input_variable = 0;

        int16_t ptr_x = mouse::Pointer_X_();
        int16_t ptr_y = mouse::Pointer_Y_();
        _pointer_offset = mouse::Get_Pointer_Offset_();

        if (key::Keyboard_Status_() != 0) {
            int16_t key_code = Interpret_Keyboard_Input_(&field_idx);
            if (key_code == 0 || field_idx < 0 || key_code == -1) {
                if (_multi_hotkey_active_field != 0) Clear_Multi_Hot_Key_Offsets_();
                return field_idx;
            }

            if (key_code == (int16_t)0x8500) {
                if (capture::Check_Release_Version_() != 1) {
                    F11_Key_();
                }
            }

            if (key_code == (int16_t)0x8600) {
                mouse::Save_Mouse_State_();
                capture::Screen_Flic_Capture_();
                int16_t cur_y = mouse::Pointer_Y_();
                int16_t cur_x = mouse::Pointer_X_();
                platform::Force_Cursor_Pos(cur_x, cur_y);
                mouse::Restore_Mouse_State_();
            }

            if (key_code == 0x011B) {
                if (_mouse_cancel_disabled == 0) {
                    if (_multi_hotkey_active_field != 0) Clear_Multi_Hot_Key_Offsets_();
                    return -1;
                }
            }

            if (key_code == 0x00C0) {
                if (capture::Check_Release_Version_() == 0) {
                    if (_field_box_mode == 0) {
                        _field_box_mode = 1;
                    } else {
                        _field_box_mode = 0;
                    }
                }
            }

            if (key_code == 0x297E) {
                if (capture::Check_Release_Version_() == 0) {
                    if (_help_box_mode == 0) {
                        _help_box_mode = 1;
                    } else {
                        _help_box_mode = 0;
                    }
                }
            }

            if (field_idx != 0 && (int16_t)_fields[field_idx].hotkey == (int16_t)key_code) {
                if (_fields[field_idx].field_type == FIELD_TYPE_SCROLL) {
                    return 0;
                }
                
                Push_Field_Down_(field_idx, ptr_x, ptr_y);
                int16_t field_type = _fields[field_idx].field_type;
                
                if (field_type == FIELD_TYPE_RADIO) {
                    s_field_radio* radio = Field_Data_Radio_(&_fields[field_idx]);
                    if (*radio->selection == 0) {
                        *radio->selection = 1;
                    } else {
                        *radio->selection = 0;
                    }
                } else if (field_type == FIELD_TYPE_LOCKED) {
                    s_field_locked* locked = Field_Data_Locked_Button_(&_fields[field_idx]);
                    if (*locked->selection == 0) {
                        *locked->selection = 1;
                    }
                } else if (field_type == FIELD_TYPE_SCROLL_RETURN) {
                    _down_mouse_button = -1;
                    return Field_Data_Scroll_(&_fields[field_idx])->return_value;
                }
                
                if (field_type != FIELD_TYPE_HIDDEN && field_type != FIELD_TYPE_MULTI_HOTKEY) {
                    Quick_Call_Auto_Function_();
                }
                _down_mouse_button = -1;
                return field_idx;
            }

            if (key_code == 0x1C0D || key_code == (int16_t)0xE00D) {
                if (_input_field_active != 0) {
                    int16_t ret = _active_input_field_number;
                    Copy_Continuous_String_();
                    if (_mouse_auto_exit == 0) {
                        Quick_Call_Auto_Function_();
                    }
                    _input_field_active = 0;
                    _down_mouse_button = -1;
                    _active_input_field_number = -1;
                    return ret;
                }
                
                int16_t scanned_field = Scan_Field_();
                if (scanned_field == 0) {
                    return 0;
                }
                
                if (scanned_field > 0) {
                    if (_fields[scanned_field].field_type == FIELD_TYPE_CONTINUOUS_INPUT) {
                        s_field_cont_input* cont_input = Field_Data_Continuous_Input_(&_fields[scanned_field]);
                        if (_input_field_active == 0) {
                            if (_active_input_field_number == scanned_field) {
                                int16_t len = 0;
                                if (_continuous_string[0] != '\0') {
                                    while (len < cont_input->max_length && _continuous_string[len] != '\0') {
                                        len++;
                                    }
                                }
                                if (len != 0 && _continuous_string[len - 1] == '_') {
                                    len--;
                                }
                                _continuous_string[len] = '\0';
                                strcpy(cont_input->buffer, _continuous_string);
                                _active_input_field_number = -1;
                                _cursor_position = 0;
                            } else {
                                Copy_Continuous_String_();
                                _active_input_field_number = scanned_field;
                                _cursor_position = 0;
                                _cursor_on = 0;
                            }
                        } else {
                            Copy_Continuous_String_();
                            _input_field_active = 1;
                            _cursor_position = 0;
                            _cursor_on = 0;
                            _active_input_field_number = scanned_field;
                        }
                    } else if (_input_field_active != 0) {
                        int16_t len = 0;
                        s_field_cont_input* active_cont_input = Field_Data_Continuous_Input_(&_fields[_active_input_field_number]);
                        if (_continuous_string[0] != '\0') {
                            while (len < active_cont_input->max_length && _continuous_string[len] != '\0') {
                                len++;
                            }
                        }
                        if (len != 0 && _continuous_string[len - 1] == '_') {
                            len--;
                        }
                        _continuous_string[len] = '\0';
                        strcpy(active_cont_input->buffer, _continuous_string);
                        _cursor_position = 0;
                        _input_field_active = 0;
                        _active_input_field_number = -1;
                    }
                }
                
                if (scanned_field != 0) {
                    if (_fields[scanned_field].field_type != FIELD_TYPE_SCROLL && _fields[scanned_field].field_type != FIELD_TYPE_CONTINUOUS_INPUT) {
                        Push_Field_Down_(scanned_field, ptr_x, ptr_y);
                    }
                    
                    int16_t field_type = _fields[scanned_field].field_type;
                    if (field_type == FIELD_TYPE_RADIO) {
                        s_field_radio* radio = Field_Data_Radio_(&_fields[scanned_field]);
                        if (*radio->selection == 0) {
                            *radio->selection = 1;
                        } else {
                            *radio->selection = 0;
                        }
                    } else if (field_type == FIELD_TYPE_LOCKED) {
                        s_field_locked* locked = Field_Data_Locked_Button_(&_fields[scanned_field]);
                        if (*locked->selection == 0) {
                            *locked->selection = 1;
                        }
                    } else if (field_type == FIELD_TYPE_SCROLL_RETURN) {
                        _down_mouse_button = -1;
                        return Field_Data_Scroll_(&_fields[scanned_field])->return_value;
                    }
                    
                    if (_mouse_auto_exit == 0) {
                        Quick_Call_Auto_Function_();
                    }
                    _down_mouse_button = -1;
                    return scanned_field;
                }
                
                if (_list_field_on != 0) {
                    for (int16_t i = 1; i < _fields_count; ++i) {
                        if (_fields[i].field_type == FIELD_TYPE_STRING_LIST) {
                            s_field_string_list* string_list = Field_Data_String_List_(&_fields[i]);
                            if (*string_list->value == string_list->help_start && string_list->is_selectable != 0) {
                                _down_mouse_button = -1;
                                return i;
                            }
                        }
                    }
                }
            } else if (key_code == 0x0D2B || key_code == 0x0C2D) {
                int16_t scanned_field = Scan_Field_();
                if (scanned_field != 0 && _fields[scanned_field].field_type == FIELD_TYPE_SCROLL) {
                    if (key_code == 0x0D2B) {
                        Increment_Scroll_Bar_(scanned_field);
                    }
                    if (key_code == 0x0C2D) {
                        Decrement_Scroll_Bar_(scanned_field);
                    }
                    _down_mouse_button = -1;
                    return scanned_field;
                }
            }
            
            if (_input_field_active > 0) {
                s_field_cont_input* active_cont_input = Field_Data_Continuous_Input_(&_fields[_active_input_field_number]);
                if (key_code == 0x011B) {
                    strlcpy(_continuous_string, active_cont_input->buffer, sizeof(_continuous_string));
                    int16_t act_num = _active_input_field_number;
                    _active_input_field_number = -1;
                    _input_field_active = 0;
                    _cursor_position = 0;
                    _cursor_on = 0;
                    return -act_num;
                }
                if (key_code == 0x0E7F) {
                    _continuous_string[0] = '\0';
                    _active_input_field_first = 0;
                } else if (key_code == 0x0E08) {
                    if (_active_input_field_first == 0) {
                        int16_t len = 0;
                        if (_continuous_string[0] != '\0') {
                            while (len < active_cont_input->max_length - 1 && _continuous_string[len] != '\0' && _continuous_string[len] != '_') {
                                len++;
                            }
                        }
                        if (len > 0) {
                            _cursor_position = 0;
                            _continuous_string[len - 1] = '\0';
                        }
                    } else {
                        _continuous_string[0] = '\0';
                        _active_input_field_first = 0;
                    }
                } else {
                    int16_t chr = (int16_t)((int32_t)key_code % 0x100);
                    if (chr >= 0x20 && chr <= 0x7E && chr != 0x5F) {
                        _active_input_field_first = 0;
                        if (_active_input_field_number >= 0 && _active_input_field_number < _fields_count) {
                            int16_t font_shadow = active_cont_input->font_shadow_style;
                            if (font_shadow == -1) {
                                fonts::Set_Font_Style_Shadow_Down_(_fields[_active_input_field_number].font_style, _fields[_active_input_field_number].font_color);
                            } else if (font_shadow == 0) {
                                fonts::Set_Font_Style_(_fields[_active_input_field_number].font_style, _fields[_active_input_field_number].font_color);
                            } else if (font_shadow == 1) {
                                fonts::Set_Font_Style_Shadow_Up_(_fields[_active_input_field_number].font_style, _fields[_active_input_field_number].font_color);
                            } else {
                                fonts::Set_Font_Style_(_fields[_active_input_field_number].font_style, _fields[_active_input_field_number].font_color);
                            }
                        }
                        int16_t len = 0;
                        while (_continuous_string[len] != '_' && _continuous_string[len] != '\0' && len < active_cont_input->max_length - 1) {
                            len++;
                        }
                        if (len < active_cont_input->max_length - 1 && fonts::Get_String_Width_(_continuous_string) + _fields[_active_input_field_number].x <= _fields[_active_input_field_number].x_end) {
                            _continuous_string[len] = (char)chr;
                            _continuous_string[len + 1] = '\0';
                        } else {
                            _continuous_string[len] = '\0';
                        }
                        _cursor_on = 0;
                        _cursor_position = 0;
                    }
                }
            }
            
            _down_mouse_button = -1;
            return 0;
        }

        int16_t mouse_btn = mouse::Mouse_Button_();
        if (mouse_btn == 0) {
            if (mouse::Mouse_Buffer_() == 0) {
                _pressed_button_flag = 0;
                return 0;
            }
            
            int16_t buf_btn = mouse::Mouse_Buffer_Button_();
            if (buf_btn == 2) {
                if (_help_list_active != 0 && Check_Help_List_() == 0) {
                    mouse::Mouse_Buffer_();
                    mouse::Mouse_Buffer2_();
                    return 0;
                }
                if (_mouse_cancel_disabled == 0) {
                    mouse::Mouse_Buffer_();
                    mouse::Mouse_Buffer2_();
                    if (_multi_hotkey_active_field != 0) Clear_Multi_Hot_Key_Offsets_();
                    return -1;
                }
            }
            
            int16_t buf_x = mouse::Mouse_Buffer_X_();
            int16_t buf_y = mouse::Mouse_Buffer_Y_();
            if (_pressed_button_flag == 0) {
                _pressed_button_flag = 1;
                _pressed_mouse_x_value = buf_x;
                _pressed_mouse_y_value = buf_y;
            }
            
            field_idx = 0;
            _pointer_offset = mouse::Get_Pointer_Offset_();
            
            for (int16_t i = 1; i < _fields_count; ++i) {
                int32_t x = (int32_t)buf_x + _pointer_offset;
                int32_t y = (int32_t)buf_y + _pointer_offset;
                
                if (_fields[i].x <= x && x <= _fields[i].x_end && _fields[i].y <= y && y <= _fields[i].y_end) {
                    if (_fields[i].field_type != FIELD_TYPE_CIRCLE) {
                        field_idx = i;
                        break;
                    }
                    
                    s_field_circle* circle = Field_Data_Circle_(&_fields[i]);
                    int dx = buf_x - circle->center_x;
                    int dy = buf_y - circle->center_y;
                    if (dx * dx + dy * dy <= circle->radius_squared) {
                        field_idx = i;
                        break;
                    }
                }
            }
            
            if (field_idx != 0) {
                if (_fields[field_idx].field_type != FIELD_TYPE_CONTINUOUS_INPUT) {
                    _auto_input_variable = field_idx;
                    Push_Field_Down_(field_idx, buf_x, buf_y);
                    Quick_Call_Auto_Function_();
                }
                
                switch (_fields[field_idx].field_type) {
                    case FIELD_TYPE_RADIO:
                        if (*Field_Data_Radio_(&_fields[field_idx])->selection == 0) {
                            *Field_Data_Radio_(&_fields[field_idx])->selection = 1;
                        } else {
                            *Field_Data_Radio_(&_fields[field_idx])->selection = 0;
                        }
                        break;
                    case FIELD_TYPE_LOCKED:
                        if (*Field_Data_Locked_Button_(&_fields[field_idx])->selection == 0) {
                            *Field_Data_Locked_Button_(&_fields[field_idx])->selection = 1;
                        }
                        break;
                    case FIELD_TYPE_LEGACY_INPUT:
                        Push_Field_Down_(field_idx, buf_x, buf_y);
                        break;
                    case FIELD_TYPE_SCROLL_RETURN:
                        _down_mouse_button = -1;
                        return Field_Data_Scroll_(&_fields[field_idx])->return_value;
                    case FIELD_TYPE_CONTINUOUS_INPUT:
                        {
                            s_field_cont_input* cont_input = Field_Data_Continuous_Input_(&_fields[field_idx]);
                        if (_input_field_active == 0) {
                            strlcpy(_continuous_string, cont_input->buffer, sizeof(_continuous_string));
                            _input_field_active = 1;
                            _cursor_on = 0;
                            _cursor_position = 0;
                            _active_input_field_number = field_idx;
                        } else if (field_idx != _active_input_field_number) {
                            strlcpy(_continuous_string, cont_input->buffer, sizeof(_continuous_string));
                            _cursor_on = 0;
                            _cursor_position = 0;
                            _active_input_field_number = field_idx;
                        }
                        break;
                        }
                    default:
                        if (_input_field_active != 0) {
                            int16_t len = 0;
                            s_field_cont_input* active_cont_input = Field_Data_Continuous_Input_(&_fields[_active_input_field_number]);
                            if (_continuous_string[0] != '\0') {
                                while (len < active_cont_input->max_length && _continuous_string[len] != '\0') {
                                    len++;
                                }
                            }
                            if (len != 0 && _continuous_string[len - 1] == '_') {
                                len--;
                            }
                            _continuous_string[len] = '\0';
                            strcpy(active_cont_input->buffer, _continuous_string);
                            _input_field_active = 0;
                            _active_input_field_number = -1;
                        }
                        break;
                }
            }
            
            _down_mouse_button = -1;
            if (field_idx != 0) {
                mouse::Mouse_Buffer2_();
                _last_button_number = buf_btn;
                _last_button_x = buf_x;
                _last_button_y = buf_y;
            }
            
            _auto_input_variable = 0;
            if (buf_btn != 2) {
                return field_idx;
            } else {
                return -field_idx;
            }
        } else {
            if (mouse_btn == 2) {
                if (_help_list_active != 0 && Check_Help_List_() == 0) {
                    mouse::Mouse_Buffer_();
                    mouse::Mouse_Buffer2_();
                    return 0;
                }
                if (_mouse_cancel_disabled == 0) {
                    while (mouse::Mouse_Button_() == 2) {
                        Quick_Call_Auto_Function_();
                    }
                    mouse::Mouse_Buffer_();
                    mouse::Mouse_Buffer2_();
                    return -1;
                }
            }
            
            ptr_x = mouse::Pointer_X_();
            ptr_y = mouse::Pointer_Y_();
            if (_pressed_button_flag == 0) {
                _pressed_button_flag = 1;
                _pressed_mouse_x_value = ptr_x;
                _pressed_mouse_y_value = ptr_y;
            }
            
            while (mouse::Mouse_Button_() != 0) {
                ptr_x = mouse::Pointer_X_();
                ptr_y = mouse::Pointer_Y_();
                _pointer_offset = mouse::Get_Pointer_Offset_();
                
                field_idx = 0;
                if (_dynamic_field_refresh != 0) {
                    field_idx = Scan_Field_();
                    _down_mouse_button = -1;
                }
                
                if (field_idx == 0) {
                    if (_down_mouse_button != -1) {
                        if (_fields[_down_mouse_button].field_type == FIELD_TYPE_SCROLL) {
                            Invoke_Auto_Function_();
                        }
                        int16_t down_type = _fields[_down_mouse_button].field_type;
                        if (down_type != FIELD_TYPE_MULTI_BUTTON && down_type != FIELD_TYPE_STRING_LIST && down_type != FIELD_TYPE_CONTINUOUS_INPUT) {
                            mouse::Save_Mouse_State_();
                            Draw_Field_(_down_mouse_button, 0);
                            platform::Force_Cursor_Pos(ptr_x, ptr_y);
                            mouse::Restore_Mouse_State_();
                        }
                        _down_mouse_button = -1;
                    }
                    break;
                }
                
                if (_down_mouse_button != field_idx && _fields[field_idx].field_type != FIELD_TYPE_LEGACY_INPUT && _fields[field_idx].field_type != FIELD_TYPE_CONTINUOUS_INPUT) {
                    bool skip_push = false;
                    if (_down_mouse_button != -1) {
                        if (_fields[field_idx].field_type == FIELD_TYPE_GRID && _fields[_down_mouse_button].field_type == FIELD_TYPE_GRID) {
                            skip_push = true;
                        } else if (_fields[_down_mouse_button].field_type == FIELD_TYPE_SCROLL) {
                            Invoke_Auto_Function_();
                        }
                    }
                    if (!skip_push) {
                        Push_Field_Down_(field_idx, ptr_x, ptr_y);
                    }
                }
                
                if (_fields[field_idx].field_type == FIELD_TYPE_CONTINUOUS_INPUT) {
                    s_field_cont_input* cont_input = Field_Data_Continuous_Input_(&_fields[field_idx]);
                    if (_input_field_active == 0 || field_idx != _active_input_field_number) {
                        strlcpy(_continuous_string, cont_input->buffer, sizeof(_continuous_string));
                        _input_field_active = 1;
                        _active_input_field_first = 1;
                        _cursor_position = 0;
                        _cursor_on = 0;
                        _active_input_field_number = field_idx;
                    }
                } else if (_input_field_active != 0) {
                    int16_t len = 0;
                    s_field_cont_input* active_cont_input = Field_Data_Continuous_Input_(&_fields[_active_input_field_number]);
                    if (_continuous_string[0] != '\0') {
                        while (len < active_cont_input->max_length && _continuous_string[len] != '\0') {
                            len++;
                        }
                    }
                    if (len != 0 && _continuous_string[len - 1] == '_') {
                        len--;
                    }
                    _continuous_string[len] = '\0';
                    strcpy(active_cont_input->buffer, _continuous_string);
                    _active_input_field_number = -1;
                    _input_field_active = 0;
                }
                
                _auto_input_variable = field_idx;
                if (_mouse_auto_exit != 0) break;
                
                if (mouse::Mouse_Button_() != 0) {
                    Call_Auto_Function_();
                }
                mouse::Mouse_Buffer_();
                mouse::Mouse_Buffer2_();
            }
            
            if (_fields[_auto_input_variable].field_type == FIELD_TYPE_SCROLL) {
                Invoke_Auto_Function_();
            }
            _auto_input_variable = 0;
            
            if (field_idx != 0) {
                mouse::Mouse_Buffer_();
                mouse::Mouse_Buffer2_();
                _last_button_number = mouse_btn;
                _last_button_y = ptr_y;
                _last_button_x = ptr_x;
                
                switch (_fields[field_idx].field_type) {
                    case FIELD_TYPE_RADIO:
                        if (*Field_Data_Radio_(&_fields[field_idx])->selection == 0) {
                            *Field_Data_Radio_(&_fields[field_idx])->selection = 1;
                        } else {
                            *Field_Data_Radio_(&_fields[field_idx])->selection = 0;
                        }
                        break;
                    case FIELD_TYPE_LOCKED:
                        if (*Field_Data_Locked_Button_(&_fields[field_idx])->selection == 0) {
                            *Field_Data_Locked_Button_(&_fields[field_idx])->selection = 1;
                        }
                        break;
                    case FIELD_TYPE_LEGACY_INPUT:
                        Push_Field_Down_(field_idx, ptr_x, ptr_y);
                        break;
                    case FIELD_TYPE_SCROLL_RETURN:
                        _down_mouse_button = -1;
                        if (mouse_btn != 2) {
                            return Field_Data_Scroll_(&_fields[field_idx])->return_value;
                        }
                        if (_mouse_cancel_disabled != 0) {
                            return -Field_Data_Scroll_(&_fields[field_idx])->return_value;
                        }
                        return -1;
                }
            }
            
            _down_mouse_button = -1;
            if (mouse_btn == 1) {
                if (_multi_hotkey_active_field != 0) Clear_Multi_Hot_Key_Offsets_();
                return field_idx;
            }
            if (mouse_btn == 2) {
                if (_multi_hotkey_active_field != 0) Clear_Multi_Hot_Key_Offsets_();
                return -field_idx;
            }
            
            return 0;
        }
    }

    void __cdecl Disable_Draw_Field_(int16_t field_idx) {
        switch (_fields[field_idx].field_type) {
            case FIELD_TYPE_BUTTON:
                _fields[field_idx].field_type = -1001;
                break;
            case FIELD_TYPE_RADIO:
                _fields[field_idx].field_type = -1;
                break;
            case FIELD_TYPE_LOCKED:
                _fields[field_idx].field_type = -2;
                break;
            case FIELD_TYPE_MULTI_BUTTON:
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
                _fields[field_idx].field_type = FIELD_TYPE_LOCKED;
            } else if (field_type == -1) {
                _fields[field_idx].field_type = FIELD_TYPE_RADIO;
            }
        } else if (field_type == -3) {
            _fields[field_idx].field_type = FIELD_TYPE_MULTI_BUTTON;
        } else if (field_type == -1001) {
            _fields[field_idx].field_type = FIELD_TYPE_BUTTON;
        }
    }

    int16_t __cdecl Get_List_Field_(
        int16_t x, int16_t y, int16_t width, const char* help, const char* list_ptr, int32_t item_size,
        int8_t* selected_index, uint8_t* item_flags, int16_t gradient_mode, uint8_t* gradient_palette,
        int32_t gradient_extra, s_animation_header* gradient_pic, int32_t unused_param_13, const char* unused_param_14)
    {
        _list_field_on = 1;
        Set_Input_Delay_(1);
        Clear_Fields_();
        _exit_the_list_field = 0;

        int16_t font_height = fonts::Get_Font_Height_();
        int16_t font_vert_spacing = fonts::Get_Font_Vertical_Spacing_();

        int first_valid_index = -1;
        bool first_valid_found = false;
        int16_t index = 0;
        bool eof = false;

        int16_t cur_y = y;
        int16_t y_step = font_height + font_vert_spacing;

        do {
            if (*list_ptr == '\0') {
                eof = true;
            } else {
                if (!first_valid_found && (item_flags == nullptr || item_flags[index] != 0)) {
                    first_valid_found = true;
                    first_valid_index = index;
                }

                cur_y += y_step;

                int16_t flag = 1;
                if (item_flags != nullptr) {
                    flag = item_flags[index];
                }

                Add_String_List_Field_(
                    x, cur_y, width, list_ptr, flag, 
                    selected_index, index, gradient_mode,
                    gradient_palette, gradient_extra, gradient_pic, unused_param_13,
                    s__00551100, unused_param_14
                );

                index++;
            }
            list_ptr += item_size;
        } while (!eof);

        int16_t cur_font_style = fonts::Get_Current_Font_Style_();
        s_colors *cur_font_color = fonts::Get_Current_Font_Color_();
        s_colors *cur_highlight = fonts::Get_Current_Highlight_Color_();
        s_colors *cur_special = fonts::Get_Current_Special_Color_();

        fonts::Set_Font_Style_(cur_font_style, cur_font_color);

        for (int i = 0; i < 8; i++) {
            _list_normal_color.colors[i] = cur_font_color->colors[i];
            _list_special_colors.colors[i] = cur_special->colors[i];
            _list_highlight_colors.colors[i] = cur_highlight->colors[i];
        }

        int8_t local_4 = 0;
        int16_t help_field_idx = Add_String_List_Field_(
            x, y, width, help, 1, &local_4, 1, 0, nullptr, 0, nullptr, 0, s__00551100, unused_param_14
        );

        int8_t cur_selection = *selected_index;
        if (cur_selection < 0 || cur_selection >= index || cur_selection < first_valid_index) {
            if (first_valid_index < 0 || first_valid_index >= index) {
                *selected_index = -1;
            } else {
                *selected_index = static_cast<int8_t>(Field_Data_String_List_(&_fields[first_valid_index + 1])->help_start);
            }
        }

        int16_t input;
        bool exit_flag;
        do {
            platform::Check_Exit_();
            timer::Mark_Time_();
            input = Get_Input_();

            exit_flag = false;
            if (input != 0) {
                exit_flag = true;
                if (input > 0 && item_flags != nullptr && item_flags[input - 1] == 0) {
                    exit_flag = false;
                }
                if (input == help_field_idx) {
                    exit_flag = false;
                }
            }

            if (exit_flag) {
                break;
            }

            if (_exit_the_list_field == 1) {
                exit_flag = true;
            }

            video::Set_Page_Off_();
            Invoke_Auto_Function_();

            if (_list_field_scan_flag != 0) {
                int16_t scan_idx = Scan_Input_();
                if (scan_idx > 0) {
                    *selected_index = static_cast<int8_t>(Field_Data_String_List_(&_fields[scan_idx])->help_start);
                }
            }

            palette::Refresh_Palette_();
            platform::Toggle_Pages_Stub_();
            timer::Release_Time_(_auto_function_delay);
        } while (!exit_flag);

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

    void __cdecl F11_Key_() {
        s_memorystatus memory_status;
        char buffer[80];

        Clear_Fields_();

        Query_Memory_Status_(&memory_status);

        snprintf(buffer, sizeof(buffer), "Avail Mem = %dk   Total Mem = %dk", (uint32_t)(memory_status.dwAvailPhys >> 10), (uint32_t)(memory_status.dwTotalPhys >> 10));

        video::Clear_Display_And_Release_Palette_();

        Show_Message_Box_(platform::g_main_window, buffer, "Microprose", true);

        platform::Prepare_Exit_();
        platform::Check_Exit_();
    }

    void __cdecl Find_Bar_Position_(int16_t field_idx) {
        s_field* field = &_fields[field_idx];
        s_field_scroll* scroll = Field_Data_Scroll_(field);
        int16_t new_value;

        if (scroll->is_vertical == 0) {
            int16_t width = field->x_end - field->x;
            int16_t mouse_x = mouse::Pointer_X_() + _pointer_offset;

            if (mouse_x >= field->x_end) {
                new_value = scroll->range_max;
            } else if (mouse_x <= field->x) {
                new_value = scroll->range_min;
            } else {
                int delta = mouse_x - field->x;
                int range = scroll->range_max - scroll->range_min;
                new_value = (int16_t)((delta * range) / width) + scroll->range_min;
            }
        } else {
            int16_t height = field->y_end - field->y;
            int16_t mouse_y = mouse::Pointer_Y_() + _pointer_offset;

            if (mouse_y >= field->y_end) {
                new_value = scroll->range_min;
            } else if (mouse_y <= field->y) {
                new_value = scroll->range_max;
            } else {
                int delta = field->y_end - mouse_y;
                int range = scroll->range_max - scroll->range_min;
                new_value = (int16_t)((delta * range) / height);
            }
        }

        if (new_value < scroll->return_value) {
            new_value = scroll->return_value;
        }

        if (new_value > scroll->set_value) {
            new_value = scroll->set_value;
        }

        *scroll->value = new_value;
    }

    int16_t __cdecl Add_Clipped_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int8_t sound) {
        if (_fields_count >= _max_fields_count) {
            exitm::Exit_With_Message("You exceeded the max number of fields (150)");
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
        _fields[_fields_count].sound = sound;
        _fields[_fields_count].hotkey = *hotkey;

        char c = _fields[_fields_count].hotkey;
        if (c >= 'a' && c <= 'z') {
            _fields[_fields_count].hotkey = c - 0x20;
        }

        int16_t result = _fields_count;
        _fields_count++;

        return result;
    }

    void __cdecl Draw_Visible_Fields_Back_() {
        int16_t x1 = video::_screen_window_x1;
        int16_t x2 = video::_screen_window_x2;
        int16_t y1 = video::_screen_window_y1;
        int16_t y2 = video::_screen_window_y2;

        graphics::Reset_Window_();
        video::Set_Page_Back_();

        if (_draw_fields_flag != 0 && _fields_count > 1) {
            for (int16_t i = 1; i < _fields_count; ++i) {
                s_field* f = &_fields[i];

                if (i == _down_mouse_button && f->field_type != FIELD_TYPE_LEGACY_INPUT && f->field_type != FIELD_TYPE_GRID && f->field_type != FIELD_TYPE_STRING_LIST) {
                    Draw_Field_(i, 1);
                    continue;
                }

                switch (f->field_type) {
                    case FIELD_TYPE_BUTTON:
                        {
                            s_field_button* button = Field_Data_Button_(f);
                            animate::Reset_Animation_Frame_(button->pic);
                            animate::Draw_(f->x, f->y, button->pic);
                            if (button->help != nullptr && *button->help != '\0') {
                                fonts::Set_Font_Style_(f->font_style, f->font_color);
                                int16_t h = fonts::Get_Font_Height_() - 1;
                                fonts::Print_Centered_(
                                    f->x + (f->x_end - f->x) / 2,
                                    f->y + (f->y_end - f->y) / 2 - h / 2,
                                    button->help
                                );
                            }
                        }
                        break;
                    case FIELD_TYPE_RADIO:
                        {
                            s_field_radio* radio = Field_Data_Radio_(f);
                            if (*radio->selection == 0) {
                                animate::Reset_Animation_Frame_(radio->pic);
                                animate::Draw_(f->x, f->y, radio->pic);
                                if (radio->help != nullptr && *radio->help != '\0') {
                                    fonts::Set_Font_Style_(f->font_style, f->font_color);
                                    int16_t h = fonts::Get_Font_Height_() - 1;
                                    fonts::Print_Centered_(
                                        f->x + (f->x_end - f->x) / 2,
                                        f->y + (f->y_end - f->y) / 2 - h / 2,
                                        radio->help
                                    );
                                }
                            } else {
                                if (radio->draw_style != 0) {
                                    animate::Set_Animation_Frame_(radio->pic, 1);
                                } else {
                                    animate::Reset_Animation_Frame_(radio->pic);
                                    animate::Draw_(f->x, f->y, radio->pic);
                                }
                                animate::Draw_(f->x, f->y, radio->pic);
                                if (radio->help != nullptr && *radio->help != '\0') {
                                    fonts::Set_Font_Style_(f->font_style, f->font_color);
                                    int16_t h = fonts::Get_Font_Height_() - 1;
                                    fonts::Print_Centered_(
                                        f->x + (f->x_end - f->x) / 2 + _selected_button_text_x_offset,
                                        f->y + (f->y_end - f->y) / 2 - h / 2 + _selected_button_text_y_offset,
                                        radio->help
                                    );
                                }
                            }
                        }
                        break;
                    case FIELD_TYPE_LOCKED:
                        {
                            s_field_locked* locked = Field_Data_Locked_Button_(f);
                            if (*locked->selection == 0) {
                                animate::Reset_Animation_Frame_(locked->pic);
                                animate::Draw_(f->x, f->y, locked->pic);
                                if (locked->help != nullptr && *locked->help != '\0') {
                                    fonts::Set_Font_Style_(f->font_style, f->font_color);
                                    int16_t h = fonts::Get_Font_Height_() - 1;
                                    fonts::Print_Centered_(
                                        f->x + (f->x_end - f->x) / 2,
                                        f->y + (f->y_end - f->y) / 2 - h / 2,
                                        locked->help
                                    );
                                }
                            } else {
                                if (locked->draw_style != 0) {
                                    animate::Set_Animation_Frame_(locked->pic, 1);
                                } else {
                                    animate::Reset_Animation_Frame_(locked->pic);
                                    animate::Draw_(f->x, f->y, locked->pic);
                                }
                                animate::Draw_(f->x, f->y, locked->pic);
                                if (locked->help != nullptr && *locked->help != '\0') {
                                    fonts::Set_Font_Style_(f->font_style, f->font_color);
                                    int16_t h = fonts::Get_Font_Height_() - 1;
                                    fonts::Print_Centered_(
                                        f->x + (f->x_end - f->x) / 2 + _selected_button_text_x_offset,
                                        f->y + (f->y_end - f->y) / 2 - h / 2 + _selected_button_text_y_offset,
                                        locked->help
                                    );
                                }
                            }
                        }
                        break;
                    case FIELD_TYPE_MULTI_BUTTON:
                        {
                            s_field_multi_button* multi_button = Field_Data_Multi_Button_(f);
                            if (*multi_button->selection == multi_button->button_value) {
                                if (multi_button->draw_style != 0) {
                                    animate::Set_Animation_Frame_(multi_button->pic, 1);
                                } else {
                                    animate::Reset_Animation_Frame_(multi_button->pic);
                                    animate::Draw_(f->x, f->y, multi_button->pic);
                                }
                                animate::Draw_(f->x, f->y, multi_button->pic);
                                if (multi_button->help != nullptr && *multi_button->help != '\0') {
                                    fonts::Set_Font_Style_(f->font_style, f->font_color);
                                    int16_t h = fonts::Get_Font_Height_() - 1;
                                    fonts::Print_Centered_(
                                        f->x + (f->x_end - f->x) / 2 + _selected_button_text_x_offset,
                                        f->y + (f->y_end - f->y) / 2 - h / 2 + _selected_button_text_y_offset,
                                        multi_button->help
                                    );
                                }
                            } else {
                                animate::Reset_Animation_Frame_(multi_button->pic);
                                animate::Draw_(f->x, f->y, multi_button->pic);
                                if (multi_button->help != nullptr && *multi_button->help != '\0') {
                                    fonts::Set_Font_Style_(f->font_style, f->font_color);
                                    int16_t h = fonts::Get_Font_Height_() - 1;
                                    fonts::Print_Centered_(
                                        f->x + (f->x_end - f->x) / 2,
                                        f->y + (f->y_end - f->y) / 2 - h / 2,
                                        multi_button->help
                                    );
                                }
                            }
                        }
                        break;
                    case FIELD_TYPE_LEGACY_INPUT:
                        if (_down_mouse_button != i) {
                            s_field_input_string* legacy_input = Field_Data_Legacy_Input_(f);
                            fonts::Set_Font_Style_(f->font_style, f->font_color);
                            fonts::Set_Font_Highlight_Colors_(legacy_input->highlight_color);
                            if (legacy_input->background_color != 0) {
                                graphics::Fill_(f->x - 1, f->y, f->x_end, f->y_end, static_cast<uint8_t>(legacy_input->background_color));
                            }
                            if (legacy_input->draw_style == 0) {
                                fonts::Print_(f->x, f->y, legacy_input->buffer);
                            } else {
                                fonts::Print_Right_(f->x_end, f->y, legacy_input->buffer);
                            }
                        }
                        break;
                    case FIELD_TYPE_SCROLL: {
                        s_field_scroll* scroll = Field_Data_Scroll_(f);
                        int16_t return_val = scroll->return_value;
                        int16_t set_val = scroll->set_value;
                        int16_t val = *scroll->value;
                        
                        if (val < return_val) {
                            val = return_val;
                        }
                        if (val > set_val) {
                            val = set_val;
                        }
                        *scroll->value = val;
                        break;
                    }
                    case FIELD_TYPE_STRING_LIST: {
                        s_field_string_list* string_list = Field_Data_String_List_(f);
                        fonts::Set_Font_Style_(f->font_style, f->font_color);
                        int16_t spacing = fonts::Get_Font_Vertical_Spacing_() / 2;
                        if (spacing == 0) {
                            spacing = 1;
                        }
                        int16_t font_height = fonts::Get_Font_Height_();

                        if (*string_list->value == string_list->help_start) {
                            graphics::Gradient_Fill_(
                                f->x - 1,
                                f->y - spacing + 1,
                                f->x_end,
                                f->y + font_height + 1,
                                string_list->gradient_mode,
                                string_list->gradient_palette,
                                string_list->gradient_extra,
                                string_list->gradient_pic,
                                string_list->gradient_colors
                            );
                            if (string_list->gradient_mode == 17) {
                                fonts::Set_Font_Style_(f->font_style, string_list->highlight_color);
                            }
                            fonts::Print_(f->x, f->y + 1, string_list->help);
                        } else if (string_list->is_selectable == 0) {
                            fonts::Set_Font_Style_(f->font_style, string_list->gradient_colors);
                            fonts::Print_(f->x, f->y + 1, string_list->help);
                        } else {
                            fonts::Print_(f->x, f->y + 1, string_list->help);
                        }
                        break;
                    }
                    case FIELD_TYPE_CONTINUOUS_INPUT: {
                        s_field_cont_input* cont_input = Field_Data_Continuous_Input_(f);
                        int16_t shadow_style = cont_input->font_shadow_style;
                        if (_active_input_field_number == i) {
                            if (shadow_style == -1) {
                                fonts::Set_Font_Style_Shadow_Down_(f->font_style, f->font_color);
                            } else if (shadow_style == 0) {
                                fonts::Set_Font_Style_(f->font_style, f->font_color);
                            } else if (shadow_style == 1) {
                                fonts::Set_Font_Style_Shadow_Up_(f->font_style, f->font_color);
                            } else {
                                fonts::Set_Font_Style_(f->font_style, f->font_color);
                            }

                            fonts::Set_Font_Highlight_Colors_(cont_input->highlight_color);
                            fonts::Set_Highlight_Colors_On_();

                            if (cont_input->background_color != 0) {
                                graphics::Fill_(f->x - 1, f->y, f->x_end, f->y_end, static_cast<uint8_t>(cont_input->background_color));
                            }

                            int16_t pos = _cursor_position / 2;
                            bool blink = (pos != _cursor_on);
                            
                            _cursor_on = pos;
                            _cursor_position++;
                            if (_cursor_position > 3) {
                                _cursor_position = 0;
                            }

                            if (blink) {
                                if (pos == 0) {
                                    int16_t len = 0;
                                    if (_continuous_string[0] != '_') {
                                        while (len < cont_input->max_length && _continuous_string[len] != '_') {
                                            len++;
                                        }
                                    }
                                    _continuous_string[len] = '\0';
                                } else {
                                    int16_t len = 0;
                                    if (_continuous_string[0] != '\0') {
                                        while (len < cont_input->max_length - 1 && _continuous_string[len] != '\0') {
                                            len++;
                                        }
                                    }
                                    if (len < cont_input->max_length - 1) {
                                        _continuous_string[len] = '_';
                                        _continuous_string[len + 1] = '\0';
                                    }
                                }
                            }

                            if (cont_input->pic != nullptr) {
                                animate::Draw_(f->x, f->y, cont_input->pic);
                            }

                            int16_t height = f->y_end - f->y;
                            int16_t font_height = fonts::Get_Font_Height_();
                            if (height > font_height) {
                                fonts::Print_(f->x + 10, f->y + (height / 2) - (font_height / 2), _continuous_string);
                            } else {
                                fonts::Print_(f->x + 10, f->y, _continuous_string);
                            }
                            fonts::Set_Normal_Colors_On_();

                        } else {
                            if (shadow_style == -1) {
                                fonts::Set_Font_Style_Shadow_Down_(f->font_style, f->font_color);
                            } else if (shadow_style == 0) {
                                fonts::Set_Font_Style_(f->font_style, f->font_color);
                            } else if (shadow_style == 1) {
                                fonts::Set_Font_Style_Shadow_Up_(f->font_style, f->font_color);
                            } else {
                                fonts::Set_Font_Style_(f->font_style, f->font_color);
                            }

                            if (cont_input->background_color != 0) {
                                graphics::Fill_(f->x - 1, f->y, f->x_end, f->y_end, static_cast<uint8_t>(cont_input->background_color));
                            }

                            int16_t height = f->y_end - f->y;
                            int16_t font_height = fonts::Get_Font_Height_();
                            if (height > font_height) {
                                fonts::Print_(f->x + 10, f->y + (height / 2) - (font_height / 2), cont_input->buffer);
                            } else {
                                fonts::Print_(f->x + 10, f->y, cont_input->buffer);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }

                if (_field_box_mode != 0 && f->field_type != FIELD_TYPE_MULTI_HOTKEY) {
                    video::Clip_On_();
                    line::Line_(f->x, f->y, f->x_end, f->y, static_cast<uint8_t>(_field_outline_color));
                    line::Line_(f->x, f->y, f->x, f->y_end, static_cast<uint8_t>(_field_outline_color));
                    line::Line_(f->x, f->y_end, f->x_end, f->y_end, static_cast<uint8_t>(_field_outline_color));
                    line::Line_(f->x_end, f->y, f->x_end, f->y_end, static_cast<uint8_t>(_field_outline_color));
                    video::Clip_Off_();
                }
            }
        }

        if (_help_box_mode != 0 && _help_list_count > 0) {
            for (int16_t i = 0; i < _help_list_count; ++i) {
                const s_help_box& help_box = _help_struct_pointer[i];
                if (help_box.help_id != -1) {
                    video::Clip_On_();
                    line::Line_(help_box.x1, help_box.y1, help_box.x2, help_box.y1, static_cast<uint8_t>(_help_field_outline_color));
                    line::Line_(help_box.x1, help_box.y1, help_box.x1, help_box.y2, static_cast<uint8_t>(_help_field_outline_color));
                    line::Line_(help_box.x1, help_box.y2, help_box.x2, help_box.y2, static_cast<uint8_t>(_help_field_outline_color));
                    line::Line_(help_box.x2, help_box.y1, help_box.x2, help_box.y2, static_cast<uint8_t>(_help_field_outline_color));
                    video::Clip_Off_();
                }
            }
        }

        graphics::Set_Window_(x1, y1, x2, y2);
    }

    void __cdecl Push_Field_Down_(int16_t field_idx, int16_t x, int16_t y) {
        if (x >= 0 && x < 640 && y >= 0 && y < 480) {
            mouse::Save_Mouse_State_();

            if (_down_mouse_button != field_idx) {
                if (_down_mouse_button == -1) {
                    _down_mouse_button = field_idx;
                    Draw_Field_(field_idx, 1);

                    if (_fields[field_idx].field_type == FIELD_TYPE_LEGACY_INPUT) {
                        x = mouse::Pointer_X_();
                        y = mouse::Pointer_Y_();
                    }

                    platform::Force_Cursor_Pos(x, y);
                    mouse::Restore_Mouse_State_();
                    return;
                }

                int16_t down_idx = _down_mouse_button;
                int16_t type_down = _fields[down_idx].field_type;

                if ((type_down != FIELD_TYPE_MULTI_BUTTON || _fields[field_idx].field_type == FIELD_TYPE_MULTI_BUTTON) &&
                    (type_down != FIELD_TYPE_STRING_LIST || (_fields[field_idx].field_type == FIELD_TYPE_STRING_LIST &&
                                         Field_Data_String_List_(&_fields[field_idx])->is_selectable != 0))) {
                    Draw_Field_(down_idx, 0);
                }

                _down_mouse_button = field_idx;
                Draw_Field_(field_idx, 1);

                if (_fields[field_idx].field_type == FIELD_TYPE_LEGACY_INPUT) {
                    x = mouse::Pointer_X_();
                    y = mouse::Pointer_Y_();
                }

                platform::Force_Cursor_Pos(x, y);
            }

            mouse::Restore_Mouse_State_();
        }
    }

    uint16_t __cdecl Pressed_Mouse_X_() {
        return _pressed_mouse_x_value;
    }

    uint16_t __cdecl Pressed_Mouse_Y_() {
        return _pressed_mouse_y_value;
    }

    void __cdecl Copy_Continuous_String_() {
        int16_t len = 0;
        s_field_cont_input* cont_input = Field_Data_Continuous_Input_(&_fields[_active_input_field_number]);

        if (_continuous_string[0] != '\0') {
            int16_t max_len = cont_input->max_length;
            do {
                if (len >= max_len) {
                    break;
                }
                len++;
            } while (_continuous_string[len] != '\0');
        }

        if (len != 0 && _continuous_string[len - 1] == '_') {
            len--;
        }

        _continuous_string[len] = '\0';

        strcpy(cont_input->buffer, _continuous_string);
    }

    void __cdecl Draw_Visible_Fields_() {
        int16_t old_x1 = video::_screen_window_x1;
        int16_t old_x2 = video::_screen_window_x2;
        int16_t old_y1 = video::_screen_window_y1;
        int16_t old_y2 = video::_screen_window_y2;

        graphics::Reset_Window_();
        video::Set_Page_Off_();

        if (_draw_fields_flag != 0 && _fields_count > 1) {
            for (int i = 1; i < _fields_count; i++) {
                if (_down_mouse_button == i && _fields[i].field_type != FIELD_TYPE_LEGACY_INPUT && _fields[i].field_type != FIELD_TYPE_GRID && _fields[i].field_type != FIELD_TYPE_STRING_LIST) {
                    Draw_Field_(i, 1);
                    continue;
                }

                switch (_fields[i].field_type) {
                    case FIELD_TYPE_BUTTON: {
                        s_field_button* button = Field_Data_Button_(&_fields[i]);
                        animate::Reset_Animation_Frame_(button->pic);
                        animate::Draw_(_fields[i].x, _fields[i].y, button->pic);
                        if (button->help != nullptr && button->help[0] != '\0') {
                            fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                            int font_height = fonts::Get_Font_Height_();
                            int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2;
                            int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2);
                            fonts::Print_Centered_(cx, cy, button->help);
                        }
                        break;
                    }
                    case FIELD_TYPE_RADIO:
                        {
                            s_field_radio* radio = Field_Data_Radio_(&_fields[i]);
                            if (*radio->selection == 0) {
                                animate::Reset_Animation_Frame_(radio->pic);
                                animate::Draw_(_fields[i].x, _fields[i].y, radio->pic);
                                if (radio->help != nullptr && radio->help[0] != '\0') {
                                    fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                                    int font_height = fonts::Get_Font_Height_();
                                    int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2;
                                    int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2);
                                    fonts::Print_Centered_(cx, cy, radio->help);
                                }
                            } else {
                                if (radio->draw_style != 0) {
                                    animate::Set_Animation_Frame_(radio->pic, 1);
                                } else {
                                    animate::Reset_Animation_Frame_(radio->pic);
                                    animate::Draw_(_fields[i].x, _fields[i].y, radio->pic);
                                }
                                animate::Draw_(_fields[i].x, _fields[i].y, radio->pic);
                                if (radio->help != nullptr && radio->help[0] != '\0') {
                                    fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                                    int font_height = fonts::Get_Font_Height_();
                                    int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2 + _selected_button_text_x_offset;
                                    int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2) + _selected_button_text_y_offset;
                                    fonts::Print_Centered_(cx, cy, radio->help);
                                }
                            }
                        }
                        break;
                    case FIELD_TYPE_LOCKED: {
                        s_field_locked* locked = Field_Data_Locked_Button_(&_fields[i]);
                        if (*locked->selection == 0) {
                            animate::Reset_Animation_Frame_(locked->pic);
                            animate::Draw_(_fields[i].x, _fields[i].y, locked->pic);
                            if (locked->help != nullptr && locked->help[0] != '\0') {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                                int font_height = fonts::Get_Font_Height_();
                                int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2;
                                int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2);
                                fonts::Print_Centered_(cx, cy, locked->help);
                            }
                        } else {
                            if (locked->draw_style != 0) {
                                animate::Set_Animation_Frame_(locked->pic, 1);
                            } else {
                                animate::Reset_Animation_Frame_(locked->pic);
                                animate::Draw_(_fields[i].x, _fields[i].y, locked->pic);
                            }
                            animate::Draw_(_fields[i].x, _fields[i].y, locked->pic);
                            if (locked->help != nullptr && locked->help[0] != '\0') {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                                int font_height = fonts::Get_Font_Height_();
                                int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2 + _selected_button_text_x_offset;
                                int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2) + _selected_button_text_y_offset;
                                fonts::Print_Centered_(cx, cy, locked->help);
                            }
                        }
                        break;
                    }
                    case FIELD_TYPE_MULTI_BUTTON: {
                        s_field_multi_button* multi_button = Field_Data_Multi_Button_(&_fields[i]);
                        if (*multi_button->selection == multi_button->button_value) {
                            if (multi_button->draw_style != 0) {
                                animate::Set_Animation_Frame_(multi_button->pic, 1);
                            } else {
                                animate::Reset_Animation_Frame_(multi_button->pic);
                                animate::Draw_(_fields[i].x, _fields[i].y, multi_button->pic);
                            }
                            animate::Draw_(_fields[i].x, _fields[i].y, multi_button->pic);
                            if (multi_button->help != nullptr && multi_button->help[0] != '\0') {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                                int font_height = fonts::Get_Font_Height_();
                                int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2 + _selected_button_text_x_offset;
                                int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2) + _selected_button_text_y_offset;
                                fonts::Print_Centered_(cx, cy, multi_button->help);
                            }
                        } else {
                            animate::Reset_Animation_Frame_(multi_button->pic);
                            animate::Draw_(_fields[i].x, _fields[i].y, multi_button->pic);
                            if (multi_button->help != nullptr && multi_button->help[0] != '\0') {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                                int font_height = fonts::Get_Font_Height_();
                                int cx = _fields[i].x + (_fields[i].x_end - _fields[i].x) / 2;
                                int cy = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - ((font_height - 1) / 2);
                                fonts::Print_Centered_(cx, cy, multi_button->help);
                            }
                        }
                        break;
                    }
                    case FIELD_TYPE_LEGACY_INPUT: {
                        s_field_input_string* legacy_input = Field_Data_Legacy_Input_(&_fields[i]);
                        if (_down_mouse_button != i) {
                            fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                            fonts::Set_Font_Highlight_Colors_(legacy_input->highlight_color);
                            if (legacy_input->background_color != 0) {
                                graphics::Fill_(_fields[i].x - 1, _fields[i].y, _fields[i].x_end, _fields[i].y_end, (uint8_t)legacy_input->background_color);
                            }
                            if (legacy_input->draw_style == 0) {
                                fonts::Print_(_fields[i].x, _fields[i].y, legacy_input->buffer);
                            } else {
                                fonts::Print_Right_(_fields[i].x_end, _fields[i].y, legacy_input->buffer);
                            }
                        }
                        break;
                    }
                    case FIELD_TYPE_SCROLL: {
                        s_field_scroll* scroll = Field_Data_Scroll_(&_fields[i]);
                        int16_t ret_val = scroll->return_value;
                        int16_t max_val = scroll->set_value;
                        int16_t cur_val = *scroll->value;
                        if (cur_val < ret_val) {
                            cur_val = ret_val;
                        }
                        if (max_val < cur_val) {
                            cur_val = max_val;
                        }
                        *scroll->value = cur_val;
                        break;
                    }
                    case FIELD_TYPE_STRING_LIST: {
                        s_field_string_list* string_list = Field_Data_String_List_(&_fields[i]);
                        fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                        int spacing = fonts::Get_Font_Vertical_Spacing_() / 2;
                        if (spacing == 0) {
                            spacing = 1;
                        }
                        int font_height = fonts::Get_Font_Height_();
                        if (*string_list->value == string_list->help_start) {
                            graphics::Gradient_Fill_(
                                _fields[i].x - 1,
                                (_fields[i].y - spacing) + 1,
                                _fields[i].x_end,
                                _fields[i].y + font_height + 1,
                                string_list->gradient_mode,
                                string_list->gradient_palette,
                                string_list->gradient_extra,
                                string_list->gradient_pic,
                                string_list->gradient_colors
                            );
                            if (string_list->gradient_mode == 17) {
                                fonts::Set_Font_Style_(_fields[i].font_style, string_list->highlight_color);
                            }
                            fonts::Print_(_fields[i].x, _fields[i].y + 1, string_list->help);
                        } else if (string_list->is_selectable == 0) {
                            fonts::Set_Font_Style_(_fields[i].font_style, string_list->gradient_colors);
                            fonts::Print_(_fields[i].x, _fields[i].y + 1, string_list->help);
                        } else {
                            fonts::Print_(_fields[i].x, _fields[i].y + 1, string_list->help);
                        }
                        break;
                    }
                    case FIELD_TYPE_CONTINUOUS_INPUT: {
                        s_field_cont_input* cont_input = Field_Data_Continuous_Input_(&_fields[i]);
                        int shadow_style = cont_input->font_shadow_style;
                        if (i != _active_input_field_number) {
                            if (shadow_style == -1) {
                                fonts::Set_Font_Style_Shadow_Down_(_fields[i].font_style, _fields[i].font_color);
                            } else if (shadow_style == 0) {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                            } else if (shadow_style == 1) {
                                fonts::Set_Font_Style_Shadow_Up_(_fields[i].font_style, _fields[i].font_color);
                            } else {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                            }

                            if (cont_input->background_color != 0) {
                                graphics::Fill_(_fields[i].x - 1, _fields[i].y, _fields[i].x_end, _fields[i].y_end, (uint8_t)cont_input->background_color);
                            }

                            int font_height = fonts::Get_Font_Height_();
                            if (font_height < _fields[i].y_end - _fields[i].y) {
                                int y_centered = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - (font_height / 2);
                                fonts::Print_(_fields[i].x + 10, y_centered, cont_input->buffer);
                            } else {
                                fonts::Print_(_fields[i].x + 10, _fields[i].y, cont_input->buffer);
                            }
                        } else {
                            if (shadow_style == -1) {
                                fonts::Set_Font_Style_Shadow_Down_(_fields[i].font_style, _fields[i].font_color);
                            } else if (shadow_style == 0) {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                            } else if (shadow_style == 1) {
                                fonts::Set_Font_Style_Shadow_Up_(_fields[i].font_style, _fields[i].font_color);
                            } else {
                                fonts::Set_Font_Style_(_fields[i].font_style, _fields[i].font_color);
                            }

                            fonts::Set_Font_Highlight_Colors_(cont_input->highlight_color);
                            fonts::Set_Highlight_Colors_On_();

                            if (cont_input->background_color != 0) {
                                graphics::Fill_(_fields[i].x - 1, _fields[i].y, _fields[i].x_end, _fields[i].y_end, (uint8_t)cont_input->background_color);
                            }

                            int cur_pos = _cursor_position / 2;
                            bool cursor_changed = cur_pos != _cursor_on;
                            _cursor_on = (int16_t)cur_pos;
                            _cursor_position++;
                            if (_cursor_position > 3) {
                                _cursor_position = 0;
                            }

                            if (cursor_changed) {
                                if (cur_pos == 0) {
                                    int len = 0;
                                    while (len < cont_input->max_length - 1 && _continuous_string[len] != '_') {
                                        len++;
                                    }
                                    _continuous_string[len] = '\0';
                                } else {
                                    int len = 0;
                                    while (len < cont_input->max_length - 2 && _continuous_string[len] != '\0') {
                                        len++;
                                    }
                                    if (len < cont_input->max_length - 1) {
                                        strlcat(_continuous_string, "_", sizeof(_continuous_string));
                                    }
                                }
                            }

                            if (cont_input->pic != nullptr) {
                                animate::Draw_(_fields[i].x, _fields[i].y, cont_input->pic);
                            }

                            int font_height = fonts::Get_Font_Height_();
                            if (font_height < _fields[i].y_end - _fields[i].y) {
                                int y_centered = _fields[i].y + ((_fields[i].y_end - _fields[i].y) / 2) - (font_height / 2);
                                fonts::Print_(_fields[i].x + 10, y_centered, _continuous_string);
                            } else {
                                fonts::Print_(_fields[i].x + 10, _fields[i].y, _continuous_string);
                            }
                            fonts::Set_Normal_Colors_On_();
                        }
                        break;
                    }
                    default:
                        break;
                }

                if (_field_box_mode != 0 && _fields[i].field_type != FIELD_TYPE_MULTI_HOTKEY) {
                    video::Clip_On_();
                    line::Line_(_fields[i].x, _fields[i].y, _fields[i].x_end, _fields[i].y, (uint8_t)_field_outline_color);
                    line::Line_(_fields[i].x, _fields[i].y, _fields[i].x, _fields[i].y_end, (uint8_t)_field_outline_color);
                    line::Line_(_fields[i].x, _fields[i].y_end, _fields[i].x_end, _fields[i].y_end, (uint8_t)_field_outline_color);
                    line::Line_(_fields[i].x_end, _fields[i].y, _fields[i].x_end, _fields[i].y_end, (uint8_t)_field_outline_color);
                    video::Clip_Off_();
                }
            }
        }

        if (_help_box_mode != 0 && _help_list_count > 0) {
            for (int16_t i = 0; i < _help_list_count; ++i) {
                const s_help_box& help_box = _help_struct_pointer[i];
                if (help_box.help_id != -1) {
                    video::Clip_On_();
                    line::Line_(help_box.x1, help_box.y1, help_box.x2, help_box.y1, (uint8_t)_help_field_outline_color);
                    line::Line_(help_box.x1, help_box.y1, help_box.x1, help_box.y2, (uint8_t)_help_field_outline_color);
                    line::Line_(help_box.x1, help_box.y2, help_box.x2, help_box.y2, (uint8_t)_help_field_outline_color);
                    line::Line_(help_box.x2, help_box.y1, help_box.x2, help_box.y2, (uint8_t)_help_field_outline_color);
                    video::Clip_Off_();
                }
            }
        }

        graphics::Set_Window_(old_x1, old_y1, old_x2, old_y2);
    }

    int16_t __cdecl Check_Multi_Hot_Key_Fields_(int16_t hotkey) {
        int16_t result = -1;

        for (int16_t i = 0; i < _fields_count; ++i) {
            if (result != -1) {
                break;
            }

            if (_fields[i].field_type == FIELD_TYPE_MULTI_HOTKEY) {
                s_field_multi_hotkey* multi_hotkey = Field_Data_Multi_Hot_Key_(&_fields[i]);
                int16_t help_start = multi_hotkey->help_start;

                if (multi_hotkey->help[help_start] == hotkey) {
                    multi_hotkey->help_start++;

                    if (multi_hotkey->help_start == multi_hotkey->help_len) {
                        result = i;
                    }
                }
            }
        }

        return result;
    }

    void __cdecl Clear_Multi_Hot_Key_Offsets_() {
        for (int16_t i = 0; i < _fields_count; ++i) {
            if (_fields[i].field_type == FIELD_TYPE_MULTI_HOTKEY) {
                Field_Data_Multi_Hot_Key_(&_fields[i])->help_start = 0;
            }
        }
    }

    int16_t __cdecl Interpret_Keyboard_Input_(int16_t* field_idx) {
        int16_t key_val = (int16_t)key::Read_Key_();

        if (_fields_count <= _selected_count) {
            _selected_count = 0;
        }

        int16_t out_cmd = 0;
        if (key_val <= 0x3B00) {
            if (key_val == 0x3B00) { out_cmd = -1001; }
            else if (key_val == 0x01F0) { out_cmd = -1500; }
        } else if (key_val <= 0x3D00) {
            if (key_val == 0x3D00) { out_cmd = -1003; }
            else if (key_val == 0x3C00) { out_cmd = -1002; }
        } else if (key_val <= 0x3F00) {
            if (key_val == 0x3F00) { out_cmd = -1005; }
            else if (key_val == 0x3E00) { out_cmd = -1004; }
        } else if (key_val <= 0x4100) {
            if (key_val == 0x4100) { out_cmd = -1007; }
            else if (key_val == 0x4000) { out_cmd = -1006; }
        } else if (key_val <= 0x4300) {
            if (key_val == 0x4300) { out_cmd = -1009; }
            else if (key_val == 0x4200) { out_cmd = -1008; }
        } else if (key_val <= 0x6800) {
            if (key_val == 0x6800) { out_cmd = -1101; }
            else if (key_val == 0x4400) { out_cmd = -1010; }
        } else if (key_val <= 0x6A00) {
            if (key_val == 0x6A00) { out_cmd = -1103; }
            else if (key_val == 0x6900) { out_cmd = -1102; }
        } else if (key_val <= 0x6C00) {
            if (key_val == 0x6C00) { out_cmd = -1105; }
            else if (key_val == 0x6B00) { out_cmd = -1104; }
        } else if (key_val <= 0x6E00) {
            if (key_val == 0x6E00) { out_cmd = -1107; }
            else if (key_val == 0x6D00) { out_cmd = -1106; }
        } else if (key_val == 0x6F00) {
            out_cmd = -1108;
        } else if (key_val == 0x7000) {
            out_cmd = -1109;
        } else if (key_val == 0x7100) {
            out_cmd = -1110;
        }

        if (out_cmd < 0) {
            *field_idx = out_cmd;
            return key_val;
        }

        if (_multi_hotkey_active_field != 0) {
            uint8_t alt_key = Get_Alt_Alpha_Key_(key_val);
            int16_t multi_hotkey_res = Check_Multi_Hot_Key_Fields_(alt_key);
            if (multi_hotkey_res > -1) {
                *field_idx = multi_hotkey_res;
                return -1;
            }
        }

        int32_t eax_val = key_val;
        int32_t edx_val = (eax_val < 0) ? -1 : 0;
        eax_val = eax_val ^ edx_val;
        eax_val = eax_val - edx_val;
        eax_val = eax_val & 0xFF;
        eax_val = eax_val ^ edx_val;
        eax_val = eax_val - edx_val;

        int16_t hotkey_char = (int16_t)eax_val;

        if (hotkey_char == 0) {
            hotkey_char = key_val;
        }

        if (hotkey_char > 0x60 && hotkey_char < 0x7B) {
            hotkey_char -= 0x20;
        }

        if (_fields_count > _fields_count) {
            *field_idx = _fields_count;
            if (_fields[_fields_count].x < 640 && _fields[_fields_count].y < 480) {
                mouse::Save_Mouse_State_();
                mouse_saved_center_x = _fields[_fields_count].x + (_fields[_fields_count].x_end - _fields[_fields_count].x) / 2;
                mouse_saved_center_y = _fields[_fields_count].y + (_fields[_fields_count].y_end - _fields[_fields_count].y) / 2;
                mouse::Restore_Mouse_State_();
            }
        }

        for (int16_t i = 0; i < _fields_count; i++) {
            if (_active_input_field_number == -1 && _fields[i].hotkey == hotkey_char) {
                *field_idx = i;
                break;
            }
        }

        if (_fields[*field_idx].hotkey != hotkey_char) {
            hotkey_char = key_val;
        }

        return hotkey_char;
    }

    void __cdecl Increment_Scroll_Bar_(int16_t field_idx) {
        s_field* field = &_fields[field_idx];
        s_field_scroll* scroll = Field_Data_Scroll_(field);
        int16_t range_min = scroll->range_min;
        int16_t* value_ptr = scroll->value;
        int32_t range_diff = scroll->range_max - range_min;

        int32_t percent = (((int32_t)*value_ptr - (int32_t)range_min) * 100) / range_diff + 5;

        int16_t new_val;
        if (percent <= 100) {
            new_val = (int16_t)(range_min + ((percent * range_diff) / 100));
        } else {
            new_val = scroll->set_value;
        }

        if (new_val > scroll->set_value) {
            new_val = scroll->set_value;
        }

        *value_ptr = new_val;
    }

    void __cdecl Decrement_Scroll_Bar_(int16_t field_idx) {
        s_field_scroll* scroll = Field_Data_Scroll_(&_fields[field_idx]);
        int16_t range_min = scroll->range_min;
        int16_t* value_ptr = scroll->value;
        int16_t range = scroll->range_max - range_min;

        int16_t pct = (int16_t)((((*value_ptr - range_min) * 100) / range) - 5);

        int16_t new_value;
        if (pct < 0) {
            new_value = scroll->return_value;
        } else {
            new_value = range_min + (int16_t)(((int32_t)pct * range) / 100);
        }

        if (new_value < scroll->return_value) {
            new_value = scroll->return_value;
        }

        *value_ptr = new_value;
    }

    void __cdecl Draw_Field_(int16_t field_idx, int16_t action) {
        if (_draw_fields_flag == 0) {
            return;
        }

        s_field* field = &_fields[field_idx];

        switch (field->field_type) {
            case FIELD_TYPE_BUTTON:
            case FIELD_TYPE_IRREGULAR_BUTTON: {
                int16_t x_pos;
                int16_t y_pos;
                const char* help_text;
                s_animation_header* pic;
                int16_t draw_style;

                if (field->field_type == FIELD_TYPE_IRREGULAR_BUTTON) {
                    const s_field_irregular_button* irregular_button = Field_Data_Irregular_Button_(field);
                    x_pos = irregular_button->draw_x;
                    y_pos = irregular_button->draw_y;
                    help_text = irregular_button->help;
                    pic = irregular_button->pic;
                    draw_style = irregular_button->draw_style;
                } else {
                    const s_field_button* button = Field_Data_Button_(field);
                    x_pos = field->x;
                    y_pos = field->y;
                    help_text = button->help;
                    pic = button->pic;
                    draw_style = button->draw_style;
                }

                if (action == 0) {
                    animate::Reset_Animation_Frame_(pic);
                    animate::Draw_(x_pos, y_pos, pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2,
                        help_text
                    );
                } else {
                    if (draw_style == 0) {
                        animate::Reset_Animation_Frame_(pic);
                        animate::Draw_(x_pos, y_pos, pic);
                    } else {
                        animate::Set_Animation_Frame_(pic, 1);
                    }
                    animate::Draw_(x_pos, y_pos, pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2 + _selected_button_text_x_offset,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2 + _selected_button_text_y_offset,
                        help_text
                    );
                }
                break;
            }
            case FIELD_TYPE_RADIO: {
                const s_field_radio* radio = Field_Data_Radio_(field);
                if (action != 0 && *radio->selection != 1) {
                    if (radio->draw_style == 0) {
                        animate::Reset_Animation_Frame_(radio->pic);
                        animate::Draw_(field->x, field->y, radio->pic);
                    } else {
                        animate::Set_Animation_Frame_(radio->pic, 1);
                    }
                    animate::Draw_(field->x, field->y, radio->pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2 + _selected_button_text_x_offset,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2 + _selected_button_text_y_offset,
                        radio->help
                    );
                } else {
                    animate::Reset_Animation_Frame_(radio->pic);
                    animate::Draw_(field->x, field->y, radio->pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2,
                        radio->help
                    );
                }
                break;
            }
            case FIELD_TYPE_LOCKED: {
                const s_field_locked* locked = Field_Data_Locked_Button_(field);
                if (*locked->selection == 0 && action != 1) {
                    animate::Reset_Animation_Frame_(locked->pic);
                    animate::Draw_(field->x, field->y, locked->pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2,
                        locked->help
                    );
                } else {
                    if (locked->draw_style == 0) {
                        animate::Reset_Animation_Frame_(locked->pic);
                        animate::Draw_(field->x, field->y, locked->pic);
                    } else {
                        animate::Set_Animation_Frame_(locked->pic, 1);
                    }
                    animate::Draw_(field->x, field->y, locked->pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2 + _selected_button_text_x_offset,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2 + _selected_button_text_y_offset,
                        locked->help
                    );
                }
                break;
            }
            case FIELD_TYPE_MULTI_BUTTON: {
                s_field_multi_button* multi_button = Field_Data_Multi_Button_(field);
                if (action == 0) {
                    *multi_button->selection = -1000;
                } else {
                    *multi_button->selection = multi_button->button_value;
                }

                if (*multi_button->selection != multi_button->button_value) {
                    animate::Reset_Animation_Frame_(multi_button->pic);
                    animate::Draw_(field->x, field->y, multi_button->pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2,
                        multi_button->help
                    );
                } else {
                    if (multi_button->draw_style == 0) {
                        animate::Reset_Animation_Frame_(multi_button->pic);
                        animate::Draw_(field->x, field->y, multi_button->pic);
                    } else {
                        animate::Set_Animation_Frame_(multi_button->pic, 1);
                    }
                    animate::Draw_(field->x, field->y, multi_button->pic);

                    fonts::Set_Font_Style_(field->font_style, field->font_color);
                    int16_t font_height = fonts::Get_Font_Height_();
                    fonts::Print_Centered_(
                        field->x + (field->x_end - field->x) / 2 + _selected_button_text_x_offset,
                        field->y + (field->y_end - field->y) / 2 - (font_height - 1) / 2 + _selected_button_text_y_offset,
                        multi_button->help
                    );
                }
                break;
            }
            case FIELD_TYPE_LEGACY_INPUT: {
                if (action == 1) {
                    Input_String_(field_idx);
                }
                break;
            }
            case FIELD_TYPE_SCROLL: {
                if (action == 1) {
                    Find_Bar_Position_(field_idx);
                }
                break;
            }
            case FIELD_TYPE_GRID: {
                s_field_grid* grid = Field_Data_Grid_(field);
                _pointer_offset = mouse::Get_Pointer_Offset_();
                if (action == 1) {
                    *grid->grid_x_ptr = (mouse::Pointer_X_() - field->x + _pointer_offset) / grid->step_x;
                    *grid->grid_y_ptr = (mouse::Pointer_Y_() - field->y + _pointer_offset) / grid->step_y;

                    int16_t max_x = (field->x_end - field->x) / grid->step_x;
                    if (max_x < *grid->grid_x_ptr) {
                        *grid->grid_x_ptr = max_x;
                    }

                    int16_t max_y = (field->y_end - field->y) / grid->step_y;
                    if (max_y < *grid->grid_y_ptr) {
                        *grid->grid_y_ptr = max_y;
                    }

                    if (*grid->grid_x_ptr < 0) {
                        *grid->grid_x_ptr = 0;
                    }

                    if (*grid->grid_y_ptr < 0) {
                        *grid->grid_y_ptr = 0;
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    uint8_t __cdecl Get_Alt_Alpha_Key_(int16_t keycode) {
        switch (keycode) {
            case 0x1000: return 0x51; /* 'Q' */
            case 0x1100: return 0x57; /* 'W' */
            case 0x1200: return 0x45; /* 'E' */
            case 0x1300: return 0x52; /* 'R' */
            case 0x1400: return 0x54; /* 'T' */
            case 0x1500: return 0x59; /* 'Y' */
            case 0x1600: return 0x55; /* 'U' */
            case 0x1700: return 0x49; /* 'I' */
            case 0x1800: return 0x4F; /* 'O' */
            case 0x1900: return 0x50; /* 'P' */
            case 0x1E00: return 0x41; /* 'A' */
            case 0x1F00: return 0x53; /* 'S' */
            case 0x2000: return 0x44; /* 'D' */
            case 0x2100: return 0x46; /* 'F' */
            case 0x2200: return 0x47; /* 'G' */
            case 0x2300: return 0x48; /* 'H' */
            case 0x2400: return 0x4A; /* 'J' */
            case 0x2500: return 0x4B; /* 'K' */
            case 0x2600: return 0x4C; /* 'L' */
            case 0x2C00: return 0x5A; /* 'Z' */
            case 0x2D00: return 0x58; /* 'X' */
            case 0x2E00: return 0x43; /* 'C' */
            case 0x2F00: return 0x56; /* 'V' */
            case 0x3000: return 0x42; /* 'B' */
            case 0x3100: return 0x4E; /* 'N' */
            case 0x3200: return 0x4D; /* 'M' */
            case 0x7800: return 0x30; /* '0' */
            case 0x7900: return 0x31; /* '1' */
            case 0x7A00: return 0x32; /* '2' */
            case 0x7B00: return 0x33; /* '3' */
            case 0x7C00: return 0x34; /* '4' */
            case 0x7D00: return 0x35; /* '5' */
            case 0x7E00: return 0x36; /* '6' */
            case 0x7F00: return 0x37; /* '7' */
            default: return 0xFF;
        }
    }

    int16_t __cdecl Check_Help_List_() {
        int16_t mouse_x = static_cast<int16_t>(mouse::Pointer_X_());
        int16_t mouse_y = static_cast<int16_t>(mouse::Pointer_Y_());

        if (_help_list_count == 0) {
            return 1;
        }

        for (int16_t i = 0; i < _help_list_count; ++i) {
            const s_help_box& entry = _help_struct_pointer[i];

            if (entry.help_id != -1 &&
                entry.x1 <= mouse_x && mouse_x <= entry.x2 &&
                entry.y1 <= mouse_y && mouse_y <= entry.y2) {
                TEXTBOX::Draw_Help_Entry_(entry.help_id);
                return 0;
            }
        }

        return 1;
    }

    void __cdecl Input_String_(int16_t field_idx) {
        platform::Check_Exit_();

        int16_t len = 0;
        _cursor_count = 0;
        _cursor_position = 0;

        s_field* field = &_fields[field_idx];
        s_field_input_string* legacy_input = Field_Data_Legacy_Input_(field);
        int16_t max_length = legacy_input->max_length;
        int16_t max_width = field->x_end - field->x;
        bool is_editing_started = false;

        mouse::Save_Mouse_State_();
        fonts::Set_Font_Style_(field->font_style, field->font_color);

        char buffer[64];
        strlcpy(buffer, legacy_input->buffer, sizeof(buffer));

        len = (int16_t)strlen(buffer);

        while (max_width < fonts::Get_String_Width_(buffer) && len > 0) {
            buffer[0] = '\0';
            fonts::Get_String_Width_(buffer);
            len = 0;
        }

        _cursor_count = len;
        if (max_length <= len) {
            _cursor_count = max_length;
        }

        strcpy(legacy_input->buffer, buffer);
        int16_t font_height = fonts::Get_Font_Height_();
        Display_Input_Field_(field_idx, buffer);

        int16_t done = 0;
        while (done == 0) {
            platform::Check_Exit_();
            int16_t key_code = 0;
            while (done == 0) {
                platform::Check_Exit_();
                key_code = key::Scan_Key_();
                if (mouse::Mouse_Button_() != 0 || mouse::Mouse_Buffer_() != 0) {
                    done = 1;
                    break;
                }

                _cursor_position++;
                if (_cursor_position > font_height * 2 - 1) {
                    _cursor_position = 0;
                }

                Display_Input_Field_(field_idx, buffer);
                if (key_code != 0) {
                    break;
                }
            }

            if (done != 0) {
                break;
            }

            if (key_code == 0x1C0D) { // Enter
                break;
            } else if (key_code == 0x0E08) { // Backspace
                if (!is_editing_started) {
                    buffer[0] = '\0';
                    is_editing_started = true;
                    len = 0;
                    _cursor_count = 0;
                    _cursor_position = 0;
                } else if (len > 0) {
                    if (_cursor_count < len) {
                        if (_cursor_count > 0) {
                            for (int16_t i = _cursor_count; i < len; i++) {
                                buffer[i - 1] = buffer[i];
                            }
                            len--;
                            _cursor_count--;
                            buffer[len] = '\0';
                            _cursor_position = 0;
                        }
                    } else {
                        _cursor_position = 0;
                        len--;
                        _cursor_count--;
                        buffer[len] = '\0';
                    }
                }
            } else if (key_code == 0x4BE0) { // Left Arrow
                is_editing_started = true;
                if (_cursor_count > 0) {
                    _cursor_position = 0;
                    _cursor_count--;
                }
            } else if (key_code == 0x4DE0) { // Right Arrow
                if (_cursor_count < max_length && _cursor_count < len) {
                    _cursor_position = 0;
                    _cursor_count++;
                    if (_cursor_count >= len) {
                        buffer[len] = ' ';
                        buffer[len + 1] = '\0';
                        if (_cursor_count >= max_length || fonts::Get_String_Width_(buffer) > max_width) {
                            _cursor_count--;
                        }
                        buffer[len] = '\0';
                    }
                }
            } else if (key_code == 0x53E0) { // Delete
                if (len > 0 && _cursor_count < len) {
                    for (int16_t i = _cursor_count; i < len; i++) {
                        buffer[i] = buffer[i + 1];
                    }
                    len--;
                    _cursor_position = 0;
                    buffer[len] = '\0';
                }
            } else {
                bool is_valid = false;
                if (key_code == 0x011B) { // Escape
                    done = 2;
                } else {
                    // Extract lower 8-bits while preserving absolute original sign logic
                    int16_t sign = key_code >> 15;
                    key_code = ((((key_code ^ sign) - sign) & 0xFF) ^ sign) - sign;

                    if ((key_code > 0x40 && key_code < 0x5D) ||
                        (key_code > 0x2D && key_code < 0x3B) ||
                        key_code == 0x20 ||
                        key_code == 0x2D ||
                        (key_code > 0x60 && key_code < 0x7B)) {
                        is_valid = true;
                    }

                    if (is_valid) {
                        is_editing_started = true;
                        char ch = static_cast<char>(key_code);
                        buffer[len] = ch;
                        buffer[len + 1] = '\0';
                        if (len < max_length && fonts::Get_String_Width_(buffer) <= max_width) {
                            buffer[len] = '\0';
                            if (_cursor_count < len) {
                                for (int16_t i = len; i > _cursor_count; i--) {
                                    buffer[i] = buffer[i - 1];
                                }
                                len++;
                                _cursor_position = 0;
                                buffer[_cursor_count++] = ch;
                                buffer[len] = '\0';
                            } else {
                                buffer[len] = ch;
                                len++;
                                buffer[len] = ' ';
                                buffer[len + 1] = '\0';
                                if (len < max_length && fonts::Get_String_Width_(buffer) <= max_width) {
                                    _cursor_count++;
                                }
                                _cursor_position = 0;
                                buffer[len] = '\0';
                            }
                        } else {
                            buffer[len] = '\0';
                        }
                    }
                }
            }

            Display_Input_Field_(field_idx, buffer);
            if (key_code == 0x1C0D) {
                break;
            }
        }

        strcpy(Field_Data_Legacy_Input_(&_fields[field_idx])->buffer, buffer);

        if (done != 0) {
            while (mouse::Mouse_Button_() != 0) {
                platform::Check_Exit_();
                // Wait for mouse release
            }
        }

        mouse::Restore_Mouse_State_();
        _cursor_count = 0;
        _cursor_position = 0;
        _down_mouse_button = -1;
    }

    void __cdecl Display_Input_Field_(int16_t field_idx, char* input_buffer) {
        platform::Check_Exit_();

        char cursor_color_ramp[16];
        char local_40[64];
        s_field_input_string* legacy_input = Field_Data_Legacy_Input_(&_fields[field_idx]);

        timer::Mark_Time_();
        video::Set_Page_Off_();

        strlcpy(cursor_color_ramp, legacy_input->cursor_color_ramp, sizeof(cursor_color_ramp));
        strlcpy(local_40, input_buffer, sizeof(local_40));

        Invoke_Auto_Function_();

        fonts::Set_Font_Style_(_fields[field_idx].font_style, _fields[field_idx].font_color);
        short font_height = fonts::Get_Font_Height_() - 1;

        if (legacy_input->background_color != 0) {
            graphics::Fill_(
                _fields[field_idx].x - 1,
                _fields[field_idx].y,
                _fields[field_idx].x_end,
                _fields[field_idx].y_end,
                static_cast<uint8_t>(legacy_input->background_color)
            );
        }

        char char_str[2];
        char_str[1] = '\0';

        short input_mode = legacy_input->input_mode;

        if (input_mode == 0) {
            char_str[0] = local_40[_cursor_count];
            local_40[_cursor_count] = '\0';
            short prefix_width = fonts::Get_String_Width_(local_40);
            short cursor_x = _fields[field_idx].x + prefix_width;

            local_40[_cursor_count] = char_str[0];
            if (char_str[0] == '\0') {
                char_str[0] = ' ';
            }
            short char_width = fonts::Get_String_Width_(char_str);

            if (_cursor_position <= 0 || font_height < _cursor_position) {
                if (_cursor_position != 0) {
                    short lines = font_height * 2 - _cursor_position;
                    if (lines > 0) {
                        short color_idx = 0;
                        do {
                            short line_y = _fields[field_idx].y + lines - 1;
                            line::Line_(cursor_x, line_y, cursor_x + char_width + 1, line_y, (uint8_t)cursor_color_ramp[font_height - color_idx - 1]);
                            color_idx++;
                            lines--;
                        } while (lines != 0);
                    }
                }
            } else {
                short line_y_base = _fields[field_idx].y + font_height;
                short lines = _cursor_position;
                short color_idx = 0;
                do {
                    short line_y = line_y_base - lines + 1;
                    line::Line_(cursor_x, line_y, cursor_x + char_width + 1, line_y, (uint8_t)cursor_color_ramp[color_idx]);
                    color_idx++;
                    lines--;
                } while (lines != 0);
            }
        } else if (input_mode == 1) {
            char_str[0] = local_40[_cursor_count];
            local_40[_cursor_count] = '\0';
            short prefix_width = fonts::Get_String_Width_(local_40);
            short cursor_x = _fields[field_idx].x + prefix_width;

            local_40[_cursor_count] = char_str[0];
            if (char_str[0] == '\0') {
                char_str[0] = ' ';
            }
            short char_width = fonts::Get_String_Width_(char_str);

            if ((_cursor_position % 3) == 0) {
                if (_cursor_on == 0) {
                    _cursor_on = 1;
                } else if (_cursor_on == 1) {
                    _cursor_on = 0;
                }
            }

            if (_cursor_on != 0) {
                short line_y = _fields[field_idx].y + font_height + 1;
                line::Line_(cursor_x, line_y, cursor_x + char_width + 1, line_y, (uint8_t)cursor_color_ramp[0]);
            }
        }

        fonts::Set_Font_Style_(_fields[field_idx].font_style, _fields[field_idx].font_color);
        fonts::Print_(_fields[field_idx].x, _fields[field_idx].y, local_40);
        palette::Refresh_Palette_();
        platform::Toggle_Pages_Stub_();
        timer::Release_Time_(_auto_function_delay);
    }
}