#pragma once

namespace smckplay {
    extern s_smack_video* _smk;
    extern uint32_t _lastkey;

    extern "C" __declspec(dllimport) void __stdcall _SmackSoundUseDirectSound(IDirectSound* direct_sound);

    extern "C" __declspec(dllimport) s_smack_video* __stdcall _SmackOpen(char* path, uint32_t flags, int32_t priority);

    extern "C" __declspec(dllimport) int32_t __stdcall _SmackWait(s_smack_video* smk);

    extern "C" __declspec(dllimport) int32_t __stdcall _SmackToBufferRect(s_smack_video* smk, int32_t mode);

    extern "C" __declspec(dllimport) void __stdcall _SmackDoFrame(s_smack_video* smk);

    extern "C" __declspec(dllimport) void __stdcall _SmackToBuffer(s_smack_video* smk,
        int32_t left,
        int32_t top,
        uint32_t pitch,
        uint32_t destheight,
        void* buf,
        uint32_t flags);

    extern "C" __declspec(dllimport) void __stdcall _SmackNextFrame(s_smack_video* smk);

    extern "C" __declspec(dllimport) void __stdcall _SmackClose(s_smack_video* smk);

    inline int32_t __stdcall _SmackWait_4(s_smack_video* smk) {
        return _SmackWait(smk);
    }

    inline int32_t __stdcall _SmackToBufferRect_8(s_smack_video* smk, int32_t mode) {
        return _SmackToBufferRect(smk, mode);
    }

    inline void __stdcall _SmackDoFrame_4(s_smack_video* smk) {
        _SmackDoFrame(smk);
    }

    inline void __stdcall _SmackToBuffer_28(s_smack_video* smk,
        int32_t left,
        int32_t top,
        uint32_t pitch,
        uint32_t destheight,
        void* buf,
        uint32_t flags) {
        _SmackToBuffer(smk, left, top, pitch, destheight, buf, flags);
    }

    inline void __stdcall _SmackNextFrame_4(s_smack_video* smk) {
        _SmackNextFrame(smk);
    }

    inline void __stdcall _SmackClose_4(s_smack_video* smk) {
        _SmackClose(smk);
    }

    /* Copies the current Smacker palette into the active video palette range. */
    void __cdecl Set_Smacker_Palette_();

    /* Opens a Smacker animation using the requested file and Smacker open flags.
     * file_name: Path to the Smacker file to open.
     * mode: When equal to 1, includes the 0xFE000 Smacker open flags.
     * options: Additional Smacker open flags.
     */
    int8_t __cdecl Open_Smacker_Animation_(const char* file_name, char mode, uint32_t options);

    /* Returns the width of the currently opened Smacker animation in pixels. */
    int32_t __cdecl Get_Smacker_Animation_Width_();

    /* Returns the height of the currently opened Smacker animation in pixels. */
    int32_t __cdecl Get_Smacker_Animation_Height_();

    /* Draws the current Smacker frame into the offscreen surface at the given position.
     * left: Destination x coordinate.
     * top: Destination y coordinate.
     */
    void __cdecl Do_Draw_Smacker_Animation_(int32_t left, int32_t top);

    /* Draws the current Smacker frame at the global Smacker draw position. */
    void __cdecl Draw_Smacker_Animation_();

    /* Returns the total number of frames in the currently opened Smacker animation. */
    uint32_t __cdecl Get_Smacker_Animation_Total_Frames_();

    /* Returns the current frame index of the opened Smacker animation. */
    uint32_t __cdecl Get_Smacker_Animation_Frame_();

    /* Closes the currently opened Smacker animation, if any. */
    void __cdecl Close_Smacker_Animation_();
}