#pragma once

extern uint8_t DAT_00578820;
extern uint32_t DAT_00578dc8;
extern uint32_t DAT_00578dcc;

struct IDirectDrawPalette;
struct tagPALETTEENTRY;
struct IDirectDrawSurface;

namespace video {
	// Variables
	extern int32_t g_screen_res_x;

	extern int32_t g_screen_res_y;

	extern IDirectDrawSurface* g_offscreen_surface_1;

	extern IDirectDrawSurface* g_offscreen_surface_2;

	extern uint8_t* _video_memory;

	extern int16_t _current_mouse_list_ptr;

	extern int16_t _current_mouse_shape;

	extern uint8_t* _current_video_page;

	extern uint32_t screen_dump_flag;

	struct s_scan_list_entry {
		int16_t min_x;
		int16_t max_x;
	};

	extern int32_t screen_refresh_flag;

	extern int16_t _old_screen_dump_flag;

	extern int16_t _old_screen_refresh_flag;

	extern s_scan_list_entry* _refresh_scan_list;

	extern s_scan_list_entry* _refresh_stencil_list;

	extern uint8_t* _mouse_palette;

	extern uint32_t _screen_pixel_size;

	extern uint8_t _glass_remap_colors[16][256];

	extern s_glass_color _glass_colors[16];

	extern IDirectDrawPalette* g_palette;

	extern tagPALETTEENTRY g_palette_entries[256];

	extern s_palette_entry _current_palette[256];

	extern uint8_t* _palette_list[65];

	extern uint8_t _palette_reds[256];

	extern uint16_t _is_initializing;
	extern s_mouse_list _default_mouse_list;
	extern uint8_t DAT_00578b58;

	extern int16_t _copy_to_on_page_flag;

	extern int16_t _screen_window_y1;
	extern int16_t _screen_window_y2;
	extern int16_t _screen_window_x1;
	extern int16_t _screen_window_x2;

	// Functions
	// Switches refresh handling to stencil mode for the next page update.
	void __cdecl Set_Refresh_Stencil_();
	// Switches refresh handling to a full-screen redraw mode.
	void __cdecl Set_Refresh_Full_();
	// Selects the off-screen render page and clears the copy-to-on-page flag.
	void __cdecl Set_Page_Off_();
	// Disables clipping in the animation subsystem.
	void __cdecl Clip_Off_();
	// Enables clipping in the animation subsystem.
	void __cdecl Clip_On_();

	// Blits the primary off-screen surface into the back surface.
	void __cdecl Copy_Off_To_Back_();

    // Selects the back render page and clears the copy-to-on-page flag.
    void __cdecl Set_Page_Back_();

    // Blits the back surface into the primary off-screen surface.
    void __cdecl Copy_Back_To_Off_();

    // Adds a horizontal pixel span to the refresh scan list for one scanline.
    // x1: Left pixel coordinate. y: Scanline index. x2: Right pixel coordinate.
    void __cdecl Add_Line_To_Scan_List_(int32_t x1, int32_t y, int32_t x2);

	// Adds a single pixel position to the refresh scan list.
	// x: Pixel x coordinate. y: Scanline index.
	void __cdecl Add_Dot_To_Scan_List_(int32_t x, int32_t y);

	// Switches refresh handling to differential mode.
	void __cdecl Set_Refresh_Differential_();
	// Returns the currently selected video page flag.
	uint8_t __cdecl Get_Current_Video_Page_Flag_();
	// Returns the current refresh mode as a 16-bit value.
	int16_t __cdecl Get_Refresh_Mode_();

	// Initializes video-driver state, cursor surfaces, and default mouse bounds.
	void __cdecl Init_Video_Drivers_();

    // Performs the masked screen-dump path used when screen_dump_flag is 1.
    void __cdecl _Masked_Screen_Dump();

    // Copies only the scan-listed regions from the alternate off-screen surface.
    void __cdecl _Partial_Screen_Dump();

    // Copies scan-listed background regions into the alternate off-screen surface.
    void __cdecl Stencil_Out_Background_();

    // Marks every stencil scan-list entry as empty.
    void __cdecl Clear_Stencil_List_();
    // Marks every refresh scan-list entry as empty.
    void __cdecl Clear_Scan_List_();

    // Presents the pending frame and updates refresh bookkeeping.
    void __cdecl Toggle_Pages_();

    // Saves the current refresh and dump flags into the legacy backup globals.
    void __cdecl Save_Refresh_();

    // Enables differential refresh while forcing the partial dump path.
    void __cdecl Set_Refresh_Differential_No_Full_();

	// Restores the previously saved refresh and dump flags.
	void __cdecl Restore_Refresh_();

    // Clears the display state and releases the active DirectDraw palette.
    void __cdecl Clear_Display_And_Release_Palette_();

    // Performs a page toggle when the current frame count is odd.
    void __cdecl Toggle_Pages_On_Odd_Frame_();

    // Allocates the refresh and stencil scan lists.
    // unused: Unused legacy argument preserved for call compatibility.
    // entry_count: Number of scan-list entries to allocate.
    void __cdecl Allocate_Refresh_Lists_(uint32_t unused, int32_t entry_count);
}