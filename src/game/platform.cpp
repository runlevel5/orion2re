#include "pch.h"

uint8_t g_ten_game_number;
int16_t g_cached_mouse_x;
int16_t g_cached_mouse_y;
int32_t g_window_focus_state;
uint32_t g_last_scan_code;

namespace platform {
	namespace {
		bool g_has_lost_focus;
		SDL_Window* g_sdl_window;
		SDL_Renderer* g_sdl_renderer;
		SDL_Texture* g_sdl_present_texture;
		uint32_t* g_sdl_present_pixels;
		uint32_t g_fps_window_start_ms;
		uint32_t g_fps_frame_count;

		void Present_Indexed_Buffer_(const uint8_t* src_pixels);
		void Composite_Cursor_On_Present_Pixels_();

		int16_t Round_Float_To_Int16_(float value) {
			long rounded_value = lroundf(value);
			if (rounded_value < INT16_MIN) {
				return INT16_MIN;
			}
			if (rounded_value > INT16_MAX) {
				return INT16_MAX;
			}

			return (int16_t)rounded_value;
		}

		int16_t Map_Window_Coord_To_Game_Coord_(float coord, int32_t window_extent, int32_t game_extent) {
			if (window_extent <= 1 || game_extent <= 1) {
				return 0;
			}

			float scaled = (coord * (float)(game_extent - 1)) / (float)(window_extent - 1);
			long rounded_value = lroundf(scaled);
			if (rounded_value < 0) {
				return 0;
			}
			if (rounded_value > game_extent - 1) {
				return (int16_t)(game_extent - 1);
			}

			return (int16_t)rounded_value;
		}

		float Map_Game_Coord_To_Window_Coord_(int16_t coord, int32_t game_extent, int32_t window_extent) {
			if (window_extent <= 1 || game_extent <= 1) {
				return 0.0f;
			}

			return ((float)coord * (float)(window_extent - 1)) / (float)(game_extent - 1);
		}

		[[noreturn]] void Force_Immediate_Process_Exit_() {
			ExitProcess(0);
			abort();
		}

		void Fill_Rect_32_(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
			if (g_sdl_present_pixels == nullptr) {
				return;
			}

			for (int32_t row = 0; row < height; ++row) {
				const int32_t dst_y = y + row;
				if (dst_y < 0 || dst_y >= video::g_screen_res_y) {
					continue;
				}

				for (int32_t col = 0; col < width; ++col) {
					const int32_t dst_x = x + col;
					if (dst_x < 0 || dst_x >= video::g_screen_res_x) {
						continue;
					}

					g_sdl_present_pixels[dst_y * video::g_screen_res_x + dst_x] = color;
				}
			}
		}

		void Draw_Glyph_3x5_(int32_t x, int32_t y, char glyph, uint32_t color) {
			static const uint8_t k_digits[10][5] = {
				{0x7, 0x5, 0x5, 0x5, 0x7},
				{0x2, 0x6, 0x2, 0x2, 0x7},
				{0x7, 0x1, 0x7, 0x4, 0x7},
				{0x7, 0x1, 0x7, 0x1, 0x7},
				{0x5, 0x5, 0x7, 0x1, 0x1},
				{0x7, 0x4, 0x7, 0x1, 0x7},
				{0x7, 0x4, 0x7, 0x5, 0x7},
				{0x7, 0x1, 0x1, 0x1, 0x1},
				{0x7, 0x5, 0x7, 0x5, 0x7},
				{0x7, 0x5, 0x7, 0x1, 0x7},
			};
			static const uint8_t k_f[5] = {0x7, 0x4, 0x6, 0x4, 0x4};
			static const uint8_t k_p[5] = {0x6, 0x5, 0x6, 0x4, 0x4};
			static const uint8_t k_s[5] = {0x7, 0x4, 0x7, 0x1, 0x7};
			static const uint8_t k_colon[5] = {0x0, 0x2, 0x0, 0x2, 0x0};
			const uint8_t* rows = nullptr;

			if (glyph >= '0' && glyph <= '9') {
				rows = k_digits[glyph - '0'];
			} else if (glyph == 'F') {
				rows = k_f;
			} else if (glyph == 'P') {
				rows = k_p;
			} else if (glyph == 'S') {
				rows = k_s;
			} else if (glyph == ':') {
				rows = k_colon;
			}

			if (rows == nullptr) {
				return;
			}

			for (int32_t row = 0; row < 5; ++row) {
				for (int32_t col = 0; col < 3; ++col) {
					if ((rows[row] & (1 << (2 - col))) == 0) {
						continue;
					}

					const int32_t dst_x = x + col;
					const int32_t dst_y = y + row;
					if (dst_x < 0 || dst_x >= video::g_screen_res_x || dst_y < 0 || dst_y >= video::g_screen_res_y) {
						continue;
					}

					g_sdl_present_pixels[dst_y * video::g_screen_res_x + dst_x] = color;
				}
			}
		}

