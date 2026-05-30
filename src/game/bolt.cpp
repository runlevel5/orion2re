#include "pch.h"

namespace bolt {
    int16_t _head_wide_bolts[192] = {
        0, -1, 0, 1, -1, -1, -1, 1, -1, -2, -1, 2,
        -1, -1, 0, 1, -1, 0, -1, 1, -2, -1, -1, 2,
        -1, 0, 1, 0, -1, 0, 0, 1, 1, 1, -1, -1,
        -1, 0, 1, 1, -1, 1, 0, 1, -2, 1, 1, 2,
        -1, 0, 0, 1, -1, 1, 1, 1, -2, 1, 2, 1,
        1, 0, -1, 1, 1, 1, 0, 1, 2, 1, -1, 2,
        1, 0, -1, 0, 1, 0, 0, 1, -1, 1, 1, -1,
        1, -1, 0, 1, 1, 0, 1, 1, 2, -1, 1, 2,
        0, -1, 0, 1, 1, -1, 1, 1, 1, -2, 1, 2,
        1, 1, 0, -1, 1, 0, 1, -1, 2, 1, 1, -2,
        1, 0, -1, 0, 1, 0, 0, -1, -1, -1, 1, 1,
        1, 0, -1, -1, 1, -1, 0, -1, 2, -1, -1, -2,
        1, 0, -1, -1, -1, -1, 1, -1, 2, -1, -1, -2,
        -1, 0, 1, -1, -1, -1, 0, -1, -2, -1, 1, -2,
        -1, 0, 1, 0, -1, 0, 0, -1, 1, -1, -1, 1,
        -1, 1, 0, -1, -1, 0, -1, -1, -2, 1, -1, -2,
    };

    int16_t _tail_wide_bolts[192] = {
        0, 1, 0, -1, 1, -1, 1, 1, 1, 2, 1, -2,
        0, -1, 1, 1, 1, 0, 1, -1, 1, -2, 2, 1,
        -1, 0, 1, 0, 1, 0, 0, -1, 1, 1, -1, -1,
        -1, -1, 1, 0, 1, -1, 0, -1, -1, -2, 2, -1,
        -1, -1, 1, 0, -1, -1, 1, -1, -1, -2, 2, -1,
        1, -1, -1, 0, -1, -1, 0, -1, 1, -2, -2, -1,
        1, 0, -1, 0, -1, 0, 0, -1, -1, 1, 1, -1,
        0, -1, -1, 1, -1, 0, -1, -1, -1, -2, -2, 1,
        0, 1, 0, -1, -1, -1, -1, 1, -1, 2, -1, -2,
        0, 1, -1, -1, -1, 0, -1, 1, -1, 2, -2, -1,
        1, 0, -1, 0, -1, 0, 0, 1, -1, -1, 1, 1,
        1, 1, -1, 0, -1, 1, 0, 1, 1, 2, -2, 1,
        0, 1, -1, 0, -1, 1, 1, 1, 2, 1, -2, 1,
        -1, 1, 1, 0, 1, 1, 0, 1, -1, 2, 2, 1,
        -1, 0, 1, 0, 1, 0, 0, 1, 1, -1, -1, 1,
        0, 1, 1, -1, 1, 0, 1, 1, 1, 2, 2, -1,
    };

