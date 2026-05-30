#include "pch.h"

namespace video {
	int32_t g_screen_res_x;
	int32_t g_screen_res_y;
	IDirectDrawSurface* g_offscreen_surface_1;
	IDirectDrawSurface* g_offscreen_surface_2;
	uint8_t* _video_memory;
	int16_t _current_mouse_list_ptr;
	int16_t _current_mouse_shape;
	uint8_t* _current_video_page;
	uint32_t screen_dump_flag;
	int32_t screen_refresh_flag;
	int16_t _old_screen_dump_flag;
	int16_t _old_screen_refresh_flag;
	s_scan_list_entry* _refresh_scan_list;
	s_scan_list_entry* _refresh_stencil_list;
	uint8_t* _mouse_palette;
	uint32_t _screen_pixel_size = 0x4B000;
	uint8_t _glass_remap_colors[16][256];
	s_glass_color _glass_colors[16];
	IDirectDrawPalette* g_palette;
	tagPALETTEENTRY g_palette_entries[256];
	s_palette_entry _current_palette[256];
	uint8_t* _palette_list[65];
	uint8_t _palette_reds[256];
	uint16_t _is_initializing;
	s_mouse_list _default_mouse_list;
	uint8_t DAT_00578b58;
	int16_t _copy_to_on_page_flag;
	int16_t _screen_window_y1;
	int16_t _screen_window_y2;
	int16_t _screen_window_x1;
	int16_t _screen_window_x2;

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

	int16_t __cdecl Get_Refresh_Mode_() {
		return static_cast<int16_t>(screen_refresh_flag);
	}

	void __cdecl Add_Line_To_Scan_List_(int32_t x1, int32_t y, int32_t x2) {
		if (x1 < 0) {
			x1 = 0;
		}
		if (y < 0) {
			y = 0;
		}
		if (x2 >= g_screen_res_x) {
			x2 = g_screen_res_x - 1;
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
				return;
			}
			if (scan_entry.min_x > min_x_scaled) {
				scan_entry.min_x = min_x_scaled;
			}
			if (scan_entry.max_x < max_x_scaled) {
				scan_entry.max_x = max_x_scaled;
			}
			return;
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
		int32_t x32 = x_scaled >> 2;
		int16_t x = (int16_t)x32;
		s_scan_list_entry* scan_entry = &_refresh_scan_list[y];

		if (screen_refresh_flag == 2) {
			if (scan_entry->min_x == -1) {
				scan_entry->min_x = x;
				scan_entry->max_x = x;
				return;
			}
			if (scan_entry->min_x > x32) {
				scan_entry->min_x = x;
				return;
			}
			if (scan_entry->max_x < x32) {
				scan_entry->max_x = x;
			}
			return;
		}

		if (scan_entry->min_x == -1) {
			scan_entry->min_x = x;
			scan_entry->max_x = x;
		}
		else if (scan_entry->min_x > x32) {
			scan_entry->min_x = x;
		}
		else if (scan_entry->max_x < x32) {
			scan_entry->max_x = x;
		}

		s_scan_list_entry* stencil_entry = &_refresh_stencil_list[y];

		if (stencil_entry->min_x == -1) {
			stencil_entry->min_x = x;
			stencil_entry->max_x = x;
			return;
		}
		if (stencil_entry->min_x > x32) {
			stencil_entry->min_x = x;
			return;
		}
		if (stencil_entry->max_x < x32) {
			stencil_entry->max_x = x;
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
	void __cdecl _Masked_Screen_Dump() {
	}

	void __cdecl Clear_Stencil_List_() {
		const uint32_t scanline_count = g_screen_res_y & 0x3FFFFFFFu;
		memset(video::_refresh_stencil_list, 0xFF, static_cast<size_t>(scanline_count) * 4u);
	}

	void __cdecl Clear_Scan_List_() {
		const uint32_t scan_line_count = g_screen_res_y & 0x3FFFFFFFu;
		memset(video::_refresh_scan_list, 0xFF, static_cast<size_t>(scan_line_count) * 4u);
	}

	void __cdecl Save_Refresh_() {
		video::_old_screen_dump_flag = video::screen_refresh_flag;
		video::_old_screen_refresh_flag = video::screen_dump_flag;
	}

	void __cdecl Restore_Refresh_() {
		video::screen_refresh_flag = video::_old_screen_dump_flag;
		video::screen_dump_flag = video::_old_screen_refresh_flag;
	}

	void __cdecl Set_Refresh_Differential_No_Full_() {
		video::screen_refresh_flag = 2;
		video::screen_dump_flag = 2;
	}

    void __cdecl Clear_Display_And_Release_Palette_() {
        fields::Clear_Fields_();
        video::Set_Refresh_Full_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
        platform::Toggle_Pages_Stub_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
        platform::Toggle_Pages_Stub_();
        video::Toggle_Pages_On_Odd_Frame_();

        if (video::g_palette != nullptr) {
            video::g_palette->Release();
            video::g_palette = nullptr;
        }
    }

    void __cdecl _Partial_Screen_Dump() {
        const int32_t primary_pitch_bytes = platform::Lock_Primary_Surface();
        if (primary_pitch_bytes < 0) {
            return;
        }

        const int32_t primary_pitch_dwords =
            (primary_pitch_bytes + ((primary_pitch_bytes >> 31) & 3)) >> 2;
        const int32_t alt_pitch_bytes = platform::Lock_Alt_Offscreen_Surface_();
        if (alt_pitch_bytes < 0) {
            platform::Unlock_Primary_Surface();
            return;
        }

        const int32_t alt_pitch_dwords =
            (alt_pitch_bytes + ((alt_pitch_bytes >> 31) & 3)) >> 2;
        uint32_t* const alt_surface = platform::_alt_offscreen_surface;
        uint32_t* const video_memory = (uint32_t*)video::_video_memory;

        if (platform::g_screen_res_y > 0) {
            int32_t dst_row_offset = 0;
            int32_t src_row_offset = 0;

            for (int32_t y = 0; y < platform::g_screen_res_y; ++y) {
                const s_scan_list_entry& scan = video::_refresh_scan_list[y];
                if (scan.min_x != -1) {
                    const int32_t min_x = scan.min_x;
                    const int32_t count = static_cast<int32_t>(scan.max_x) - min_x + 1;

                    platform::Copy_Dwords_(
                        video_memory + (dst_row_offset + min_x),
                        alt_surface + (src_row_offset + min_x),
                        static_cast<uint32_t>(count));
                }

                dst_row_offset += primary_pitch_dwords;
                src_row_offset += alt_pitch_dwords;
            }
        }

        platform::Unlock_Primary_Surface();
        platform::Unlock_Alt_Offscreen_Surface_();
    }

    void __cdecl Stencil_Out_Background_() {
        platform::Lock_Offscreen_Surface_();
        platform::Lock_Alt_Offscreen_Surface_();

        uint8_t* offscreen_surface = platform::_offscreen_surface;
        uint32_t* alt_offscreen_surface = platform::_alt_offscreen_surface;
        const int32_t dwords_per_row =
            (g_screen_res_x + ((g_screen_res_x >> 31) & 3)) >> 2;

        int32_t row_dword_offset = 0;
        for (int32_t row = 0; row < g_screen_res_y; ++row) {
            const s_scan_list_entry& scan = video::_refresh_stencil_list[row];
            const int32_t min_x = scan.min_x;

            if (min_x != -1) {
                const int32_t count = static_cast<int32_t>(scan.max_x) - min_x + 1;
                const int32_t dword_offset = row_dword_offset + min_x;
                platform::Copy_Dwords_(
                    alt_offscreen_surface + dword_offset,
                    (uint32_t*)(offscreen_surface + dword_offset * 4),
                    static_cast<uint32_t>(count)
                );
            }

            row_dword_offset += dwords_per_row;
        }

        platform::Unlock_Alt_Offscreen_Surface_();
        platform::Unlock_Offscreen_Surface_();
    }

    void __cdecl Toggle_Pages_On_Odd_Frame_() {
        if ((platform::g_frame_count & 1u) != 0) {
            Set_Page_Off_();
            platform::Toggle_Pages_Stub_();
        }
    }

    void __cdecl Init_Video_Drivers_() {
        video::_is_initializing = 1;
        ::DAT_00578dcc = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(&video::DAT_00578b58));
        ::DAT_00578dc8 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(&::DAT_00578820));

        platform::g_mouse_x = 0;
        ::g_cached_mouse_x = 0;
        platform::g_mouse_y = 0;
        ::g_cached_mouse_y = 0;

        mouse::_mouse_enabled = 0;
        mouse::_mouse_buffer_flag = 0;
        mouse::_mouse_buffer_flag2 = 0;

        platform::Create_Cursor_Surface();

        video::_default_mouse_list.x2 =
            static_cast<int16_t>(static_cast<uint16_t>(platform::g_screen_res_x) - 1);
        video::_default_mouse_list.y2 =
            static_cast<int16_t>(static_cast<uint16_t>(platform::g_screen_res_y) - 1);

        mouse::Set_Mouse_List_(&video::_default_mouse_list, 1);

        platform::Force_Cursor_Pos(platform::g_screen_res_x / 2, platform::g_screen_res_y / 2);

        video::_is_initializing = 0;
        mouse::_mouse_enabled = 1;
    }

