#include "pch.h"

namespace farload {

const char s_vga_frames_error[53] = " Vga file animation frames cannot exceed 65536 bytes";
const char s_error_defined_records[35] = " exceeds number of defined records";
const char s_error_record_size[30] = " has an incorrect record size";
const char s_error_lbx_entries[31] = " exceeds number of LBX entries";
const char s_error_not_lbx[20] = " is not an LBX file";
const char s_error_reload_failed[40] = " failed to reload. Allocation too small";
const char s_error_improper_entry[31] = " has an improper entry number.";
const char s_error_insufficient_memory[74] = "Insufficient memory. You need at least 575K free. Try removing all TSR's.";
const char s_error_corrupted[21] = " has been corrupted.";
const char s_error_not_found[21] = " could not be found.";
const char s_entry_suffix[3] = "] ";
const char s_entry_prefix[9] = " [entry ";

uint32_t _farload_file_flag;
char _farload_file_name[16];
FILE* _farload_fptr;
s_lbx_header _lbx_header;
char _secondary_drive_path[52];

void* __cdecl Farload_Library_(const char* lib_name, int idx, void* param_3, int alloc_type) {
    char temp_path[44];
    void* dest_buf = nullptr;

    if (idx < 0) {
        farload::Error_Handler_(lib_name, 4, idx);
    }

    bool file_already_open = false;
    if (farload::_farload_file_flag != 0) {
        if (_strcmpi(lib_name, farload::_farload_file_name) == 0) {
            file_already_open = true;
        } else {
            fclose(farload::_farload_fptr);
        }
    }

    if (!file_already_open) {
        // Copy library name to global tracker
        const char* src = lib_name;
        char* dst = farload::_farload_file_name;
        while ((*dst++ = *src++) != '\0');

        farload::_farload_fptr = fopen(lib_name, s_rb_0055117c);
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

            farload::_farload_fptr = fopen(temp_path, s_rb_0055117c);
            if (farload::_farload_fptr == nullptr) {
                farload::Error_Handler_(lib_name, 1, idx);
            }
        }

        fread(&farload::_lbx_header, 0x800, 1, farload::_farload_fptr);
        if (farload::_lbx_header.magic != 0xFEAD) {
            farload::Error_Handler_(lib_name, 7, idx);
        }
        farload::_farload_file_flag = 1;
    }

    if ((uint32_t)farload::_lbx_header.entry_count <= (uint32_t)idx) {
        farload::Error_Handler_(lib_name, 8, idx);
    }

    uint32_t offset = farload::_lbx_header.offsets[idx];
    uint32_t size = farload::_lbx_header.offsets[idx + 1] - offset;
    fseek(farload::_farload_fptr, (long)offset, 0);

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
        memcpy(&dest_buf, temp_path, sizeof(dest_buf));
    }

    fread(dest_buf, size, 1, farload::_farload_fptr);
    return dest_buf;
}

void* __cdecl Far_Reload_(const char* path, int32_t idx, void* param3) {
    return Farload_Library_(path, idx, param3, 1);
}

void* __cdecl Far_Reload_Next_(const char* filename, int entry, void* buffer) {
    return Farload_Library_(filename, entry, buffer, 2);
}

