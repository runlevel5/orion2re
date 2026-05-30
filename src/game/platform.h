#pragma once

extern uint8_t g_ten_game_number;
extern int16_t g_cached_mouse_x;
extern int16_t g_cached_mouse_y;
extern int g_window_focus_state;
extern uint32_t g_last_scan_code;

namespace platform {
	// Variables
	extern uint32_t g_os_ver;
	extern uint32_t g_win_minor;
	extern uint32_t g_win_major;
	extern uint32_t g_win_ver;
	extern char* g_command_line;
	extern void* g_env_strings;
	extern MSG g_main_msg;
	extern HANDLE g_worker_thread_handle;
	using ::g_window_focus_state;
	extern uint32_t _updating_cursor_img;
	extern uint8_t g_should_exit;

	// Fixed string literal "/ten=" used for pattern matching.
	inline char* const s_slash_ten = reinterpret_cast<char*>(std::uintptr_t{ 0x0055b1c4 });

	extern LPDIRECTDRAW g_direct_draw;

	extern HWND g_main_window;

	extern bool g_pending_toggle_pages;

	extern int16_t g_video_mode;

	extern uint32_t g_available_mem;

	extern LPDIRECTDRAWSURFACE g_primary_surface;

	extern LPDIRECTDRAWSURFACE g_backbuffer_surface;

	extern LPDIRECTDRAWSURFACE g_cursor_surface_1;

	extern int32_t _offscreen_mouse_shape;

	extern LPDIRECTDRAWSURFACE g_cursor_surface_2;

	extern LPVOID _cursor_surface;

	extern uint32_t DAT_00578e20;

	extern uint16_t DAT_0057931c;

	extern uint8_t _need_quit;

	extern uint32_t g_frame_count;

	extern int8_t g_render_buffer;

	extern DDSURFACEDESC DDSURFACEDESC_00578e70;

	inline LONG& _surface_lock_flag =
		platform::DDSURFACEDESC_00578e70.lPitch;

	inline LPVOID& _current_locked_page =
		platform::DDSURFACEDESC_00578e70.lpSurface;

	extern uint8_t* _offscreen_surface;

	extern DDSURFACEDESC _cursor_surface_desc;

	extern int32_t DAT_578B3C;

	extern int16_t g_mouse_x;

	extern int16_t g_mouse_y;

	using ::g_cached_mouse_x;
	using ::g_cached_mouse_y;

	// Functions
	void __cdecl entry();
	void Parse_Ten_Cmd_Parameter(const char* input_str);
	HWND Create_Direct_Draw(int16_t video_mode);
	void __cdecl Flip_Surface();
	uint32_t Lock_Appropriate_Render_Buffer();
	HRESULT	Unlock_Appropriate_Render_Buffer();
	int32_t Lock_Primary_Surface();
	void Unlock_Primary_Surface();
	void Blit_To_Back_Buffer();
	void Draw_Cursor_XY(int32_t x, int32_t y);
	void Force_Cursor_Pos(int16_t x, int16_t y);
	void __cdecl Create_Cursor_Surface();
	HRESULT Restore_Surfaces();
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LONG __cdecl Lock_Offscreen_Surface_();
	void __cdecl Unlock_Offscreen_Surface_();
	void __cdecl Save_Mouse_(int16_t mouse_x, int16_t mouse_y);
	void __cdecl Save_Mouse_Offscreen_(int16_t mouse_x, int16_t mouse_y);
	void __cdecl Draw_Cursor_XY_Offscreen_(int16_t mouse_x, int16_t mouse_y);
	void __cdecl Restore_Mouse_Off_Page_();
	void Draw_Auto_Cursor();
	void Cleanup_Surfaces();
	void __cdecl Prepare_Exit_();
	void __cdecl Lock_Cursor_Surface_();
	void __cdecl Unlock_Cursor_Surface_();
	void __cdecl Update_Cursor_Image_();
	void __cdecl Toggle_Pages_Stub_();
	int __stdcall WinMain_Proxy(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd);
	void __cdecl Copy_Dwords_(uint32_t* destination, const uint32_t* source, uint32_t dword_count);

	extern uint32_t* _alt_offscreen_surface;

	int32_t __cdecl Lock_Alt_Offscreen_Surface_();
	void __cdecl Unlock_Alt_Offscreen_Surface_();
}