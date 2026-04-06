#pragma once

namespace platform {
	// Variables
	inline uint32_t& g_os_ver = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005617a4 });
	inline uint32_t& g_win_minor = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005617b0 });
	inline uint32_t& g_win_major = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005617ac });
	inline uint32_t& g_win_ver = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005617a8 });
	inline char*& g_command_line = *reinterpret_cast<char**>(std::uintptr_t{ 0x005a9cf4 });
	inline void*& g_env_strings = *reinterpret_cast<void**>(std::uintptr_t{ 0x005617f0 });
	inline uint8_t& g_parsed_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0055b1c0 });
	inline MSG& g_main_msg = *reinterpret_cast<MSG*>(std::uintptr_t{ 0x00579300 });
	inline HANDLE& g_worker_thread_handle = *reinterpret_cast<HANDLE*>(std::uintptr_t{ 0x0055B1BC });
	inline int32_t& g_window_focus_state = *reinterpret_cast<int32_t*>(std::uintptr_t{ 0x00579328 });
	inline uint8_t& g_should_exit = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0055B1A4 });

	// Fixed string literal "/ten" used for pattern matching.
	inline char* const s_slash_ten = reinterpret_cast<char*>(std::uintptr_t{ 0x0055b1c4 });

	inline LPDIRECTDRAW& g_direct_draw =
		*reinterpret_cast<LPDIRECTDRAW*>(std::uintptr_t{ 0x00578e04 });

	inline HWND& g_main_window =
		*reinterpret_cast<HWND*>(std::uintptr_t{ 0x00579320 });

	inline bool& g_pending_toggle_pages =
		*reinterpret_cast<bool*>(std::uintptr_t{ 0x005792FC });

	inline int16_t& g_video_mode =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578e5c });

	inline uint32_t& g_available_mem =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00578e10 });

	inline int32_t& g_screen_res_x =
		*reinterpret_cast<int32_t*>(std::uintptr_t{ 0x0055b1a8 });

	inline int32_t& g_screen_res_y =
		*reinterpret_cast<int32_t*>(std::uintptr_t{ 0x0055b1ac });

	inline LPDIRECTDRAWSURFACE& g_primary_surface =
		*reinterpret_cast<LPDIRECTDRAWSURFACE*>(std::uintptr_t{ 0x005792ec });

	inline LPDIRECTDRAWSURFACE& g_backbuffer_surface =
		*reinterpret_cast<LPDIRECTDRAWSURFACE*>(std::uintptr_t{ 0x00578e18 });

	inline LPDIRECTDRAWSURFACE& g_offscreen_surface_1 =
		*reinterpret_cast<LPDIRECTDRAWSURFACE*>(std::uintptr_t{ 0x00578e58 });

	inline LPDIRECTDRAWSURFACE& g_offscreen_surface_2 =
		*reinterpret_cast<LPDIRECTDRAWSURFACE*>(std::uintptr_t{ 0x00579334 });

	inline LPDIRECTDRAWSURFACE& g_cursor_surface_1 =
		*reinterpret_cast<LPDIRECTDRAWSURFACE*>(std::uintptr_t{ 0x00578810 });

	inline LPDIRECTDRAWSURFACE& g_cursor_surface_2 =
		*reinterpret_cast<LPDIRECTDRAWSURFACE*>(std::uintptr_t{ 0x00578814 });

	inline LPDIRECTDRAWPALETTE& g_palette =
		*reinterpret_cast<LPDIRECTDRAWPALETTE*>(std::uintptr_t{ 0x00578e64 });

	inline uint32_t& g_palette_entries =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00578ee8 });

	inline uint32_t& DAT_00578e20 =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00578e20 });

	inline uint32_t& DAT_00579350 =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00579350 });

	inline uint16_t& DAT_0057931c =
		*reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0057931c });

	inline uint8_t& DAT_00578e68 =
		*reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00578e68 });

	inline uint32_t& g_frame_count =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x055b1b8 });

	inline uint8_t& g_render_buffer =
		*reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00578edc });

	inline uint16_t& DAT_578AC4 =
		*reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578ac4 });

	inline uint32_t& DAT_578B3C =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00578b3c });

	inline int16_t& g_mouse_x =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578aa0 });

	inline int16_t& g_mouse_y =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578a9c });

	inline int16_t& g_cached_mouse_x =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578abc });

	inline int16_t& g_cached_mouse_y =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578ab8 });

	inline void*& g_surface_mem =
		*reinterpret_cast<void**>(std::uintptr_t{ 0x005792F8 });

	// Functions
	void __cdecl entry();
	void Parse_Ten_Cmd_Parameter(const char* input_str);
	HWND Create_Direct_Draw(int16_t video_mode);
	void __cdecl Flip_Surface();
	uint32_t Lock_Appropriate_Render_Buffer();
	HRESULT	Unlock_Appropriate_Render_Buffer();
	uint32_t Lock_Primary_Surface();
	void Unlock_Primary_Surface();
	void Blit_To_Back_Buffer();
	void Draw_Cursor_XY(int16_t x, int16_t y);
	void Force_Cursor_Pos(int16_t x, int16_t y);
	void Create_Cursor_Surfaces();
	HRESULT Restore_Surfaces();
	void Draw_Auto_Cursor();
	void Cleanup_Surfaces();
	inline void __cdecl Update_Cursor_Image_() {
		using Fn = void(__cdecl*)();
		reinterpret_cast<Fn>(std::uintptr_t{ 0x00526F80 })();
	}
	void __cdecl Toggle_Pages_Stub_();
	int __stdcall WinMain_Proxy(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd);
}