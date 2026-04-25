#include "pch.h"

namespace FIELDSAV {
    void __cdecl Save_Field_Stats_(s_fieldsav_data* data) {
        data->field_count = fields::_fields_count;
        data->input_delay = fields::_input_delay;
        data->down_mouse_button = fields::_down_mouse_button;
        data->auto_input_variable = fields::_auto_input_variable;
        data->input_field_active = fields::_input_field_active;
        data->active_input_field_number = fields::_active_input_field_number;
        data->max_fields_count = fields::_max_fields_count;
        data->fields = fields::_fields;

        fields::_fields += fields::_fields_count;
        fields::_max_fields_count -= fields::_fields_count;

        fields::Clear_Fields_();
    }

    void __cdecl Restore_Field_Stats_(s_fieldsav_data* data) {
        fields::_fields_count = data->field_count;
        fields::_input_delay = data->input_delay;
        fields::_down_mouse_button = data->down_mouse_button;
        fields::_auto_input_variable = data->auto_input_variable;
        fields::_input_field_active = data->input_field_active;
        fields::_active_input_field_number = data->active_input_field_number;
        fields::_fields = data->fields;
        fields::_max_fields_count = data->max_fields_count;
    }
}