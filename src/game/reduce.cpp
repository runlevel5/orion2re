#include "pch.h"

namespace reduce {
    void __cdecl Reduce_Palette_(s_palette_entry* palette, int16_t start_index, int16_t num_colors_src, int16_t num_colors_dst) {
        if (num_colors_src > 512) {
            exitm::Exit_With_Message(s_Too_many_colors_to_reduce_from_);
        }
        if (num_colors_dst > 256) {
            exitm::Exit_With_Message(s_Too_many_colors_to_reduce_to_);
        }

        Initial_Cube_(palette, num_colors_src);

        while (reduce::_active_cube_count < num_colors_dst) {
            int16_t cube_idx = Find_Next_Lowest_Cube_Level_();
            if (cube_idx == -1) {
                break;
            }

            int16_t axis = Find_Longest_Axis_(cube_idx);

            int16_t first_hist_index = reduce::_cube_list[cube_idx].first_hist_index;
            int16_t last_hist_index = reduce::_cube_list[cube_idx].last_hist_index;

            Sort_Cube_Points_(
                palette,
                &reduce::_HistPtr[first_hist_index],
                (last_hist_index - first_hist_index) + 1,
                axis
            );

            Split_Cube_(palette, cube_idx, axis);
        }

        int16_t i = 0;
        for (; i < reduce::_active_cube_count; ++i) {
            s_palette_entry* entry = &palette[start_index + i];
            entry->changed = 1;
            entry->r = (uint8_t)((reduce::_cube_list[i].r_min + reduce::_cube_list[i].r_max) / 2);
            entry->g = (uint8_t)((reduce::_cube_list[i].g_min + reduce::_cube_list[i].g_max) / 2);
            entry->b = (uint8_t)((reduce::_cube_list[i].b_min + reduce::_cube_list[i].b_max) / 2);
        }

        for (; i < num_colors_dst; ++i) {
            s_palette_entry* entry = &palette[start_index + i];
            entry->changed = 1;
            entry->r = palette[start_index + i - 1].r;
            entry->g = palette[start_index + i - 1].g;
            entry->b = palette[start_index + i - 1].b;
        }
    }

    void __cdecl Clear_Reduced_Palette_Groups_() {
        _ncubes = 0;
    }

    void __cdecl Add_Reduced_Palette_Group_(s_animation_header* param_1) {
        animate::_animation_header = param_1;

        if ((param_1->flags & 0x10) == 0) {
            exitm::Exit_With_Message("Palette Reduction Pictures Must Have Palette");
        }

        animate::_palette_header = reinterpret_cast<animate::s_palette_header*>(reinterpret_cast<uint8_t*>(animate::_animation_header) +
                                   (animate::_animation_header->frame_count * 4) + 0x10);

        int16_t count = animate::_palette_header->count;

        if (count > 0) {
            s_palette_entry* src_pal = reinterpret_cast<s_palette_entry*>(animate::_palette_header + 1);

            for (int16_t i = 0; i < count; ++i) {
                int16_t cube_idx = reduce::_ncubes;
                reduce::_reduced_palette_list[cube_idx].r = src_pal[i].r;
                reduce::_reduced_palette_list[cube_idx].g = src_pal[i].g;
                reduce::_reduced_palette_list[cube_idx].b = src_pal[i].b;

                reduce::_ncubes++;

                if (reduce::_ncubes >= 512) {
                    exitm::Exit_With_Message("Too many colors to reduce from (Max 512)");
                }
            }
        }
    }

    void __cdecl Compress_Reduced_Palette_Groups_(int16_t start_index, int16_t count) {
        if (count < _ncubes) {
            Reduce_Palette_(_reduced_palette_list, 0, _ncubes, count);
        }

        if (count > 0) {
            for (int16_t i = 0; i < count; ++i) {
                int16_t dest_idx = start_index + i;
                video::_current_palette[dest_idx].changed = 1;
                video::_current_palette[dest_idx].r = _reduced_palette_list[i].r;
                video::_current_palette[dest_idx].g = _reduced_palette_list[i].g;
                video::_current_palette[dest_idx].b = _reduced_palette_list[i].b;
            }
        }
    }

