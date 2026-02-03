#pragma once
#include <cstdint>

namespace LOADSAVE {
    inline const char* s_INVALID_STR = reinterpret_cast<const char*>(0x0055AC58);

    inline char (&_months)[12][6] = *reinterpret_cast<char(*)[12][6]>(std::uintptr_t{0x005722F8});

    void __cdecl Load_Month_Names_();

    void __cdecl Restore_Main_Screen_();

    void __cdecl Restore_Game_Popup_();

    bool __cdecl Game_Popup_User_Box_(char* text, int16_t box_type);

    inline uint8_t& _draw_game_popup_to_back =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722F4});
    inline uint8_t& _draw_down = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AC50});
    inline char (&_empty_save_slot_message)[20] = *reinterpret_cast<char(*)[20]>(std::uintptr_t{0x005722D8});

    #pragma pack(push, 1)
    struct s_game_anims {
        struct s_animation_header* pictures[25];
        int8_t field1_0x64;
    };

    struct s_game_popup_fields {
        int32_t values[10];
        short field_l;
        short field_s;
        short field_q;
        short field_n;
        short field_o;
        short field_esc;
        undefined field7_0x34;
        undefined field8_0x35;
        undefined field9_0x36;
        undefined field10_0x37;
        short saved_game_fields[10];
        short field_d;
        undefined field13_0x4e;
        undefined field14_0x4f;
        undefined field15_0x50;
        undefined field16_0x51;
        short field17_0x52[13];
        short field_A;
        short field19_0x6e;
        short field_esc_2;
        short field_space;
        short field22_0x74;
        short field23_0x76;
        undefined field24_0x78;
        undefined field25_0x79;
        undefined field26_0x7a;
        undefined field27_0x7b;
        short field28_0x7c[10];
        undefined field29_0x90;
        undefined field30_0x91;
        undefined field31_0x92;
        undefined field32_0x93;
        undefined field33_0x94;
        undefined field34_0x95;
        undefined field35_0x96;
        undefined field36_0x97;
        undefined field37_0x98;
        undefined field38_0x99;
        undefined field39_0x9a;
        undefined field40_0x9b;
        undefined field41_0x9c;
        undefined field42_0x9d;
        undefined field43_0x9e;
        undefined field44_0x9f;
        undefined field45_0xa0;
        undefined field46_0xa1;
        undefined field47_0xa2;
        undefined field48_0xa3;
        undefined field49_0xa4;
        undefined field50_0xa5;
        undefined field51_0xa6;
        undefined field52_0xa7;
        undefined field53_0xa8;
        undefined field54_0xa9;
        int8_t saved_games[10];
        undefined field56_0xb4;
        undefined field57_0xb5;
        undefined field58_0xb6;
        undefined field59_0xb7;
        undefined field60_0xb8;
        undefined field61_0xb9;
        undefined field62_0xba;
        undefined field63_0xbb;
        undefined field64_0xbc;
        undefined field65_0xbd;
        undefined field66_0xbe;
        undefined field67_0xbf;
        undefined field68_0xc0;
        undefined field69_0xc1;
        undefined field70_0xc2;
        undefined field71_0xc3;
        undefined field72_0xc4;
        undefined field73_0xc5;
        undefined field74_0xc6;
        undefined field75_0xc7;
        undefined field76_0xc8;
        undefined field77_0xc9;
        undefined field78_0xca;
        undefined field79_0xcb;
        undefined field80_0xcc;
        undefined field81_0xcd;
        undefined field82_0xce;
        undefined field83_0xcf;
        undefined field84_0xd0;
        undefined field85_0xd1;
        undefined field86_0xd2;
        undefined field87_0xd3;
        undefined field88_0xd4;
        undefined field89_0xd5;
        undefined field90_0xd6;
        undefined field91_0xd7;
        undefined field92_0xd8;
        undefined field93_0xd9;
        undefined field94_0xda;
        undefined field95_0xdb;
        undefined field96_0xdc;
        undefined field97_0xdd;
        undefined field98_0xde;
        undefined field99_0xdf;
        undefined field100_0xe0;
        undefined field101_0xe1;
        undefined field102_0xe2;
        undefined field103_0xe3;
        undefined field104_0xe4;
        undefined field105_0xe5;
        undefined field106_0xe6;
        undefined field107_0xe7;
        undefined field108_0xe8;
        undefined field109_0xe9;
        undefined field110_0xea;
        undefined field111_0xeb;
        undefined field112_0xec;
        undefined field113_0xed;
        undefined field114_0xee;
        undefined field115_0xef;
        undefined field116_0xf0;
        undefined field117_0xf1;
        undefined field118_0xf2;
        undefined field119_0xf3;
        undefined field120_0xf4;
        undefined field121_0xf5;
        undefined field122_0xf6;
        undefined field123_0xf7;
        undefined field124_0xf8;
        undefined field125_0xf9;
        undefined field126_0xfa;
        undefined field127_0xfb;
        undefined field128_0xfc;
        undefined field129_0xfd;
        undefined field130_0xfe;
        undefined field131_0xff;
        undefined field132_0x100;
        undefined field133_0x101;
        undefined field134_0x102;
        undefined field135_0x103;
        undefined field136_0x104;
        undefined field137_0x105;
        undefined field138_0x106;
        undefined field139_0x107;
        undefined field140_0x108;
        undefined field141_0x109;
        undefined field142_0x10a;
        undefined field143_0x10b;
        undefined field144_0x10c;
        undefined field145_0x10d;
        undefined field146_0x10e;
        undefined field147_0x10f;
        undefined field148_0x110;
        undefined field149_0x111;
        undefined field150_0x112;
        undefined field151_0x113;
        undefined field152_0x114;
        undefined field153_0x115;
        undefined field154_0x116;
        undefined field155_0x117;
        undefined field156_0x118;
        undefined field157_0x119;
        undefined field158_0x11a;
        undefined field159_0x11b;
        undefined field160_0x11c;
        undefined field161_0x11d;
        undefined field162_0x11e;
        undefined field163_0x11f;
        undefined field164_0x120;
        undefined field165_0x121;
        undefined field166_0x122;
        undefined field167_0x123;
        undefined field168_0x124;
        undefined field169_0x125;
        undefined field170_0x126;
        undefined field171_0x127;
        undefined field172_0x128;
        undefined field173_0x129;
        undefined field174_0x12a;
        undefined field175_0x12b;
        undefined field176_0x12c;
        undefined field177_0x12d;
        undefined field178_0x12e;
        undefined field179_0x12f;
        undefined field180_0x130;
        undefined field181_0x131;
        undefined field182_0x132;
        undefined field183_0x133;
        undefined field184_0x134;
        undefined field185_0x135;
        undefined field186_0x136;
        undefined field187_0x137;
        undefined field188_0x138;
        undefined field189_0x139;
        undefined field190_0x13a;
        undefined field191_0x13b;
        undefined field192_0x13c;
        undefined field193_0x13d;
        undefined field194_0x13e;
        undefined field195_0x13f;
        undefined field196_0x140;
        undefined field197_0x141;
        undefined field198_0x142;
        undefined field199_0x143;
        undefined field200_0x144;
        undefined field201_0x145;
        undefined field202_0x146;
        undefined field203_0x147;
        undefined field204_0x148;
        undefined field205_0x149;
        undefined field206_0x14a;
        undefined field207_0x14b;
        undefined field208_0x14c;
        undefined field209_0x14d;
        undefined field210_0x14e;
        undefined field211_0x14f;
        undefined field212_0x150;
        undefined field213_0x151;
        undefined field214_0x152;
        undefined field215_0x153;
        undefined field216_0x154;
        undefined field217_0x155;
        undefined field218_0x156;
        undefined field219_0x157;
        undefined field220_0x158;
        undefined field221_0x159;
        undefined field222_0x15a;
        undefined field223_0x15b;
        undefined field224_0x15c;
        undefined field225_0x15d;
        undefined field226_0x15e;
        undefined field227_0x15f;
        undefined field228_0x160;
        undefined field229_0x161;
        undefined field230_0x162;
        undefined field231_0x163;
        undefined field232_0x164;
        undefined field233_0x165;
        undefined field234_0x166;
        undefined field235_0x167;
        undefined field236_0x168;
        undefined field237_0x169;
        undefined field238_0x16a;
        undefined field239_0x16b;
        undefined field240_0x16c;
        undefined field241_0x16d;
        undefined field242_0x16e;
        undefined field243_0x16f;
        undefined field244_0x170;
        undefined field245_0x171;
        undefined field246_0x172;
        undefined field247_0x173;
        undefined field248_0x174;
        undefined field249_0x175;
        undefined field250_0x176;
        undefined field251_0x177;
        undefined field252_0x178;
        undefined field253_0x179;
        undefined field254_0x17a;
        undefined field255_0x17b;
        undefined field256_0x17c;
        undefined field257_0x17d;
        undefined field258_0x17e;
        undefined field259_0x17f;
        undefined field260_0x180;
        undefined field261_0x181;
        undefined field262_0x182;
        undefined field263_0x183;
        undefined field264_0x184;
        undefined field265_0x185;
        undefined field266_0x186;
        undefined field267_0x187;
        undefined field268_0x188;
        undefined field269_0x189;
        undefined field270_0x18a;
        undefined field271_0x18b;
        undefined field272_0x18c;
        undefined field273_0x18d;
        undefined field274_0x18e;
        undefined field275_0x18f;
        undefined field276_0x190;
        undefined field277_0x191;
        undefined field278_0x192;
        undefined field279_0x193;
        undefined field280_0x194;
        undefined field281_0x195;
        undefined field282_0x196;
        undefined field283_0x197;
        undefined field284_0x198;
        undefined field285_0x199;
        undefined field286_0x19a;
        undefined field287_0x19b;
        undefined field288_0x19c;
        undefined field289_0x19d;
        undefined field290_0x19e;
        undefined field291_0x19f;
        undefined field292_0x1a0;
        undefined field293_0x1a1;
        undefined field294_0x1a2;
        undefined field295_0x1a3;
        undefined field296_0x1a4;
        undefined field297_0x1a5;
        undefined field298_0x1a6;
        undefined field299_0x1a7;
        undefined field300_0x1a8;
        undefined field301_0x1a9;
        undefined field302_0x1aa;
        undefined field303_0x1ab;
        undefined field304_0x1ac;
        undefined field305_0x1ad;
        undefined field306_0x1ae;
        undefined field307_0x1af;
        undefined field308_0x1b0;
        undefined field309_0x1b1;
        undefined field310_0x1b2;
        undefined field311_0x1b3;
        undefined field312_0x1b4;
        undefined field313_0x1b5;
        undefined field314_0x1b6;
        undefined field315_0x1b7;
        undefined field316_0x1b8;
        undefined field317_0x1b9;
        undefined field318_0x1ba;
        undefined field319_0x1bb;
        undefined field320_0x1bc;
        undefined field321_0x1bd;
        undefined field322_0x1be;
        undefined field323_0x1bf;
        undefined field324_0x1c0;
        undefined field325_0x1c1;
        undefined field326_0x1c2;
        undefined field327_0x1c3;
        undefined field328_0x1c4;
        undefined field329_0x1c5;
        undefined field330_0x1c6;
        undefined field331_0x1c7;
        undefined field332_0x1c8;
        undefined field333_0x1c9;
        undefined field334_0x1ca;
        undefined field335_0x1cb;
        undefined field336_0x1cc;
        undefined field337_0x1cd;
        undefined field338_0x1ce;
        undefined field339_0x1cf;
        undefined field340_0x1d0;
        undefined field341_0x1d1;
        undefined field342_0x1d2;
        undefined field343_0x1d3;
        undefined field344_0x1d4;
        undefined field345_0x1d5;
        undefined field346_0x1d6;
        undefined field347_0x1d7;
        undefined field348_0x1d8;
        undefined field349_0x1d9;
        undefined field350_0x1da;
        undefined field351_0x1db;
        undefined field352_0x1dc;
        undefined field353_0x1dd;
        undefined field354_0x1de;
        undefined field355_0x1df;
        undefined field356_0x1e0;
        undefined field357_0x1e1;
        undefined field358_0x1e2;
        undefined field359_0x1e3;
        undefined field360_0x1e4;
        undefined field361_0x1e5;
        undefined field362_0x1e6;
        undefined field363_0x1e7;
        undefined field364_0x1e8;
        undefined field365_0x1e9;
        undefined field366_0x1ea;
        undefined field367_0x1eb;
        undefined field368_0x1ec;
        undefined field369_0x1ed;
        undefined field370_0x1ee;
        undefined field371_0x1ef;
        undefined field372_0x1f0;
        undefined field373_0x1f1;
        undefined field374_0x1f2;
        undefined field375_0x1f3;
        undefined field376_0x1f4;
        undefined field377_0x1f5;
        undefined field378_0x1f6;
        undefined field379_0x1f7;
        undefined field380_0x1f8;
        undefined field381_0x1f9;
        undefined field382_0x1fa;
        undefined field383_0x1fb;
        undefined field384_0x1fc;
        undefined field385_0x1fd;
        undefined field386_0x1fe;
        undefined field387_0x1ff;
        undefined field388_0x200;
        undefined field389_0x201;
        undefined field390_0x202;
        undefined field391_0x203;
        undefined field392_0x204;
        undefined field393_0x205;
        undefined field394_0x206;
        undefined field395_0x207;
        undefined field396_0x208;
        undefined field397_0x209;
        undefined field398_0x20a;
        undefined field399_0x20b;
        undefined field400_0x20c;
        undefined field401_0x20d;
        undefined field402_0x20e;
        undefined field403_0x20f;
        undefined field404_0x210;
        undefined field405_0x211;
        undefined field406_0x212;
        undefined field407_0x213;
        undefined field408_0x214;
        undefined field409_0x215;
        undefined field410_0x216;
        undefined field411_0x217;
        undefined field412_0x218;
        undefined field413_0x219;
        undefined field414_0x21a;
        undefined field415_0x21b;
        undefined field416_0x21c;
        undefined field417_0x21d;
        undefined field418_0x21e;
        undefined field419_0x21f;
        undefined field420_0x220;
        undefined field421_0x221;
        undefined field422_0x222;
        undefined field423_0x223;
        undefined field424_0x224;
        undefined field425_0x225;
        int8_t saved_game_types[10];
        short field_l2; /* Created by Rename Structure Field action */
        short field_s2;
        undefined field429_0x234;
        undefined field430_0x235;
        undefined field431_0x236;
        undefined field432_0x237;
        undefined field433_0x238;
        undefined field434_0x239;
        undefined field435_0x23a;
        undefined field436_0x23b;
        undefined field437_0x23c;
        undefined field438_0x23d;
        undefined field439_0x23e;
        undefined field440_0x23f;
        undefined field441_0x240;
        undefined field442_0x241;
        undefined field443_0x242;
        undefined field444_0x243;
        short field445_0x244[13];
        undefined field446_0x25e;
        undefined field447_0x25f;
        undefined field448_0x260;
        undefined field449_0x261;
        undefined field450_0x262;
        undefined field451_0x263;
        undefined field452_0x264;
        undefined field453_0x265;
        undefined field454_0x266;
        undefined field455_0x267;
        undefined field456_0x268;
        undefined field457_0x269;
        undefined field458_0x26a;
        undefined field459_0x26b;
        undefined field460_0x26c;
        undefined field461_0x26d;
        undefined field462_0x26e;
        undefined field463_0x26f;
        undefined field464_0x270;
        undefined field465_0x271;
        undefined field466_0x272;
        undefined field467_0x273;
        undefined field468_0x274;
        undefined field469_0x275;
        undefined field470_0x276;
        undefined field471_0x277;
    };
    #pragma pack(pop)

    inline int16_t& _music_level_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572344});
    inline int16_t& _sfx_level_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005722B8});
    inline s_game_anims*& _game_anims = *reinterpret_cast<s_game_anims**>(std::uintptr_t{0x00572354});

    inline s_game_popup_fields*& _game_popup_fields = *reinterpret_cast<s_game_popup_fields**>(std::uintptr_t{0x0059537c});
    inline uint8_t& _loaded_new_game = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572340});
    inline uint8_t& _loaded_debug_save = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572341});

    inline int32_t __cdecl Check_Magic_Number_(const char* filename) {
        using Fn = int32_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043d0d0});
        return fn(filename);
    }

    inline bool __cdecl Get_Game_Type_From_Saved_Game_(const char* filename) {
        using Fn = bool(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043d070});
        return fn(filename);
    }

    void __cdecl Check_For_Saved_Games_(int8_t* list);

    inline void __cdecl Get_Game_Popup_Colors_(int type, int* col1, int* col2) {
        using Fn = void(__cdecl*)(int, int*, int*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0050f730})(type, col1, col2);
    }

    void __cdecl Add_Game_Popup_Fields_(int16_t *out_x, int16_t *out_y);

    int8_t __cdecl Get_Save_Slot_();

    void __cdecl Set_Up_Load_Save_Popup_();

    int8_t __cdecl Do_Load_Game_Popup_(int16_t* exit_flag);

    inline void __cdecl _Draw_Load_Save_Game_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0050fee0})();
    }

    inline void __cdecl Save_Session_Related_Settings_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0043d1d0})();
    }

    inline void __cdecl Bad_Game_File_Message_(uint16_t error_code, int16_t slot) {
        using Fn = void(__cdecl*)(uint16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0050f4c0})(error_code, slot);
    }

    inline void __cdecl Animate_Game_Popup_(void* picture, int32_t speed) {
        using Fn = void(__cdecl*)(void*, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0050fc70})(picture, speed);
    }

    inline void __cdecl _Game_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00510db0})();
    }

    void __cdecl Get_Star_Date_Strings_(s_game_popup_fields* fields, char* output_buf);

    inline const char* const s_dot_GAM_005568f4 = reinterpret_cast<const char*>(0x005568F4);
    inline const char* const s_SAVE_005568fc = reinterpret_cast<const char*>(0x005568FC);

    void __cdecl Get_Save_Game_Date_Strings_(char* output_buffer);

    inline void __cdecl Embed_Special_Color_Codes_(char* str, int code) {
        using Fn = void(__cdecl*)(char*, int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00512290 })(str, code);
    }

    inline void __cdecl Remove_Embedded_Special_Codes_(char* str) {
        using Fn = void(__cdecl*)(char*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00512230 })(str);
    }
}