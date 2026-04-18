#include "pch.h"

namespace special {

    /**
     * Calculates an approximation of the distance (range) between two points.
     * Uses the formula: max(abs(dx), abs(dy)) + min(abs(dx), abs(dy)) / 2.
     * @param x1 x1
     * @param y1 y1
     * @param x2 x2
     * @param y2 y2
     */
    int16_t __cdecl Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        int16_t dx = x2 - x1;
        int16_t dy = y2 - y1;

        if (dx < 0) {
            dx = -dx;
        }

        if (dy < 0) {
            dy = -dy;
        }

        if (dy < dx) {
            // Path for dx being the major axis
            return (int16_t)(dy / 2 + dx);
        }
        else {
            // Path for dy being the major axis or equal
            return (int16_t)(dx / 2 + dy);
        }
    }

    void __cdecl Interpolate_Line_(int16_t* x_ptr, int16_t* y_ptr, int16_t target_x, int16_t target_y, int16_t step) {
        if (step <= 0) {
            return;
        }

        int16_t current_x = *x_ptr;
        int16_t current_y = *y_ptr;

        // Exit if already at destination
        if (current_x == target_x && current_y == target_y) {
            return;
        }

        int32_t dx = (int32_t)target_x - (int32_t)current_x;
        int32_t dy = (int32_t)target_y - (int32_t)current_y;

        int32_t angle_dx = dx;
        int32_t angle_dy = dy;

        // Downscale coordinates if delta is too large to fit in Get_Angle_ precision
        if (abs(dx) > 255 || abs(dy) > 255) {
            angle_dx /= 2;
            angle_dy /= 2;
        }

        int16_t angle = angle::Get_Angle_(angle_dx, angle_dy);

        // Update X coordinate
        int16_t move_x = angle::Cos_(angle, step);
        int16_t new_x = current_x + move_x;
        *x_ptr = new_x;

        // Clamp to target X
        if (target_x > current_x) {
            if (new_x > target_x) {
                *x_ptr = target_x;
            }
        } else {
            if (new_x < target_x) {
                *x_ptr = target_x;
            }
        }

        // Update Y coordinate
        int16_t move_y = angle::Sin_(angle, step);
        int16_t new_y = current_y + move_y;
        *y_ptr = new_y;

        // Clamp to target Y
        if (target_y > current_y) {
            if (new_y > target_y) {
                *y_ptr = target_y;
            }
        } else {
            if (new_y < target_y) {
                *y_ptr = target_y;
            }
        }
    }

    void __cdecl Absolute_Interpolate_Line_(int16_t* x_ptr, int16_t* y_ptr, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t step) {
        if (step <= 0) {
            return;
        }

        int16_t curr_x = *x_ptr;
        if (curr_x == x2 && *y_ptr == y2) {
            return;
        }

        int16_t abs_dx = x2 - x1;
        if (abs_dx < 0) {
            abs_dx = -abs_dx;
        }

        int16_t abs_dy = y2 - y1;
        if (abs_dy < 0) {
            abs_dy = -abs_dy;
        }

        if (abs_dy < abs_dx) {
            if (x1 < x2) {
                int16_t new_x = curr_x + step;
                *x_ptr = new_x;
                if (new_x > x2) {
                    *x_ptr = x2;
                }
            } else {
                int16_t new_x = curr_x - step;
                *x_ptr = new_x;
                if (new_x < x2) {
                    *x_ptr = x2;
                }
            }
            *y_ptr = y1 + static_cast<int16_t>(
                static_cast<int32_t>(
                    static_cast<uint32_t>(*x_ptr - x1) * static_cast<uint32_t>(y2 - y1)
                ) / (x2 - x1)
            );
        } else {
            int16_t curr_y = *y_ptr;
            if (y1 < y2) {
                int16_t new_y = curr_y + step;
                *y_ptr = new_y;
                if (new_y > y2) {
                    *y_ptr = y2;
                }
            } else {
                int16_t new_y = curr_y - step;
                *y_ptr = new_y;
                if (new_y < y2) {
                    *y_ptr = y2;
                }
            }
            *x_ptr = x1 + static_cast<int16_t>(
                static_cast<int32_t>(
                    static_cast<uint32_t>(x2 - x1) * static_cast<uint32_t>(*y_ptr - y1)
                ) / (y2 - y1)
            );
        }
    }

}