#include "pch.h"

namespace arith1e {
    const char s_Failure_allocating_context__d_00560838[] = "Failure allocating context %d";
    const char s_Fatal_error__00560ab0[] = "Fatal error: ";

    struct SYMBOL {
        uint16_t low_count;
        uint16_t high_count;
        uint16_t scale;
    };

    constexpr uint16_t MAXIMUM_SCALE = 16383;
    constexpr uint32_t END_OF_STREAM = 257;
    constexpr uint32_t ESCAPE = 256;

    static int* totals[END_OF_STREAM];
    static uint16_t code;
    static uint16_t low;
    static uint16_t high;
    static int32_t underflow_bits;

    static void __cdecl Fatal_Error_(const char* format, ...) {
        va_list args;

        va_start(args, format);
        printf(s_Fatal_error__00560ab0);
        vprintf(format, args);
        va_end(args);
        __exit(-1);
    }

    static void __cdecl update_model_(int symbol, int context) {
        for (int i = symbol + 1; i <= (END_OF_STREAM + 1); i++) {
            totals[context][i]++;
        }
        if (totals[context][END_OF_STREAM + 1] < MAXIMUM_SCALE) {
            return;
        }
        for (int i = 1; i <= (END_OF_STREAM + 1); i++) {
            totals[context][i] /= 2;
        }
        totals[context][END_OF_STREAM] = totals[context][ESCAPE] + 1;
        totals[context][END_OF_STREAM + 1] = totals[context][END_OF_STREAM] + 1;
    }

    static void __cdecl initialize_model_() {
        for (int context = 0; context < END_OF_STREAM; context++) {
            totals[context] = (int*)calloc(END_OF_STREAM + 2, sizeof(int));
            if (totals[context] == nullptr) {
                Fatal_Error_(s_Failure_allocating_context__d_00560838, context);
            }
            update_model_(ESCAPE, context);
        }
        for (int i = 0; i <= (END_OF_STREAM + 1); i++) {
            totals[ESCAPE][i] = i;
        }
    }

    static void __cdecl uninit_model_() {
        for (int context = 0; context < END_OF_STREAM; context++) {
            free(totals[context]);
        }
    }

    static void __cdecl initialize_arithmetic_encoder_() {
        low = 0;
        high = 0xffff;
        underflow_bits = 0;
    }

    static int __cdecl convert_int_to_symbol_(int c, int context, SYMBOL* s) {
        s->scale = (uint16_t)totals[context][END_OF_STREAM + 1];
        s->low_count = (uint16_t)totals[context][c];
        s->high_count = (uint16_t)totals[context][c + 1];
        if (s->low_count < s->high_count) {
            return 0;
        }
        s->low_count = (uint16_t)totals[context][ESCAPE];
        s->high_count = (uint16_t)totals[context][ESCAPE + 1];
        return 1;
    }

    static void __cdecl buffer_encode_symbol_(BIT_BUF* stream, SYMBOL* s) {
        int32_t range = (int32_t)(high - low) + 1;
        high = low + (uint16_t)((range * s->high_count) / s->scale - 1);
        low = low + (uint16_t)((range * s->low_count) / s->scale);

        for (;;) {
            if ((high & 0x8000) == (low & 0x8000)) {
                bitio::BufferOutputBit_(stream, high & 0x8000);
                while (underflow_bits > 0) {
                    bitio::BufferOutputBit_(stream, ~high & 0x8000);
                    underflow_bits--;
                }
            } else if ((low & 0x4000) && !(high & 0x4000)) {
                underflow_bits += 1;
                low &= 0x3fff;
                high |= 0x4000;
            } else {
                return;
            }
            low <<= 1;
            high <<= 1;
            high |= 1;
        }
    }

    static void __cdecl buffer_flush_arithmetic_encoder_(BIT_BUF* stream) {
        bitio::BufferOutputBit_(stream, low & 0x4000);
        underflow_bits++;
        while (underflow_bits-- > 0) {
            bitio::BufferOutputBit_(stream, ~low & 0x4000);
        }
        bitio::BufferOutputBits_(stream, 0, 16);
    }

