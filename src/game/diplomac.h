#pragma once

namespace diplomac {
    inline int16_t* _starting_relations = reinterpret_cast<int16_t*>(std::uintptr_t{0x00556B40});

    void Init_Diplomatic_Relations_();
}