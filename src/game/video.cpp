#include "pch.h"

uint8_t DAT_00578820;
uint32_t DAT_00578dc8;
uint32_t DAT_00578dcc;

namespace video {
	int32_t g_screen_res_x;
	int32_t g_screen_res_y;
	s_page_buffer g_page_off = {};
	s_page_buffer g_page_back = {};
	s_page_buffer g_page_published = {};
	uint8_t* _video_memory;
	int16_t _current_mouse_list_ptr;
	int16_t _current_mouse_shape;
	uint8_t* _current_video_page;
	int32_t screen_refresh_flag;
	int16_t _old_screen_dump_flag;
	s_scan_list_entry* _refresh_scan_list;
	s_scan_list_entry* _refresh_stencil_list;
	uint8_t* _mouse_palette;
	uint32_t _screen_pixel_size = 0x4B000;
	uint8_t _glass_remap_colors[16][256];
	s_glass_color _glass_colors[16];
	tagPALETTEENTRY g_palette_entries[256];
	s_palette_entry _current_palette[256];
	uint8_t* _palette_list[65];
	uint8_t _palette_reds[256];
	uint16_t _is_initializing;
	s_mouse_list _default_mouse_list;
	uint8_t DAT_00578b58;
	int16_t _screen_window_y1;
	int16_t _screen_window_y2;
	int16_t _screen_window_x1;
	int16_t _screen_window_x2;
	SDL_Mutex* g_off_page_mutex;
	SDL_Condition* g_off_page_condition;
	uint8_t g_off_page_state;
	uint32_t g_off_page_generation;
	uint32_t g_last_presented_generation;

	void __cdecl Allocate_Page_Buffers_(int32_t width, int32_t height) {
		const size_t page_size = (size_t)width * (size_t)height;

		Free_Page_Buffers_();

		g_page_off.pixels = (uint8_t*)malloc(page_size);
		g_page_back.pixels = (uint8_t*)malloc(page_size);
		g_page_published.pixels = (uint8_t*)malloc(page_size);

		if (g_page_off.pixels == nullptr || g_page_back.pixels == nullptr || g_page_published.pixels == nullptr) {
			exitm::Exit_With_Message("Allocate_Page_Buffers_ failed");
		}

		memset(g_page_off.pixels, 0, page_size);
		memset(g_page_back.pixels, 0, page_size);
		memset(g_page_published.pixels, 0, page_size);

		g_page_off.width = width;
		g_page_off.height = height;
		g_page_back.width = width;
		g_page_back.height = height;
		g_page_published.width = width;
		g_page_published.height = height;
	}

	void __cdecl Free_Page_Buffers_() {
		if (g_page_off.pixels != nullptr) {
			free(g_page_off.pixels);
			g_page_off.pixels = nullptr;
		}

		if (g_page_back.pixels != nullptr) {
			free(g_page_back.pixels);
			g_page_back.pixels = nullptr;
		}

		if (g_page_published.pixels != nullptr) {
			free(g_page_published.pixels);
			g_page_published.pixels = nullptr;
		}

		g_page_off.width = 0;
		g_page_off.height = 0;
		g_page_back.width = 0;
		g_page_back.height = 0;
		g_page_published.width = 0;
		g_page_published.height = 0;
	}

	uint8_t* __cdecl Get_Page_Pixels_(int32_t page_index) {
		if (page_index == 1) {
			return g_page_off.pixels;
		}

		if (page_index == 2) {
			return g_page_back.pixels;
		}

		return nullptr;
	}

	uint8_t* __cdecl Get_Selected_Page_Pixels_() {
		return Get_Page_Pixels_(platform::g_render_buffer);
	}

	void __cdecl Init_Off_Page_Ownership_() {
		if (g_off_page_mutex == nullptr) {
			g_off_page_mutex = SDL_CreateMutex();
		}

		if (g_off_page_condition == nullptr) {
			g_off_page_condition = SDL_CreateCondition();
		}

		g_off_page_state = OFF_PAGE_WRITABLE;
		g_off_page_generation = 0;
		g_last_presented_generation = 0;
	}