		void Draw_Text_3x5_(int32_t x, int32_t y, const char* text, uint32_t color) {
			if (text == nullptr) {
				return;
			}

			for (int32_t i = 0; text[i] != '\0'; ++i) {
				Draw_Glyph_3x5_(x + i * 4, y, text[i], color);
			}
		}

		void Queue_Key_Code_(uint32_t key_code, uint32_t scan_code) {
			key::_key_pressed = true;
			key::_last_key_pressed[key::_key_write] = (int32_t)key_code;
			key::_key_write = (key::_key_write + 1) % 10;
			g_last_scan_code = scan_code;
		}

		bool Translate_Non_Character_Key_(SDL_Keycode keycode, uint32_t* translated_key_code) {
			switch (keycode) {
			case SDLK_BACKSPACE: *translated_key_code = 0x0E08; return true;
			case SDLK_TAB: *translated_key_code = 0x0F09; return true;
			case SDLK_RETURN: *translated_key_code = 0x1C0D; return true;
			case SDLK_PAGEUP: *translated_key_code = 0x49E0; return true;
			case SDLK_PAGEDOWN: *translated_key_code = 0x51E0; return true;
			case SDLK_END: *translated_key_code = 0x4FE0; return true;
			case SDLK_HOME: *translated_key_code = 0x47E0; return true;
			case SDLK_LEFT: *translated_key_code = 0x4BE0; return true;
			case SDLK_UP: *translated_key_code = 0x48E0; return true;
			case SDLK_RIGHT: *translated_key_code = 0x4DE0; return true;
			case SDLK_DOWN: *translated_key_code = 0x50E0; return true;
			case SDLK_INSERT: *translated_key_code = 0x52E0; return true;
			case SDLK_DELETE: *translated_key_code = 0x10000; return true;
			case SDLK_KP_PLUS: *translated_key_code = 0x0D2B; return true;
			case SDLK_KP_MINUS: *translated_key_code = 0x0C2D; return true;
			case SDLK_F1: *translated_key_code = 0x3B00; return true;
			case SDLK_F2: *translated_key_code = 0x3C00; return true;
			case SDLK_F3: *translated_key_code = 0x3D00; return true;
			case SDLK_F4: *translated_key_code = 0x3E00; return true;
			case SDLK_F5: *translated_key_code = 0x3F00; return true;
			case SDLK_F6: *translated_key_code = 0x4000; return true;
			case SDLK_F7: *translated_key_code = 0x4100; return true;
			case SDLK_F8: *translated_key_code = 0x4200; return true;
			case SDLK_F9: *translated_key_code = 0x4300; return true;
			case SDLK_F10: *translated_key_code = 0x4400; return true;
			case SDLK_F11: *translated_key_code = 0xFFFF8500; return true;
			case SDLK_F12: *translated_key_code = 0xFFFF8600; return true;
			default:
				return false;
			}
		}

