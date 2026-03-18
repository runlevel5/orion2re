#pragma once
#include <cstdint>

namespace string {
    inline const char* s_Antares = reinterpret_cast<const char*>(std::uintptr_t{0x00551288}); // "Antares"
    inline const char* s_Dash = reinterpret_cast<const char*>(std::uintptr_t{0x0055111C}); // "-"
    inline const char* s_Newline = reinterpret_cast<const char*>(std::uintptr_t{0x005569F8}); // "\n"
    inline const char* s_1 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B10}); // "1"
    inline const char* s_2 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E5C}); // " 2"
    inline const char* s_Newline2 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E60}); // "\n\n"
    inline const char* s_X312 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E64}); // " X312."
    inline const char* s_X250 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E6C}); // " X250."
    inline const char* s_X120 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E74}); // " X120."
    inline const char* s_X80 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E7C}); // " X80."
    inline const char* s_X18 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E84}); // " X18."
    inline const char* s_Xplus21 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E8C}); // " X+21."
    inline const char* s_X227 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E94}); // " X227."
    inline const char* s_Xplus25 = reinterpret_cast<const char*>(std::uintptr_t{0x00557E9C}); // " X+25."
    inline const char* s_X255 = reinterpret_cast<const char*>(std::uintptr_t{0x00557EA4}); // " X255."
    inline const char* s_X35 = reinterpret_cast<const char*>(std::uintptr_t{0x00557EAC}); // " X35."
}