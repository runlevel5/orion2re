#pragma once
#include <cstdint>

namespace shear {
    inline uint16_t& _x_step_8_8 =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055C0A8});

    inline uint16_t& _dst_width =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055C0AE});

    inline uint32_t& _y_accum_8_8 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0B4});

    inline int32_t& _y_step_8_8 =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055C0B8});

    inline uint8_t& _noise_threshold =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055C0BC});

    inline uint32_t& _src_width =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0C0});

    inline uint32_t& _src_height =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0C4});

    inline uint32_t& _row_dword_count =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0C8});

    inline uint32_t& _row_dword_counter =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0CC});

    inline uint32_t& _noise_start_index =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0D0});

    inline uint32_t& _jumble_mask =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0EC});

    inline uint32_t& _jumble_direction_toggle =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0F0});

    inline int32_t& _src_row_skip =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055C0F4});

    inline int32_t& _dst_row_skip =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055C0F8});

    inline uint32_t& _col_temp =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0FC});

    inline uint32_t& _outline_color =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0D4});

    inline uint32_t& _temp =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C0DC});

    inline uint8_t*& _src_bitmap_end =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x0055C0E0});

    inline uint32_t& _row_temp =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C100});

    inline uint8_t* _noise_table =
        reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055C104});

    void __cdecl Outline_Bitmap_Pixels_(uint8_t* src, uint32_t outline_color, uint32_t unused, uint8_t* dest, uint32_t width, uint32_t height);

    void __cdecl Outline_Bitmap_Pixels_No_Glass_(uint8_t* src, uint32_t outline_color, uint32_t unused, uint8_t* dest, uint32_t width, uint32_t height);

    void __cdecl Capture_Screen_Block_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst);
    void __cdecl Flip_Bitmap_Down_Different_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst);
    void __cdecl Flip_Bitmap_Down_Same_(uint8_t* data, uint32_t width, uint32_t height);
    void __cdecl Flip_Bitmap_Left_Different_(uint8_t* src, uint32_t width, uint32_t height, uint8_t* dst);
    void __cdecl Flip_Bitmap_Left_Same_(uint8_t* pixels, uint32_t width, uint32_t height);

    void __cdecl Interlace_Scale_Off_Page_ASM_(uint8_t* page, uint8_t* lut);

    void __cdecl Mask_Bitmap_Pixels_(uint8_t* src, int32_t src_row_skip, int32_t dst_row_skip, uint8_t* dst, int32_t width, int32_t height);
    void __cdecl Mask_Bitmap_Pixels_Inverse_(uint8_t* src, int32_t src_row_skip, int32_t dst_row_skip, uint8_t* dst, int32_t width, int32_t height);

    void __cdecl Mask_Brighten_Bitmap_Pixels_(uint8_t* src, int32_t src_row_skip, int32_t dst_row_skip, uint8_t* dst, int32_t width, int32_t height);

    void __cdecl Mask_Bitmap_Colors_Pixels_(uint32_t* pixels, uint32_t min_color, uint32_t max_color, uint32_t unused_param, uint32_t width, uint32_t height);

    void __cdecl Enlarge_Bitmap_(uint8_t* src_ptr, uint32_t scale_x, uint32_t scale_y, uint8_t* dst_ptr, uint32_t src_width, uint32_t src_height, uint32_t dst_width, uint32_t dst_height);

    void __cdecl Reduce_Bitmap_(uint8_t* src_ptr, uint32_t src_width, uint32_t src_height, uint8_t* dst_ptr, uint32_t src_pitch, uint32_t unused_param, uint32_t dst_width, uint32_t dst_height);

    void __cdecl Vanish_Bitmap_Pixels_(uint8_t* src, uint32_t noise_threshold, uint32_t unused_param, uint8_t* dst, uint32_t width, uint32_t height);

    void __cdecl Jumble_Bitmap_Pixels_(uint8_t* src_buf, uint32_t jumble_mask, uint32_t noise_threshold, uint8_t* dst_buf, uint32_t width, uint32_t height);

    void __cdecl Gray_Scale_Fill_Pixels_(uint8_t* pixels, uint8_t* gray_scale_table, int32_t y_step_8_8, uint32_t pixels_per_row, uint32_t row_count);
}