		bool Translate_Alt_Key_(SDL_Keycode keycode, uint32_t* translated_key_code) {
			switch (keycode) {
			case SDLK_A: *translated_key_code = 0x1E00; return true;
			case SDLK_B: *translated_key_code = 0x3000; return true;
			case SDLK_C: *translated_key_code = 0x2E00; return true;
			case SDLK_D: *translated_key_code = 0x2000; return true;
			case SDLK_E: *translated_key_code = 0x1200; return true;
			case SDLK_F: *translated_key_code = 0x2100; return true;
			case SDLK_G: *translated_key_code = 0x2200; return true;
			case SDLK_H: *translated_key_code = 0x2300; return true;
			case SDLK_I: *translated_key_code = 0x1700; return true;
			case SDLK_J: *translated_key_code = 0x2400; return true;
			case SDLK_K: *translated_key_code = 0x2500; return true;
			case SDLK_L: *translated_key_code = 0x2600; return true;
			case SDLK_M: *translated_key_code = 0x3200; return true;
			case SDLK_N: *translated_key_code = 0x3100; return true;
			case SDLK_O: *translated_key_code = 0x1800; return true;
			case SDLK_P: *translated_key_code = 0x1900; return true;
			case SDLK_Q: *translated_key_code = 0x1000; return true;
			case SDLK_R: *translated_key_code = 0x1300; return true;
			case SDLK_S: *translated_key_code = 0x1F00; return true;
			case SDLK_T: *translated_key_code = 0x1400; return true;
			case SDLK_U: *translated_key_code = 0x1600; return true;
			case SDLK_V: *translated_key_code = 0x2F00; return true;
			case SDLK_W: *translated_key_code = 0x1100; return true;
			case SDLK_X: *translated_key_code = 0x2D00; return true;
			case SDLK_Y: *translated_key_code = 0x1500; return true;
			case SDLK_Z: *translated_key_code = 0x2C00; return true;
			case SDLK_F1: *translated_key_code = 0x6800; return true;
			case SDLK_F2: *translated_key_code = 0x6900; return true;
			case SDLK_F3: *translated_key_code = 0x6A00; return true;
			case SDLK_F4: *translated_key_code = 0x6B00; return true;
			case SDLK_F5: *translated_key_code = 0x6C00; return true;
			case SDLK_F6: *translated_key_code = 0x6D00; return true;
			case SDLK_F7: *translated_key_code = 0x6E00; return true;
			case SDLK_F8: *translated_key_code = 0x6F00; return true;
			case SDLK_F9: *translated_key_code = 0x7000; return true;
			case SDLK_F10: *translated_key_code = 0x7100; return true;
			default:
				return false;
			}
		}

		bool Translate_Printable_Key_(SDL_Keycode keycode, SDL_Keymod modifiers, uint32_t* translated_character) {
			const bool shift_down = (modifiers & SDL_KMOD_SHIFT) != 0;

			if (keycode >= SDLK_A && keycode <= SDLK_Z) {
				*translated_character = shift_down ? (uint32_t)(keycode - 0x20) : (uint32_t)keycode;
				return true;
			}

			if (!shift_down) {
				if (keycode >= 0x20 && keycode <= 0x7E) {
					*translated_character = (uint32_t)keycode;
					return true;
				}
				return false;
			}

			switch (keycode) {
			case SDLK_1: *translated_character = '!'; return true;
			case SDLK_2: *translated_character = '@'; return true;
			case SDLK_3: *translated_character = '#'; return true;
			case SDLK_4: *translated_character = '$'; return true;
			case SDLK_5: *translated_character = '%'; return true;
			case SDLK_6: *translated_character = '^'; return true;
			case SDLK_7: *translated_character = '&'; return true;
			case SDLK_8: *translated_character = '*'; return true;
			case SDLK_9: *translated_character = '('; return true;
			case SDLK_0: *translated_character = ')'; return true;
			case SDLK_MINUS: *translated_character = '_'; return true;
			case SDLK_EQUALS: *translated_character = '+'; return true;
			case SDLK_LEFTBRACKET: *translated_character = '{'; return true;
			case SDLK_RIGHTBRACKET: *translated_character = '}'; return true;
			case SDLK_BACKSLASH: *translated_character = '|'; return true;
			case SDLK_SEMICOLON: *translated_character = ':'; return true;
			case SDLK_APOSTROPHE: *translated_character = '"'; return true;
			case SDLK_COMMA: *translated_character = '<'; return true;
			case SDLK_PERIOD: *translated_character = '>'; return true;
			case SDLK_SLASH: *translated_character = '?'; return true;
			case SDLK_GRAVE: *translated_character = '~'; return true;
			default:
				if (keycode >= 0x20 && keycode <= 0x7E) {
					*translated_character = (uint32_t)keycode;
					return true;
				}
				return false;
			}
		}

