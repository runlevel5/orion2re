#pragma once

struct IDirectSound;

namespace sound {
    inline IDirectSound*& _dsound =
        *reinterpret_cast<IDirectSound**>(std::uintptr_t{0x0057C754});

    inline void*& _soundBuffer =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0055B520 });

    inline uint32_t& _soundBufferSize =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B524 });

    inline void __cdecl Audio_Clear_Cache_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E4A0 });
        fn();
    }

    inline void __cdecl Fade_Stream_Down_(uint8_t dur) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005324E0 });
        fn(dur);
    }

    inline int16_t __cdecl Get_Stream_Volume_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00532080 });
        return fn();
    }

    inline void __cdecl Stop_Stream_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00531FE0 });
        fn();
    }

    inline void __cdecl Stream_On_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00532040 });
        fn();
    }

    inline void __cdecl Set_Stream_Max_Volume_(int32_t volume) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00532060 });
        fn(volume);
    }

    inline void __cdecl Set_Stream_Filename_(const char* filename) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00531FA0 });
        fn(filename);
    }

    inline void __cdecl Play_Stream_(int32_t stream_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00532130 });
        fn(stream_id);
    }

    inline void __cdecl Loop_Stream_(int32_t loop_count) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00531FC0 });
        fn(loop_count);
    }

    inline void __cdecl Set_Stream_Volume_(int32_t volume) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005320E0 });
        fn(volume);
    }

    inline void __cdecl Fade_Stream_Up_(uint8_t duration) {
        using Fn = void(__cdecl*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005324C0 });
        fn(duration);
    }

    void __cdecl Set_Audio_Buffer_(void* buffer);

    inline void __cdecl Sound_On_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E610 });
        fn();
    }

    inline void __cdecl Stop_All_Sounds_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E710 });
        fn();
    }

    inline void __cdecl Resume_Sound_(int32_t channel) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052DCC0 });
        fn(channel);
    }

    inline void __cdecl Set_Sound_Max_Volume_(int32_t volume) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D500 });
        fn(volume);
    }

    inline void __cdecl Stop_Sound_(int32_t handle) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052e6b0});
        fn(handle);
    }

    inline void __cdecl Sound_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052E630});
        fn();
    }

    inline int __cdecl Play_Sound_(int16_t sound_id) {
        using Fn = int(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052de10});
        return fn(sound_id);
    }

    inline void __cdecl Fadeout_Sound_(int32_t handle, int32_t duration) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        reinterpret_cast<Fn>(0x0052e850)(handle, duration);
    }

    inline void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052D520})(handle, pitch);
    }

    inline void __cdecl Loop_Sound_(int handle, int loop_count) {
        using Fn = void(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052d560});
        fn(handle, loop_count);
    }

    inline void __cdecl Init_Sound_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052d5d0 });
        fn();
    }
}