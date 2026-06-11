#include "pch.h"

namespace farload {

uint32_t _farload_file_flag;
char _farload_file_name[16];
FILE* _farload_fptr;
s_lbx_header _lbx_header;
char _secondary_drive_path[52];

void* __cdecl Farload_Library_(const char* lib_name, int32_t entry_idx, void* allocation_base, int32_t allocation_mode) {
    char fallback_path[sizeof(_secondary_drive_path) + sizeof(_farload_file_name)];
    void* dest_buf = nullptr;

    if (entry_idx < 0) {
        farload::Error_Handler_(lib_name, 4, entry_idx);
    }

    bool file_already_open = false;
    if (farload::_farload_file_flag != 0) {
        if (strcasecmp(lib_name, farload::_farload_file_name) == 0) {
            file_already_open = true;
        } else {
            fclose(farload::_farload_fptr);
        }
    }

    if (!file_already_open) {
        const char* src = lib_name;
        char* dst = farload::_farload_file_name;
        while ((*dst++ = *src++) != '\0');

        farload::_farload_fptr = fopen_case(lib_name, "rb");
        if (farload::_farload_fptr == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                farload::Error_Handler_(lib_name, 1, entry_idx);
            }

            char* p = fallback_path;
            char* s = farload::_secondary_drive_path;
            while ((*p++ = *s++) != '\0');
            --p;
            s = farload::_farload_file_name;
            while ((*p++ = *s++) != '\0');

            farload::_farload_fptr = fopen_case(fallback_path, "rb");
            if (farload::_farload_fptr == nullptr) {
                farload::Error_Handler_(lib_name, 1, entry_idx);
            }
        }

        fread(&farload::_lbx_header, sizeof(farload::_lbx_header), 1, farload::_farload_fptr);
        if (farload::_lbx_header.magic != 0xFEAD) {
            farload::Error_Handler_(lib_name, 7, entry_idx);
        }
        farload::_farload_file_flag = 1;
    }

    if ((uint32_t)farload::_lbx_header.entry_count <= (uint32_t)entry_idx) {
        farload::Error_Handler_(lib_name, 8, entry_idx);
    }

    uint32_t offset = farload::_lbx_header.offsets[entry_idx];
    uint32_t size = farload::_lbx_header.offsets[entry_idx + 1] - offset;
    fseek(farload::_farload_fptr, (long)offset, 0);

    if (allocation_mode == 0) {
        dest_buf = allocate::Allocate_Space_No_Header_(size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 3, entry_idx);
        }
    } else if (allocation_mode == 1) {
        dest_buf = allocate::Allocate_First_Block_No_Header_(allocation_base, size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 5, entry_idx);
        }
    } else if (allocation_mode == 2) {
        dest_buf = allocate::Allocate_Next_Block_No_Header_(allocation_base, size);
        if (dest_buf == nullptr) {
            farload::Error_Handler_(lib_name, 5, entry_idx);
        }
    } else {
        memcpy(&dest_buf, fallback_path, sizeof(dest_buf));
    }

    fread(dest_buf, size, 1, farload::_farload_fptr);
    return dest_buf;
}

void* __cdecl Far_Reload_(const char* path, int32_t entry_idx, void* allocation_base) {
    return Farload_Library_(path, entry_idx, allocation_base, 1);
}

void* __cdecl Far_Reload_Next_(const char* filename, int32_t entry_idx, void* buffer) {
    return Farload_Library_(filename, entry_idx, buffer, 2);
}

void* __cdecl Farload_(char* file_name, int32_t entry_idx) {
    return Farload_Library_(file_name, entry_idx, nullptr, 0);
}

    void* __cdecl Farload_Library_Data_(const char* library_name, int32_t entry_idx, void* buffer, int16_t allocation_mode, uint32_t start_idx, uint16_t record_count, uint16_t expected_element_size) {
        char fallback_path[sizeof(_secondary_drive_path) + sizeof(_farload_file_name)];

        if (entry_idx < 0) {
            Error_Handler_(library_name, 4, entry_idx);
        }

        bool load_header = true;
        if (_farload_file_flag != 0) {
            if (strcasecmp(library_name, _farload_file_name) == 0) {
                load_header = false;
            } else {
                if (_farload_file_flag != 0) {
                    fclose(_farload_fptr);
                }
            }
        }

        if (load_header) {
            strcpy(_farload_file_name, library_name);
            _farload_fptr = fopen_case(library_name, "rb");
            
            if (_farload_fptr == nullptr) {
                if (_secondary_drive_path[0] == '\0') {
                    Error_Handler_(library_name, 1, entry_idx);
                }

                strcpy(fallback_path, _secondary_drive_path);
                strcat(fallback_path, _farload_file_name);

                _farload_fptr = fopen_case(fallback_path, "rb");
                if (_farload_fptr == nullptr) {
                    Error_Handler_(library_name, 1, entry_idx);
                }
            }

            fread(&_lbx_header, sizeof(_lbx_header), 1, _farload_fptr);
            if (_lbx_header.magic != 0xfead) {
                Error_Handler_(library_name, 7, entry_idx);
            }
            _farload_file_flag = 1;
        }

        if ((uint32_t)_lbx_header.entry_count <= (uint32_t)entry_idx) {
            Error_Handler_(library_name, 8, entry_idx);
        }

        fseek(_farload_fptr, _lbx_header.offsets[entry_idx], 0);

        uint16_t total_count;
        uint16_t element_size;

        fread(&total_count, sizeof(total_count), 1, _farload_fptr);
        fread(&element_size, sizeof(element_size), 1, _farload_fptr);

        if (record_count == 0xFFFFu) {
            record_count = (uint16_t)(total_count - start_idx);
        }

        if (total_count <= start_idx) {
            Error_Handler_(library_name, 10, entry_idx);
        }

        if (element_size != expected_element_size) {
            Error_Handler_(library_name, 9, entry_idx);
        }

        fseek(_farload_fptr, element_size * start_idx, 1);

        uint32_t read_size = element_size * record_count;

        switch (allocation_mode) {
            case 0:
                buffer = allocate::Allocate_Space_No_Header_(read_size);
                if (buffer == nullptr) {
                    Error_Handler_(library_name, 3, entry_idx);
                }
                break;
            case 1:
                buffer = allocate::Allocate_First_Block_No_Header_(buffer, read_size);
                if (buffer == nullptr) {
                    Error_Handler_(library_name, 5, entry_idx);
                }
                break;
            case 2:
                buffer = allocate::Allocate_Next_Block_No_Header_(buffer, read_size);
                if (buffer == nullptr) {
                    Error_Handler_(library_name, 5, entry_idx);
                }
                break;
            case 3:
                break;
            default:
                buffer = *(void**)&fallback_path[0];
                break;
        }

        fread(buffer, read_size, 1, _farload_fptr);

        return buffer;
    }

