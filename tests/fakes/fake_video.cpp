#include "pch.h"

namespace video {
    int32_t g_screen_res_x = 640;
    int32_t g_screen_res_y = 480;
    s_page_buffer g_page_off{};
    s_page_buffer g_page_back{};
    s_page_buffer g_page_published{};
    uint8_t* _video_memory = nullptr;
    int16_t _current_mouse_list_ptr = 0;
    int16_t _current_mouse_shape = 0;
    uint8_t* _current_video_page = nullptr;
    int32_t screen_refresh_flag = 0;
    int16_t _old_screen_dump_flag = 0;
    s_scan_list_entry* _refresh_scan_list = nullptr;
    s_scan_list_entry* _refresh_stencil_list = nullptr;
    uint8_t* _mouse_palette = nullptr;
    uint32_t _screen_pixel_size = 0;
    uint8_t _glass_remap_colors[16][256]{};
    s_glass_color _glass_colors[16]{};
    s_palette_color g_palette_entries[256]{};
    s_palette_color g_submitted_palette_entries[256]{};
    s_palette_entry _current_palette[256]{};
    uint8_t* _palette_list[65]{};
    uint8_t _palette_reds[256]{};
    uint16_t _is_initializing = 0;
    s_mouse_list _default_mouse_list{};
    uint8_t _video_driver_state_marker = 0;
    int16_t _screen_window_y1 = 0;
    int16_t _screen_window_y2 = 479;
    int16_t _screen_window_x1 = 0;
    int16_t _screen_window_x2 = 639;
    SDL_Mutex* g_off_page_mutex = nullptr;
    SDL_Condition* g_off_page_condition = nullptr;
    SDL_Mutex* g_palette_mutex = nullptr;
    uint8_t g_off_page_state = OFF_PAGE_WRITABLE;
    uint32_t g_off_page_generation = 0;
    uint32_t g_last_presented_generation = 0;

    void __cdecl Allocate_Page_Buffers_(int32_t width, int32_t height) {
        (void)width;
        (void)height;
    }

    void __cdecl Free_Page_Buffers_() {
    }

    uint8_t* __cdecl Get_Page_Pixels_(int32_t page_index) {
        (void)page_index;
        return nullptr;
    }

    uint8_t* __cdecl Get_Selected_Page_Pixels_() {
        return nullptr;
    }

    void __cdecl Init_Off_Page_Ownership_() {
    }

    void __cdecl Shutdown_Off_Page_Ownership_() {
    }

    void __cdecl Publish_Off_Page_() {
    }

    void __cdecl Submit_Palette_() {
    }

    bool __cdecl Try_Begin_Off_Page_Present_() {
        return true;
    }

    void __cdecl End_Off_Page_Present_() {
    }

    void __cdecl Set_Refresh_Stencil_() {
    }

    void __cdecl Set_Refresh_Full_() {
    }

    void __cdecl Set_Page_Off_() {
    }

    void __cdecl Clip_Off_() {
    }

    void __cdecl Clip_On_() {
    }

    void __cdecl Copy_Off_To_Back_() {
    }

    void __cdecl Set_Page_Back_() {
    }

    void __cdecl Copy_Back_To_Off_() {
    }

    void __cdecl Add_Line_To_Scan_List_(int32_t x1, int32_t y, int32_t x2) {
        (void)x1;
        (void)y;
        (void)x2;
    }

    void __cdecl Add_Dot_To_Scan_List_(int32_t x, int32_t y) {
        (void)x;
        (void)y;
    }

    void __cdecl Set_Refresh_Differential_() {
    }

    uint8_t __cdecl Get_Current_Video_Page_Flag_() {
        return 0;
    }

    int16_t __cdecl Get_Refresh_Mode_() {
        return 0;
    }

    void __cdecl Init_Video_Drivers_() {
    }

    void __cdecl Stencil_Out_Background_() {
    }

    void __cdecl Clear_Stencil_List_() {
    }

    void __cdecl Clear_Scan_List_() {
    }

    void __cdecl Toggle_Pages_() {
    }

    void __cdecl Save_Refresh_() {
    }

    void __cdecl Set_Refresh_Differential_No_Full_() {
    }

    void __cdecl Restore_Refresh_() {
    }

    void __cdecl Clear_Display_And_Release_Palette_() {
    }

    void __cdecl Toggle_Pages_On_Odd_Frame_() {
    }

    void __cdecl Allocate_Refresh_Lists_(uint32_t unused, int32_t entry_count) {
        (void)unused;
        (void)entry_count;
    }
}