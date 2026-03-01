#include "pch.h"

namespace farload {

void* __cdecl Farload_Library_(const char* lib_name, int idx, void* param_3, int alloc_type) {
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
        const char* src = lib_name;
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
        dest_buf = allocate::Allocate_First_Block_No_Header_(param_3, size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 5, idx);
        }
    } else if (alloc_type == 2) {
        dest_buf = allocate::Allocate_Next_Block_No_Header_(param_3, size);
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

void* __cdecl Far_Reload_(const char* path, int32_t idx, void* param3) {
    return Farload_Library_(path, idx, param3, 1);
}

void* __cdecl Far_Reload_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6) {
    return Farload_Library_Data_(path, idx, out, 1, size, param_5, param_6);
}

void* __cdecl Far_Reload_Next_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6) {
    return Farload_Library_Data_(path, idx, out, 2, size, param_5, param_6);
}

void __cdecl Error_Handler_(const char* filename, int error_code, int entry_num) {
    char message[120];
    char itoa_buf[20];

    auto strcpy_custom = [](char* dest, const char* src) -> char* {
        while ((*dest = *src) != '\0') {
            dest++;
            src++;
        }
        return dest;
    };

    auto strcat_custom = [](char* dest, const char* src) -> char* {
        while (*dest != '\0') dest++;
        while ((*dest = *src) != '\0') {
            dest++;
            src++;
        }
        return dest;
    };

    char* current = strcpy_custom(message, filename);
    current = strcat_custom(message, s_entry_prefix);

    __itoa(entry_num, itoa_buf, 10);
    current = strcat_custom(message, itoa_buf);
    current = strcat_custom(message, s_entry_suffix);

    const char* detail = nullptr;

    if (error_code >= 1 && error_code <= 16) {
        switch (error_code) {
        case 1:  detail = s_error_not_found; break;
        case 2:  detail = s_error_corrupted; break;
        case 3:
            strcpy_custom(message, s_error_insufficient_memory);
            exitm::Exit_With_Message(message);
            return;
        case 4:  detail = s_error_improper_entry; break;
        case 5:  detail = s_error_reload_failed; break;
        case 7:  detail = s_error_not_lbx; break;
        case 8:  detail = s_error_lbx_entries; break;
        case 9:  detail = s_error_record_size; break;
        case 10: detail = s_error_defined_records; break;
        case 16: detail = s_vga_frames_error; break;
        default: break;
        }
    }

    if (detail) {
        strcat_custom(message, detail);
    }

    exitm::Exit_With_Message(message);
}

}
