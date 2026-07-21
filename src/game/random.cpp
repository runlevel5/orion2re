#include "pch.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <execinfo.h>
#endif

#if defined(__linux__)
extern "C" char __executable_start;
extern "C" char _end;
#endif

namespace {
    constexpr uint32_t k_return_address_capacity = 10;
    constexpr uint32_t k_stack_frame_capacity = 62;

    struct s_local_image {
        uintptr_t base;
        uintptr_t end;
    };

    s_local_image Local_Image_() {
#if defined(_WIN32)
        const HMODULE module = GetModuleHandleW(nullptr);
        const uintptr_t base = reinterpret_cast<uintptr_t>(module);
        if (module == nullptr) {
            return {};
        }

        const IMAGE_DOS_HEADER* dos_header = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
            return {base, base};
        }
        const IMAGE_NT_HEADERS* nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS*>(
            base + dos_header->e_lfanew);
        if (nt_headers->Signature != IMAGE_NT_SIGNATURE) {
            return {base, base};
        }
        return {base, base + nt_headers->OptionalHeader.SizeOfImage};
#elif defined(__linux__)
        return {
            reinterpret_cast<uintptr_t>(&__executable_start),
            reinterpret_cast<uintptr_t>(&_end)
        };
#else
        return {};
#endif
    }

    uint32_t Capture_Return_Addresses_(uintptr_t* return_addresses,
        const s_local_image& image) {
        void* frames[k_stack_frame_capacity] = {};
        uint32_t frame_count = 0;
#if defined(_WIN32)
        frame_count = CaptureStackBackTrace(1, k_stack_frame_capacity, frames, nullptr);
#elif defined(__linux__) || defined(__APPLE__)
        const int captured = backtrace(frames, k_stack_frame_capacity);
        frame_count = captured > 1 ? (uint32_t)captured : 0;
#endif

        uint32_t return_address_count = 0;
        for (uint32_t i = 1; i < frame_count &&
                return_address_count < k_return_address_capacity; ++i) {
            const uintptr_t address = reinterpret_cast<uintptr_t>(frames[i]);
            if (image.base != 0 && (address < image.base || address >= image.end)) {
                continue;
            }
            return_addresses[return_address_count++] = address;
        }
        return return_address_count;
    }
}

namespace game_random {
    uint32_t _random_seed;
    uint32_t _saved_random_seed;

    uint32_t __cdecl Get_Random_Seed_() {
        return _random_seed;
    }

    void __cdecl Save_Random_Seed_() {
        _saved_random_seed = _random_seed;
    }

    uint32_t __cdecl Set_Random_Seed_(uint32_t seed) {
        _random_seed = seed;
        return seed;
    }

    uint32_t __cdecl Randomize_() {
        uint32_t current_time = (uint32_t)time(nullptr);
        return Set_Random_Seed_(current_time);
    }

    uint32_t __cdecl Random_(uint32_t max_val) {
        uint32_t quotient = 0xFFFFFFFFu / max_val;
        uint32_t limit = max_val * quotient;

        uint32_t seed = _random_seed;
        do {
            seed = seed * 0x41C64E6D + 0x3039;
        } while (limit <= seed);

        _random_seed = seed;
        const uint32_t value = (seed / quotient) + 1;

        uintptr_t return_addresses[k_return_address_capacity] = {};
        const s_local_image image = Local_Image_();
        const uint32_t return_address_count = Capture_Return_Addresses_(
            return_addresses, image);
        oracle::Verify_Random_(max_val, value, seed, return_addresses,
            return_address_count, image.base);
        return value;
    }

    void __cdecl Restore_Random_Seed_() {
        _random_seed = _saved_random_seed;
    }
}