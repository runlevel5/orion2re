#pragma once

namespace SERIAL {
    class Reader_ {
    public:
        Reader_(const void* data, uint32_t len, const char* error_message);

        void Require_(size_t size) const;
        void Skip_(size_t size);

        char Read_Char_();
        uint8_t Read_Uint8_();
        int8_t Read_Int8_();
        uint16_t Read_Uint16_();
        int16_t Read_Int16_();
        uint32_t Read_Uint32_();
        int32_t Read_Int32_();
        uint64_t Read_Uint64_();

        void Read_Bytes_(void* dest, size_t size);
        uint32_t Length_() const;
        const uint8_t* Ptr_() const;

    private:
        [[noreturn]] void Fail_() const;

        const uint8_t* _start;
        const uint8_t* _ptr;
        uint32_t _len;
        const char* _error_message;
    };

    class Writer_ {
    public:
        Writer_(void* data, uint32_t len, const char* error_message);

        void Require_(size_t size) const;
        void Skip_(size_t size);

        void Write_Char_(char value);
        void Write_Uint8_(uint8_t value);
        void Write_Int8_(int8_t value);
        void Write_Uint16_(uint16_t value);
        void Write_Int16_(int16_t value);
        void Write_Uint32_(uint32_t value);
        void Write_Int32_(int32_t value);
        void Write_Uint64_(uint64_t value);

        void Write_Bytes_(const void* src, size_t size);
        uint32_t Length_() const;
        uint8_t* Ptr_() const;

    private:
        [[noreturn]] void Fail_() const;

        uint8_t* _start;
        uint8_t* _ptr;
        uint32_t _len;
        const char* _error_message;
    };
}