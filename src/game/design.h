#pragma once
#include <cstdint>

namespace DESIGN {

#pragma pack(push, 1)
    struct s_current_design {
        char name[16];
        undefined field1_0x10;
        undefined field2_0x11;
        undefined field3_0x12;
        undefined field4_0x13;
        undefined field5_0x14;
        undefined field6_0x15;
        undefined field7_0x16;
        undefined field8_0x17;
        undefined field9_0x18;
        undefined field10_0x19;
        undefined field11_0x1a;
        short ftl_type;
        short drive_speed;
        undefined field14_0x1f;
        undefined field15_0x20;
        undefined field16_0x21;
        undefined field17_0x22;
        undefined field18_0x23;
        undefined field19_0x24;
        short base_combat_speed; /* Created by Rename Structure Field action */
        undefined field21_0x27;
        undefined field22_0x28;
        undefined field23_0x29;
        undefined field24_0x2a;
        short computer_type;
        undefined field45_0x2d;
        undefined field46_0x2e;
        short fuel_type;
        short armor_type;
        undefined field30_0x33;
        undefined field31_0x34;
        short weapon_type[8];
        short weapon_count[8];
        char weapon_firing_arc[8]; /* Created by Rename Structure Field action */
        short weapon_specials[8]; /* Created by Rename Structure Field action */
        undefined field36_0x6d;
        undefined field37_0x6e;
        undefined field38_0x6f;
        undefined field39_0x70;
        undefined field40_0x71;
        undefined field41_0x72;
        undefined field42_0x73;
        undefined field43_0x74;
        undefined field44_0x75;
        undefined field45_0x76;
        undefined field46_0x77;
        undefined field47_0x78;
        undefined field48_0x79;
        undefined field49_0x7a;
        undefined field50_0x7b;
        undefined field51_0x7c;
        undefined field52_0x7d;
        undefined field53_0x7e;
        undefined field54_0x7f;
        undefined field55_0x80;
        undefined field56_0x81;
        undefined field57_0x82;
        undefined field58_0x83;
        undefined field59_0x84;
        undefined field60_0x85;
        undefined field61_0x86;
        undefined field62_0x87;
        undefined field63_0x88;
        undefined field64_0x89;
        undefined field65_0x8a;
        undefined field66_0x8b;
        undefined field67_0x8c;
        undefined field68_0x8d;
        undefined field69_0x8e;
        undefined field70_0x8f;
        undefined field71_0x90;
        undefined field72_0x91;
        undefined field73_0x92;
        undefined field74_0x93;
        undefined field75_0x94;
        undefined field76_0x95;
        undefined field77_0x96;
        undefined field78_0x97;
        undefined field79_0x98;
        undefined field80_0x99;
        undefined field81_0x9a;
        undefined field82_0x9b;
        undefined field83_0x9c;
        undefined field84_0x9d;
        undefined field85_0x9e;
        undefined field86_0x9f;
        undefined field87_0xa0;
        undefined field88_0xa1;
        undefined field89_0xa2;
        undefined field90_0xa3;
        undefined field91_0xa4;
        undefined field92_0xa5;
        undefined field93_0xa6;
        undefined field94_0xa7;
        undefined field95_0xa8;
        undefined field96_0xa9;
        undefined field97_0xaa;
        undefined field98_0xab;
        undefined field99_0xac;
        char weapon_ammo[8]; /* Created by Rename Structure Field action */
        short special_devices[8];
        short field102_0xc5[8];
        short field103_0xd5[8];
        short ship_size;
        short picture_type;
        int hull_space;
        undefined field107_0xed;
        undefined field108_0xee;
        undefined field109_0xef;
        undefined field110_0xf0;
        short field111_0xf1;
        undefined field112_0xf3;
        undefined field113_0xf4;
        undefined field114_0xf5;
        undefined field115_0xf6;
        undefined field116_0xf7;
        undefined field117_0xf8;
        undefined field118_0xf9;
        undefined field119_0xfa;
        undefined field120_0xfb;
        undefined field121_0xfc;
        undefined field122_0xfd;
        undefined field123_0xfe;
        undefined field124_0xff;
        undefined field125_0x100;
        undefined field126_0x101;
        undefined field127_0x102;
        undefined field128_0x103;
        undefined field129_0x104;
        undefined field130_0x105;
        undefined field131_0x106;
        undefined field132_0x107;
        undefined field133_0x108;
        undefined field134_0x109;
        undefined field135_0x10a;
        undefined field136_0x10b;
        undefined field137_0x10c;
        undefined field138_0x10d;
        undefined field139_0x10e;
        undefined field140_0x10f;
        undefined field141_0x110;
        undefined field142_0x111;
        undefined field143_0x112;
        undefined field144_0x113;
        undefined field145_0x114;
        undefined field146_0x115;
        undefined field147_0x116;
        undefined field148_0x117;
        undefined field149_0x118;
        undefined field150_0x119;
        undefined field151_0x11a;
        undefined field152_0x11b;
        undefined field153_0x11c;
        undefined field154_0x11d;
        undefined field155_0x11e;
        undefined field156_0x11f;
        undefined field157_0x120;
        undefined field158_0x121;
        undefined field159_0x122;
        undefined field160_0x123;
        undefined field161_0x124;
    };
#pragma pack(pop)

    inline s_current_design*& _design = *reinterpret_cast<s_current_design**>(std::uintptr_t{ 0x0059B330 });
    inline uint8_t& _use_augmented_engines = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056503C});

    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, struct s_ship_design* ship_design);

    int8_t __cdecl Get_Base_Combat_Speed_(s_current_design* design);

    inline uint32_t __cdecl Current_Design_Base_Combat_Speed_(s_current_design* design) {
        using Fn = uint32_t(__cdecl*)(s_current_design*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042e5b0});
        return fn(design);
    }

    inline uint8_t __cdecl Current_Design_Has_Special_System_(s_current_design* design, uint32_t system_id) {
        using Fn = uint8_t(__cdecl*)(s_current_design*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042e580});
        return fn(design, system_id);
    }

    inline void __cdecl Update_Calculated_Design_Data_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004321E0})(player_idx);
    }

    inline void __cdecl Design_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0042e6b0})();
    }
}