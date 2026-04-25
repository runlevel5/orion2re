#pragma once

namespace FIREPTS {
    inline uint8_t* (&_fp_data_ptr)[8] = *reinterpret_cast<uint8_t*(*)[8]>(std::uintptr_t{0x00571E48});
    inline uint8_t*& _fp_seg = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00571E68});

    void __cdecl Load_FP_Data_From_LBX_();
}