	void __cdecl Shutdown_Off_Page_Ownership_() {
		if (g_off_page_condition != nullptr) {
			SDL_DestroyCondition(g_off_page_condition);
			g_off_page_condition = nullptr;
		}

		if (g_off_page_mutex != nullptr) {
			SDL_DestroyMutex(g_off_page_mutex);
			g_off_page_mutex = nullptr;
		}

		g_off_page_state = OFF_PAGE_WRITABLE;
		g_off_page_generation = 0;
		g_last_presented_generation = 0;
	}

	void __cdecl Publish_Off_Page_() {
		if (g_off_page_mutex == nullptr || g_off_page_condition == nullptr) {
			return;
		}

		SDL_LockMutex(g_off_page_mutex);
		while (g_off_page_state != OFF_PAGE_WRITABLE) {
			SDL_WaitCondition(g_off_page_condition, g_off_page_mutex);
		}

		const size_t page_size = (size_t)video::g_screen_res_x * (size_t)video::g_screen_res_y;
		memcpy(g_page_published.pixels, g_page_off.pixels, page_size);
		g_off_page_state = OFF_PAGE_PUBLISHED;
		++g_off_page_generation;
		SDL_UnlockMutex(g_off_page_mutex);
	}

	bool __cdecl Try_Begin_Off_Page_Present_() {
		if (g_off_page_mutex == nullptr) {
			return false;
		}

		bool should_present = false;
		SDL_LockMutex(g_off_page_mutex);
		if (g_off_page_state == OFF_PAGE_PUBLISHED &&
			g_off_page_generation != g_last_presented_generation) {
			g_off_page_state = OFF_PAGE_PRESENTING;
			g_last_presented_generation = g_off_page_generation;
			should_present = true;
		}
		SDL_UnlockMutex(g_off_page_mutex);

		return should_present;
	}

	void __cdecl End_Off_Page_Present_() {
		if (g_off_page_mutex == nullptr || g_off_page_condition == nullptr) {
			return;
		}

		SDL_LockMutex(g_off_page_mutex);
		g_off_page_state = OFF_PAGE_WRITABLE;
		SDL_SignalCondition(g_off_page_condition);
		SDL_UnlockMutex(g_off_page_mutex);
	}

	void Set_Page_Off_() {
		platform::g_render_buffer = 1;
		_current_video_page = g_page_off.pixels;
	}

	void Set_Refresh_Stencil_() {
		screen_refresh_flag = 3;
	}

	void Set_Refresh_Full_() {
		screen_refresh_flag = 0;
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
	}

