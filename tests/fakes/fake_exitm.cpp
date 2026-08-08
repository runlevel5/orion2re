#include "pch.h"

namespace exitm {
    [[noreturn]] void __cdecl Exit_With_Message(const char* message) {
        platform::Request_Fatal_Exit_(message);
        platform::Check_Exit_();
        std::abort();
    }

    [[noreturn]] void __cdecl Exit_Normally(const char* message) {
        platform::Request_Exit_(message);
        platform::Check_Exit_();
        std::abort();
    }

    [[noreturn]] void __cdecl Exit_With_message2_(const char* message) {
        Exit_With_Message(message);
    }
}