void* __cdecl Farload_(char* file_name, int param_2) {
    return Farload_Library_(file_name, param_2, nullptr, 0);
}

    void* __cdecl Farload_Library_Data_(const char* fname, int32_t idx, void* out, int16_t alloc_type, uint32_t start_idx, uint16_t count, uint16_t expected_element_size) {
        uint8_t local_3c[60];

        if (idx < 0) {
            Error_Handler_(fname, 4, idx);
        }

        bool load_header = true;
        if (_farload_file_flag != 0) {
            if (_strcmpi(fname, _farload_file_name) == 0) {
                load_header = false;
            } else {
                if (_farload_file_flag != 0) {
                    fclose(_farload_fptr);
                }
            }
        }

        if (load_header) {
            strcpy(_farload_file_name, fname);
            _farload_fptr = fopen(fname, s_rb_0055117c);
            
            if (_farload_fptr == nullptr) {
                if (_secondary_drive_path[0] == '\0') {
                    Error_Handler_(fname, 1, idx);
                }

                strcpy((char*)local_3c, _secondary_drive_path);
                strcat((char*)local_3c, _farload_file_name);

                _farload_fptr = fopen((char*)local_3c, s_rb_0055117c);
                if (_farload_fptr == nullptr) {
                    Error_Handler_(fname, 1, idx);
                }
            }

            fread(&_lbx_header, 0x800, 1, _farload_fptr);
            if (_lbx_header.magic != 0xfead) {
                Error_Handler_(fname, 7, idx);
            }
            _farload_file_flag = 1;
        }

        if ((uint32_t)_lbx_header.entry_count <= (uint32_t)idx) {
            Error_Handler_(fname, 8, idx);
        }

        fseek(_farload_fptr, _lbx_header.offsets[idx], 0);

        uint16_t total_count;
        uint16_t element_size;

        fread(&total_count, 2, 1, _farload_fptr);
        fread(&element_size, 2, 1, _farload_fptr);

        if (count == 0xFFFFu) {
            count = (uint16_t)(total_count - start_idx);
        }

        if (total_count <= start_idx) {
            Error_Handler_(fname, 10, idx);
        }

        if (element_size != expected_element_size) {
            Error_Handler_(fname, 9, idx);
        }

        fseek(_farload_fptr, element_size * start_idx, 1);

        uint32_t read_size = element_size * count;

        switch (alloc_type) {
            case 0:
                out = allocate::Allocate_Space_No_Header_(read_size);
                if (out == nullptr) {
                    Error_Handler_(fname, 3, idx);
                }
                break;
            case 1:
                out = allocate::Allocate_First_Block_No_Header_(out, read_size);
                if (out == nullptr) {
                    Error_Handler_(fname, 5, idx);
                }
                break;
            case 2:
                out = allocate::Allocate_Next_Block_No_Header_(out, read_size);
                if (out == nullptr) {
                    Error_Handler_(fname, 5, idx);
                }
                break;
            case 3:
                break;
            default:
                out = *reinterpret_cast<void**>(&local_3c[0]);
                break;
        }

        fread(out, read_size, 1, _farload_fptr);

        return out;
    }

void* __cdecl Far_Reload_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6) {
    return Farload_Library_Data_(path, idx, out, 1, size, param_5, param_6);
}

void* __cdecl Far_Reload_Next_Data_(const char* filename, int32_t idx, void* buffer, uint32_t entry_num, uint16_t type, uint16_t size) {
    return Farload_Library_Data_(filename, idx, buffer, 2, entry_num, type, size);
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

    itoa(entry_num, itoa_buf, 10);
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

int16_t __cdecl Get_Farload_Data_Record_Count_(const char* file_name, int entry_idx) {
    if (entry_idx < 0) {
        Error_Handler_(file_name, 4, entry_idx);
    }

    bool need_to_open = true;
    if (_farload_file_flag != 0) {
        if (_strcmpi(file_name, _farload_file_name) == 0) {
            need_to_open = false;
        } else {
            fclose(_farload_fptr);
        }
    }

    if (need_to_open) {
        strcpy(_farload_file_name, file_name);
        _farload_fptr = fopen(file_name, s_rb_0055117c);

        if (_farload_fptr == nullptr) {
            if (_secondary_drive_path[0] == '\0') {
                Error_Handler_(file_name, 1, entry_idx);
            }

            char full_path[60];
            strcpy(full_path, _secondary_drive_path);
            strcat(full_path, _farload_file_name);

            _farload_fptr = fopen(full_path, s_rb_0055117c);
            if (_farload_fptr == nullptr) {
                Error_Handler_(file_name, 1, entry_idx);
            }
        }

        fread(&_lbx_header, 0x800, 1, _farload_fptr);

        if (_lbx_header.magic != 0xFEAD) {
            Error_Handler_(file_name, 7, entry_idx);
        }

        _farload_file_flag = 1;
    }

    if ((int32_t)_lbx_header.entry_count <= entry_idx) {
        Error_Handler_(file_name, 8, entry_idx);
    }

    fseek(_farload_fptr, _lbx_header.offsets[entry_idx], 0 /* SEEK_SET */);

    int16_t record_count;
    fread(&record_count, 2, 1, _farload_fptr);

    return record_count;
}

void* __cdecl Farload_Data_Static_(const char* filename, int32_t resource_id, void* buffer, int32_t param_4, int32_t param_5, int32_t param_6) {
    return farload::Farload_Library_Data_(filename, resource_id, buffer, 3, (uint32_t)param_4, (uint16_t)param_5, (uint16_t)param_6);
}

void __cdecl Set_Alternate_Path_(char* input_path) {
    strcpy(_secondary_drive_path, input_path);

    int32_t path_length = 0;
    while (path_length < 100 && _secondary_drive_path[path_length] != '\0') {
        ++path_length;
    }

    // Potential bug: empty input reads one byte before _secondary_drive_path.
    if (*((char*)_secondary_drive_path + path_length - 1) != '\\') {
        strcat(_secondary_drive_path, k_backslash);
    }
}

}
