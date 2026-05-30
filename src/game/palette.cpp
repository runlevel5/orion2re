#include "pch.h"

namespace palette {
    void __cdecl Save_Palette_() {
        struct_::Copy_Structure_Aligned_(_saved_palette, video::_current_palette, 0x400);
    }

    void __cdecl Set_Palette_Changes_(int16_t start_index, int16_t end_index) {
        int32_t start = start_index;
        int32_t end = end_index;

        if (start <= end) {
            // Pointer to the first palette status entry to modify
            s_palette_entry* entry = &video::_current_palette[start];
            
            // Calculate number of entries to mark (inclusive)
            int32_t count = (end - start) + 1;
            
            do {
                // Set the changed flag (first byte of the 4-byte Palette_Entry)
                entry->changed = 1;
                
                // Move to the next 4-byte entry
                entry++;
                count--;
            } while (count != 0);
        }
    }

    void __cdecl Clear_Palette_Changes_(int16_t start, int16_t end) {
        if (start <= end) {
            // Calculate how many entries to clear
            int32_t count = end - start + 1;
            s_palette_entry* entry = &video::_current_palette[start];

            do {
                // Clear only the 'changed' status byte (offset 0 in the 4-byte record)
                entry->changed = 0;
                entry++;
                count--;
            } while (count != 0);
        }
    }

    void __cdecl Reset_Cycle_Palette_Color_() {
        _cycle_direction_flag = -1;
    }

    void __cdecl Restore_Palette_() {
        struct_::Copy_Structure_Aligned_(video::_current_palette, _saved_palette, 0x400);
        Set_Palette_Changes_(0, 0xFF);
    }

    void __cdecl Set_Palette_Gradient_(int32_t start_index, int32_t count, int32_t start_r, int32_t start_g, int32_t start_b, int32_t end_r, int32_t end_g, int32_t end_b) {
        if (count > 0) {
            int32_t current_r_diff = 0;
            int32_t current_g_diff = 0;
            int32_t current_b_diff = 0;

            int32_t count_minus_1 = count - 1;

            int32_t r_diff = end_r - start_r;
            int32_t g_diff = end_g - start_g;
            int32_t b_diff = end_b - start_b;

            s_palette_entry* entry = &video::_current_palette[start_index];
            do {
                entry->changed = 1;

                entry->r = start_r + (current_r_diff / count_minus_1);
                entry->g = start_g + (current_g_diff / count_minus_1);
                entry->b = start_b + (current_b_diff / count_minus_1);

                current_r_diff += r_diff;
                current_g_diff += g_diff;
                current_b_diff += b_diff;

                entry++;
                count--;
            } while (count != 0);
        }
    }

    void __cdecl Refresh_Palette_() {
        const int32_t palette_entry_count =
            (int32_t)(sizeof(video::_current_palette) / sizeof(video::_current_palette[0]));
        int32_t i = 0;
        s_palette_entry* const palette_end = video::_current_palette + palette_entry_count;

        while (i < palette_entry_count) {
            s_palette_entry* entry = &video::_current_palette[i];
            while (entry < palette_end && entry->changed == 0) {
                ++i;
                ++entry;
            }

            if (entry >= palette_end) {
                break;
            }

            int32_t count = 1;
            s_palette_entry* run = entry + 1;
            while (run < palette_end && run->changed != 0) {
                ++count;
                ++run;
            }

            platform::g_direct_draw->WaitForVerticalBlank(1, nullptr);
            
            palstore::Store_Palette_Block_(i, count);
            
            i += count;
        }
        
        Clear_Palette_Changes_(0, 255);
    }

    void __cdecl Update_Cycle_(int16_t* bound1, int16_t* bound2) {
        if (_cycle_direction_flag == 0) {
            if (*bound1 < *bound2) {
                _cycle_color_value += _cycle_step_value;
                if (*bound2 < _cycle_color_value) {
                    _cycle_direction_flag = 1;
                    _cycle_color_value = *bound2 - 1;
                }
            } else {
                _cycle_color_value -= _cycle_step_value;
                if (_cycle_color_value < *bound1) {
                    _cycle_direction_flag = 1;
                    _cycle_color_value = *bound1 + 1;
                }
            }
        } else {
            if (*bound1 < *bound2) {
                _cycle_color_value -= _cycle_step_value;
                if (_cycle_color_value < *bound1) {
                    _cycle_direction_flag = 0;
                    _cycle_color_value = *bound1 + 1;
                }
            } else {
                _cycle_color_value += _cycle_step_value;
                if (*bound2 < _cycle_color_value) {
                    _cycle_direction_flag = 0;
                    _cycle_color_value = *bound2 - 1;
                }
            }
        }
    }