		void Handle_SDL_Key_Down_(const SDL_KeyboardEvent& key_event) {
			if (key_event.repeat) {
				return;
			}

			uint32_t translated_key_code = 0;
			const uint32_t scan_code = key_event.raw;

			if ((key_event.mod & SDL_KMOD_ALT) != 0) {
				if (Translate_Alt_Key_(key_event.key, &translated_key_code)) {
					Queue_Key_Code_(translated_key_code, scan_code);
				}
				return;
			}

			if (Translate_Non_Character_Key_(key_event.key, &translated_key_code)) {
				Queue_Key_Code_(translated_key_code, scan_code);
				return;
			}

			if (Translate_Printable_Key_(key_event.key, key_event.mod, &translated_key_code)) {
				Queue_Key_Code_(translated_key_code + (scan_code << 8), scan_code);
			}
		}

		void Present_Published_Off_Page_() {
			Update_Active_World_Frame_();
			Present_Indexed_Buffer_(g_present_surface);
		}

		bool Ensure_Present_Backend_() {
			if (g_sdl_renderer != nullptr && g_sdl_present_texture != nullptr && g_sdl_present_pixels != nullptr) {
				return true;
			}

			g_sdl_renderer = SDL_CreateRenderer(g_sdl_window, nullptr);
			if (g_sdl_renderer == nullptr) {
				g_sdl_renderer = SDL_CreateRenderer(g_sdl_window, "software");
			}
			if (g_sdl_renderer == nullptr) {
				return false;
			}

			g_sdl_present_texture = SDL_CreateTexture(
				g_sdl_renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				video::g_screen_res_x,
				video::g_screen_res_y);
			if (g_sdl_present_texture == nullptr) {
				return false;
			}

			g_sdl_present_pixels = (uint32_t*)malloc(
				(size_t)video::g_screen_res_x *
				(size_t)video::g_screen_res_y *
				sizeof(uint32_t));
			return g_sdl_present_pixels != nullptr;
		}

		void Destroy_Present_Backend_() {
			if (g_sdl_present_texture != nullptr) {
				SDL_DestroyTexture(g_sdl_present_texture);
				g_sdl_present_texture = nullptr;
			}

			if (g_sdl_renderer != nullptr) {
				SDL_DestroyRenderer(g_sdl_renderer);
				g_sdl_renderer = nullptr;
			}

			if (g_sdl_present_pixels != nullptr) {
				free(g_sdl_present_pixels);
				g_sdl_present_pixels = nullptr;
			}
		}

