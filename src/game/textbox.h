#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct s_help_record {
    char title[84];
    char anim_lbx[10];
    uint32_t anim_info;
    uint8_t unknown_98;
    uint32_t next_help_idx;
    char body[1300];
};
#pragma pack(pop)

namespace TEXTBOX {
    inline int16_t& _bottom_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565EC8});
    inline uint32_t& _textbox_seg = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565ED8});
    inline uint32_t& tmp_block = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565EDC});
    inline uint16_t& _field_save_ix = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00557434});
    inline const char* s_textbox_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00557444});
    inline const char* s_textbox_lbx_00557444 = reinterpret_cast<const char*>(std::uintptr_t{0x00557444});
    inline const char* const s_SPA_HELP_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{0x00557450}); // "SPA_HELP.LBX"
    inline const char* const s_ITA_HELP_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{0x00557460}); // "ITA_HELP.LBX"
    inline const char* const s_FRE_HELP_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{0x00557470}); // "FRE_HELP.LBX"
    inline const char* const s_GER_HELP_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{0x00557480}); // "GER_HELP.LBX"
    inline const char* const s_HELP_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{0x00557490}); // "HELP.LBX"
    inline int16_t& _textbox_field_save = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565EA0});
    inline uint8_t& _reload_saved_block = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565ECC});
    inline int16_t& _box_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565EE4});
    inline int16_t& _box_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565EE0});
    inline uint16_t& _old_draw_fields_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565ED0});
    inline MOX2::action_func_t& _textbox_af = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x00565ED4});
    inline uint16_t& _old_help_list_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565EEC});
    inline const char* s__00551120 = reinterpret_cast<const char*>(std::uintptr_t{0x00551120});

    void __cdecl Get_Help_Lbx_Name_(char* help_lbx_name);

    inline void __cdecl Get_Mox_Text_Box_Base_Colors_(int unknown, uint8_t* color_out_1, uint8_t* color_out_2) {
        using Fn = void(__cdecl*)(int, uint8_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471790});
        fn(unknown, color_out_1, color_out_2);
    }

    void __cdecl Text_Box_Startup_(int32_t size);

    void __cdecl Box_Centered_On_XY_(int16_t* x_ptr, int16_t* y_ptr, int16_t height, int32_t flag);

    void __cdecl Text_Box_Get_Input_(int16_t ticks);

    void __cdecl Text_Box_Cleanup_();

    void __cdecl Do_Text_Box_(const char* title, const char* text, int16_t input_flag);

    void __cdecl Text_Box_(const char* title, const char* text);

    void __cdecl Draw_Help_Entry_(int16_t help_id);
}