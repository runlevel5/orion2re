#include "pch.h"
#include "fonts.h"
#include "mox.h"
#include "namestar.h"
#include "funcs.h"
#include <cstring>

namespace namestar {
    int16_t Star_Name_Popup_Screen_Center_X_() {
        return 0xb1;
    }

    int16_t Star_Name_Popup_Screen_Center_Y_() {
        return 125;
    }

    int16_t Star_Name_Popup_Map_Center_X_() {
        return 131;
    }

    int16_t Star_Name_Popup_Map_Center_Y_() {
        return 0x92;
    }

    void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t mode) {
        char* message = const_cast<char*>(HAROLD::H_Message_(HESTR_100_ENTER_STAR_NAME));
        Do_Change_Star_Name_(x, y, star_idx, mode, message);
    }

    void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx) {
        char* prompt = const_cast<char*>(HAROLD::H_Message_(HESTR_101_ENTER_HOME_STAR_NAME));
        Do_Change_Star_Name_(x, y, star_idx, 0, prompt);
    }

    int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t use_popup2, char* title) {
        s_colors font_colors;
        char original_name[20];

        fonts::Set_Font_Style_(3, &font_colors);

        s_star_data* star = &MOX::_star[star_idx];

        int16_t current_name_width = fonts::Get_String_Width_(star->name);
        int16_t www_width = fonts::Get_String_Width_(s_WWWWWWW_0055137c);
        int16_t max_allowed_width = Max_Pixel_Width_Star_Name_Can_Be_(star_idx);

        int16_t input_width = max_allowed_width;
        if (current_name_width > input_width) {
            input_width = current_name_width;
        }
        if (input_width > www_width) {
            input_width = www_width;
        }
        if (input_width > 205) {
            input_width = 205;
        }

        strcpy(original_name, star->name);

        int8_t popup_result;
        if (use_popup2 != 0) {
            popup_result = Input_Box_Popup2_(star->name, 15, input_width, title, x, y);
        } else {
            popup_result = Input_Box_Popup_(star->name, 15, input_width, title, x, y, 0);
        }

        if (star->name[0] == '\0') {
            strcpy(star->name, original_name);
        }

        fonts::Get_String_Width_(star->name);

        return popup_result;
    }

    int16_t __cdecl Max_Pixel_Width_Star_Name_Can_Be_(int16_t star_idx) {
        int16_t x_orig, y_orig, width_orig;
        int16_t x_target, y_target, width_target;
        int16_t min_dist_left = 999;
        int16_t min_dist_right = 999;
        int16_t idx_left = -1;
        int16_t idx_right = -1;
        int16_t width_at_min_left;
        int16_t font_h_plus_2;

        MAINSCR::Get_Star_Name_Info_(star_idx, &x_orig, &y_orig, &width_orig);

        font_h_plus_2 = fonts::Get_Font_Height_() + 2;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (i != star_idx) {
                MAINSCR::Get_Star_Name_Info_(i, &x_target, &y_target, &width_target);

                if (abs(y_target - y_orig) < font_h_plus_2) {
                    int16_t dist = x_target - x_orig;
                    if (dist < 0) {
                        int16_t abs_dist = (int16_t)abs(dist);
                        if (abs_dist < min_dist_left) {
                            min_dist_left = abs_dist;
                            width_at_min_left = width_target;
                            idx_left = i;
                        }
                    } else {
                        int16_t abs_dist = (int16_t)abs(dist);
                        if (abs_dist < min_dist_right) {
                            min_dist_right = abs_dist;
                            idx_right = i;
                        }
                    }
                }
            }

            if (idx_left == -1) {
                width_at_min_left = 0;
                min_dist_left = (int16_t)abs(x_orig - 21 /* 0x15 */);
            }

            if (idx_right == -1) {
                min_dist_right = (int16_t)abs(x_orig - 527 /* 0x20f */);
                if (min_dist_right < width_orig) {
                    min_dist_right = width_orig;
                }
            }
        }

        if (min_dist_right < width_orig) {
            min_dist_right = width_orig;
        }

        int16_t avail_left = min_dist_left - width_at_min_left;
        int16_t avail_right = min_dist_right - width_orig;

        int16_t max_offset = (avail_left < avail_right) ? avail_left : avail_right;
        int16_t result = max_offset * 2 + width_orig;

        if (result < width_orig) {
            result = width_orig + 5;
        }

        return result;
    }

    /**
     * @brief Checks if a star's name can be changed.
     * Star names can only be changed if no planets in the system have colonies.
     * 
     * @param star_idx The index of the star system to check.
     * @return int8_t 1 if the name can be changed (no colonies), 0 otherwise.
     */
    int8_t __cdecl Can_Change_Star_Name_(short star_idx) {
        int8_t can_change = 1;
        bool has_colony = false;

        for (int i = 0; i < 5; ++i) {
            if (has_colony) {
                break;
            }

            short planet_idx = MOX::_star[star_idx].planet_index[i];

            // If planet exists (index is not -1)
            if (planet_idx > -1) {
                // If the planet has a colony (colony_index is not -1)
                if (MOX::_planet[planet_idx].colony_index > -1) {
                    can_change = 0;
                    has_colony = true;
                }
            }
        }

        return can_change;
    }
}