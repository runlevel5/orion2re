#include "deps.h"

namespace allocate {
    void Allocate_Data_Space_() {
        MOX::_global_screen_seg = reinterpret_cast<uint32_t>(Allocate_Space_(0x1B5030));
        MOX::_techname_seg = reinterpret_cast<uint32_t>(Allocate_Space_(30000));
        MOX::_special_names_seg = reinterpret_cast<uint32_t>(Allocate_Space_(1000));

        // Allocate and clear planet structures
        MOX::_planet = reinterpret_cast<s_planet_data*>(Allocate_Space_(0x17E8));
        struct_::Clear_Structure_(MOX::_planet, 0x17E8);

        // Allocate and clear player structures
        MOX::_player = reinterpret_cast<s_player*>(Allocate_Space_(0x7548));
        struct_::Clear_Structure_(MOX::_player, 0x7548);

        // Allocate and clear colony structures
        MOX::_colony = reinterpret_cast<s_colony*>(Allocate_Space_(0x1608A));
        struct_::Clear_Structure_(MOX::_colony, 0x1608A);

        MOX::DAT_005a2bec = reinterpret_cast<uint32_t>(Allocate_Space_(0x3E800));

        // Global cache allocation
        MOX::_global_cache_seg = FUN_0052c200(0x174508);
        MOX::_global_cache_size = 0x174508;

        // DOS-space allocations for low-level segments
        MOX::_global_data_seg = reinterpret_cast<uint32_t>(Allocate_Dos_Space_(0x2800));

        MOX::_nebula = reinterpret_cast<uint32_t>(Allocate_Dos_Space_(0x14));
        struct_::Clear_Structure_(reinterpret_cast<void*>(MOX::_nebula), 0x14);

        MOX::_star = reinterpret_cast<s_star_data*>(Allocate_Dos_Space_(0x1FC8));
        struct_::Clear_Structure_(MOX::_star, 0x1FC8);

        MOX::_ship = reinterpret_cast<s_ship_data*>(Allocate_Dos_Space_(0xFBF4));
        struct_::Clear_Structure_(MOX::_ship, 0xFBF4);

        MOX::_leaders = reinterpret_cast<s_leader_data*>(Allocate_Dos_Space_(0xF71));
        struct_::Clear_Structure_(MOX::_leaders, 0xF71);
    }
}