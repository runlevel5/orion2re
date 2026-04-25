#pragma once
#include <cstdint>

struct s_field;

#pragma pack(push, 1)
struct s_fieldsav_data {
    int16_t field_count;
    uint16_t input_delay;
    int16_t down_mouse_button;
    int16_t auto_input_variable;
    uint16_t input_field_active;
    int16_t active_input_field_number;
    s_field* fields;
    int16_t max_fields_count;
};
#pragma pack(pop)

namespace FIELDSAV {
    void __cdecl Save_Field_Stats_(s_fieldsav_data* data);
    void __cdecl Restore_Field_Stats_(s_fieldsav_data* data);
}