		void Present_Indexed_Buffer_(const uint8_t* src_pixels) {
			if (src_pixels == nullptr || !Ensure_Present_Backend_()) {
				return;
			}

			const int32_t pixel_count = video::g_screen_res_x * video::g_screen_res_y;
			for (int32_t i = 0; i < pixel_count; ++i) {
				const tagPALETTEENTRY& pal = video::g_palette_entries[src_pixels[i]];
				g_sdl_present_pixels[i] =
					0xFF000000u |
					((uint32_t)pal.peRed << 16) |
					((uint32_t)pal.peGreen << 8) |
					(uint32_t)pal.peBlue;
			}

			Composite_Cursor_On_Present_Pixels_();

#if ORION2RE_ENABLE_FPS_COUNTER
			if (g_sdl_window != nullptr) {
				const uint32_t now_ms = (uint32_t)SDL_GetTicks();
				if (g_fps_window_start_ms == 0) {
					g_fps_window_start_ms = now_ms;
				}

				++g_fps_frame_count;

				static uint32_t s_displayed_fps_value = 0;
				const uint32_t elapsed_ms = now_ms - g_fps_window_start_ms;
				if (elapsed_ms >= 1000) {
					s_displayed_fps_value = (g_fps_frame_count * 1000u) / elapsed_ms;
					g_fps_window_start_ms = now_ms;
					g_fps_frame_count = 0;
				}

				char fps_text[16];
				snprintf(fps_text, sizeof(fps_text), "FPS:%u", s_displayed_fps_value);
				Fill_Rect_32_(4, 4, 4 * (int32_t)strlen(fps_text) + 4, 9, 0xC0000000u);
				Draw_Text_3x5_(6, 6, fps_text, 0xFFFFFFFFu);
			}
#endif

			SDL_UpdateTexture(
				g_sdl_present_texture,
				nullptr,
				g_sdl_present_pixels,
				video::g_screen_res_x * (int32_t)sizeof(uint32_t));

			SDL_RenderClear(g_sdl_renderer);
			SDL_RenderTexture(g_sdl_renderer, g_sdl_present_texture, nullptr, nullptr);
			SDL_RenderPresent(g_sdl_renderer);
		}

		void Composite_Cursor_On_Present_Pixels_() {
			if (g_sdl_present_pixels == nullptr ||
				mouse::_mouse_enabled == 0 ||
				video::_current_mouse_shape <= 0) {
				return;
			}

			if ((int32_t)video::_current_mouse_shape != DAT_578B3C) {
				Update_Cursor_Image_();
				DAT_578B3C = (int32_t)video::_current_mouse_shape;
			}

			const int32_t offset = mouse::Get_Pointer_Offset_();
			const int32_t dst_x0 = (int32_t)g_mouse_x + offset;
			const int32_t dst_y0 = (int32_t)g_mouse_y + offset;

			for (int32_t row = 0; row < 24; ++row) {
				const int32_t dst_y = dst_y0 + row;
				if (dst_y < 0 || dst_y >= video::g_screen_res_y) {
					continue;
				}

				for (int32_t col = 0; col < 24; ++col) {
					const int32_t dst_x = dst_x0 + col;
					if (dst_x < 0 || dst_x >= video::g_screen_res_x) {
						continue;
					}

					const uint8_t pixel = g_cursor_surface_1[row * 24 + col];
					if (pixel == 0) {
						continue;
					}

					const tagPALETTEENTRY& pal = video::g_palette_entries[pixel];
					g_sdl_present_pixels[dst_y * video::g_screen_res_x + dst_x] =
						0xFF000000u |
						((uint32_t)pal.peRed << 16) |
						((uint32_t)pal.peGreen << 8) |
						(uint32_t)pal.peBlue;
				}
			}
		}
	}

	uint32_t g_os_ver;
	uint32_t g_win_minor;
	uint32_t g_win_major;
	uint32_t g_win_ver;
	char* g_command_line;
	void* g_env_strings;
	SDL_Thread* g_worker_thread_handle;
	uint32_t _updating_cursor_img;
	uint8_t g_should_exit;
	HWND g_main_window;
	volatile bool g_pending_toggle_pages;
	int16_t g_video_mode;
	uint32_t g_available_mem;
	uint8_t* g_present_surface;
	uint8_t* g_cursor_surface_1;
	uint8_t* _cursor_surface;
	uint32_t g_frame_count;
	int8_t g_render_buffer;
	uint8_t* _offscreen_surface;
	int32_t DAT_578B3C;
	int16_t g_mouse_x;
	int16_t g_mouse_y;

