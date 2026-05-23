#include "pch.h"

namespace bitio {
    BIT_BUF* __cdecl OpenInputBitBuffer_(uint8_t* buffer) {
        BIT_BUF* bit_buf = (BIT_BUF*)_malloc(sizeof(BIT_BUF));
        if (bit_buf != nullptr) {
            bit_buf->buf = buffer;
            bit_buf->rack = 0;
            bit_buf->mask = 0x80;
            bit_buf->bit_index = 0;
        }
        return bit_buf;
    }

    BIT_BUF* __cdecl OpenOutputBitBuffer_(uint8_t* buffer) {
        BIT_BUF* bit_buf = (BIT_BUF*)_malloc(sizeof(BIT_BUF));
        if (bit_buf != nullptr) {
            bit_buf->buf = buffer;
            bit_buf->rack = 0;
            bit_buf->mask = 0x80;
            bit_buf->bit_index = 0;
        }
        return bit_buf;
    }

    void __cdecl CloseInputBitBuffer_(BIT_BUF* bit_buf) {
        _free(bit_buf);
    }

    uint32_t __cdecl CloseOutputBitBuffer_(BIT_BUF* bit_buf) {
        if (bit_buf->mask != 0x80) {
            bit_buf->buf[bit_buf->bit_index] = (uint8_t)bit_buf->rack;
            bit_buf->bit_index++;
        }
        uint32_t size = bit_buf->bit_index;
        _free(bit_buf);
        return size;
    }

    uint8_t __cdecl BufferInputBit_(BIT_BUF* bit_buf) {
        if (bit_buf->mask == 0x80) {
            uint32_t next_index = bit_buf->bit_index + 1;

            // Load the next byte into the rack and advance the buffer index
            bit_buf->rack = bit_buf->buf[bit_buf->bit_index];
            bit_buf->bit_index = next_index;
        }

        uint8_t current_mask = bit_buf->mask;
        uint32_t masked_val = bit_buf->rack & (uint32_t)current_mask;

        // Shift the mask for the next bit
        bit_buf->mask >>= 1;

        // If the mask is exhausted, reset it to the MSB for the next byte fetch
        if (bit_buf->mask == 0) {
            bit_buf->mask = 0x80;
        }

        // Return 1 if the bit was set, 0 otherwise
        return (masked_val != 0) ? 1 : 0;
    }

    /**
     * @brief Outputs a single bit into the provided BIT_BUF structure.
     * 
     * If the current byte (rack) is full (8 bits), it writes it to the buffer.
     * Every 2048 bytes written, a '.' character is sent to a tracking stream.
     * 
     * @param bit_buf Pointer to the BIT_BUF management structure.
     * @param bit The bit value to write (non-zero for 1, zero for 0).
     */
    void __cdecl BufferOutputBit_(BIT_BUF* bit_buf, int bit) {
        if (bit != 0) {
            bit_buf->rack |= (uint32_t)bit_buf->mask;
        }

        bit_buf->mask >>= 1;

        if (bit_buf->mask == 0) {
            // Write the accumulated byte to the buffer
            bit_buf->buf[bit_buf->bit_index] = (uint8_t)bit_buf->rack;
            bit_buf->bit_index++;

            // Reset for next byte
            bit_buf->rack = 0;
            bit_buf->mask = 0x80;
        }
    }

    /**
     * @brief Buffers a specified number of bits from data into a bit buffer.
     * 
     * @param bit_buf Pointer to the BIT_BUF structure.
     * @param data The source value containing bits to output.
     * @param count The number of bits to output (usually 1-32).
     */
    void __cdecl BufferOutputBits_(BIT_BUF* bit_buf, uint32_t data, uint8_t count) {
        // Match x86 CL-based shift semantics from the original binary.
        uint8_t shift = (uint8_t)(count - 1);
        uint32_t bit_selector = 1u << (shift & 0x1f);

        while (bit_selector != 0) {
            // If the bit at the current selector position is set in data,
            // set the bit in the current byte rack using the current bit mask.
            if ((data & bit_selector) != 0) {
                bit_buf->rack |= bit_buf->mask;
            }

            // Move to the next bit position within the current byte
            bit_buf->mask >>= 1;

            // If the byte mask reaches zero, we've filled the current rack byte.
            if (bit_buf->mask == 0) {
                // Write the rack byte to the buffer and advance the byte index
                bit_buf->buf[bit_buf->bit_index] = static_cast<uint8_t>(bit_buf->rack);
                bit_buf->bit_index++;

                // Reset the rack and bit mask for the next byte
                bit_buf->rack = 0;
                bit_buf->mask = 0x80;
            }

            // Move the data selector to the next lower bit
            bit_selector >>= 1;
        }
    }
}