    void __cdecl Shrink_Cube_(s_palette_cube* cube, s_palette_entry* hist) {
        cube->r_min = 0xFF;
        cube->r_max = 0x00;
        cube->g_min = 0xFF;
        cube->g_max = 0x00;
        cube->b_min = 0xFF;
        cube->b_max = 0x00;

        for (int16_t i = cube->first_hist_index; i <= static_cast<int16_t>(cube->last_hist_index); ++i) {
            int16_t hist_idx = _HistPtr[i];

            uint8_t r = hist[hist_idx].r;
            if (r < cube->r_min) {
                cube->r_min = r;
            }
            if (cube->r_max < r) {
                cube->r_max = r;
            }

            uint8_t g = hist[hist_idx].g;
            if (g < cube->g_min) {
                cube->g_min = g;
            }
            if (cube->g_max < g) {
                cube->g_max = g;
            }

            uint8_t b = hist[hist_idx].b;
            if (b < cube->b_min) {
                cube->b_min = b;
            }
            if (cube->b_max < b) {
                cube->b_max = b;
            }
        }
    }

    int16_t __cdecl Find_Next_Lowest_Cube_Level_() {
        int16_t best_idx = -1;
        int16_t max_diff = 0;

        for (int16_t i = 0; i < static_cast<int32_t>(_active_cube_count); ++i) {
            if (_cube_list[i].first_hist_index != _cube_list[i].last_hist_index &&
                static_cast<int16_t>(_cube_list[i].level) < 30000) {

                int16_t r_diff = _cube_list[i].r_max - _cube_list[i].r_min;
                int16_t g_diff = _cube_list[i].g_max - _cube_list[i].g_min;
                int16_t b_diff = _cube_list[i].b_max - _cube_list[i].b_min;

                if (max_diff < r_diff) {
                    max_diff = r_diff;
                    best_idx = i;
                }
                if (max_diff < g_diff) {
                    max_diff = g_diff;
                    best_idx = i;
                }
                if (max_diff < b_diff) {
                    max_diff = b_diff;
                    best_idx = i;
                }
            }
        }
        return best_idx;
    }

    void __cdecl Initial_Cube_(s_palette_entry* pal, int16_t num_colors) {
        if (num_colors > 0) {
            for (int16_t i = 0; i < num_colors; ++i) {
                _HistPtr[i] = i;
            }
        }

        _cube_list[0].first_hist_index = 0;
        _cube_list[0].last_hist_index = num_colors - 1;
        _cube_list[0].level = 0;

        Shrink_Cube_(&_cube_list[0], pal);

        _active_cube_count = 1;
    }

    int16_t __cdecl Find_Longest_Axis_(int16_t cube_idx) {
        uint8_t r_diff = _cube_list[cube_idx].r_max - _cube_list[cube_idx].r_min;
        uint8_t g_diff = _cube_list[cube_idx].g_max - _cube_list[cube_idx].g_min;
        uint8_t b_diff = _cube_list[cube_idx].b_max - _cube_list[cube_idx].b_min;

        if (g_diff <= r_diff && b_diff <= r_diff) {
            return 0;
        }
        if (r_diff <= g_diff && b_diff <= g_diff) {
            return 1;
        }
        return 2;
    }

    void __cdecl Sort_Cube_Points_(s_palette_entry* palette, int16_t* indices, int16_t num_indices, int16_t component) {
        int16_t pass;
        bool swapped;
        int16_t i;

        if (component == 0) {
            pass = 0;
            swapped = true;
            if (num_indices > 0) {
                while (swapped) {
                    swapped = false;
                    if (num_indices - 1 > 0) {
                        for (i = 0; i < num_indices - 1; ++i) {
                            if (palette[indices[i + 1]].r < palette[indices[i]].r) {
                                strings::Swap_Short_(&indices[i], &indices[i + 1]);
                                swapped = true;
                            }
                        }
                    }
                    pass++;
                    if (pass >= num_indices) {
                        return;
                    }
                }
            }
        } else if (component == 1) {
            pass = 0;
            swapped = true;
            if (num_indices > 0) {
                while (swapped) {
                    swapped = false;
                    if (num_indices - 1 > 0) {
                        for (i = 0; i < num_indices - 1; ++i) {
                            if (palette[indices[i + 1]].g < palette[indices[i]].g) {
                                strings::Swap_Short_(&indices[i], &indices[i + 1]);
                                swapped = true;
                            }
                        }
                    }
                    pass++;
                    if (pass >= num_indices) {
                        return;
                    }
                }
            }
        } else if (component == 2) {
            swapped = true;
            pass = 0;
            if (num_indices > 0) {
                while (swapped) {
                    swapped = false;
                    if (num_indices - 1 > 0) {
                        for (i = 0; i < num_indices - 1; ++i) {
                            if (palette[indices[i + 1]].b < palette[indices[i]].b) {
                                strings::Swap_Short_(&indices[i], &indices[i + 1]);
                                swapped = true;
                            }
                        }
                    }
                    pass++;
                    if (pass >= num_indices) {
                        return;
                    }
                }
            }
        }
    }

