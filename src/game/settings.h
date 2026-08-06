#pragma once

namespace ENHANCED_UX {
    typedef struct s_enhanced_ux_settings s_enhanced_ux_settings, *Ps_enhanced_ux_settings;

    struct s_enhanced_ux_settings {
        uint8_t enabled;
    };

    extern s_enhanced_ux_settings enhanced_ux_settings;
}
