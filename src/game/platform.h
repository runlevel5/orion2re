#pragma once

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) || defined(__ppc64__) || defined(__LP64__)
#define ORION2RE_64BIT_BUILD 1
#else
#define ORION2RE_64BIT_BUILD 0
#endif

extern int16_t g_cached_mouse_x;
extern int16_t g_cached_mouse_y;
extern int32_t g_window_focus_state;
extern uint32_t g_last_scan_code;

namespace platform {
	constexpr bool k_keep_rendering_when_unfocused = true;
	constexpr int32_t k_exit_message_size = 256;

	struct process_exit_requested {};

	// Variables
	extern uint32_t g_os_ver;
	extern uint32_t g_win_minor;
	extern uint32_t g_win_major;
	extern uint32_t g_win_ver;
	extern char* g_command_line;
	extern void* g_env_strings;
	extern SDL_Thread* g_worker_thread_handle;
	extern uint32_t _updating_cursor_img;
	extern char g_exit_message[k_exit_message_size];

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

	// Functions
	bool __cdecl Init_Exit_();
	void __cdecl Shutdown_Exit_();
	void __cdecl Reset_Exit_();
	void __cdecl Request_Exit_(const char* message = nullptr);
	bool __cdecl Exit_Requested_();
	const char* __cdecl Exit_Message_();
	void __cdecl Check_Exit_();
	void __cdecl Mark_Game_Thread_Exited_();
	bool __cdecl Game_Thread_Exited_();
	bool Create_Video_Buffers_(int16_t video_mode);
	void __cdecl Service_Pending_Window_Events_();
	void __cdecl Pump_Game_Input_Queue_();
	// Discards all pending input observations (wheel steps, middle clicks,
	// observed left clicks); called when a screen or popup opens.
	void __cdecl Discard_Input_Observations_();
	// Returns and consumes whole accumulated mouse-wheel steps (positive = up).
	int16_t __cdecl Consume_Mouse_Wheel_Steps_();
	// Returns and consumes whole accumulated horizontal mouse-wheel steps
	// (positive = right; Shift + vertical wheel counts as horizontal).
	int16_t __cdecl Consume_Mouse_Wheel_Steps_X_();
	// Returns and consumes the number of middle-button clicks since last call.
	int16_t __cdecl Consume_Middle_Clicks_();

	// Observes (without consuming from the input queue) left-button presses:
	// pops the oldest observed press with its position and press timestamp;
	// returns 0 when none are pending.
	uint8_t __cdecl Consume_Left_Click_(int16_t* x, int16_t* y, uint32_t* ticks);

	// Audio control actions decoded from the key buffer. The media keys and
	// the Ctrl/Alt + -/=/0 combinations arrive as platform-private synthetic
	// key codes; Decode_Audio_Key_ turns them into an action and the targeted
	// channels. Plain -/=/0 stay ordinary printable keys.
	enum e_audio_key_action : uint8_t {
		AUDIO_KEY_NONE = 0,
		AUDIO_KEY_VOLUME_DOWN,
		AUDIO_KEY_VOLUME_UP,
		AUDIO_KEY_MUTE_TOGGLE,
	};
	e_audio_key_action __cdecl Decode_Audio_Key_(int32_t key_code, bool* for_music, bool* for_sfx);

	constexpr uint8_t ARROW_KEY_UP = 1;
	constexpr uint8_t ARROW_KEY_DOWN = 2;
	constexpr uint8_t ARROW_KEY_LEFT = 4;
	constexpr uint8_t ARROW_KEY_RIGHT = 8;
	// Returns the ARROW_KEY_* bits of the arrow keys currently held down.
	uint8_t __cdecl Arrow_Keys_Held_();
	// Returns nonzero while a Ctrl key is held down.
	uint8_t __cdecl Ctrl_Key_Held_();
	int16_t __cdecl Get_Mouse_X_();
	int16_t __cdecl Get_Mouse_Y_();
	void __cdecl Set_Logical_Mouse_Position_(int16_t x, int16_t y);
	bool __cdecl Update_Active_Screen_Frame_();
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