    void __cdecl Split_Cube_(s_palette_entry* entries, int16_t cube_index, int16_t color_channel) {
        uint16_t first = _cube_list[cube_index].first_hist_index;
        uint16_t last = _cube_list[cube_index].last_hist_index;

        if (color_channel == 0) {
            if (first <= last) {
                for (uint16_t i = first; i <= last; ++i) {
                    if (i == first) {
                        _color_weight[i] = entries[_HistPtr[i + 1]].r - entries[_HistPtr[i]].r;
                    } else if (i == last) {
                        _color_weight[i] = entries[_HistPtr[i]].r - entries[_HistPtr[i - 1]].r;
                    } else {
                        uint8_t diff1 = entries[_HistPtr[i]].r - entries[_HistPtr[i - 1]].r;
                        uint8_t diff2 = entries[_HistPtr[i + 1]].r - entries[_HistPtr[i]].r;
                        _color_weight[i] = (diff2 > diff1) ? diff2 : diff1;
                    }

                    if (_color_weight[i] == 0) {
                        _color_weight[i] = 1;
                    }
                }
            }
        } else if (color_channel == 1) {
            if (first <= last) {
                for (uint16_t i = first; i <= last; ++i) {
                    if (i == first) {
                        _color_weight[i] = entries[_HistPtr[i + 1]].g - entries[_HistPtr[i]].g;
                    } else if (i == last) {
                        _color_weight[i] = entries[_HistPtr[i]].g - entries[_HistPtr[i - 1]].g;
                    } else {
                        uint8_t diff1 = entries[_HistPtr[i]].g - entries[_HistPtr[i - 1]].g;
                        uint8_t diff2 = entries[_HistPtr[i + 1]].g - entries[_HistPtr[i]].g;
                        _color_weight[i] = (diff2 > diff1) ? diff2 : diff1;
                    }

                    if (_color_weight[i] == 0) {
                        _color_weight[i] = 1;
                    }
                }
            }
        } else if (color_channel == 2) {
            if (first <= last) {
                for (uint16_t i = first; i <= last; ++i) {
                    if (i == first) {
                        _color_weight[i] = entries[_HistPtr[i + 1]].b - entries[_HistPtr[i]].b;
                    } else if (i == last) {
                        _color_weight[i] = entries[_HistPtr[i]].b - entries[_HistPtr[i - 1]].b;
                    } else {
                        uint8_t diff1 = entries[_HistPtr[i]].b - entries[_HistPtr[i - 1]].b;
                        uint8_t diff2 = entries[_HistPtr[i + 1]].b - entries[_HistPtr[i]].b;
                        _color_weight[i] = (diff2 > diff1) ? diff2 : diff1;
                    }

                    if (_color_weight[i] == 0) {
                        _color_weight[i] = 1;
                    }
                }
            }
        }

        int32_t sum = 0;
        for (uint16_t i = first; i <= last; ++i) {
            sum += _color_weight[i];
        }

        sum = (sum / 2) - _color_weight[first];
        uint16_t split_index = first;

        while (sum > 0 && split_index < last) {
            split_index++;
            sum -= _color_weight[split_index];
        }

        uint16_t active = _active_cube_count;
        _cube_list[active].first_hist_index = split_index + 1;
        _cube_list[active].last_hist_index = _cube_list[cube_index].last_hist_index;
        _cube_list[active].level = _cube_list[cube_index].level + 1;

        _cube_list[cube_index].last_hist_index = split_index;
        _cube_list[cube_index].level++;

        Shrink_Cube_(&_cube_list[cube_index], entries);
        Shrink_Cube_(&_cube_list[active], entries);

        _active_cube_count++;
    }
}