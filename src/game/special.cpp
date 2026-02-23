#include "deps.h"
#include <cstdlib>

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

}