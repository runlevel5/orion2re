#pragma once

#include <cstdint>
#include <cstdio>

namespace bitio {
    BIT_BUF* __cdecl OpenInputBitBuffer_(uint8_t* buffer);
    BIT_BUF* __cdecl OpenOutputBitBuffer_(uint8_t* buffer);
    void __cdecl CloseInputBitBuffer_(BIT_BUF* bit_buf);
    uint32_t __cdecl CloseOutputBitBuffer_(BIT_BUF* bit_buf);
    uint8_t __cdecl BufferInputBit_(BIT_BUF* bit_buf);
    void __cdecl BufferOutputBit_(BIT_BUF* bit_buf, int bit);
    void __cdecl BufferOutputBits_(BIT_BUF* bit_buf, uint32_t data, uint8_t count);
}