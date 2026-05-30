#pragma once
#include <cstdint>

namespace FAKEBUTS {
    extern uint8_t _n_fake_buttons;

    extern s_fake_button _fake_buttons[25];

    void __cdecl Clear_Fake_Buttons_();
    void __cdecl Draw_Fake_Buttons_();

    extern uint8_t _n_click_thru_fields;

    extern s_click_through_field _click_thru_fields[25];

    void __cdecl Clear_Click_Thru_Fields_();
    void __cdecl Evaluate_Click_Thru_Fields_(int16_t field_id);

    void __cdecl Draw_Click_Thru_Fields_();

    void __cdecl Add_Click_Thru_Field_To_List_(
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, 
        int16_t field_id1, char **strings, int16_t state_count, 
        int16_t *value, uint8_t color, int16_t field_id2);

    int16_t __cdecl Add_Click_Thru_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, char **data, int16_t count, int16_t *var, uint8_t color, const char* str, int16_t unk2, int16_t hidden_id);
}