    void __cdecl Wide_Bolt_Line_(
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t param_5, int16_t param_6,
        int16_t param_7, int16_t param_8, uint16_t param_9, int16_t param_10, int16_t param_11, int16_t param_12,
        int16_t param_13, int16_t param_14, int16_t param_15, int16_t param_16, int16_t param_17, int16_t param_18,
        int16_t param_19, int16_t param_20, int16_t param_21)
    {
        int16_t angle = angle::Get_Angle_(x2 - x1, y2 - y1);
        int16_t dir_idx = (((371 - angle) % 360) * 2) / 45;

        int16_t x1_1, y1_1, x2_1, y2_1;
        int16_t x1_2, y1_2, x2_2, y2_2;

        if (param_11 == 1) {
            x1_1 = x1;
            y1_1 = y1;
            x2_1 = _head_wide_bolts[dir_idx * 12 + 8];
            y2_1 = _head_wide_bolts[dir_idx * 12 + 9];

            x1_2 = x1;
            y1_2 = y1;
            x2_2 = _head_wide_bolts[dir_idx * 12 + 10];
            y2_2 = _head_wide_bolts[dir_idx * 12 + 11];
        } else {
            x1_1 = _tail_wide_bolts[dir_idx * 12 + 8] + x1;
            y1_1 = _tail_wide_bolts[dir_idx * 12 + 9] + y1;
            x2_1 = _head_wide_bolts[dir_idx * 12 + 8];
            y2_1 = _head_wide_bolts[dir_idx * 12 + 9];

            x1_2 = _tail_wide_bolts[dir_idx * 12 + 10] + x1;
            y1_2 = _tail_wide_bolts[dir_idx * 12 + 11] + y1;
            x2_2 = _head_wide_bolts[dir_idx * 12 + 10];
            y2_2 = _head_wide_bolts[dir_idx * 12 + 11];
        }

        int16_t v6 = param_5 + param_6 - 1;
        int16_t v4 = param_8 / 2;

        Bolt_Segment_(x1_1, y1_1, x2_1 + x2, y2_1 + y2, param_5 + param_21, param_20 - param_21, param_7, v4, param_5, v6, 0, param_14, param_9, 0);
        Bolt_Segment_(x1_2, y1_2, x2_2 + x2, y2_2 + y2, param_5 + param_21, param_20 - param_21, param_7, v4, param_5, v6, 0, param_14, param_9, 0);

        int16_t x1_3, y1_3, x2_3, y2_3;
        int16_t x1_4, y1_4, x2_4, y2_4;
        int16_t x1_5, y1_5, x2_5, y2_5;
        int16_t x1_6, y1_6, x2_6, y2_6;

        if (param_11 == 1) {
            x1_3 = x1;
            y1_3 = y1;
            x2_3 = _head_wide_bolts[dir_idx * 12 + 0];
            y2_3 = _head_wide_bolts[dir_idx * 12 + 1];

            x1_4 = x1;
            y1_4 = y1;
            x2_4 = _head_wide_bolts[dir_idx * 12 + 2];
            y2_4 = _head_wide_bolts[dir_idx * 12 + 3];

            x1_5 = x1;
            y1_5 = y1;
            x2_5 = _head_wide_bolts[dir_idx * 12 + 4];
            y2_5 = _head_wide_bolts[dir_idx * 12 + 5];

            x1_6 = x1;
            y1_6 = y1;
            x2_6 = _head_wide_bolts[dir_idx * 12 + 6];
            y2_6 = _head_wide_bolts[dir_idx * 12 + 7];
        } else {
            x1_3 = _tail_wide_bolts[dir_idx * 12 + 0] + x1;
            y1_3 = _tail_wide_bolts[dir_idx * 12 + 1] + y1;
            x2_3 = _head_wide_bolts[dir_idx * 12 + 0];
            y2_3 = _head_wide_bolts[dir_idx * 12 + 1];

            x1_4 = _tail_wide_bolts[dir_idx * 12 + 2] + x1;
            y1_4 = _tail_wide_bolts[dir_idx * 12 + 3] + y1;
            x2_4 = _head_wide_bolts[dir_idx * 12 + 2];
            y2_4 = _head_wide_bolts[dir_idx * 12 + 3];

            x1_5 = _tail_wide_bolts[dir_idx * 12 + 4] + x1;
            y1_5 = _tail_wide_bolts[dir_idx * 12 + 5] + y1;
            x2_5 = _head_wide_bolts[dir_idx * 12 + 4];
            y2_5 = _head_wide_bolts[dir_idx * 12 + 5];

            x1_6 = _tail_wide_bolts[dir_idx * 12 + 6] + x1;
            y1_6 = _tail_wide_bolts[dir_idx * 12 + 7] + y1;
            x2_6 = _head_wide_bolts[dir_idx * 12 + 6];
            y2_6 = _head_wide_bolts[dir_idx * 12 + 7];
        }

        int16_t v8 = param_19 + param_5;
        int16_t v9 = param_18 - param_19;

        Bolt_Segment_(x1_3, y1_3, x2_3 + x2, y2_3 + y2, v8, v9, param_7, v4, param_5, v6, 0, param_14, param_9, param_12);
        Bolt_Segment_(x1_4, y1_4, x2_4 + x2, y2_4 + y2, v8, v9, param_7, v4, param_5, v6, 0, param_14, param_9, param_12);
        Bolt_Segment_(x1_5, y1_5, x2_5 + x2, y2_5 + y2, v8, v9, param_7, v4, param_5, v6, 0, param_14, param_9, param_12);
        Bolt_Segment_(x1_6, y1_6, x2_6 + x2, y2_6 + y2, v8, v9, param_7, v4, param_5, v6, 0, param_14, param_9, param_12);

        int16_t v3 = param_17 + param_5;
        int16_t v7 = param_16 - param_17;

        if (param_13 == 0) {
            v3 += (v7 * 3) / 4;
            v7 = 0;
        }

        Bolt_Segment_(x1, y1, x2, y2, v3, v7, param_7, param_8, param_5, v6, param_13, param_14, param_9, param_12);
    }


