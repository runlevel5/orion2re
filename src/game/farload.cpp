#include "deps.h"

namespace farload {

void* __cdecl Farload_Library_(char* lib_name, int idx, void* param_3, int alloc_type) {
    char temp_path[60];
    void* dest_buf = nullptr;

    if (idx < 0) {
        farload::Error_Handler_(lib_name, 4, idx);
    }

    bool file_already_open = false;
    if (farload::_farload_file_flag != 0) {
        if (__strcmpi(lib_name, farload::_farload_file_name) == 0) {
            file_already_open = true;
        } else {
            _fclose(farload::_farload_fptr);
        }
    }

    if (!file_already_open) {
        // Copy library name to global tracker
        char* src = lib_name;
        char* dst = farload::_farload_file_name;
        while ((*dst++ = *src++) != '\0');

        farload::_farload_fptr = _wfopen(lib_name, s_rb_0055117c);
        if (farload::_farload_fptr == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                farload::Error_Handler_(lib_name, 1, idx);
            }

            // Construct fallback path: drive + filename
            char* p = temp_path;
            char* s = farload::_secondary_drive_path;
            while ((*p++ = *s++) != '\0');
            --p; // Point to null terminator to append
            s = farload::_farload_file_name;
            while ((*p++ = *s++) != '\0');

            farload::_farload_fptr = _wfopen(temp_path, s_rb_0055117c);
            if (farload::_farload_fptr == nullptr) {
                farload::Error_Handler_(lib_name, 1, idx);
            }
        }

        _fread(&farload::_lbx_header, 0x800, 1, farload::_farload_fptr);
        if (farload::_lbx_header.magic != 0xFEAD) {
            farload::Error_Handler_(lib_name, 7, idx);
        }
        farload::_farload_file_flag = 1;
    }

    if (static_cast<int>(farload::_lbx_header.entry_count) <= idx) {
        farload::Error_Handler_(lib_name, 8, idx);
    }

    uint32_t offset = farload::_lbx_header.offsets[idx];
    uint32_t size = farload::_lbx_header.offsets[idx + 1] - offset;
    _fseek(farload::_farload_fptr, (long)offset, 0);

    if (alloc_type == 0) {
        dest_buf = allocate::Allocate_Space_No_Header_(size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 3, idx);
        }
    } else if (alloc_type == 1) {
        dest_buf = allocate::Allocate_First_Block_No_Header_((int)param_3, size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 5, idx);
        }
    } else if (alloc_type == 2) {
        dest_buf = allocate::Allocate_Next_Block_No_Header_((int)param_3, size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 5, idx);
        }
    } else {
        // Fallback: uses the start of the local scratch buffer as a pointer
        dest_buf = *reinterpret_cast<void**>(temp_path);
    }

    _fread(dest_buf, size, 1, farload::_farload_fptr);
    return dest_buf;
}

void* __cdecl Far_Reload_(char* path, int32_t idx, void* param3) {
    return Farload_Library_(path, idx, param3, 1);
}

}
