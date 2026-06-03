#pragma once
#include <cstdint>

namespace FAKEBUTS {
    extern uint8_t _n_fake_buttons;

    extern s_fake_button _fake_buttons[25];

    // Resets the registered fake-button list.
    void __cdecl Clear_Fake_Buttons_();
    // Draws all registered fake buttons, highlighting the current auto-input field.
    void __cdecl Draw_Fake_Buttons_();

    extern uint8_t _n_click_thru_fields;

    extern s_click_through_field _click_thru_fields[25];

    // Clears all registered click-through fields.
    void __cdecl Clear_Click_Thru_Fields_();
    // Advances the selected state for click-through fields triggered by the given field ID.
    void __cdecl Evaluate_Click_Thru_Fields_(int16_t activated_field_id);

    // Draws registered click-through fields using their current string state.
    void __cdecl Draw_Click_Thru_Fields_();

    // Adds a click-through field entry to the internal registry.
    void __cdecl Add_Click_Thru_Field_To_List_(
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, 
        int16_t primary_field_id, char **strings, int16_t state_count, 
        int16_t *value, uint8_t color, int16_t secondary_field_id);

    // Creates a hidden field and registers a matching click-through field entry.
    int16_t __cdecl Add_Click_Thru_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, char **strings, int16_t state_count, int16_t *value, uint8_t color, const char* help_text, int16_t hidden_field_style, int16_t secondary_field_id);
}