#pragma once

namespace smckplay {
    inline s_smack_video*& _smk =
        *reinterpret_cast<s_smack_video**>(std::uintptr_t{0x0055C07C});
    inline uint32_t& _lastkey =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C080});

    using SmackWaitFn = int32_t(__stdcall*)(s_smack_video*);
    inline SmackWaitFn& PTR__SmackWait_4_005aa640 =
        *reinterpret_cast<SmackWaitFn*>(std::uintptr_t{ 0x005aa640 });

    using SmackToBufferRectFn = int32_t(__stdcall*)(s_smack_video*, int32_t);
    inline SmackToBufferRectFn& PTR__SmackToBufferRect_8_005aa634 =
        *reinterpret_cast<SmackToBufferRectFn*>(std::uintptr_t{ 0x005aa634 });

    using SmackDoFrameFn = void(__stdcall*)(s_smack_video*);
    inline SmackDoFrameFn& PTR__SmackDoFrame_4_005aa638 =
        *reinterpret_cast<SmackDoFrameFn*>(std::uintptr_t{ 0x005aa638 });

    using SmackToBufferFn = void(__stdcall*)(s_smack_video*, int32_t, int32_t, uint32_t, uint32_t, void*, uint32_t);
    inline SmackToBufferFn& PTR__SmackToBuffer_28_005aa63c =
        *reinterpret_cast<SmackToBufferFn*>(std::uintptr_t{ 0x005aa63c });

    using SmackNextFrameFn = void(__stdcall*)(s_smack_video*);
    inline SmackNextFrameFn& PTR__SmackNextFrame_4_005aa630 =
        *reinterpret_cast<SmackNextFrameFn*>(std::uintptr_t{ 0x005aa630 });

    using SmackCloseFn = void(__stdcall*)(s_smack_video*);
    inline SmackCloseFn& PTR__SmackClose_4_005aa644 =
        *reinterpret_cast<SmackCloseFn*>(std::uintptr_t{ 0x005aa644 });

    inline int32_t __stdcall _SmackWait_4(s_smack_video* smk) {
        return PTR__SmackWait_4_005aa640(smk);
    }

    inline int32_t __stdcall _SmackToBufferRect_8(s_smack_video* smk, int32_t mode) {
        return PTR__SmackToBufferRect_8_005aa634(smk, mode);
    }

    inline void __stdcall _SmackDoFrame_4(s_smack_video* smk) {
        PTR__SmackDoFrame_4_005aa638(smk);
    }

    inline void __stdcall _SmackToBuffer_28(s_smack_video* smk,
        int32_t left,
        int32_t top,
        uint32_t pitch,
        uint32_t destheight,
        void* buf,
        uint32_t flags) {
        PTR__SmackToBuffer_28_005aa63c(smk, left, top, pitch, destheight, buf, flags);
    }

    inline void __stdcall _SmackNextFrame_4(s_smack_video* smk) {
        PTR__SmackNextFrame_4_005aa630(smk);
    }

    inline void __stdcall _SmackClose_4(s_smack_video* smk) {
        PTR__SmackClose_4_005aa644(smk);
    }

    void __cdecl Set_Smacker_Palette_();

    int8_t __cdecl Open_Smacker_Animation_(const char* file_name, char mode, uint32_t options);

    int32_t __cdecl Get_Smacker_Animation_Width_();

    uint32_t __cdecl Get_Smacker_Animation_Height_();

    void __cdecl Do_Draw_Smacker_Animation_(int32_t left, int32_t top);

    void __cdecl Draw_Smacker_Animation_();

    uint32_t __cdecl Get_Smacker_Animation_Total_Frames_();

    uint32_t __cdecl Get_Smacker_Animation_Frame_();

    void __cdecl Close_Smacker_Animation_();
}