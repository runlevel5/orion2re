#include "pch.h"

namespace angle {

    int16_t __cdecl Get_Angle_(int32_t dx, int32_t dy) {
        if (dx == 0) {
            if (dy < 0) return 270;
            if (dy > 0) return 90;
            return 0;
        }

        if (dy == 0) {
            if (dx < 0) return 180;
            return 0;
        }

        uint32_t u, v;
        int16_t offset;

        if (dx < 0) {
            if (dy < 0) {
                u = 0u - (uint32_t)dx;
                v = 0u - (uint32_t)dy;
                offset = 180;
            } else {
                u = dy;
                v = 0u - (uint32_t)dx;
                offset = 90;
            }
        } else {
            if (dy > 0) {
                u = dx;
                v = dy;
                offset = 0;
            } else {
                u = 0u - (uint32_t)dy;
                v = dx;
                offset = 270;
            }
        }

        int16_t angle;
        if (v == u) {
            angle = 45;
        } else if (v > u) {
            angle = 90 - _atan_lookup_table[(u << 7) / v];
        } else {
            angle = _atan_lookup_table[(v << 7) / u];
        }

        angle += offset;
        if (angle == 360) {
            angle = 0;
        }

        return angle;
    }

    int32_t __cdecl Cos_(int16_t angle, int32_t magnitude) {
        return Sin_(angle + 90, magnitude);
    }

    int32_t __cdecl Sin_(int16_t angle, int32_t magnitude) {
        int32_t angle_deg = angle;

        if (magnitude < 0) {
            if (magnitude != INT32_MIN) {
                magnitude = -magnitude;
            }

            uint16_t angle_bits = (uint16_t)angle;
            angle_bits = (uint16_t)(angle_bits + 180u);
            if (angle_bits < 0x8000u) {
                angle_deg = angle_bits;
            } else {
                angle_deg = (int32_t)angle_bits - 0x10000;
            }
        }

        if (angle_deg >= 360) {
            do {
                angle_deg -= 360;
            } while (angle_deg >= 360);
        } else {
            while (angle_deg < 0) {
                angle_deg += 360;
            }
        }

        if (angle_deg < 180) {
            if (angle_deg < 90) {
                return static_cast<int32_t>((static_cast<uint32_t>(static_cast<uint16_t>(_sin_table[angle_deg])) * static_cast<uint32_t>(magnitude) + 0x8000) >> 16);
            }
            if (angle_deg == 90) {
                return magnitude;
            }
            return static_cast<int32_t>((static_cast<uint32_t>(static_cast<uint16_t>(_sin_table[180 - angle_deg])) * static_cast<uint32_t>(magnitude) + 0x8000) >> 16);
        }

        if (angle_deg < 270) {
            return -static_cast<int32_t>((static_cast<uint32_t>(static_cast<uint16_t>(_sin_table[angle_deg - 180])) * static_cast<uint32_t>(magnitude) + 0x8000) >> 16);
        }
        if (angle_deg == 270) {
            if (magnitude == INT32_MIN) {
                return INT32_MIN;
            }
            return -magnitude;
        }

        return -static_cast<int32_t>((static_cast<uint32_t>(static_cast<uint16_t>(_sin_table[360 - angle_deg])) * static_cast<uint32_t>(magnitude) + 0x8000) >> 16);
    }

    void __cdecl Rotate_Coordinates_(int16_t angle_deg, int32_t* x_ptr, int32_t* y_ptr) {
        const auto mul_lo32 = [](uint16_t a, int32_t b) {
            return (int32_t)((uint32_t)a * (uint32_t)b);
        };
        const auto neg_lo32 = [](int32_t value) {
            return (int32_t)(0u - (uint32_t)value);
        };
        const auto add_lo32 = [](int32_t a, int32_t b) {
            return (int32_t)((uint32_t)a + (uint32_t)b);
        };
        const auto sub_lo32 = [](int32_t a, int32_t b) {
            return (int32_t)((uint32_t)a - (uint32_t)b);
        };
        const auto round_shift_16 = [&](int32_t value) {
            if (value < 0) {
                return neg_lo32((int32_t)((0x8000u - (uint32_t)value) >> 16));
            }
            return (int32_t)(((uint32_t)value + 0x8000u) >> 16);
        };

        bool skip_calc = false;

        if (angle_deg >= 360) {
            do {
                angle_deg = (int16_t)(angle_deg - 360);
            } while (angle_deg >= 360);
        } else if (angle_deg < 0) {
            do {
                angle_deg = (int16_t)(angle_deg + 360);
            } while (angle_deg < 0);
        }

        int32_t x = *x_ptr;
        int32_t y = *y_ptr;
        int32_t y_sin = 0;
        int32_t x_cos = 0;
        int32_t x_sin = 0;
        int32_t y_cos = 0;

        if (angle_deg < 180) {
            if (angle_deg < 90) {
                if (angle_deg == 0) {
                    skip_calc = true;
                } else {
                    uint16_t sin_val = _sin_table[angle_deg];
                    uint16_t cos_val = _sin_table[90 - angle_deg];

                    y_sin = mul_lo32(sin_val, y);
                    x_cos = mul_lo32(cos_val, x);
                    x_sin = mul_lo32(sin_val, x);
                    y_cos = mul_lo32(cos_val, y);
                }
            } else if (angle_deg == 90) {
                *y_ptr = x;
                *x_ptr = neg_lo32(y);
                skip_calc = true;
            } else {
                uint16_t sin_val = _sin_table[180 - angle_deg];
                uint16_t cos_val_abs = _sin_table[angle_deg - 90];

                y_sin = mul_lo32(sin_val, y);
                x_cos = neg_lo32(mul_lo32(cos_val_abs, x));
                x_sin = mul_lo32(sin_val, x);
                y_cos = neg_lo32(mul_lo32(cos_val_abs, y));
            }
        } else if (angle_deg < 270) {
            if (angle_deg == 180) {
                *x_ptr = neg_lo32(x);
                *y_ptr = neg_lo32(y);
                skip_calc = true;
            } else {
                uint16_t sin_val_abs = _sin_table[angle_deg - 180];
                uint16_t cos_val_abs = _sin_table[270 - angle_deg];

                y_sin = neg_lo32(mul_lo32(sin_val_abs, y));
                x_cos = neg_lo32(mul_lo32(cos_val_abs, x));
                x_sin = neg_lo32(mul_lo32(sin_val_abs, x));
                y_cos = neg_lo32(mul_lo32(cos_val_abs, y));
            }
        } else if (angle_deg == 270) {
            *y_ptr = neg_lo32(x);
            *x_ptr = y;
            skip_calc = true;
        } else {
            uint16_t sin_val_abs = _sin_table[360 - angle_deg];
            uint16_t cos_val = _sin_table[angle_deg - 270];

            y_sin = neg_lo32(mul_lo32(sin_val_abs, y));
            x_cos = mul_lo32(cos_val, x);
            x_sin = neg_lo32(mul_lo32(sin_val_abs, x));
            y_cos = mul_lo32(cos_val, y);
        }

        if (!skip_calc) {
            int32_t x_prime = sub_lo32(x_cos, y_sin);
            int32_t y_prime = add_lo32(x_sin, y_cos);

            *x_ptr = round_shift_16(x_prime);
            *y_ptr = round_shift_16(y_prime);
        }
    }

}