#include "pch.h"

namespace exitm {
    [[noreturn]] void Exit_With_Message(const char* message) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "fatal exit: %s", message != nullptr ? message : "<null>");

        platform::Request_Fatal_Exit_(message);
        platform::Check_Exit_();
        std::abort();
    }

    [[noreturn]] void Exit_Normally(const char* message) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "normal exit: %s", message != nullptr ? message : "<null>");

        platform::Request_Exit_(message);
        platform::Check_Exit_();
        std::abort();
    }

    [[noreturn]] void __cdecl Exit_With_message2_(const char* message) {
        Exit_With_Message(message);
    }
}