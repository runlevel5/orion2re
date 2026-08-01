#include "pch.h"

#include "strbuild.h"

namespace STRBUILD {
    static uint32_t Remaining_(const s_builder* builder) {
        if (builder == nullptr || builder->cur == nullptr || builder->end == nullptr || builder->cur >= builder->end) {
            return 0;
        }

        return (uint32_t)(builder->end - builder->cur);
    }

    s_builder Init(char* buffer, uint32_t buffer_size) {
        s_builder builder = {buffer, buffer, buffer};

        if (buffer != nullptr && buffer_size != 0) {
            builder.end = buffer + buffer_size;
            buffer[0] = '\0';
        }

        return builder;
    }

    void Copy(s_builder* builder, const char* source) {
        if (builder == nullptr) {
            return;
        }

        builder->cur = builder->start;
        if (builder->cur != nullptr && builder->cur < builder->end) {
            builder->cur[0] = '\0';
        }

        Cat(builder, source);
    }

    void Cat(s_builder* builder, const char* source) {
        if (builder == nullptr || source == nullptr) {
            return;
        }

        uint32_t remaining = Remaining_(builder);
        if (remaining == 0) {
            return;
        }

        uint32_t source_len = (uint32_t)strlen(source);
        if (source_len >= remaining) {
            memcpy(builder->cur, source, remaining - 1);
            builder->cur += remaining - 1;
            builder->cur[0] = '\0';
            return;
        }

        memcpy(builder->cur, source, source_len + 1);
        builder->cur += source_len;
    }

    void Sprintf(s_builder* builder, const char* format, ...) {
        if (builder == nullptr || format == nullptr) {
            return;
        }

        uint32_t remaining = Remaining_(builder);
        if (remaining == 0) {
            return;
        }

        va_list args;
        va_start(args, format);
        int written = vsnprintf(builder->cur, remaining, format, args);
        va_end(args);

        if (written < 0) {
            if (builder->end != nullptr && builder->end > builder->start) {
                builder->cur = builder->end - 1;
            }
            return;
        }

        if ((uint32_t)written >= remaining) {
            builder->cur = builder->end - 1;
            return;
        }

        builder->cur += written;
    }
}