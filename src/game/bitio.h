#pragma once

namespace bitio {
    /**
     * @brief Allocates and initializes a bit buffer for reading from a byte stream.
     * @param buffer Source byte buffer to read bits from.
     * @return Initialized bit-buffer state, or `nullptr` on allocation failure.
     */
    BIT_BUF* __cdecl OpenInputBitBuffer_(uint8_t* buffer);
    /**
     * @brief Allocates and initializes a bit buffer for writing into a byte stream.
     * @param buffer Destination byte buffer to receive packed bits.
     * @return Initialized bit-buffer state, or `nullptr` on allocation failure.
     */
    BIT_BUF* __cdecl OpenOutputBitBuffer_(uint8_t* buffer);
    /**
     * @brief Releases a bit buffer that was opened for reading.
     * @param bit_buf Bit-buffer state to destroy.
     */
    void __cdecl CloseInputBitBuffer_(BIT_BUF* bit_buf);
    /**
     * @brief Flushes any partially written byte, frees the bit buffer, and returns the output size.
     * @param bit_buf Bit-buffer state to flush and destroy.
     * @return Number of bytes written into the output buffer.
     */
    uint32_t __cdecl CloseOutputBitBuffer_(BIT_BUF* bit_buf);
    /**
     * @brief Reads the next single bit from the input buffer.
     * @param bit_buf Bit-buffer state tracking the source buffer and current bit position.
     * @return `1` when the next bit is set, otherwise `0`.
     */
    uint8_t __cdecl BufferInputBit_(BIT_BUF* bit_buf);
    /**
     * @brief Appends one bit to the output buffer.
     * @param bit_buf Bit-buffer state tracking the destination buffer and current bit position.
     * @param bit Bit value to write; non-zero writes `1`, zero writes `0`.
     */
    void __cdecl BufferOutputBit_(BIT_BUF* bit_buf, int32_t bit);
    /**
     * @brief Appends multiple high-to-low bits from a value to the output buffer.
     * @param bit_buf Bit-buffer state tracking the destination buffer and current bit position.
     * @param data Source value whose bits will be written.
     * @param count Number of bits to write from `data`.
     */
    void __cdecl BufferOutputBits_(BIT_BUF* bit_buf, uint32_t data, uint8_t count);
}