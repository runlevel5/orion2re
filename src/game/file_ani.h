#pragma once

#include <cstdint>

namespace file_ani {
    extern uint8_t* _file_animation_buffer;

    extern uint32_t _file_animation_open;

    extern uint32_t _file_animation_length;

    extern uint32_t _file_animation_end;

    extern FILE* _file_animation_fptr;

    extern uint32_t _file_animation_opened_flag[3];

    extern FILE* _file_animation[3];

    extern uint32_t _file_animation_start_multi[3];

    extern uint32_t _file_animation_end_multi[3];

    extern uint32_t* _file_lbx_header;

    extern uint32_t _file_animation_length_multi[3];

    extern uint32_t _file_animation_start;

    extern uint8_t* _file_animation_header;

    extern const char s_File_Animation_Frame_Too_Large[31];
    extern const char s_Refresh_Mask_can_t_be_drawn[28];
    extern const char (&s_Refresh_Mask_cant_be_drawn)[28];

    extern const char s_Too_many_frames_for_multi_file_a[41];
    extern const char s_Differentail_File_Animation_Can_n_masked[46];
    extern const char s_Differentail_File_Animation_Can_n_bitmap[46];

    extern const char (&s_Differential_Error_868)[46];
    extern const char (&s_Differential_Error_898)[46];
    extern const char s_Differential_Error_8C8[47];

    void __cdecl Set_File_Animation_Buffer_(void* buffer);

    void __cdecl Reset_File_Animation_Buffer_();

    void __cdecl Open_File_Animation_(const char* filename, int16_t entry_index);

    void __cdecl Open_Multi_File_Animation_(const char* filename, int16_t file_index, int16_t animation_slot);

    void __cdecl Set_File_Animation_Frame_(int16_t frame_index);

    void __cdecl thunk_Draw_File_Animation_();

    void __cdecl Draw_File_Animation_();

    void __cdecl Draw_File_Animation_Stencil_();

    int16_t __cdecl Get_File_Animation_Frame_();

    int16_t __cdecl Get_File_Animation_Total_Frames_();

    void __cdecl Draw_Multi_File_Animation_(int16_t anim_idx);

    void __cdecl Draw_Multi_File_Animation_Stencil_(int16_t anim_idx);

    int16_t __cdecl Get_Multi_File_Animation_Frame_(int16_t index);

    void __cdecl Set_Multi_File_Animation_Frame_(int16_t anim_idx, int16_t frame);

    void __cdecl Draw_Multi_File_Animation_Palette_(int32_t index);
}