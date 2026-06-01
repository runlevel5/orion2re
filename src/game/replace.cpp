#include "pch.h"

namespace replace {
    uint8_t _graphics_buffer[1023];
    uint8_t* _palette_color_list;
    const char s_Shift_Palette_Range_exceeds_25_0055b7c4[41] = "Shift_Palette_Range() exceeds 256 colors";

    void __cdecl Replace_Color_All_(s_animation_header *anim, uint8_t color)
    {
        animate::_animation_header = anim;
        if (anim->frame_count <= 0) {
            return;
        }

        for (int16_t frame = 0; frame < animate::_animation_header->frame_count; frame++) {
            uint8_t *frame_data = (uint8_t*)animate::_animation_header + animate::_animation_header->frame_offsets[frame];
            uint8_t flags = animate::_animation_header->flags & 3;

            switch (flags) {
            case 0:
            {
                int32_t current_y = *(uint16_t*)(frame_data + 2);
                int32_t word_offset = 2;
                int32_t height = animate::_animation_header->height;

                while (current_y < height) {
                    uint8_t *ptr = frame_data + word_offset * 2;
                    word_offset += 2;
                    
                    int16_t count = *(int16_t*)ptr;
                    int16_t arg = *(int16_t*)(ptr + 2);

                    if (count == 0) {
                        current_y += arg;
                    } else {
                        if (count > 0) {
                            uint8_t *pixels = frame_data + word_offset * 2;
                            for (int32_t i = 0; i < count; i++) {
                                if (pixels[i] != 0) {
                                    pixels[i] = color;
                                }
                            }
                        }
                        word_offset += (count + 1) / 2;
                    }
                }
                break;
            }
            case 1:
            {
                int32_t size = animate::_animation_header->height * animate::_animation_header->width;
                for (int32_t i = 0; i < size; i++) {
                    if (frame_data[i] != 0) {
                        frame_data[i] = color;
                    }
                }
                break;
            }
            case 2:
            {
                int16_t chunks = 5;
                int32_t offset = 0;

                while (chunks > 0) {
                    while (true) {
                        uint8_t *ptr = frame_data + offset;
                        offset += 8;

                        int16_t v1 = *(int16_t*)ptr;
                        int16_t quad_count = *(int16_t*)(ptr + 4);

                        if (v1 == 0 && quad_count == 0) {
                            break;
                        }

                        int16_t count = (int16_t)((uint16_t)quad_count << 2);
                        if (count > 0) {
                            for (int32_t i = 0; i < count; i++) {
                                if (frame_data[offset + i] != 0) {
                                    frame_data[offset + i] = color;
                                }
                            }
                        }
                        offset += count;
                    }
                    chunks--;
                }
                break;
            }
            case 3:
                Exit_With_Message_();
                break;
            }
        }
    }

    void __cdecl Replace_Color_(s_animation_header *anim, int16_t old_color, uint8_t new_color) {
        animate::_animation_header = anim;

        if (anim->frame_count > 0) {
            for (int16_t frame_idx = 0; frame_idx < animate::_animation_header->frame_count; ++frame_idx) {
                s_animation_header *current_anim = animate::_animation_header;
                uint8_t *frame_data = reinterpret_cast<uint8_t*>(current_anim) + current_anim->frame_offsets[frame_idx];

                switch (current_anim->flags & 3) {
                    case 0: {
                        int32_t y = *reinterpret_cast<uint16_t*>(frame_data + 2);
                        int32_t data_offset_words = 2;
                        int32_t height = current_anim->height;

                        while (y < height) {
                            int16_t cmd = *reinterpret_cast<int16_t*>(frame_data + data_offset_words * 2);
                            int16_t arg = *reinterpret_cast<int16_t*>(frame_data + data_offset_words * 2 + 2);
                            data_offset_words += 2;

                            if (cmd == 0) {
                                y += arg;
                            } else {
                                int32_t byte_offset = data_offset_words * 2;
                                if (cmd > 0) {
                                    for (int32_t i = 0; i < cmd; ++i) {
                                        if (static_cast<int16_t>(*reinterpret_cast<int8_t*>(frame_data + byte_offset)) == old_color) {
                                            frame_data[byte_offset] = static_cast<uint8_t>(new_color);
                                        }
                                        byte_offset++;
                                    }
                                }
                                data_offset_words += (cmd + 1) / 2;
                            }
                        }
                        break;
                    }
                    case 1: {
                        int32_t size = static_cast<int32_t>(current_anim->height) * current_anim->width;
                        if (size > 0) {
                            for (int32_t i = 0; i < size; ++i) {
                                if (frame_data[i] == static_cast<uint8_t>(old_color)) {
                                    frame_data[i] = static_cast<uint8_t>(new_color);
                                }
                            }
                        }
                        break;
                    }
                    case 2: {
                        int16_t loop_count = 5;
                        int32_t offset = 0;
                        while (loop_count != 0) {
                            while (true) {
                                int16_t chunk_header_word0 = *reinterpret_cast<int16_t*>(frame_data + offset);
                                int16_t chunk_quad_count = *reinterpret_cast<int16_t*>(frame_data + offset + 4);
                                offset += 8;

                                if (chunk_quad_count == 0 && chunk_header_word0 == 0) {
                                    break;
                                }

                                uint16_t chunk_byte_count_bits = (uint16_t)chunk_quad_count << 2;
                                int16_t chunk_byte_count16 = (int16_t)chunk_byte_count_bits;
                                int32_t chunk_byte_count = chunk_byte_count16;
                                if (chunk_byte_count > 0) {
                                    for (int32_t i = 0; i < chunk_byte_count; ++i) {
                                        if (static_cast<int16_t>(*reinterpret_cast<int8_t*>(frame_data + offset + i)) == old_color) {
                                            frame_data[offset + i] = static_cast<uint8_t>(new_color);
                                        }
                                    }
                                }
                                offset += chunk_byte_count;
                            }
                            loop_count--;
                        }
                        break;
                    }
                    case 3: {
                        Exit_With_Message_();
                        break;
                    }
                }
            }
        }
    }