	int16_t __cdecl Get_Refresh_Mode_() {
		return (int16_t)screen_refresh_flag;
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

	void __cdecl Add_Dot_To_Scan_List_(int32_t x, int32_t y) {
		int32_t scaled_x32 = x >> 2;
		int16_t scaled_x = (int16_t)scaled_x32;
		s_scan_list_entry* scan_entry = &_refresh_scan_list[y];

		if (screen_refresh_flag == 2) {
			if (scan_entry->min_x == -1) {
				scan_entry->min_x = scaled_x;
				scan_entry->max_x = scaled_x;
				return;
			}
			if (scan_entry->min_x > scaled_x32) {
				scan_entry->min_x = scaled_x;
				return;
			}
			if (scan_entry->max_x < scaled_x32) {
				scan_entry->max_x = scaled_x;
			}
			return;
		}

		if (scan_entry->min_x == -1) {
			scan_entry->min_x = scaled_x;
			scan_entry->max_x = scaled_x;
		}
		else if (scan_entry->min_x > scaled_x32) {
			scan_entry->min_x = scaled_x;
		}
		else if (scan_entry->max_x < scaled_x32) {
			scan_entry->max_x = scaled_x;
		}

		s_scan_list_entry* stencil_entry = &_refresh_stencil_list[y];

		if (stencil_entry->min_x == -1) {
			stencil_entry->min_x = scaled_x;
			stencil_entry->max_x = scaled_x;
			return;
		}
		if (stencil_entry->min_x > scaled_x32) {
			stencil_entry->min_x = scaled_x;
			return;
		}
		if (stencil_entry->max_x < scaled_x32) {
			stencil_entry->max_x = scaled_x;
		}
	}

    void __cdecl Toggle_Pages_() {
        platform::g_pending_toggle_pages = true;
        fields::Draw_Visible_Fields_();
        mouse::Disable_Mouse_();

        g_cached_mouse_x = platform::g_mouse_x;
        g_cached_mouse_y = platform::g_mouse_y;

        if (g_window_focus_state != 0) {
            palette::Refresh_Palette_();
            mouse::Enable_Mouse_();

            // Snapshot the completed Off page before any legacy stencil restore
            // mutates it to prepare the next incremental frame.
            video::Publish_Off_Page_();

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

	void __cdecl Clear_Stencil_List_() {
		const uint32_t scanline_count = g_screen_res_y & 0x3FFFFFFFu;
		memset(
			video::_refresh_stencil_list,
			0xFF,
			(size_t)scanline_count * sizeof(video::_refresh_stencil_list[0]));
	}

	void __cdecl Clear_Scan_List_() {
		const uint32_t scan_line_count = g_screen_res_y & 0x3FFFFFFFu;
		memset(
			video::_refresh_scan_list,
			0xFF,
			(size_t)scan_line_count * sizeof(video::_refresh_scan_list[0]));
	}

	void __cdecl Save_Refresh_() {
		video::_old_screen_dump_flag = video::screen_refresh_flag;
	}

	void __cdecl Restore_Refresh_() {
		video::screen_refresh_flag = video::_old_screen_dump_flag;
	}

	void __cdecl Set_Refresh_Differential_No_Full_() {
		video::screen_refresh_flag = 2;
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

        if (g_screen_res_y > 0) {
            int32_t dst_row_offset = 0;
            int32_t src_row_offset = 0;

            for (int32_t y = 0; y < g_screen_res_y; ++y) {
                const s_scan_list_entry& scan = video::_refresh_scan_list[y];
                if (scan.min_x != -1) {
                    const int32_t min_x = scan.min_x;
                    const int32_t count = (int32_t)scan.max_x - min_x + 1;

                    platform::Copy_Dwords_(
                        video_memory + (dst_row_offset + min_x),
                        alt_surface + (src_row_offset + min_x),
                        (uint32_t)count);
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
                const int32_t count = (int32_t)scan.max_x - min_x + 1;
                const int32_t dword_offset = row_dword_offset + min_x;
                platform::Copy_Dwords_(
                    alt_offscreen_surface + dword_offset,
                    (uint32_t*)(offscreen_surface + dword_offset * (int32_t)sizeof(*alt_offscreen_surface)),
                    (uint32_t)count
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
            (int16_t)((uint16_t)g_screen_res_x - 1);
        video::_default_mouse_list.y2 =
            (int16_t)((uint16_t)g_screen_res_y - 1);

        mouse::Set_Mouse_List_(&video::_default_mouse_list, 1);

        platform::Force_Cursor_Pos(g_screen_res_x / 2, g_screen_res_y / 2);

        video::_is_initializing = 0;
        mouse::_mouse_enabled = 1;
    }

    void __cdecl Set_Page_Back_() {
        platform::g_render_buffer = 2;
        _current_video_page = g_page_back.pixels;
    }

    void __cdecl Copy_Back_To_Off_() {
        const size_t page_size = (size_t)video::g_screen_res_x * (size_t)video::g_screen_res_y;
        memcpy(video::g_page_off.pixels, video::g_page_back.pixels, page_size);
    }

    void __cdecl Copy_Off_To_Back_() {
        const size_t page_size = (size_t)video::g_screen_res_x * (size_t)video::g_screen_res_y;
        memcpy(video::g_page_back.pixels, video::g_page_off.pixels, page_size);
    }

    void __cdecl Allocate_Refresh_Lists_(uint32_t unused, int32_t entry_count) {
        (void)unused;
        const uint32_t allocation_size =
            (uint32_t)entry_count *
            (uint32_t)sizeof(s_scan_list_entry);

        _refresh_scan_list =
            (s_scan_list_entry*)allocate::Allocate_Space_(allocation_size);
        _refresh_stencil_list =
            (s_scan_list_entry*)allocate::Allocate_Space_(allocation_size);

        if (entry_count > 0) {
            for (int32_t i = 0; i < entry_count; ++i) {
                _refresh_scan_list[i].min_x = (int16_t)-1;
                _refresh_stencil_list[i].max_x = (int16_t)-1;
            }
        }
    }
}