    void __cdecl Set_Page_Back_() {
        platform::g_render_buffer = 2;
        _copy_to_on_page_flag = 0;
    }

    void __cdecl Copy_Back_To_Off_() {
        RECT source_rect;

        source_rect.left = 0;
        source_rect.top = 0;
        source_rect.right = static_cast<LONG>(video::g_screen_res_x);
        source_rect.bottom = static_cast<LONG>(video::g_screen_res_y);

        video::g_offscreen_surface_1->BltFast(
            0,
            0,
            video::g_offscreen_surface_2,
            &source_rect,
            DDBLTFAST_WAIT);
    }

    void __cdecl Copy_Off_To_Back_() {
        RECT copy_rect;

        copy_rect.left = 0;
        copy_rect.top = 0;
        copy_rect.right = static_cast<LONG>(video::g_screen_res_x);
        copy_rect.bottom = static_cast<LONG>(video::g_screen_res_y);

        video::g_offscreen_surface_2->BltFast(
            0,
            0,
            video::g_offscreen_surface_1,
            &copy_rect,
            DDBLTFAST_WAIT);
    }

    void __cdecl Allocate_Refresh_Lists_(uint32_t, int32_t entry_count) {
        const uint32_t allocation_size =
            static_cast<uint32_t>(entry_count) *
            static_cast<uint32_t>(sizeof(s_scan_list_entry));

        _refresh_scan_list =
            reinterpret_cast<s_scan_list_entry*>(allocate::Allocate_Space_(allocation_size));
        _refresh_stencil_list =
            reinterpret_cast<s_scan_list_entry*>(allocate::Allocate_Space_(allocation_size));

        if (entry_count > 0) {
            for (int32_t i = 0; i < entry_count; ++i) {
                _refresh_scan_list[i].min_x = static_cast<int16_t>(-1);
                _refresh_stencil_list[i].max_x = static_cast<int16_t>(-1);
            }
        }
    }
}