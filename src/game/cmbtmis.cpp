#include "pch.h"

namespace CMBTMIS {
    /**
     * @address 004fd9c0
     */
    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t step, int16_t* out_x, int16_t* out_y) {
        int16_t dx = x2 - x1;
        int16_t dy = y2 - y1;

        // Calculate absolute values for dominance check
        int abs_dx = (dx < 0) ? -dx : dx;
        int abs_dy = (dy < 0) ? -dy : dy;

        if (abs_dx < abs_dy) {
            // Movement is dominated by Y-axis (vertical)
            int16_t sign_y = (dy < 0) ? -1 : 1;
            int16_t scaled_step_y = (int16_t)(sign_y * (int16_t)step);
            
            *out_y = scaled_step_y;
            
            // Calculate proportional X delta: (scaled_step_y * dx) / dy
            // Using int to preserve precision during intermediate multiplication
            *out_x = (int16_t)((int)scaled_step_y * (int)dx / (int)dy);
        } else {
            // Movement is dominated by X-axis (horizontal) or is exactly 45 degrees
            int16_t sign_x = (dx < 0) ? -1 : 1;
            int16_t scaled_step_x = (int16_t)(sign_x * (int16_t)step);
            
            *out_x = scaled_step_x;
            
            // Calculate proportional Y delta: (scaled_step_x * dy) / dx
            // Division by zero risk if dx is 0, but logic implies abs_dx >= abs_dy
            if (dx != 0) {
                *out_y = (int16_t)((int)scaled_step_x * (int)dy / (int)dx);
            } else {
                *out_y = 0;
            }
        }
    }
}