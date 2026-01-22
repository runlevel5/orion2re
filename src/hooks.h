#pragma once

namespace hooks {
    struct hook {
        uint32_t address;
        void* handler;
    };

    void Install_Hooks();
};