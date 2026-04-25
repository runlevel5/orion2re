#pragma once

namespace OCPOLPUP {
    inline uint16_t& _value =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00564758});

    inline MOX2::action_func_t& _saved_autofunction =
        *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x0056475C});

    void __cdecl Occupation_Policy_Popup_();
}