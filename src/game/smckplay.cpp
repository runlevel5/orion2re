#include "pch.h"

namespace smckplay {
    int8_t __cdecl Open_Smacker_Animation_(const char* file_name, char mode, uint32_t options) {
        char path_buffer[100];
        IDirectSound* direct_sound = sound::_dsound;

        ::SmackSoundUseDirectSound(direct_sound);

        ::strcpy(path_buffer, file_name);

        uint32_t open_flags = ((mode == 1) ? 0xFE000u : 0u) | options;
        _smk = ::SmackOpen(path_buffer, open_flags, -1);
        if (_smk == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                return 0;
            }

            ::strcpy(path_buffer, farload::_secondary_drive_path);
            ::strcat(path_buffer, file_name);
            _smk = ::SmackOpen(path_buffer, open_flags, -1);
        }

        return static_cast<int8_t>(_smk != nullptr);
    }

    int32_t __cdecl Get_Smacker_Animation_Width_() {
        if (_smk == nullptr) {
            return 0;
        }
        return static_cast<int32_t>(_smk->width);
    }

    int32_t __cdecl Get_Smacker_Animation_Height_() {
        if (_smk == nullptr) {
            return 0;
        }
        return _smk->height;
    }

    void __cdecl Draw_Smacker_Animation_() {
        Do_Draw_Smacker_Animation_(SMACK::_smack_x, SMACK::_smack_y);
    }

    void __cdecl Do_Draw_Smacker_Animation_(int32_t left, int32_t top) {
        if (smckplay::_smk != nullptr) {
            int32_t wait_result;
            do {
                wait_result = smckplay::_SmackWait_4(smckplay::_smk);
            } while (wait_result != 0);

            if (smckplay::_smk->has_palette != 0) {
                smckplay::Set_Smacker_Palette_();
            }

            DDSURFACEDESC surface_desc;
            LPDDSURFACEDESC surface_desc_ptr = &surface_desc;

            surface_desc.dwSize = 0x6c;
            surface_desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
            platform::g_offscreen_surface_1->GetSurfaceDesc(surface_desc_ptr);
            platform::g_offscreen_surface_1->Lock(nullptr, surface_desc_ptr, DDLOCK_WAIT, nullptr);

            smckplay::_SmackToBuffer_28(smckplay::_smk,
                left,
                top,
                surface_desc.lPitch,
                smckplay::_smk->height,
                surface_desc.lpSurface,
                0);

            smckplay::_SmackDoFrame_4(smckplay::_smk);
            platform::g_offscreen_surface_1->Unlock(nullptr);

            int32_t rect_result = smckplay::_SmackToBufferRect_8(smckplay::_smk, 0);
            while (rect_result != 0) {
                setvesa::Add_Square_To_Scan_List_(smckplay::_smk->rect_left,
                    smckplay::_smk->rect_top,
                    smckplay::_smk->rect_width + smckplay::_smk->rect_left,
                    smckplay::_smk->rect_height + smckplay::_smk->rect_top);

                if (smckplay::_smk->rect_left == 0 &&
                    smckplay::_smk->rect_top == 0 &&
                    smckplay::_smk->rect_width == smckplay::_smk->width &&
                    smckplay::_smk->rect_height == smckplay::_smk->height) {
                    smckplay::_lastkey = smckplay::_smk->current_frame + 1;
                }

                rect_result = smckplay::_SmackToBufferRect_8(smckplay::_smk, 0);
            }

            if (smckplay::_smk->frame_count - smckplay::_smk->current_frame != 1U) {
                smckplay::_SmackNextFrame_4(smckplay::_smk);
            }
        }
    }

    void __cdecl Set_Smacker_Palette_() {
        uint8_t(*smk_palette)[3] = _smk->palette;

        for (s_palette_entry* current_entry = video::_current_palette + 10;
            current_entry < video::_current_palette + 246;
            ++current_entry, ++smk_palette) {
            current_entry->changed = 1;
            current_entry->r = (*smk_palette)[0];
            current_entry->g = (*smk_palette)[1];
            current_entry->b = (*smk_palette)[2];
        }
    }

    uint32_t __cdecl Get_Smacker_Animation_Total_Frames_() {
        if (_smk == nullptr) {
            return 0;
        }
        return _smk->frame_count;
    }

    uint32_t __cdecl Get_Smacker_Animation_Frame_() {
        if (smckplay::_smk == nullptr) {
            return 0;
        }
        return smckplay::_smk->current_frame;
    }

    void __cdecl Close_Smacker_Animation_(void) {
        if (_smk != nullptr) {
            PTR__SmackClose_4_005aa644(_smk);
            _smk = nullptr;
        }
    }
}