    void __cdecl Establish_Color_Gradient_(int16_t scale, int16_t invert) {
        _palette_color_list = _graphics_buffer;

        for (int i = 0; i < 256; ++i) {
            uint8_t r = video::_current_palette[i].r;
            uint8_t g = video::_current_palette[i].g;
            uint8_t b = video::_current_palette[i].b;

            int32_t intensity = r + g + (b >> 1);
            uint8_t color = static_cast<uint8_t>((intensity * scale) / 158);

            _palette_color_list[i] = color;

            if (invert == 1) {
                _palette_color_list[i] = static_cast<uint8_t>(scale - color - 1);
            }
        }
    }

    void __cdecl Exit_With_Message_() {
        exitm::Exit_With_Message(s_Cannot_color_replace_refresh_mas_0055b7a0);
    }

    void __cdecl Replace_Color_Range_(s_animation_header* anim, uint8_t* palette_map, int16_t start_color, int16_t num_colors) {
        int32_t end_color = num_colors + start_color - 1;
        animate::_animation_header = anim;

        if (animate::_animation_header->frame_count > 0) {
            for (int16_t frame = 0; frame < animate::_animation_header->frame_count; frame++) {
                uint8_t* frame_data =
                    reinterpret_cast<uint8_t*>(animate::_animation_header) +
                    animate::_animation_header->frame_offsets[frame];

                switch (animate::_animation_header->flags & 3) {
                case 0: {
                    int32_t line_y = reinterpret_cast<uint16_t*>(frame_data)[1];
                    int32_t height = animate::_animation_header->height;
                    int32_t word_offset = 2;

                    while (line_y < height) {
                        int16_t* current = reinterpret_cast<int16_t*>(frame_data + word_offset * 2);
                        word_offset += 2;

                        int16_t count = current[0];
                        if (count == 0) {
                            line_y += current[1];
                        } else {
                            uint8_t* data = frame_data + word_offset * 2;
                            for (int32_t i = 0; i < count; i++) {
                                uint8_t pixel = data[i];
                                if (pixel != 0 && pixel >= start_color && pixel <= end_color) {
                                    data[i] = palette_map[pixel - start_color];
                                }
                            }
                            word_offset += (count + 1) / 2;
                        }
                    }
                    break;
                }
                case 1: {
                    int32_t area = animate::_animation_header->width * animate::_animation_header->height;
                    if (area > 0) {
                        for (int32_t i = 0; i < area; i++) {
                            uint8_t pixel = frame_data[i];
                            if (pixel != 0 && pixel >= start_color && pixel <= end_color) {
                                frame_data[i] = palette_map[pixel - start_color];
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    int16_t blocks = 5;
                    int32_t offset = 0;
                    do {
                        while (true) {
                            int16_t quad_count = *reinterpret_cast<int16_t*>(frame_data + offset + 4);
                            int16_t header0 = *reinterpret_cast<int16_t*>(frame_data + offset);
                            offset += 8;

                            if (quad_count == 0 && header0 == 0) {
                                break;
                            }

                            uint16_t shifted = (uint16_t)quad_count << 2;
                            int16_t byte_count16 = (int16_t)shifted;
                            int32_t byte_count = byte_count16;
                            if (byte_count > 0) {
                                for (int32_t i = 0; i < byte_count; i++) {
                                    uint8_t pixel = frame_data[offset + i];
                                    if (pixel != 0 && pixel >= start_color && pixel <= end_color) {
                                        frame_data[offset + i] = palette_map[pixel - start_color];
                                    }
                                }
                            }
                            offset += byte_count;
                        }
                        blocks--;
                    } while (blocks != 0);
                    break;
                }
                case 3: {
                    Exit_With_Message_();
                    break;
                }
                }
            }
        }
    }

    void __cdecl Shift_Palette_Range_(s_animation_header* anim, int shift) {
        animate::_animation_header = anim;

        uint8_t* pal_header_ptr = reinterpret_cast<uint8_t*>(anim) + anim->frame_count * 4 + 0x10;
        animate::_palette_header = reinterpret_cast<s_palette_header*>(pal_header_ptr);

        if (animate::_palette_header->count + shift > 256) {
            exitm::Exit_With_Message(s_Shift_Palette_Range_exceeds_25_0055b7c4);
        }

        animate::_palette_header->start_index = static_cast<int16_t>(shift);
    }

    void __cdecl Interlaced_Gray_Scale_Off_Page_(uint8_t color_offset1, int16_t gradient_start1, uint8_t color_offset2, int16_t gradient_start2, int16_t gradient_end) {
        replace::_palette_color_list = replace::_graphics_buffer;
        
        replace::Establish_Color_Gradient_(gradient_start1, gradient_end);
        
        for (int i = 0; i < 256; ++i) {
            replace::_palette_color_list[i] += color_offset1;
        }
        
        video::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
        shear::Interlace_Scale_Off_Page_ASM_(video::_current_video_page, replace::_palette_color_list);
        platform::Unlock_Appropriate_Render_Buffer();
        
        replace::Establish_Color_Gradient_(gradient_start2, gradient_end);
        
        for (int i = 0; i < 256; ++i) {
            replace::_palette_color_list[i] += color_offset2;
        }
        
        video::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
        shear::Interlace_Scale_Off_Page_ASM_(video::_current_video_page + 0x280, replace::_palette_color_list);
        platform::Unlock_Appropriate_Render_Buffer();
    }
}