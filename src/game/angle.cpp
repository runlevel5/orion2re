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

    void __cdecl Rotate_Coordinates_(int32_t angle_deg, int32_t* x_ptr, int32_t* y_ptr) {
        bool skip_calc = false;

        if (angle_deg >= 360) {
            do {
                angle_deg -= 360;
            } while (angle_deg >= 360);
        } else if (angle_deg < 0) {
            do {
                angle_deg += 360;
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
                    int32_t sin_val = _sin_table[angle_deg];
                    int32_t cos_val = _sin_table[90 - angle_deg];

                    y_sin = y * sin_val;
                    x_cos = x * cos_val;
                    x_sin = x * sin_val;
                    y_cos = y * cos_val;
                }
            } else if (angle_deg == 90) {
                *y_ptr = x;
                *x_ptr = -y;
                skip_calc = true;
            } else {
                int32_t sin_val = _sin_table[180 - angle_deg];
                int32_t cos_val_abs = _sin_table[angle_deg - 90];

                y_sin = y * sin_val;
                x_cos = -(x * cos_val_abs);
                x_sin = x * sin_val;
                y_cos = -(y * cos_val_abs);
            }
        } else if (angle_deg < 270) {
            if (angle_deg == 180) {
                *x_ptr = -x;
                *y_ptr = -y;
                skip_calc = true;
            } else {
                int32_t sin_val_abs = _sin_table[angle_deg - 180];
                int32_t cos_val_abs = _sin_table[270 - angle_deg];

                y_sin = -(y * sin_val_abs);
                x_cos = -(x * cos_val_abs);
                x_sin = -(x * sin_val_abs);
                y_cos = -(y * cos_val_abs);
            }
        } else if (angle_deg == 270) {
            *y_ptr = -x;
            *x_ptr = y;
            skip_calc = true;
        } else {
            int32_t sin_val_abs = _sin_table[360 - angle_deg];
            int32_t cos_val = _sin_table[angle_deg - 270];

            y_sin = -(y * sin_val_abs);
            x_cos = x * cos_val;
            x_sin = -(x * sin_val_abs);
            y_cos = y * cos_val;
        }

        if (!skip_calc) {
            int32_t x_prime = x_cos - y_sin;
            int32_t y_prime = x_sin + y_cos;

            if (x_prime < 0) {
                *x_ptr = -static_cast<int32_t>((0x8000 - static_cast<uint32_t>(x_prime)) >> 16);
            } else {
                *x_ptr = static_cast<int32_t>((static_cast<uint32_t>(x_prime) + 0x8000) >> 16);
            }

            if (y_prime < 0) {
                *y_ptr = -static_cast<int32_t>((0x8000 - static_cast<uint32_t>(y_prime)) >> 16);
            } else {
                *y_ptr = static_cast<int32_t>((static_cast<uint32_t>(y_prime) + 0x8000) >> 16);
            }
        }
    }

}