    void __cdecl Bolt_Segment_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color_base,
                               int16_t color_range, int16_t rand_chance, int16_t rand_range, int16_t min_color, int16_t max_color,
                               int16_t unused_11, int16_t do_sparkles, uint16_t sparkle_offset, int16_t tip_fade)
    {
        int32_t rand_range2 = rand_range * 2 + 1;
        int32_t dx = x2 - x1;
        int32_t dy = y2 - y1;

        if (dx < 0) {
            dx = -dx;
        }
        if (dy < 0) {
            dy = -dy;
        }

        int32_t max_d = dy;
        if (max_d < dx) {
            max_d = dx;
        }

        int32_t len = max_d + 1;
        int32_t step_val = ((int32_t)color_range << 16) / len;
        int32_t color_accum = 32000 / len;
        int32_t rand_val = 0;

        if (y2 < y1) {
            if (len >= 0) {
                for (int i = 0; i <= len; ++i) {
                    uint8_t c = (uint8_t)((color_accum >> 16) + rand_val + color_base);
                    replace::_graphics_buffer[len - i + 1] = c;
                    color_accum += step_val;

                    if ((int16_t)random::Random_(100) < rand_chance) {
                        rand_val = random::Random_(rand_range2) - rand_range;
                    } else {
                        rand_val = 0;
                    }
                }
            }
            replace::_graphics_buffer[len] = (uint8_t)color_base;
            replace::_graphics_buffer[0] = (uint8_t)(color_base + color_range - 1);
        } else {
            if (len >= 0) {
                for (int i = 0; i <= len; ++i) {
                    uint8_t c = (uint8_t)((color_accum >> 16) + rand_val + color_base);
                    replace::_graphics_buffer[i + 1] = c;
                    color_accum += step_val;

                    if ((int16_t)random::Random_(100) < rand_chance) {
                        rand_val = random::Random_(rand_range2) - rand_range;
                    } else {
                        rand_val = 0;
                    }
                }
            }
            replace::_graphics_buffer[0] = (uint8_t)color_base;
            replace::_graphics_buffer[len] = (uint8_t)(color_base + color_range - 1);
        }

        if (do_sparkles == 1) {
            int32_t i = sparkle_offset % 8;
            int32_t end_i = len + 2;
            if (i < end_i) {
                do {
                    replace::_graphics_buffer[i] += 3;
                    i += 8;
                } while (i < end_i);
            }
        }

        int32_t buf_len = len + 2;
        if (buf_len > 0) {
            for (int i = 0; i < buf_len; ++i) {
                if (replace::_graphics_buffer[i] < min_color) {
                    replace::_graphics_buffer[i] = (uint8_t)min_color;
                }
                if (replace::_graphics_buffer[i] > max_color) {
                    replace::_graphics_buffer[i] = (uint8_t)max_color;
                }
            }
        }

        if (tip_fade != 0) {
            int32_t fade_val = 3 / tip_fade;
            if (y2 < y1) {
                for (int i = 0; i <= 5; ++i) {
                    replace::_graphics_buffer[i + 1] += fade_val;
                }
            } else {
                if (len > 5) {
                    for (int i = 0; i <= 5; ++i) {
                        replace::_graphics_buffer[len - i - 1] += fade_val;
                    }
                }
            }
        }

        if (buf_len > 0) {
            for (int i = 0; i < buf_len; ++i) {
                if (replace::_graphics_buffer[i] > color_base + 15) {
                    replace::_graphics_buffer[i] = (uint8_t)(color_base + 15);
                }
            }
        }

        line::Plasma_Line_(x1, y1, x2, y2, replace::_graphics_buffer, 0);
    }

    void __cdecl Bolt_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t p5,
        int16_t p6, int16_t p7, int16_t p8, uint16_t p9, int16_t draw_caps,
        int16_t unused_11, int16_t p12, int16_t p13, int16_t p14, int16_t p15,
        int16_t color_base, int16_t p17, int16_t p18, int16_t p19) {

        if (draw_caps == 1) {
            uint8_t c_cross = static_cast<uint8_t>(p5 + 1);
            uint8_t c_diag = static_cast<uint8_t>(p5);
            if (p12 == 1) {
                c_cross = static_cast<uint8_t>(p5 + 4);
                c_diag = static_cast<uint8_t>(p5 + 1);
            }

            uint8_t c_center = static_cast<uint8_t>(color_base + p5);

            graphics::Dot_(x2, y2, c_center);
            graphics::Dot_(x2, y2 - 1, c_cross);
            graphics::Dot_(x2, y2 + 1, c_cross);
            graphics::Dot_(x2 - 1, y2, c_cross);
            graphics::Dot_(x2 + 1, y2, c_cross);

            graphics::Dot_(x2 - 1, y2 - 1, c_diag);
            graphics::Dot_(x2 - 1, y2 + 1, c_diag);
            graphics::Dot_(x2 + 1, y2 - 1, c_diag);
            graphics::Dot_(x2 + 1, y2 + 1, c_diag);

            if (p13 == 0) {
                graphics::Dot_(x1, y1, c_center);
                graphics::Dot_(x1, y1 - 1, c_cross);
                graphics::Dot_(x1, y1 + 1, c_cross);
                graphics::Dot_(x1 - 1, y1, c_cross);
                graphics::Dot_(x1 + 1, y1, c_cross);

                graphics::Dot_(x1 - 1, y1 - 1, c_diag);
                graphics::Dot_(x1 - 1, y1 + 1, c_diag);
                graphics::Dot_(x1 + 1, y1 - 1, c_diag);
                graphics::Dot_(x1 + 1, y1 + 1, c_diag);
            }
        }

        int16_t x_off = 0;
        int16_t y_off = 0;
        if (abs(y2 - y1) >= abs(x2 - x1)) {
            x_off = 1;
        }
        else {
            y_off = 1;
        }

        int16_t arg6_val = p18 - p19;
        if (p12 == 1) {
            arg6_val += 3;
        }

        if (p15 == 0) {
            bolt::Bolt_Segment_(
                x1 - x_off, y1 - y_off,
                x2 - x_off, y2 - y_off,
                p19 + p5, arg6_val,
                p7, p8 / 2, p5, p6 + p5 - 1,
                0, p14, p9, 0
            );

            bolt::Bolt_Segment_(
                x1 + x_off, y1 + y_off,
                x2 + x_off, y2 + y_off,
                p19 + p5, arg6_val,
                p7, p8 / 2, p5, p6 + p5 - 1,
                0, p14, p9, 0
            );
        }

        int16_t arg6_main = color_base - p17;
        int16_t arg5_main = p17 + p5;

        if (p13 == 0) {
            int32_t t = (int32_t)arg6_main * 3;
            arg5_main += (int16_t)((t + ((t >> 31) & 3)) >> 2);
            arg6_main = 0;
        }

        bolt::Bolt_Segment_(
            x1, y1, x2, y2,
            arg5_main, arg6_main,
            p7, p8, p5, p6 + p5 - 1,
            p13, p14, p9, p12
        );
    }
}