#pragma once

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) || defined(__ppc64__) || defined(__LP64__)
#define ORION2RE_64BIT_BUILD 1
#else
#define ORION2RE_64BIT_BUILD 0
#endif

#ifndef ORION2RE_ENABLE_FPS_COUNTER
#define ORION2RE_ENABLE_FPS_COUNTER 1
#endif

extern uint8_t g_ten_game_number;
extern int16_t g_cached_mouse_x;
extern int16_t g_cached_mouse_y;
extern int32_t g_window_focus_state;
extern uint32_t g_last_scan_code;

namespace platform {
	// Variables
	extern uint32_t g_os_ver;
	extern uint32_t g_win_minor;
	extern uint32_t g_win_major;
	extern uint32_t g_win_ver;
	extern char* g_command_line;
	extern void* g_env_strings;
	extern SDL_Thread* g_worker_thread_handle;
	extern uint32_t _updating_cursor_img;
	extern uint8_t g_should_exit;

	extern void* g_main_window;

	extern volatile bool g_pending_toggle_pages;

	extern int16_t g_video_mode;

	extern uint32_t g_available_mem;

	extern uint8_t* g_present_surface;

	extern uint8_t* g_cursor_surface_1;

	extern uint8_t* _cursor_surface;

	extern uint32_t g_frame_count;

	extern int8_t g_render_buffer;

	extern uint8_t* _offscreen_surface;

	extern int32_t g_last_cursor_shape;

	extern int16_t g_mouse_x;

	extern int16_t g_mouse_y;

	// Functions
	bool Create_Video_Buffers_(int16_t video_mode);
	void __cdecl Service_Pending_Window_Events_();
	void __cdecl Pump_Game_Input_Queue_();
	bool __cdecl Update_Active_World_Frame_();
	uint32_t Lock_Appropriate_Render_Buffer();
	void Unlock_Appropriate_Render_Buffer();
	void Copy_Off_Page_To_Present_Surface_();
	void Force_Cursor_Pos(int16_t x, int16_t y);
	void __cdecl Create_Cursor_Surface();

	int32_t __cdecl Lock_Offscreen_Surface_();
	void __cdecl Unlock_Offscreen_Surface_();
	void Cleanup_Surfaces();
	void Destroy_Main_Window_();
	void __cdecl Prepare_Exit_();
	void __cdecl Update_Cursor_Image_();
	void __cdecl Toggle_Pages_Stub_();
	int Run_Main_Loop(const char* command_line);
	void __cdecl Copy_Dwords_(uint32_t* destination, const uint32_t* source, uint32_t dword_count);
}