void* __cdecl Far_Reload_Data_(const char* path, int32_t entry_idx, void* buffer, uint32_t start_idx, uint16_t record_count, uint16_t expected_element_size) {
    return Farload_Library_Data_(path, entry_idx, buffer, 1, start_idx, record_count, expected_element_size);
}

void* __cdecl Far_Reload_Next_Data_(const char* filename, int32_t entry_idx, void* buffer, uint32_t start_idx, uint16_t record_count, uint16_t expected_element_size) {
    return Farload_Library_Data_(filename, entry_idx, buffer, 2, start_idx, record_count, expected_element_size);
}

void __cdecl Error_Handler_(const char* filename, int32_t error_code, int32_t entry_num) {
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
    current = strcat_custom(message, " [entry ");

    itoa(entry_num, itoa_buf, 10);
    current = strcat_custom(message, itoa_buf);
    current = strcat_custom(message, "] ");

    const char* detail = nullptr;

    if (error_code >= 1 && error_code <= 16) {
        switch (error_code) {
        case 1:  detail = " could not be found."; break;
        case 2:  detail = " has been corrupted."; break;
        case 3:
            strcpy_custom(message, "Insufficient memory. You need at least 575K free. Try removing all TSR's.");
            exitm::Exit_With_Message(message);
            return;
        case 4:  detail = " has an improper entry number."; break;
        case 5:  detail = " failed to reload. Allocation too small"; break;
        case 7:  detail = " is not an LBX file"; break;
        case 8:  detail = " exceeds number of LBX entries"; break;
        case 9:  detail = " has an incorrect record size"; break;
        case 10: detail = " exceeds number of defined records"; break;
        case 16: detail = " Vga file animation frames cannot exceed 65536 bytes"; break;
        default: break;
        }
    }

    if (detail) {
        strcat_custom(message, detail);
    }

    exitm::Exit_With_Message(message);
}

int16_t __cdecl Get_Farload_Data_Record_Count_(const char* file_name, int32_t entry_idx) {
    if (entry_idx < 0) {
        Error_Handler_(file_name, 4, entry_idx);
    }

    bool need_to_open = true;
    if (_farload_file_flag != 0) {
        if (strcasecmp(file_name, _farload_file_name) == 0) {
            need_to_open = false;
        } else {
            fclose(_farload_fptr);
        }
    }

    if (need_to_open) {
        strcpy(_farload_file_name, file_name);
        _farload_fptr = fopen_case(file_name, "rb");

        if (_farload_fptr == nullptr) {
            if (_secondary_drive_path[0] == '\0') {
                Error_Handler_(file_name, 1, entry_idx);
            }

            char full_path[sizeof(_secondary_drive_path) + sizeof(_farload_file_name)];
            strcpy(full_path, _secondary_drive_path);
            strcat(full_path, _farload_file_name);

            _farload_fptr = fopen_case(full_path, "rb");
            if (_farload_fptr == nullptr) {
                Error_Handler_(file_name, 1, entry_idx);
            }
        }

        fread(&_lbx_header, sizeof(_lbx_header), 1, _farload_fptr);

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
    fread(&record_count, sizeof(record_count), 1, _farload_fptr);

    return record_count;
}

void* __cdecl Farload_Data_Static_(const char* filename, int32_t resource_id, void* buffer, int32_t start_idx, int32_t record_count, int32_t expected_element_size) {
    return farload::Farload_Library_Data_(filename, resource_id, buffer, 3, (uint32_t)start_idx, (uint16_t)record_count, (uint16_t)expected_element_size);
}

void __cdecl Set_Alternate_Path_(char* input_path) {
    strcpy(_secondary_drive_path, input_path);

    int32_t path_length = 0;
    while (path_length < (int32_t)sizeof(_secondary_drive_path) && _secondary_drive_path[path_length] != '\0') {
        ++path_length;
    }

    if (path_length == 0 || _secondary_drive_path[path_length - 1] != '\\') {
        strcat(_secondary_drive_path, "\\");
    }
}

}
