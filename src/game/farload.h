#pragma once

#include <cstdint>
#include <cstdio>

struct s_lbx_header;
struct s_animation_header;

namespace farload {
    // Suggested name: farload::Error_Handler
    void __cdecl Error_Handler_(const char* lib_name, int code, int idx);

    extern const char s_vga_frames_error[53];
    extern const char s_error_defined_records[35];
    extern const char s_error_record_size[30];
    extern const char s_error_lbx_entries[31];
    extern const char s_error_not_lbx[20];
    extern const char s_error_reload_failed[40];
    extern const char s_error_improper_entry[31];
    extern const char s_error_insufficient_memory[74];
    extern const char s_error_corrupted[21];
    extern const char s_error_not_found[21];
    extern const char s_entry_suffix[3];
    extern const char s_entry_prefix[9];

    extern uint32_t _farload_file_flag;
    extern char _farload_file_name[16];
    extern FILE* _farload_fptr;
    extern s_lbx_header _lbx_header;
    extern char _secondary_drive_path[52];

    void* __cdecl Far_Reload_(const char* path, int32_t idx, void* param3);

    void* __cdecl Farload_Data_Static_(const char* filename, int32_t resource_id, void* buffer, int32_t param_4, int32_t param_5, int32_t param_6);

    void* __cdecl Far_Reload_Next_(const char* filename, int entry, void* buffer);

    void* __cdecl Farload_Library_Data_(const char* fname, int32_t idx, void* out, int16_t alloc_type, uint32_t start_idx, uint16_t count, uint16_t expected_element_size);

    void* __cdecl Far_Reload_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6);

    void* __cdecl Far_Reload_Next_Data_(const char* filename, int32_t idx, void* buffer, uint32_t entry_num, uint16_t type, uint16_t size);

    void* __cdecl Farload_Library_(const char* lib_name, int idx, void* param_3, int alloc_type);

    int16_t __cdecl Get_Farload_Data_Record_Count_(const char* file_name, int entry_idx);

    void* __cdecl Farload_(char* file_name, int param_2);

    void __cdecl Set_Alternate_Path_(char* path);
}