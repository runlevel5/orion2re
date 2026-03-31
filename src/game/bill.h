#pragma once
#include <cstdint>

namespace BILL {
    inline void __cdecl Bill_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00406780})();
    }

    void __cdecl Cache_Player_Scientist_Anim_();
    void __cdecl Record_History_();

    void __cdecl Copy_File_To_File_(const char* src_path, const char* dst_path);

    char* __cdecl Cache_Player_Scientist_Anim_Imp_(int16_t player_index);

    inline int16_t __cdecl Get_History_Params_(int16_t* n_points) {
        using Fn = int16_t(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004071C0});
        return fn(n_points);
    }

    void __cdecl Bill_Default_(void);

    int16_t __cdecl Get_Their_Spy_Number_(int16_t player_idx, int16_t target_idx);
    void __cdecl Set_Their_Spy_Number_(int16_t player_idx, int16_t target_player_idx, uint8_t count);

    void __cdecl Squeeze_Print_(int16_t x, int16_t y, const char* text, int16_t max_width);

    inline void __cdecl _Squeeze_Print_Paragraph_(int16_t x, int16_t y, int16_t width, int16_t height, const char* text, int16_t color, s_animation_header* anim, bool flag1, bool flag2) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, const char*, int16_t, s_animation_header*, bool, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407480});
        fn(x, y, width, height, text, color, anim, flag1, flag2);
    }

    void __cdecl Squeeze_Print_Paragraph_(uint16_t x, int16_t y, int16_t w, int16_t h, char* str, uint16_t align);

    void __cdecl Squeeze_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color);

    void __cdecl Squeeze_Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t flags);

    void __cdecl Squeeze_Formatted_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color);

    void __cdecl Get_Players_Dead_Or_Alive_Or_Omniscient_(int32_t param_1, char param_2);

    inline void __cdecl Turn_Off_Palettes_(int16_t start, int16_t end) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(0x004071F0)(start, end);
    }

    inline void __cdecl Get_Active_Players_(int player_id, char is_active) {
        using Fn = void(__cdecl*)(int, char);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00406CD0});
        fn(player_id, is_active);
    }

    void __cdecl Get_Players_Alive_Or_Omniscient_(int32_t param_1, char param_2);

    int16_t __cdecl Get_My_Spy_Number_(int16_t spy_idx);

    int8_t __cdecl Get_My_Spy_Mission_(int16_t target_player_idx);

    void __cdecl Set_Their_Spy_Mission_(int16_t player_idx, int16_t target_player_idx, int8_t mission);

    int16_t __cdecl Get_My_Agent_Number_();

    void __cdecl Set_My_Spy_Mission_(int16_t target_player_idx, int8_t mission);

    void __cdecl Set_My_Spy_Number_(int16_t target_player_idx, uint8_t n_spies);

    void __cdecl Set_My_Agent_Number_(uint8_t agent_num);

    void __cdecl Squeeze_Print_Centered_(int16_t x, int16_t y, const char* str, int16_t width);

    void __cdecl Print_1_Or_More_Lines_Centered_To_Bitmap_(int x, int y, int width, char* str, s_animation_header* bmp);

    inline void __cdecl _Print_1_Or_More_Lines_Centered_(uint16_t x, int16_t y, uint16_t width, uint8_t* bitmap, int16_t* line_y_offsets) {
        using Fn = void(__cdecl*)(uint16_t, int16_t, uint16_t, uint8_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407730});
        fn(x, y, width, bitmap, line_y_offsets);
    }

    void __cdecl Squeeze_Centered_Print_(uint16_t x, int16_t y, char* text, uint16_t limit);

    void __cdecl _Print_Paragraph_(int16_t x, int16_t y, int16_t width, const char* text, int16_t alignment);

    inline int16_t& __pp_max_width = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A8BBE});
    inline int16_t& __pp_max_height = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A8BC0});

    inline void __cdecl _Print_Paragraph_Bill_(int16_t x, int16_t y, int16_t width, const char* text, int16_t alignment, int32_t flag, void* reserved) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, const char*, int16_t, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407DD0});
        fn(x, y, width, text, alignment, flag, reserved);
    }

    int16_t __cdecl _Get_Paragraph_Max_Width_(int16_t width, char* text, int16_t height);

    int16_t __cdecl _Get_Paragraph_Max_Height_(int16_t width, const char* text);

    inline uint8_t (&_space_limits)[8] =
        *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x00551138});

    inline void __cdecl _Print_String_Bill_(int16_t x, int16_t y, int16_t limit_w, const char* str, int16_t p6, int32_t p7, void* p5) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, const char*, int16_t, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407C20});
        fn(x, y, limit_w, str, p6, p7, p5);
    }

    void __cdecl _Squeeze_Print_(int16_t x, int16_t y, const char* str, int16_t limit_w, void* p5, int16_t p6, bool adjust_y);

    inline void __cdecl Record_History_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004067d0 });
        fn();
    }

    inline void __cdecl Set_Plyr_Info_Btns_(int32_t button_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004299E0 });
        fn(button_id);
    }

    inline char* _dest_file_name = reinterpret_cast<char*>(std::uintptr_t{0x00564738});
    inline const char* s_SR_R_x_SC_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x005512B0}); // "SR_R%x_SC.LBX"
    inline const char* s_SR_R_wildcard_SC_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x005512E0}); // "SR_R?_SC.LBX"

    inline const char* const s_Could_not_open_s = reinterpret_cast<const char*>(std::uintptr_t{0x00551140}); // "Could not open %s"
    inline const char* const s_Error_copying_s_to_s_bytes_i = reinterpret_cast<const char*>(std::uintptr_t{0x00551154}); // "Error copying %s to %s (bytes: %i)"
    inline const char* const s_wb = reinterpret_cast<const char*>(std::uintptr_t{0x00551178}); // "wb"
    inline const char* const s_rb = reinterpret_cast<const char*>(std::uintptr_t{0x0055117c}); // "rb"
}