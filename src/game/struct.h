#pragma once

#pragma pack(push, 1)
struct s_star_dist {
    int16_t dist;
    int16_t index;
};
#pragma pack(pop)

namespace struct_ {
    void __cdecl Clear_Structure_(void* s, uint32_t len);
    void __cdecl Copy_Structure_(void* dest, const void* src, uint32_t size);
    bool __cdecl Test_Bit_Field_(void* field, uint16_t bit_index);
    void __cdecl Set_Bit_Field_(void* field, uint16_t bit);
}