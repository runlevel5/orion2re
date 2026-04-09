#pragma once

namespace NETSTRAT {
    inline char*& _comm_failure_string = 
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE88});
}