    void __cdecl Cycle_Palette_Color_(int16_t pal_idx, int16_t r1, int16_t g1, int16_t b1, int16_t r2, int16_t g2, int16_t b2, int16_t step_val) {
        int32_t delta_r = r2 - r1;
        int32_t delta_g = g2 - g1;
        int32_t delta_b = b2 - b1;

        int32_t abs_r = delta_r;
        if (abs_r < 0) {
            abs_r = -abs_r;
        }

        int32_t abs_g = delta_g;
        if (abs_g < 0) {
            abs_g = -abs_g;
        }

        int32_t abs_b = delta_b;
        if (abs_b < 0) {
            abs_b = -abs_b;
        }

        _cycle_step_value = step_val;

        int32_t max_idx = 2;
        if ((abs_r < abs_g) || (abs_r < abs_b)) {
            if ((abs_r <= abs_g) && (abs_b <= abs_g)) {
                max_idx = 1;
            }
        } else {
            max_idx = 0;
        }

        if (_cycle_direction_flag == -1) {
            _cycle_direction_flag = 0;
            if (max_idx == 0) {
                _cycle_color_value = r1;
            } else if (max_idx == 1) {
                _cycle_color_value = g1;
            } else if (max_idx == 2) {
                _cycle_color_value = b1;
            }
        }

        int32_t out_r = 0;
        int32_t out_g = 0;
        int32_t out_b = 0;

        if (max_idx == 0) {
            out_r = _cycle_color_value;
            out_g = (delta_g * (_cycle_color_value - r1)) / delta_r + g1;
            out_b = (delta_b * (_cycle_color_value - r1)) / delta_r + b1;
        } else if (max_idx == 1) {
            out_g = _cycle_color_value;
            out_r = (delta_r * (_cycle_color_value - g1)) / delta_g + r1;
            out_b = (delta_b * (_cycle_color_value - g1)) / delta_g + b1;
        } else if (max_idx == 2) {
            out_b = _cycle_color_value;
            out_g = (delta_g * (_cycle_color_value - b1)) / delta_b + g1;
            out_r = (delta_r * (_cycle_color_value - b1)) / delta_b + r1;
        }

        if (max_idx == 0) {
            Update_Cycle_(&r1, &r2);
        } else if (max_idx == 1) {
            Update_Cycle_(&g1, &g2);
        } else if (max_idx == 2) {
            Update_Cycle_(&b1, &b2);
        }

        palstore::Store_Single_Palette_Color_(pal_idx, static_cast<uint8_t>(out_r), static_cast<uint8_t>(out_g), static_cast<uint8_t>(out_b));
    }

    void __cdecl Save_Glass_Remap_Block_(void* buffer) {
        struct_::Copy_Structure_(buffer, video::_glass_remap_colors, 0x1000);
        struct_::Copy_Structure_(static_cast<uint8_t*>(buffer) + 0x1000, video::_glass_colors, 0x40);
    }

    void* __cdecl Allocate_Glass_Remap_Next_Block_(void* block) {
        return allocate::Allocate_Next_Block_(block, 0x1040);
    }

    void __cdecl Set_Glass_Remap_Color_(int16_t idx, uint8_t r, uint8_t g, uint8_t b, uint8_t c) {
        video::_glass_colors[idx].intensity = c;
        video::_glass_colors[idx].r = r;
        video::_glass_colors[idx].g = g;
        video::_glass_colors[idx].b = b;
    }

    void __cdecl Restore_Glass_Remap_Block_(void* buffer) {
        struct_::Copy_Structure_(video::_glass_remap_colors, buffer, 0x1000);
        struct_::Copy_Structure_(video::_glass_colors, static_cast<uint8_t*>(buffer) + 0x1000, 0x40);
    }

    void __cdecl RGB_To_HLS_(int8_t r, int8_t g, int8_t b, int32_t* h, int32_t* l, int32_t* s) {
        int32_t r_scaled = (r * 1000) / 63;
        int32_t g_scaled = (g * 1000) / 63;
        int32_t b_scaled = (b * 1000) / 63;

        int32_t max_val = g_scaled;
        if (g_scaled <= r_scaled) {
            max_val = r_scaled;
        }
        if (max_val <= b_scaled) {
            max_val = b_scaled;
        }

        int32_t min_val = g_scaled;
        if (r_scaled <= g_scaled) {
            min_val = r_scaled;
        }
        if (b_scaled <= min_val) {
            min_val = b_scaled;
        }

        int32_t lightness = (max_val + min_val) / 2;
        *l = lightness;

        if (min_val != max_val) {
            int32_t delta = max_val - min_val;

            if (lightness <= 500) {
                *s = (delta * 1000) / (max_val + min_val);
            } else {
                *s = (delta * 1000) / ((2000 - min_val) - max_val);
            }

            if (r_scaled == max_val) {
                *h = ((g_scaled - b_scaled) * 1000) / delta;
            } else if (g_scaled == max_val) {
                *h = ((b_scaled - r_scaled) * 1000) / delta + 2000;
            } else if (b_scaled == max_val) {
                *h = ((r_scaled - g_scaled) * 1000) / delta + 4000;
            }

            int32_t hue = *h * 60;
            *h = hue;
            if (hue < 0) {
                *h = hue + 360;
            }
            *h = *h / 1000;
        } else {
            *s = 0;
            *h = -1;
        }
    }

    int32_t __cdecl Value_(int32_t p, int32_t q, int32_t t) {
        if (t > 360) {
            t -= 360;
        } else if (t < 0) {
            t += 360;
        }

        if (t < 60) {
            return (((q - p) * (t * 1000 / 60)) / 1000 + p) * 63 / 1000;
        }
        if (t < 180) {
            return q * 63 / 1000;
        }
        if (t < 240) {
            return ((((240 - t) * 1000 / 60) * (q - p)) / 1000 + p) * 63 / 1000;
        }
        
        return p * 63 / 1000;
    }

    void __cdecl HLS_To_RGB_(int32_t h, int32_t l, int32_t s, uint8_t* r, uint8_t* g, uint8_t* b) {
        int32_t m2;

        if (l <= 500) {
            m2 = ((s + 1000) * l) / 1000;
        } else {
            m2 = (s * l) / -1000 + s + l;
        }

        int32_t m1 = l * 2 - m2;

        if (s == 0) {
            if (h == -1) {
                uint8_t val = (l * 63) / 1000;
                *b = val;
                *g = val;
                *r = val;
            } else {
                *b = 0;
                *g = 0;
                *r = 0;
            }
            return;
        }

        *r = Value_(m1, m2, h + 120);
        *g = Value_(m1, m2, h);
        *b = Value_(m1, m2, h - 120);
    }
}

