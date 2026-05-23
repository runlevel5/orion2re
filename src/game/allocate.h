#pragma once

#include <cstdint>
#include <cstddef>

namespace allocate {
#pragma pack(push, 1)
    struct s_alloc_header {
        uint32_t current;
        uint32_t size;
        uint32_t reserved2;
    };
#pragma pack(pop)

    void* __cdecl Allocate_Space_(uint32_t size);

    void Allocate_Data_Space_();

    int32_t __cdecl Get_Remaining_Block_Space_(void* block);

    uint32_t __cdecl Get_Max_Block_Space_(void* ptr);

    void __cdecl Allocation_Error_(int bytes_requested);

    inline uint32_t (&_mark_stack_pos)[2] =
        *reinterpret_cast<uint32_t (*)[2]>(std::uintptr_t{ 0x0055B650 });

    inline uint32_t (&_mark_stack)[39][2] =
        *reinterpret_cast<uint32_t (*)[39][2]>(std::uintptr_t{ 0x0057DF00 });

    inline const char* s_Mark_Block_Stack_Size_Exceeded_0055b6dc =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B6DC }); // "Mark Block Stack Size Exceeded"

    inline const char* s_Mark_Block_Stack_Empty_0055b6fc =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x0055B6FC }); // "Mark Block Stack Empty"

    void __cdecl Push_Block_(void* block);

    void __cdecl Pop_Block_(void* block);

    int32_t __cdecl Mark_Block_(void* block_ptr);

    void* __cdecl Release_Block_(void* block);

    void __cdecl Reallocation_Error_(uint32_t size, int32_t arg2, int32_t arg3);

    void* __cdecl Allocate_Next_Block_(void* block, uint32_t size);

    void* __cdecl Allocate_Dos_Space_(uint32_t size);

    void* __cdecl Allocate_Far_Space_(uint32_t size);

    void* __cdecl Allocate_Space_No_Header_(uint32_t size);

    void* __cdecl Allocate_First_Block_No_Header_(void* block_ptr, uint32_t size);

    void* __cdecl Allocate_Next_Block_No_Header_(void* block_ptr, uint32_t size);

    void* __cdecl Allocate_First_Block_(void* block, uint32_t size);
}