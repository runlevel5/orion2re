#include "pch.h"

#include "game/netproto.h"

namespace NETPROTO {
    Packet_Reader_::Packet_Reader_(void* data, uint32_t len, const char* error_message)
        : _start(static_cast<uint8_t*>(data)),
          _ptr(static_cast<uint8_t*>(data)),
          _len(len),
          _error_message(error_message) {
        if (_start == nullptr) {
            Fail_();
        }
    }

    void Packet_Reader_::Require_(size_t size) const {
        if (_ptr == nullptr || _ptr < _start ||
            (size_t)(_ptr - _start) > _len || size > _len - (size_t)(_ptr - _start)) {
            Fail_();
        }
    }

    void Packet_Reader_::Skip_(size_t size) {
        Require_(size);
        _ptr += size;
    }

    char Packet_Reader_::Read_Char_() {
        return static_cast<char>(Read_Uint8_());
    }

    uint8_t Packet_Reader_::Read_Uint8_() {
        Require_(sizeof(uint8_t));
        uint8_t value = *_ptr;
        _ptr += sizeof(uint8_t);
        return value;
    }

    int8_t Packet_Reader_::Read_Int8_() {
        return static_cast<int8_t>(Read_Uint8_());
    }

    uint16_t Packet_Reader_::Read_Uint16_() {
        uint16_t value;
        Read_Bytes_(&value, sizeof(value));
        return value;
    }

    int16_t Packet_Reader_::Read_Int16_() {
        int16_t value;
        Read_Bytes_(&value, sizeof(value));
        return value;
    }

    uint32_t Packet_Reader_::Read_Uint32_() {
        uint32_t value;
        Read_Bytes_(&value, sizeof(value));
        return value;
    }

    int32_t Packet_Reader_::Read_Int32_() {
        int32_t value;
        Read_Bytes_(&value, sizeof(value));
        return value;
    }

    void Packet_Reader_::Read_Bytes_(void* dest, size_t size) {
        Require_(size);
        memcpy(dest, _ptr, size);
        _ptr += size;
    }

    uint8_t* Packet_Reader_::Ptr_() const {
        return _ptr;
    }

    [[noreturn]] void Packet_Reader_::Fail_() const {
        exitm::Exit_With_Message(_error_message);
    }

    Packet_Writer_::Packet_Writer_(void* data, uint32_t len, const char* error_message)
        : _start(static_cast<uint8_t*>(data)),
          _ptr(static_cast<uint8_t*>(data)),
          _len(len),
          _error_message(error_message) {
        if (_start == nullptr) {
            Fail_();
        }
    }

    void Packet_Writer_::Require_(size_t size) const {
        if (_ptr == nullptr || _ptr < _start ||
            (size_t)(_ptr - _start) > _len || size > _len - (size_t)(_ptr - _start)) {
            Fail_();
        }
    }

    void Packet_Writer_::Skip_(size_t size) {
        Require_(size);
        _ptr += size;
    }

    void Packet_Writer_::Write_Char_(char value) {
        Write_Uint8_(static_cast<uint8_t>(value));
    }

    void Packet_Writer_::Write_Uint8_(uint8_t value) {
        Require_(sizeof(uint8_t));
        *_ptr = value;
        _ptr += sizeof(uint8_t);
    }

    void Packet_Writer_::Write_Int8_(int8_t value) {
        Write_Uint8_(static_cast<uint8_t>(value));
    }

    void Packet_Writer_::Write_Uint16_(uint16_t value) {
        Write_Bytes_(&value, sizeof(value));
    }

    void Packet_Writer_::Write_Int16_(int16_t value) {
        Write_Bytes_(&value, sizeof(value));
    }

    void Packet_Writer_::Write_Uint32_(uint32_t value) {
        Write_Bytes_(&value, sizeof(value));
    }

    void Packet_Writer_::Write_Int32_(int32_t value) {
        Write_Bytes_(&value, sizeof(value));
    }

    void Packet_Writer_::Write_Bytes_(const void* src, size_t size) {
        Require_(size);
        memcpy(_ptr, src, size);
        _ptr += size;
    }

    uint8_t* Packet_Writer_::Ptr_() const {
        return _ptr;
    }

    [[noreturn]] void Packet_Writer_::Fail_() const {
        exitm::Exit_With_Message(_error_message);
    }
}