    void __cdecl Compress_Data_(uint8_t* in_data, uint32_t in_len, uint8_t* out_data, uint32_t* out_len) {
        if (in_len != 0) {
            uint32_t i = 0;
            BIT_BUF* stream = bitio::OpenOutputBitBuffer_(out_data);
            initialize_model_();
            initialize_arithmetic_encoder_();
            
            uint32_t context = 0;
            for (;;) {
                uint32_t c = END_OF_STREAM;
                if (i != in_len) {
                    c = in_data[i];
                    i++;
                }
                
                SYMBOL s;
                int escaped = convert_int_to_symbol_(c, context, &s);
                buffer_encode_symbol_(stream, &s);
                
                if (escaped) {
                    convert_int_to_symbol_(c, ESCAPE, &s);
                    buffer_encode_symbol_(stream, &s);
                }
                
                if (c == END_OF_STREAM) {
                    break;
                }
                
                update_model_(c, context);
                context = c;
            }
            
            buffer_flush_arithmetic_encoder_(stream);
            *out_len = bitio::CloseOutputBitBuffer_(stream);
            uninit_model_();
        }
    }

    static void __cdecl get_symbol_scale_(int context, SYMBOL* s) {
        s->scale = (uint16_t)totals[context][END_OF_STREAM + 1];
    }

    static int16_t __cdecl get_current_count_(SYMBOL* s) {
        int32_t range = (int32_t)(high - low) + 1;
        int16_t count = (int16_t)((((int32_t)(code - low) + 1) * s->scale - 1) / range);
        return count;
    }

    static int __cdecl convert_symbol_to_int_(int count, int context, SYMBOL* s) {
        int c;
        for (c = 0; count >= totals[context][c + 1]; c++) {}
        s->high_count = (uint16_t)totals[context][c + 1];
        s->low_count = (uint16_t)totals[context][c];
        return c;
    }

    static void __cdecl initialize_arithmetic_decoder_(BIT_BUF* stream) {
        code = 0;
        for (int i = 0; i < 16; i++) {
            code <<= 1;
            code += bitio::BufferInputBit_(stream);
        }
        low = 0;
        high = 0xffff;
    }

    static void __cdecl remove_symbol_from_stream_(BIT_BUF* stream, SYMBOL* s) {
        int32_t range = (int32_t)(high - low) + 1;
        high = low + (uint16_t)((range * s->high_count) / s->scale - 1);
        low = low + (uint16_t)((range * s->low_count) / s->scale);

        for (;;) {
            if ((high & 0x8000) == (low & 0x8000)) {
                // MSBs match, do nothing here as they will be shifted out
            } else if ((low & 0x4000) == 0x4000 && (high & 0x4000) == 0) {
                code ^= 0x4000;
                low &= 0x3fff;
                high |= 0x4000;
            } else {
                return;
            }
            low <<= 1;
            high <<= 1;
            high |= 1;
            code <<= 1;
            code += bitio::BufferInputBit_(stream);
        }
    }

    void __cdecl Uncompress_Data_(uint8_t* src, uint32_t src_len, uint8_t* dst, uint32_t* dst_len_ptr) {
        if (src_len != 0) {
            BIT_BUF* stream = bitio::OpenInputBitBuffer_(src);
            initialize_model_();
            initialize_arithmetic_decoder_(stream);
            
            uint32_t out_pos = 0;
            int context = 0;
            for (;;) {
                int last_context = context;
                int c;
                do {
                    SYMBOL s;
                    get_symbol_scale_(context, &s);
                    int count = get_current_count_(&s);
                    c = convert_symbol_to_int_(count, context, &s);
                    remove_symbol_from_stream_(stream, &s);
                    context = c;
                } while (c == ESCAPE);
                
                if (c == END_OF_STREAM) {
                    break;
                }
                
                dst[out_pos++] = (uint8_t)c;
                update_model_(c, last_context);
                context = c;
            }
            
            bitio::CloseInputBitBuffer_(stream);
            *dst_len_ptr = out_pos;
            uninit_model_();
        }
    }

}