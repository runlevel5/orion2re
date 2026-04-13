#include "pch.h"

namespace video {
	void Set_Page_Off_() {
		platform::g_render_buffer = 1;
		_copy_to_on_page_flag = 0;
	}

	void Set_Refresh_Stencil_() {
		screen_refresh_flag = 3;
		screen_dump_flag = 0;
	}

	void Set_Refresh_Full_() {
		screen_refresh_flag = 0;
		screen_dump_flag = 0;
	}

	void Clip_Off_() {
		animate::_clipping_flag = 0;
	}

	void __cdecl Clip_On_() {
		animate::_clipping_flag = 1;
	}

	uint8_t __cdecl Get_Current_Video_Page_Flag_() {
		return platform::g_render_buffer;
	}

	void __cdecl Set_Refresh_Differential_() {
		screen_refresh_flag = 2;
		screen_dump_flag = 0;
	}

	void __cdecl Add_Line_To_Scan_List_(int32_t x1, int32_t y, int32_t x2) {
		if (x1 < 0) {
			x1 = 0;
		}
		if (y < 0) {
			y = 0;
		}
		if (x2 >= (int32_t)g_screen_res_x) {
			x2 = (int32_t)g_screen_res_x - 1;
		}
		if (y >= g_screen_res_y) {
			y = g_screen_res_y - 1;
		}

		int16_t min_x_scaled = (int16_t)(x1 >> 2);
		int16_t max_x_scaled = (int16_t)(x2 >> 2);

		s_scan_list_entry* scan_list = _refresh_scan_list;
		s_scan_list_entry& scan_entry = scan_list[y];

		if (screen_refresh_flag == 2) {
			if (scan_entry.min_x == -1) {
				scan_entry.min_x = min_x_scaled;
				scan_entry.max_x = max_x_scaled;
			}
			else {
				if (scan_entry.min_x > min_x_scaled) {
					scan_entry.min_x = min_x_scaled;
				}
				if (scan_entry.max_x < max_x_scaled) {
					scan_entry.max_x = max_x_scaled;
				}
			}
		}
		else {
			// Update the primary scan list
			if (scan_entry.min_x == -1) {
				scan_entry.min_x = min_x_scaled;
				scan_entry.max_x = max_x_scaled;
			}
			else {
				if (scan_entry.min_x > min_x_scaled) {
					scan_entry.min_x = min_x_scaled;
				}
				if (scan_entry.max_x < max_x_scaled) {
					scan_entry.max_x = max_x_scaled;
				}
			}

			// Update the stencil list
			s_scan_list_entry* stencil_list = _refresh_stencil_list;
			s_scan_list_entry& stencil_entry = stencil_list[y];

			if (stencil_entry.min_x == -1) {
				stencil_entry.min_x = min_x_scaled;
				stencil_entry.max_x = max_x_scaled;
			}
			else {
				if (stencil_entry.min_x > min_x_scaled) {
					stencil_entry.min_x = min_x_scaled;
				}
				if (stencil_entry.max_x < max_x_scaled) {
					stencil_entry.max_x = max_x_scaled;
				}
			}
		}
	}

	void __cdecl Add_Dot_To_Scan_List_(int32_t x_scaled, int32_t y) {
		int16_t x = (int16_t)(x_scaled >> 2);
		s_scan_list_entry* scan_entry = &_refresh_scan_list[y];

		if (scan_entry->min_x == -1) {
			scan_entry->min_x = x;
			scan_entry->max_x = x;
		}
		else if (x < (int32_t)scan_entry->min_x) {
			scan_entry->min_x = x;
		}
		else if (x > (int32_t)scan_entry->max_x) {
			scan_entry->max_x = x;
		}

		if (screen_refresh_flag != 2) {
			s_scan_list_entry* stencil_entry = &_refresh_stencil_list[y];

			if (stencil_entry->min_x == -1) {
				stencil_entry->min_x = x;
				stencil_entry->max_x = x;
			}
			else if (x < (int32_t)stencil_entry->min_x) {
				stencil_entry->min_x = x;
			}
			else if (x > (int32_t)stencil_entry->max_x) {
				stencil_entry->max_x = x;
			}
		}
	}

    void __cdecl Toggle_Pages_() {
        platform::g_pending_toggle_pages = true;
        fields::Draw_Visible_Fields_();
        mouse::Disable_Mouse_();

        g_cached_mouse_x = platform::g_mouse_x;
        g_cached_mouse_y = platform::g_mouse_y;

        if (g_window_focus_state != 0) {
            if (video::screen_dump_flag == 0) {
                platform::Blit_To_Back_Buffer();
                platform::Flip_Surface();

                if (video::screen_refresh_flag == 3 || video::screen_refresh_flag == 2) {
                    video::screen_dump_flag = 2;
                }

                if (video::screen_refresh_flag == 1) {
                    video::screen_dump_flag = 1;
                }

                platform::Save_Mouse_(platform::g_mouse_x, platform::g_mouse_y);
                platform::Draw_Cursor_XY(static_cast<int>(platform::g_mouse_x), static_cast<int>(platform::g_mouse_y));
            } else {
                platform::Save_Mouse_Offscreen_(platform::g_mouse_x, platform::g_mouse_y);
                platform::Draw_Cursor_XY_Offscreen_(platform::g_mouse_x, platform::g_mouse_y);
                
                setvesa::Add_Square_To_Scan_List_(
                    static_cast<int>(platform::g_mouse_x),
                    static_cast<int>(platform::g_mouse_y),
                    static_cast<int>(platform::g_mouse_x) + 24,
                    static_cast<int>(platform::g_mouse_y) + 24
                );

                if (video::screen_dump_flag == 1) {
                    video::_Masked_Screen_Dump();
                } else {
                    video::_Partial_Screen_Dump();
                }

                platform::Restore_Mouse_Off_Page_();
            }

            palette::Refresh_Palette_();
            mouse::Enable_Mouse_();

            if (video::screen_refresh_flag == 3) {
                video::Stencil_Out_Background_();
                struct_::Copy_Structure_(
                    video::_refresh_scan_list,
                    video::_refresh_stencil_list,
                    g_screen_res_y * sizeof(s_scan_list_entry)
                );
                video::Clear_Stencil_List_();
            } else {
                video::Clear_Scan_List_();
            }
        }

        video::Set_Page_Off_();
        platform::g_pending_toggle_pages = false;
    }
}