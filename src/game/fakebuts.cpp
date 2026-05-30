#include "pch.h"

namespace FAKEBUTS {
    uint8_t _n_fake_buttons;

    s_fake_button _fake_buttons[25];

    uint8_t _n_click_thru_fields;

    s_click_through_field _click_thru_fields[25];

    void __cdecl Clear_Click_Thru_Fields_() {
        memset(_click_thru_fields, 0, sizeof(_click_thru_fields));
        _n_click_thru_fields = 0;
    }

    void __cdecl Clear_Fake_Buttons_() {
        memset(_fake_buttons, 0, sizeof(_fake_buttons));
        _n_fake_buttons = 0;
    }

    void __cdecl Evaluate_Click_Thru_Fields_(int16_t field_id) {
        if (_n_click_thru_fields != 0) {
            for (int16_t i = 0; i < _n_click_thru_fields; ++i) {
                if (_click_thru_fields[i].field_id1 == field_id ||
                    _click_thru_fields[i].field_id2 == field_id) {
                    *_click_thru_fields[i].value =
                        (*_click_thru_fields[i].value + 1) % _click_thru_fields[i].state_count;
                }
            }
        }
    }

    void __cdecl Draw_Click_Thru_Fields_() {
        if (_n_click_thru_fields != 0) {
            for (int16_t i = 0; i < _n_click_thru_fields; ++i) {
                s_click_through_field* field = &_click_thru_fields[i];

                if (field->color != 0) {
                    graphics::Box_(
                        field->x1,
                        field->y1,
                        field->x2 - field->x1,
                        field->y2 - field->y1,
                        1,
                        field->color
                    );
                }

                graphics::Set_Window_(field->x1, field->y1, field->x2, field->y2);
                video::Clip_On_();

                BILL::Squeeze_Paragraph_Centered_(
                    field->x1,
                    field->y1 + 2,
                    field->x2 - field->x1,
                    field->y2 - field->y1 - 2,
                    field->strings[*field->value],
                    2
                );

                video::Clip_Off_();
            }
        }
    }

    int16_t __cdecl Add_Click_Thru_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, char **data, int16_t count, int16_t *var, uint8_t color, const char* str, int16_t unk2, int16_t hidden_id) {
        int16_t field_id = fields::Add_Hidden_Field_(x1, y1, x2, y2, str, (int8_t)unk2);
        Add_Click_Thru_Field_To_List_(x1, y1, x2, y2, field_id, data, count, var, color, hidden_id);
        return field_id;
    }

    void __cdecl Add_Click_Thru_Field_To_List_(
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, 
        int16_t field_id1, char **strings, int16_t state_count, 
        int16_t *value, uint8_t color, int16_t field_id2)
    {
        uint8_t idx = _n_click_thru_fields;
        
        _click_thru_fields[idx].x1 = x1;
        _click_thru_fields[idx].y1 = y1;
        _click_thru_fields[idx].x2 = x2;
        _click_thru_fields[idx].y2 = y2;
        _click_thru_fields[idx].field_id1 = field_id1;
        _click_thru_fields[idx].strings = strings;

        _n_click_thru_fields = idx + 1;

        _click_thru_fields[idx].state_count = state_count;
        _click_thru_fields[idx].value = value;
        _click_thru_fields[idx].color = color;
        _click_thru_fields[idx].field_id2 = field_id2;
    }

    void __cdecl Draw_Fake_Buttons_() {
        int16_t auto_input = fields::Auto_Input_();

        for (int16_t i = 0; i < _n_fake_buttons; i++) {
            if (_fake_buttons[i].field_id == auto_input) {
                if (animate::Get_Animation_Frame_Count_(_fake_buttons[i].pic) <= 0) {
                    continue;
                }
                animate::Set_Animation_Frame_(_fake_buttons[i].pic, 1);
            } else {
                animate::Reset_Animation_Frame_(_fake_buttons[i].pic);
            }
            animate::Draw_(_fake_buttons[i].x, _fake_buttons[i].y, _fake_buttons[i].pic);
        }
    }
}