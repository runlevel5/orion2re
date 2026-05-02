#include "pch.h"

namespace remap {
    void __cdecl Sort_Palette_List_() {
        for (int i = 0; i < 255; ++i) {
            int insert_pos = i;
            bool found = false;

            for (int j = 0; j < i; ++j) {
                if (video::_current_palette[i].r < video::_current_palette[video::_palette_reds[j]].r) {
                    insert_pos = j;
                    found = true;
                    break;
                }
            }

            if (found) {
                for (int k = i; k >= insert_pos; --k) {
                    video::_palette_reds[k + 1] = video::_palette_reds[k];
                }
            }
            video::_palette_reds[insert_pos] = static_cast<uint8_t>(i);
        }

        video::_palette_list[0] = &video::_palette_reds[0];

        for (int intensity = 1; intensity < 64; ++intensity) {
            bool found = false;
            for (int j = 0; j < 256; ++j) {
                if (video::_current_palette[video::_palette_reds[j]].r == intensity) {
                    video::_palette_list[intensity] = &video::_palette_reds[j];
                    found = true;
                    break;
                }
            }

            if (!found) {
                video::_palette_list[intensity] = video::_palette_list[intensity - 1];
            }
        }

        video::_palette_list[64] = &video::_palette_reds[255];
    }

    uint8_t __cdecl Find_Closest_Color_(uint8_t r, uint8_t g, uint8_t b) {
        int r_min_idx = (int)r;
        int r_max_idx = (int)r;
        int g_min = (int)g;
        int g_max = (int)g;
        int b_min = (int)b;
        int b_max = (int)b;

        int best_color_idx = -1;

        for (int retry_count = 25; retry_count > 0; --retry_count) {
            if (r_min_idx <= 10) {
                r_min_idx = 0;
            } else {
                r_min_idx -= 10;
            }

            if (r_max_idx < 53) {
                r_max_idx += 10;
            } else {
                r_max_idx = 64;
            }

            if (g_min <= 10) {
                g_min = 0;
            } else {
                g_min -= 10;
            }

            if (g_max < 53) {
                g_max += 10;
            } else {
                g_max = 63;
            }

            if (b_min <= 10) {
                b_min = 0;
            } else {
                b_min -= 10;
            }

            if (b_max < 53) {
                b_max += 10;
            } else {
                b_max = 63;
            }

            int min_dist_sq = 13000;

            uint8_t* start_ptr = video::_palette_list[r_min_idx];
            uint8_t* end_ptr = video::_palette_list[r_max_idx];
            int count = (int)(end_ptr - start_ptr);

            if (count > 0) {
                for (int i = 0; i < count; ++i) {
                    uint8_t color_idx = start_ptr[i];
                    
                    int cur_g = (int)video::_current_palette[color_idx].g;
                    if (cur_g >= g_min && cur_g <= g_max) {
                        int cur_b = (int)video::_current_palette[color_idx].b;
                        if (cur_b >= b_min && cur_b <= b_max) {
                            int cur_r = (int)video::_current_palette[color_idx].r;
                            
                            int dr = cur_r - (int)r;
                            int dg = cur_g - (int)g;
                            int db = cur_b - (int)b;
                            int dist_sq = (dr * dr) + (dg * dg) + (db * db);

                            if (dist_sq < min_dist_sq && color_idx != 0) {
                                best_color_idx = (int)color_idx;
                                min_dist_sq = dist_sq;
                            }
                        }
                    }
                }
            }

            if (best_color_idx != -1) {
                return (uint8_t)best_color_idx;
            }
        }

        return 0;
    }

    void __cdecl Create_Picture_Remap_Palette_(uint8_t* src_palette, uint8_t* remap_table) {
        Sort_Palette_List_();

        for (int i = 0; i < 256; ++i) {
            remap_table[i] = (uint8_t)i;
        }

        animate::s_palette_header* header = animate::_palette_header;
        if (header->count > 0) {
            for (int i = 0; i < (int)header->count; ++i) {
                uint8_t r = src_palette[i * 4 + 1];
                uint8_t g = src_palette[i * 4 + 2];
                uint8_t b = src_palette[i * 4 + 3];

                uint8_t closest_color = Find_Closest_Color_(r, g, b);

                remap_table[header->start_index + i] = closest_color;
            }
        }
    }