    constexpr int32_t k_windowed_logical_width = 640;
    constexpr int32_t k_windowed_logical_height = 480;

    void Parse_Ten_Cmd_Parameter_(const char* input_str) {
        if (input_str == nullptr) {
            return;
        }

        uint32_t input_len = 0;
        while (input_str[input_len] != '\0') {
            input_len++;
        }

        if (input_len == 0) {
            return;
        }

        char* copy_buffer = (char*)malloc(input_len + 4);
        if (copy_buffer == nullptr) {
            return;
        }

        for (uint32_t i = 0; i <= input_len; ++i) {
            copy_buffer[i] = input_str[i];
        }

        _strlwr(copy_buffer);

        // Search for "/ten=" in the lowercased command line.
        char* match_ptr = strstr(copy_buffer, "/ten=");
        if (match_ptr != nullptr) {
            char* target = match_ptr + (sizeof("/ten=") - 1);
            target[1] = '\0';

            int parsed_val = atoi(target);
            ::g_ten_game_number = (uint8_t)parsed_val;
        }

        free(copy_buffer);
    }

    bool cleanup() {
        Prepare_Exit_();
        Destroy_Main_Window_();
        return false;
    }

    bool Create_Video_Buffers_(int16_t video_mode) {
        MEMORYSTATUS mem_status;
        mem_status.dwLength = sizeof(MEMORYSTATUS);
        GlobalMemoryStatus(&mem_status);

        g_video_mode = video_mode;
        g_available_mem = mem_status.dwAvailPhys;

        switch (video_mode) {
        case 0:
            video::g_screen_res_x = 320;
            video::g_screen_res_y = 200;
            break;
        case 2:
            video::g_screen_res_x = 320;
            video::g_screen_res_y = 240;
            break;
        case 4:
            video::g_screen_res_x = 640;
            video::g_screen_res_y = 480;
            break;
        case 5:
            video::g_screen_res_x = k_windowed_logical_width;
            video::g_screen_res_y = k_windowed_logical_height;
        default:
            break;
        }

        video::_screen_pixel_size = video::g_screen_res_x * video::g_screen_res_y;
        video::Allocate_Refresh_Lists_(video::_screen_pixel_size, video::g_screen_res_y);
        video::Allocate_Page_Buffers_(video::g_screen_res_x, video::g_screen_res_y);
        video::Init_Off_Page_Ownership_();

        g_present_surface = (uint8_t*)malloc((size_t)video::g_screen_res_x * (size_t)video::g_screen_res_y);
        if (g_present_surface == nullptr) {
            return cleanup();
        }

        memset(g_present_surface, 0, (size_t)video::g_screen_res_x * (size_t)video::g_screen_res_y);

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);
        video::Set_Page_Back_();
        graphics::Fill_(0, 0, 639, 479, 0);
        video::Clear_Scan_List_();
        video::Clear_Stencil_List_();
        video::screen_refresh_flag = 0;
        graphics::Reset_Window_();
        return true;
    }

    void Restore_Surfaces()
    {
        g_frame_count = 0;
        if (video::screen_refresh_flag == 3)
        {
            video::Set_Refresh_Stencil_();
        }
    }

    void __cdecl Prepare_Exit_() {
        sound::Destroy_Sound_();
        netcode::Net_Uninit_();
        g_should_exit = 1;
    }

    void Cleanup_Surfaces()
    {
        if (g_present_surface != nullptr) {
            free(g_present_surface);
            g_present_surface = nullptr;
        }

        if (g_cursor_surface_1 != nullptr) {
            free(g_cursor_surface_1);
            g_cursor_surface_1 = nullptr;
        }

        video::Free_Page_Buffers_();
        video::Shutdown_Off_Page_Ownership_();
    }

    void Destroy_Main_Window_() {
        if (g_sdl_window != nullptr) {
            Destroy_Present_Backend_();
            SDL_DestroyWindow(g_sdl_window);
            g_sdl_window = nullptr;
            g_main_window = nullptr;
        }
    }

	bool __cdecl Update_Active_World_Frame_() {
		if (!video::Try_Begin_Off_Page_Present_()) {
			return false;
		}

		// Copy the published Off page into a separate presenter-owned surface.
		// The gameplay Back page remains a stable software page and is not part of present-page alternation.
		Copy_Off_Page_To_Present_Surface_();
		video::End_Off_Page_Present_();
		g_frame_count++;
		return true;
	}

    void __cdecl Service_Pending_Window_Events_() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                Force_Immediate_Process_Exit_();
            }

            if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                g_window_focus_state = 0;
                continue;
            }

            if (event.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
                g_window_focus_state = 1;
                SDL_HideCursor();
                continue;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                Handle_SDL_Key_Down_(event.key);
                continue;
            }

            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                int32_t window_width = 0;
                int32_t window_height = 0;
                SDL_GetWindowSize(g_sdl_window, &window_width, &window_height);
                mouse::User_Mouse_Handler_(
                    Map_Window_Coord_To_Game_Coord_(event.motion.x, window_width, video::g_screen_res_x),
                    Map_Window_Coord_To_Game_Coord_(event.motion.y, window_height, video::g_screen_res_y),
                    (uint16_t)mouse::_mouse_buttons);
                continue;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                uint16_t mouse_buttons = 0;
                int32_t window_width = 0;
                int32_t window_height = 0;
                SDL_GetWindowSize(g_sdl_window, &window_width, &window_height);

                if ((event.button.button == SDL_BUTTON_LEFT && event.button.down) ||
                    (event.button.button != SDL_BUTTON_LEFT && (mouse::_mouse_buttons & 1) != 0)) {
                    mouse_buttons |= 1;
                }

                if ((event.button.button == SDL_BUTTON_RIGHT && event.button.down) ||
                    (event.button.button != SDL_BUTTON_RIGHT && (mouse::_mouse_buttons & 2) != 0)) {
                    mouse_buttons |= 2;
                }

                mouse::User_Mouse_Handler_(
                    Map_Window_Coord_To_Game_Coord_(event.button.x, window_width, video::g_screen_res_x),
                    Map_Window_Coord_To_Game_Coord_(event.button.y, window_height, video::g_screen_res_y),
                    mouse_buttons);
                continue;
            }
        }

        if (g_window_focus_state == 0) {
            g_has_lost_focus = true;
        }

        if (g_has_lost_focus && g_window_focus_state == 1) {
            g_has_lost_focus = false;
            Restore_Surfaces();
        }
    }

    int32_t __cdecl Lock_Offscreen_Surface_() {
        platform::_offscreen_surface = video::g_page_back.pixels;
        return video::g_screen_res_x;
    }

    void __cdecl Unlock_Offscreen_Surface_() {
    }

    uint32_t Lock_Appropriate_Render_Buffer()
    {
        video::_current_video_page = video::Get_Selected_Page_Pixels_();
        return (uint32_t)video::g_screen_res_x;
    }


    void Unlock_Appropriate_Render_Buffer()
    {
    }

    void	Copy_Off_Page_To_Present_Surface_()
    {
        uint8_t* dst = g_present_surface;
        const uint8_t* src = video::g_page_published.pixels;
        for (int32_t y = 0; y < video::g_screen_res_y; ++y) {
            memcpy(dst + y * video::g_screen_res_x, src + y * video::g_screen_res_x, (size_t)video::g_screen_res_x);
        }
    }

    void Force_Cursor_Pos(int16_t x, int16_t y)
    {
        g_mouse_x = x;
        g_mouse_y = y;

        if (g_sdl_window != nullptr) {
            int32_t window_width = 0;
            int32_t window_height = 0;
            SDL_GetWindowSize(g_sdl_window, &window_width, &window_height);
            SDL_WarpMouseInWindow(
                g_sdl_window,
                Map_Game_Coord_To_Window_Coord_(x, video::g_screen_res_x, window_width),
                Map_Game_Coord_To_Window_Coord_(y, video::g_screen_res_y, window_height));
        }
    }

    void __cdecl Create_Cursor_Surface() {
        if (platform::g_cursor_surface_1 == nullptr) {
            platform::g_cursor_surface_1 = (uint8_t*)malloc(24 * 24);
        }

        if (platform::g_cursor_surface_1 == nullptr) {
            exitm::Exit_With_Message("Create_Cursor_Surface failed");
        }

        memset(platform::g_cursor_surface_1, 0, 24 * 24);
        platform::_cursor_surface = platform::g_cursor_surface_1;
    }

    void __cdecl Toggle_Pages_Stub_() {
        video::Toggle_Pages_();
    }

    int Run_Main_Loop(const char* command_line) {
        Parse_Ten_Cmd_Parameter_(command_line);

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            return 0;
        }

        g_should_exit = 0;
        g_pending_toggle_pages = false;
        g_window_focus_state = 1;
        g_has_lost_focus = false;
		g_fps_window_start_ms = 0;
		g_fps_frame_count = 0;

