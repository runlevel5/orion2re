#pragma once

namespace OCPOLPUP {
    inline uint16_t& _value =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00564758});

    inline void*& DAT_0056475c =
        *reinterpret_cast<void**>(std::uintptr_t{0x0056475C});

    void __cdecl Occupation_Policy_Popup_();
}