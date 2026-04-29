#pragma once
#include <cstdint>

struct s_animation_header;

namespace BILL {
    void __cdecl Bill_Init_();

    void __cdecl Cache_Player_Scientist_Anim_();
    void __cdecl Record_History_();

    void __cdecl Copy_File_To_File_(const char* src_path, const char* dst_path);

    char* __cdecl Cache_Player_Scientist_Anim_Imp_(int16_t player_index);

    int16_t __cdecl Get_History_Params_(int16_t* out_history_param);

    void __cdecl Bill_Default_(void);

    int16_t __cdecl Get_Their_Spy_Number_(int16_t player_idx, int16_t target_idx);
    void __cdecl Set_Their_Spy_Number_(int16_t player_idx, int16_t target_player_idx, uint8_t count);

    void __cdecl Squeeze_Print_(int16_t x, int16_t y, const char* text, int16_t max_width);

    void __cdecl _Squeeze_Print_Paragraph_(
        int16_t x, int16_t y, int16_t width, int16_t max_height,
        char* text, int32_t param_6, s_animation_header* anim,
        bool is_formatted, bool center_y);

    void __cdecl Squeeze_Print_Paragraph_(uint16_t x, int16_t y, int16_t w, int16_t h, char* str, uint16_t align);

    void __cdecl Squeeze_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color);

    void __cdecl Squeeze_Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t flags);

    void __cdecl Squeeze_Formatted_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color);

    void __cdecl Get_Players_Dead_Or_Alive_Or_Omniscient_(int32_t param_1, char param_2);

    void __cdecl Turn_Off_Palettes_(int16_t start, int16_t end);

    void __cdecl Get_Active_Players_(int32_t start_idx, int8_t include_previously_contacted);

    void __cdecl Get_Players_Alive_Or_Omniscient_(int32_t param_1, char param_2);

    int16_t __cdecl Get_My_Spy_Number_(int16_t spy_idx);

    int8_t __cdecl Get_My_Spy_Mission_(int16_t target_player_idx);

    int16_t __cdecl Get_Their_Spy_Mission_(int16_t player_idx, int16_t target_player_idx);

    void __cdecl Set_Their_Spy_Mission_(int16_t player_idx, int16_t target_player_idx, int8_t mission);

    int16_t __cdecl Get_My_Agent_Number_();

    void __cdecl Set_My_Spy_Mission_(int16_t target_player_idx, int8_t mission);

    void __cdecl Set_My_Spy_Number_(int16_t target_player_idx, uint8_t n_spies);

    void __cdecl Set_My_Agent_Number_(uint8_t agent_num);

    void __cdecl Squeeze_Print_Centered_(int16_t x, int16_t y, const char* str, int16_t width);

    void __cdecl Print_1_Or_More_Lines_Centered_To_Bitmap_(int x, int y, int width, char* str, s_animation_header* bmp);

    void __cdecl _Print_1_Or_More_Lines_Centered_(int16_t x, int16_t y, int16_t width, char* text, int16_t* aux);

    void __cdecl Squeeze_Centered_Print_(uint16_t x, int16_t y, char* text, uint16_t limit);

    void __cdecl _Print_Paragraph_(int16_t x, int16_t y, int16_t width, const char* text, int16_t alignment);

    void __cdecl _Print_Paragraph_Bill_Aux_(int16_t param_1, int16_t param_2, int16_t param_3, char* param_4, int16_t param_5, int16_t* param_6);

    inline int16_t& __pp_max_width = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A8BBE});
    inline int16_t& __pp_max_height = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A8BC0});

    void __cdecl _Print_Paragraph_Bill_(int16_t x, int16_t y, int16_t width, const char* text, uint16_t color, int32_t param_6, int16_t* param_7);

    int16_t __cdecl _Get_Paragraph_Max_Width_(int16_t max_width, char* text, int16_t font_style);

    int16_t __cdecl _Get_Paragraph_Max_Height_(int16_t width, const char* text);

    inline uint8_t (&_space_limits)[8] =
        *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x00551138});

    void __cdecl Print_Display_Auto_Width_(int16_t x, int16_t y, const char* text, int32_t width);

    void __cdecl Print_Right_Aligned_To_Bitmap_(int16_t x, int16_t y, const char* str, s_animation_header* bitmap);

    void __cdecl Print_Display_To_Bitmap_Auto_Width_(int16_t x, int16_t y, const char* str, int16_t width, s_animation_header* bitmap);

    void __cdecl _Print_String_Bill_(int16_t x, int16_t y, int16_t width, const char* str, uint16_t mode, uint8_t draw_flag, s_animation_header* bitmap);

    void __cdecl _Squeeze_Print_(int16_t x, int16_t y, const char* str, int16_t limit_w, s_animation_header* pic, int16_t p6, bool adjust_y);

    void __cdecl Record_History_();

    void __cdecl Set_Plyr_Info_Btns_(int16_t btn_val);

    inline char (&_dest_file_name)[32] = *reinterpret_cast<char(*)[32]>(std::uintptr_t{0x00564738});
    inline const char* s_SR_R_x_SC_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x005512B0}); // "SR_R%x_SC.LBX"
    inline const char* s_SR_R_wildcard_SC_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x005512E0}); // "SR_R?_SC.LBX"

    inline const char* const s_Could_not_open_s = reinterpret_cast<const char*>(std::uintptr_t{0x00551140}); // "Could not open %s"
    inline const char* const s_Error_copying_s_to_s_bytes_i = reinterpret_cast<const char*>(std::uintptr_t{0x00551154}); // "Error copying %s to %s (bytes: %i)"
    inline const char* const s_wb = reinterpret_cast<const char*>(std::uintptr_t{0x00551178}); // "wb"
    inline const char* const s_rb = reinterpret_cast<const char*>(std::uintptr_t{0x0055117c}); // "rb"

    char* __cdecl Cache_Player_Spy_Anim_();

    inline char* __cdecl Cache_Player_Trooper_Anim_() {
        using Fn = char*(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407B20});
        return fn();
    }
}