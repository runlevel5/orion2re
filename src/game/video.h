#pragma once

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

	// Address 0x579350 is the base of the current palette status array
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
	void __cdecl Set_Refresh_Stencil_();
	void __cdecl Set_Refresh_Full_();
	void __cdecl Set_Page_Off_();
	void __cdecl Clip_Off_();
	void __cdecl Clip_On_();

	void __cdecl Copy_Off_To_Back_();

    void __cdecl Set_Page_Back_();

    void __cdecl Copy_Back_To_Off_();

    void __cdecl Add_Line_To_Scan_List_(int32_t x1, int32_t y, int32_t x2);

	void __cdecl Add_Dot_To_Scan_List_(int32_t x_scaled, int32_t y);

	void __cdecl Set_Refresh_Differential_();
	uint8_t __cdecl Get_Current_Video_Page_Flag_();
	int16_t __cdecl Get_Refresh_Mode_();

	void __cdecl Init_Video_Drivers_();

    void __cdecl _Masked_Screen_Dump();

    void __cdecl _Partial_Screen_Dump();

    void __cdecl Stencil_Out_Background_();

    void __cdecl Clear_Stencil_List_();
    void __cdecl Clear_Scan_List_();

    void __cdecl Toggle_Pages_();

    void __cdecl Save_Refresh_();

    void __cdecl Set_Refresh_Differential_No_Full_();

	void __cdecl Restore_Refresh_();

    void __cdecl Clear_Display_And_Release_Palette_();

    void __cdecl Toggle_Pages_On_Odd_Frame_();

    void __cdecl Allocate_Refresh_Lists_(uint32_t, int32_t entry_count);
}