#ifdef FULLSCREEN
        int nWidth = k_windowed_logical_width;
        int nHeight = k_windowed_logical_height;
        SDL_WindowFlags window_flags = SDL_WINDOW_FULLSCREEN;
#else
        int nWidth = k_windowed_logical_width;
        int nHeight = k_windowed_logical_height;
        SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE;
#endif

        g_sdl_window = SDL_CreateWindow("Microprose", nWidth, nHeight, window_flags);
        if (g_sdl_window == nullptr) {
            SDL_Quit();
            return 0;
        }

        SDL_PropertiesID window_props = SDL_GetWindowProperties(g_sdl_window);
        g_main_window = (HWND)SDL_GetPointerProperty(
            window_props,
            SDL_PROP_WINDOW_WIN32_HWND_POINTER,
            nullptr);

        SDL_HideCursor();

        if (!Create_Video_Buffers_(4)) {
            Destroy_Main_Window_();
            SDL_Quit();
            return 0;
        }

        g_worker_thread_handle = SDL_CreateThread(MOX2::thunk_main2_, "MOX2::main2_", nullptr);
        if (g_worker_thread_handle == nullptr) {
            Cleanup_Surfaces();
            Destroy_Main_Window_();
            SDL_Quit();
            return 0;
        }

        while (g_should_exit != 1) {
            Service_Pending_Window_Events_();

            if (_updating_cursor_img == 1) {
                Update_Cursor_Image_();
                _updating_cursor_img = 0;
            }

            Present_Published_Off_Page_();
            SDL_Delay(1);
        }

        if (g_worker_thread_handle != nullptr) {
            SDL_WaitThread(g_worker_thread_handle, nullptr);
            g_worker_thread_handle = nullptr;
        }

        Cleanup_Surfaces();
        Destroy_Main_Window_();
        SDL_Quit();

        return 0;
    }

    void __cdecl Update_Cursor_Image_() {
        if (video::_current_mouse_shape > 0) {
            uint8_t* const src =
                &video::_mouse_palette[((int32_t)video::_current_mouse_shape - 1) * 0x240];

            for (int32_t row = 0; row < 0x18; ++row) {
                memcpy(&platform::_cursor_surface[row * 0x18], &src[row * 0x18], 0x18);
            }
        }
    }

    void __cdecl Copy_Dwords_(uint32_t* destination, const uint32_t* source, uint32_t dword_count) {
        memcpy(destination, source, dword_count * sizeof(uint32_t));
    }

}