    void __cdecl Update_Glass_Remap_Colors_() {
        video::_glass_colors[0].intensity = 50 /* 0x32 */;
        video::_glass_colors[0].r = 0;
        video::_glass_colors[0].g = 0;
        video::_glass_colors[0].b = 0;

        Sort_Palette_List_();

        for (int pal_idx = 0; pal_idx < 256; ++pal_idx) {
            for (int glass_idx = 0; glass_idx < 16; ++glass_idx) {
                uint8_t intensity = video::_glass_colors[glass_idx].intensity;

                if (intensity == 0) {
                    video::_glass_remap_colors[glass_idx][pal_idx] = (uint8_t)pal_idx;
                } else {
                    uint8_t target_r, target_g, target_b;

                    if (intensity == 100 /* 0x64 */) {
                        target_r = video::_glass_colors[glass_idx].r;
                        target_g = video::_glass_colors[glass_idx].g;
                        target_b = video::_glass_colors[glass_idx].b;
                    } else {
                        int weight_glass = (int)intensity;
                        int weight_pal = 100 - weight_glass;

                        target_r = (uint8_t)((video::_current_palette[pal_idx].r * weight_pal + video::_glass_colors[glass_idx].r * weight_glass) / 100);
                        target_g = (uint8_t)((video::_current_palette[pal_idx].g * weight_pal + video::_glass_colors[glass_idx].g * weight_glass) / 100);
                        target_b = (uint8_t)((video::_current_palette[pal_idx].b * weight_pal + video::_glass_colors[glass_idx].b * weight_glass) / 100);
                    }

                    video::_glass_remap_colors[glass_idx][pal_idx] = Find_Closest_Color_(target_r, target_g, target_b);
                }
            }
        }
    }

    void __cdecl Calculate_Picture_Remap_Colors_(s_animation_header* header) {
        animate::_animation_header = header;

        if (!(animate::_animation_header->flags & 0x10)) {
            exitm::Exit_With_Message(s_Remapped_Picture_Must_Have_Palet_0055b2ac);
        }

        animate::_palette_header = reinterpret_cast<animate::s_palette_header*>(reinterpret_cast<uint8_t*>(&animate::_animation_header->frame_offsets[animate::_animation_header->frame_count]) + 4);

        Create_Picture_Remap_Palette_(reinterpret_cast<uint8_t*>(animate::_palette_header) + 4, _picture_remap_color_list);
    }

    uint8_t __cdecl Get_Remap_Palette_Color_(int32_t index) {
        // TYPE MISMATCH: index was uint8_t
        return _picture_remap_color_list[static_cast<uint8_t>(index)];
    }

    void __cdecl Get_Palette_Animation_Color_(s_animation_header* animation_header,
                                              int16_t color_index,
                                              uint8_t* out_r,
                                              uint8_t* out_g,
                                              uint8_t* out_b) {
        char error_text[100];

        animate::_animation_header = animation_header;
        if ((animation_header->flags & 0x10) == 0) {
            exitm::Exit_With_Message(s_Get_Picture_Color_Picture_Must_H_0055b2dc);
        }

        animate::_palette_header = reinterpret_cast<animate::s_palette_header*>(
                &animate::_animation_header
                         ->frame_offsets[static_cast<int32_t>(animate::_animation_header->frame_count) + 1]);

        const int32_t palette_start = animate::_palette_header->start_index;
        const int32_t palette_end = palette_start + animate::_palette_header->count;

        if ((static_cast<int32_t>(color_index) >= palette_start) &&
            (static_cast<int32_t>(color_index) < palette_end)) {
            const uint8_t* entry =
                    reinterpret_cast<const uint8_t*>(animate::_palette_header) + 4 + (static_cast<int32_t>(color_index) - palette_start) * 4;

            *out_r = entry[1];
            *out_g = entry[2];
            *out_b = entry[3];
            return;
        }

        sprintf(error_text, s__d___d___d_0055b2d0, color_index, palette_start, palette_end);
        exitm::Exit_With_Message(error_text);
    }
}