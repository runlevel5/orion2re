#pragma once
#include <cstdint>

namespace LOADSAVE {
    inline const char* s_INVALID_STR = reinterpret_cast<const char*>(0x0055AC58);

    inline char (&_months)[12][6] = *reinterpret_cast<char(*)[12][6]>(std::uintptr_t{0x005722F8});

    void __cdecl Load_Month_Names_();

    void __cdecl Allocate_Data_Space_For_Game_Popup_();

    void __cdecl Restore_Main_Screen_();

    void __cdecl Restore_Game_Popup_();

    /**
     * @brief Displays a popup message box based on the specified type.
     * 
     * @param message The message content.
     * @param type The type of popup: 0 = Message, 1 = Warning with result (Yes/No), 3 = Warning Notification.
     * @return For type 1, returns the user selection; otherwise returns 0.
     */
    int16_t __cdecl Game_Popup_User_Box_(char* message, int16_t type);

    inline uint8_t& _draw_game_popup_to_back =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722F4});
    inline uint8_t& _dont_draw_darkened_buttons = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722C4});
    inline uint8_t& _draw_down = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AC50});
    inline char (&_empty_save_slot_message)[20] = *reinterpret_cast<char(*)[20]>(std::uintptr_t{0x005722D8});

    #pragma pack(push, 1)
    struct s_game_anims {
        struct s_animation_header* pictures[25];
        int8_t field1_0x64;
    };


    struct s_game_popup_fields {
        int32_t field0_0x0[10];
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
        short option_names[13];
        short field_A;
        short field19_0x6e;
        short field_esc_2;
        short field_space;
        short music_level;
        short sound_fx_level;
        short music_on;
        uint16_t sound_fx_on;
        short save_slot_fields[10];
        uint16_t options[13];
        int8_t saved_games[10];
        undefined field29_0xb4;
        undefined field30_0xb5;
        undefined field31_0xb6;
        undefined field32_0xb7;
        undefined field33_0xb8;
        undefined field34_0xb9;
        undefined field35_0xba;
        undefined field36_0xbb;
        undefined field37_0xbc;
        undefined field38_0xbd;
        undefined field39_0xbe;
        undefined field40_0xbf;
        undefined field41_0xc0;
        undefined field42_0xc1;
        undefined field43_0xc2;
        undefined field44_0xc3;
        undefined field45_0xc4;
        undefined field46_0xc5;
        undefined field47_0xc6;
        undefined field48_0xc7;
        undefined field49_0xc8;
        undefined field50_0xc9;
        undefined field51_0xca;
        undefined field52_0xcb;
        undefined field53_0xcc;
        undefined field54_0xcd;
        undefined field55_0xce;
        undefined field56_0xcf;
        undefined field57_0xd0;
        undefined field58_0xd1;
        undefined field59_0xd2;
        undefined field60_0xd3;
        undefined field61_0xd4;
        undefined field62_0xd5;
        undefined field63_0xd6;
        undefined field64_0xd7;
        undefined field65_0xd8;
        undefined field66_0xd9;
        undefined field67_0xda;
        undefined field68_0xdb;
        undefined field69_0xdc;
        undefined field70_0xdd;
        undefined field71_0xde;
        undefined field72_0xdf;
        undefined field73_0xe0;
        undefined field74_0xe1;
        undefined field75_0xe2;
        undefined field76_0xe3;
        undefined field77_0xe4;
        undefined field78_0xe5;
        undefined field79_0xe6;
        undefined field80_0xe7;
        undefined field81_0xe8;
        undefined field82_0xe9;
        undefined field83_0xea;
        undefined field84_0xeb;
        undefined field85_0xec;
        undefined field86_0xed;
        undefined field87_0xee;
        undefined field88_0xef;
        undefined field89_0xf0;
        undefined field90_0xf1;
        undefined field91_0xf2;
        undefined field92_0xf3;
        undefined field93_0xf4;
        undefined field94_0xf5;
        undefined field95_0xf6;
        undefined field96_0xf7;
        undefined field97_0xf8;
        undefined field98_0xf9;
        undefined field99_0xfa;
        undefined field100_0xfb;
        undefined field101_0xfc;
        undefined field102_0xfd;
        undefined field103_0xfe;
        undefined field104_0xff;
        undefined field105_0x100;
        undefined field106_0x101;
        undefined field107_0x102;
        undefined field108_0x103;
        undefined field109_0x104;
        undefined field110_0x105;
        undefined field111_0x106;
        undefined field112_0x107;
        undefined field113_0x108;
        undefined field114_0x109;
        undefined field115_0x10a;
        undefined field116_0x10b;
        undefined field117_0x10c;
        undefined field118_0x10d;
        undefined field119_0x10e;
        undefined field120_0x10f;
        undefined field121_0x110;
        undefined field122_0x111;
        undefined field123_0x112;
        undefined field124_0x113;
        undefined field125_0x114;
        undefined field126_0x115;
        undefined field127_0x116;
        undefined field128_0x117;
        undefined field129_0x118;
        undefined field130_0x119;
        undefined field131_0x11a;
        undefined field132_0x11b;
        undefined field133_0x11c;
        undefined field134_0x11d;
        undefined field135_0x11e;
        undefined field136_0x11f;
        undefined field137_0x120;
        undefined field138_0x121;
        undefined field139_0x122;
        undefined field140_0x123;
        undefined field141_0x124;
        undefined field142_0x125;
        undefined field143_0x126;
        undefined field144_0x127;
        undefined field145_0x128;
        undefined field146_0x129;
        undefined field147_0x12a;
        undefined field148_0x12b;
        undefined field149_0x12c;
        undefined field150_0x12d;
        undefined field151_0x12e;
        undefined field152_0x12f;
        undefined field153_0x130;
        undefined field154_0x131;
        undefined field155_0x132;
        undefined field156_0x133;
        undefined field157_0x134;
        undefined field158_0x135;
        undefined field159_0x136;
        undefined field160_0x137;
        undefined field161_0x138;
        undefined field162_0x139;
        undefined field163_0x13a;
        undefined field164_0x13b;
        undefined field165_0x13c;
        undefined field166_0x13d;
        undefined field167_0x13e;
        undefined field168_0x13f;
        undefined field169_0x140;
        undefined field170_0x141;
        undefined field171_0x142;
        undefined field172_0x143;
        undefined field173_0x144;
        undefined field174_0x145;
        undefined field175_0x146;
        undefined field176_0x147;
        undefined field177_0x148;
        undefined field178_0x149;
        undefined field179_0x14a;
        undefined field180_0x14b;
        undefined field181_0x14c;
        undefined field182_0x14d;
        undefined field183_0x14e;
        undefined field184_0x14f;
        undefined field185_0x150;
        undefined field186_0x151;
        undefined field187_0x152;
        undefined field188_0x153;
        undefined field189_0x154;
        undefined field190_0x155;
        undefined field191_0x156;
        undefined field192_0x157;
        undefined field193_0x158;
        undefined field194_0x159;
        undefined field195_0x15a;
        undefined field196_0x15b;
        undefined field197_0x15c;
        undefined field198_0x15d;
        undefined field199_0x15e;
        undefined field200_0x15f;
        undefined field201_0x160;
        undefined field202_0x161;
        undefined field203_0x162;
        undefined field204_0x163;
        undefined field205_0x164;
        undefined field206_0x165;
        undefined field207_0x166;
        undefined field208_0x167;
        undefined field209_0x168;
        undefined field210_0x169;
        undefined field211_0x16a;
        undefined field212_0x16b;
        undefined field213_0x16c;
        undefined field214_0x16d;
        undefined field215_0x16e;
        undefined field216_0x16f;
        undefined field217_0x170;
        undefined field218_0x171;
        undefined field219_0x172;
        undefined field220_0x173;
        undefined field221_0x174;
        undefined field222_0x175;
        undefined field223_0x176;
        undefined field224_0x177;
        undefined field225_0x178;
        undefined field226_0x179;
        undefined field227_0x17a;
        undefined field228_0x17b;
        undefined field229_0x17c;
        undefined field230_0x17d;
        undefined field231_0x17e;
        undefined field232_0x17f;
        undefined field233_0x180;
        undefined field234_0x181;
        undefined field235_0x182;
        undefined field236_0x183;
        undefined field237_0x184;
        undefined field238_0x185;
        undefined field239_0x186;
        undefined field240_0x187;
        undefined field241_0x188;
        undefined field242_0x189;
        undefined field243_0x18a;
        undefined field244_0x18b;
        undefined field245_0x18c;
        undefined field246_0x18d;
        undefined field247_0x18e;
        undefined field248_0x18f;
        undefined field249_0x190;
        undefined field250_0x191;
        undefined field251_0x192;
        undefined field252_0x193;
        undefined field253_0x194;
        undefined field254_0x195;
        undefined field255_0x196;
        undefined field256_0x197;
        undefined field257_0x198;
        undefined field258_0x199;
        undefined field259_0x19a;
        undefined field260_0x19b;
        undefined field261_0x19c;
        undefined field262_0x19d;
        undefined field263_0x19e;
        undefined field264_0x19f;
        undefined field265_0x1a0;
        undefined field266_0x1a1;
        undefined field267_0x1a2;
        undefined field268_0x1a3;
        undefined field269_0x1a4;
        undefined field270_0x1a5;
        undefined field271_0x1a6;
        undefined field272_0x1a7;
        undefined field273_0x1a8;
        undefined field274_0x1a9;
        undefined field275_0x1aa;
        undefined field276_0x1ab;
        undefined field277_0x1ac;
        undefined field278_0x1ad;
        undefined field279_0x1ae;
        undefined field280_0x1af;
        undefined field281_0x1b0;
        undefined field282_0x1b1;
        undefined field283_0x1b2;
        undefined field284_0x1b3;
        undefined field285_0x1b4;
        undefined field286_0x1b5;
        undefined field287_0x1b6;
        undefined field288_0x1b7;
        undefined field289_0x1b8;
        undefined field290_0x1b9;
        undefined field291_0x1ba;
        undefined field292_0x1bb;
        undefined field293_0x1bc;
        undefined field294_0x1bd;
        undefined field295_0x1be;
        undefined field296_0x1bf;
        undefined field297_0x1c0;
        undefined field298_0x1c1;
        undefined field299_0x1c2;
        undefined field300_0x1c3;
        undefined field301_0x1c4;
        undefined field302_0x1c5;
        undefined field303_0x1c6;
        undefined field304_0x1c7;
        undefined field305_0x1c8;
        undefined field306_0x1c9;
        undefined field307_0x1ca;
        undefined field308_0x1cb;
        undefined field309_0x1cc;
        undefined field310_0x1cd;
        undefined field311_0x1ce;
        undefined field312_0x1cf;
        undefined field313_0x1d0;
        undefined field314_0x1d1;
        undefined field315_0x1d2;
        undefined field316_0x1d3;
        undefined field317_0x1d4;
        undefined field318_0x1d5;
        undefined field319_0x1d6;
        undefined field320_0x1d7;
        undefined field321_0x1d8;
        undefined field322_0x1d9;
        undefined field323_0x1da;
        undefined field324_0x1db;
        undefined field325_0x1dc;
        undefined field326_0x1dd;
        undefined field327_0x1de;
        undefined field328_0x1df;
        undefined field329_0x1e0;
        undefined field330_0x1e1;
        undefined field331_0x1e2;
        undefined field332_0x1e3;
        undefined field333_0x1e4;
        undefined field334_0x1e5;
        undefined field335_0x1e6;
        undefined field336_0x1e7;
        undefined field337_0x1e8;
        undefined field338_0x1e9;
        undefined field339_0x1ea;
        undefined field340_0x1eb;
        undefined field341_0x1ec;
        undefined field342_0x1ed;
        undefined field343_0x1ee;
        undefined field344_0x1ef;
        undefined field345_0x1f0;
        undefined field346_0x1f1;
        undefined field347_0x1f2;
        undefined field348_0x1f3;
        undefined field349_0x1f4;
        undefined field350_0x1f5;
        undefined field351_0x1f6;
        undefined field352_0x1f7;
        undefined field353_0x1f8;
        undefined field354_0x1f9;
        undefined field355_0x1fa;
        undefined field356_0x1fb;
        undefined field357_0x1fc;
        undefined field358_0x1fd;
        undefined field359_0x1fe;
        undefined field360_0x1ff;
        undefined field361_0x200;
        undefined field362_0x201;
        undefined field363_0x202;
        undefined field364_0x203;
        undefined field365_0x204;
        undefined field366_0x205;
        undefined field367_0x206;
        undefined field368_0x207;
        undefined field369_0x208;
        undefined field370_0x209;
        undefined field371_0x20a;
        undefined field372_0x20b;
        undefined field373_0x20c;
        undefined field374_0x20d;
        undefined field375_0x20e;
        undefined field376_0x20f;
        undefined field377_0x210;
        undefined field378_0x211;
        undefined field379_0x212;
        undefined field380_0x213;
        undefined field381_0x214;
        undefined field382_0x215;
        undefined field383_0x216;
        undefined field384_0x217;
        undefined field385_0x218;
        undefined field386_0x219;
        undefined field387_0x21a;
        undefined field388_0x21b;
        undefined field389_0x21c;
        undefined field390_0x21d;
        undefined field391_0x21e;
        undefined field392_0x21f;
        undefined field393_0x220;
        undefined field394_0x221;
        undefined field395_0x222;
        undefined field396_0x223;
        undefined field397_0x224;
        undefined field398_0x225;
        int8_t saved_game_types[10];
        short field_l2; /* Created by Rename Structure Field action */
        short field_s2;
        struct s_colors popup_color1;
        struct s_colors popup_color2;
        short option_toggles[13];
        short language;
        undefined field406_0x260;
        undefined field407_0x261;
        undefined field408_0x262;
        undefined field409_0x263;
        undefined field410_0x264;
        undefined field411_0x265;
        undefined field412_0x266;
        undefined field413_0x267;
        undefined field414_0x268;
        undefined field415_0x269;
        undefined field416_0x26a;
        undefined field417_0x26b;
        undefined field418_0x26c;
        undefined field419_0x26d;
        undefined field420_0x26e;
        undefined field421_0x26f;
        undefined field422_0x270;
        undefined field423_0x271;
        undefined field424_0x272;
        undefined field425_0x273;
        undefined field426_0x274;
        undefined field427_0x275;
        undefined field428_0x276;
        undefined field429_0x277;
    };
#pragma pack(pop)

    inline int16_t& _music_level_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572344});
    inline int16_t& _sfx_level_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005722B8});
    inline uint8_t& _print_data_to_bitmap = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722EC});
    inline struct s_animation_header*& _bitmap = *reinterpret_cast<struct s_animation_header**>(std::uintptr_t{0x005722F0});

    inline s_game_anims*& _game_anims = *reinterpret_cast<s_game_anims**>(std::uintptr_t{0x00572354});

    inline s_game_popup_fields*& _game_popup_fields = *reinterpret_cast<s_game_popup_fields**>(std::uintptr_t{0x0059537c});
    inline uint8_t& _loaded_new_game = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572340});
    inline uint8_t& _loaded_debug_save = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572350});

    int32_t __cdecl Check_Magic_Number_(const char* filename);

    bool __cdecl Get_Game_Type_From_Saved_Game_(const char* filename);

    bool __cdecl Is_Valid_Game_Type_(uint32_t game_type);

    void __cdecl Check_For_Saved_Games_(int8_t* list);

    void __cdecl Get_Game_Popup_Colors_(int16_t font_idx, s_colors* out_colors_primary, s_colors* out_colors_secondary);

    void __cdecl Add_Game_Popup_Fields_(int16_t *out_x, int16_t *out_y);

    int8_t __cdecl Get_Save_Slot_();

    void __cdecl Set_Up_Load_Save_Popup_();

    int8_t __cdecl Do_Load_Game_Popup_(int16_t* exit_flag);

    inline void __cdecl Do_Main_Game_Popup_(int16_t* out_val) {
        using Fn = void(__cdecl*)(int16_t*);
        reinterpret_cast<Fn>(0x005110a0)(out_val);
    }

    void __cdecl Do_Options_Game_Popup_(int16_t* out_val);

    void __cdecl Do_Save_Game_Popup_(int16_t* out_val);

    void __cdecl _Draw_Load_Save_Game_Popup_();

    void __cdecl _Draw_Main_Game_Popup_();

    void __cdecl _Draw_Main_Game_Popup_To_Bitmap_(struct s_animation_header* bitmap);

    void __cdecl _Draw_Load_Save_Game_Popup_To_Bitmap_(struct s_animation_header* bitmap);

    void __cdecl _Draw_Options_Game_Popup_To_Bitmap_(s_animation_header* bitmap);

    void __cdecl Animate_Game_Popup_(s_animation_header* header, int16_t y_offset, int16_t direction, uint16_t type);

    void __cdecl Animate_Game_Popup_Pic_Height_(s_animation_header* animation, uint16_t type);

    void __cdecl Save_Session_Related_Settings_();

    void __cdecl Bad_Game_File_Message_(int16_t type, int16_t file_index);

    void __cdecl _Draw_Options_Game_Popup_();

    void __cdecl _Game_Popup_();

    void __cdecl Get_Star_Date_Strings_(s_game_popup_fields* fields, char* output_buf);

    inline const char* const s_dot_GAM_005568f4 = reinterpret_cast<const char*>(0x005568F4);
    inline const char* const s_SAVE_005568fc = reinterpret_cast<const char*>(0x005568FC);
    inline const char* s_format_0055ad30 = reinterpret_cast<const char*>(0x0055AD30); // "%s, %s, %d %s"

    inline const char* s__ALT_F1__0055ad24 = reinterpret_cast<const char*>(0x0055AD24); // "(ALT F1)"
    inline const char* s__ALT_F2__0055ad18 = reinterpret_cast<const char*>(0x0055AD18); // "(ALT F2)"
    inline const char* s__ALT_F3__0055ad0c = reinterpret_cast<const char*>(0x0055AD0C); // "(ALT F3)"
    inline const char* s__ALT_F5__0055ad00 = reinterpret_cast<const char*>(0x0055AD00); // "(ALT F5)"
    inline const char* s__ALT_F6__0055acf4 = reinterpret_cast<const char*>(0x0055ACF4); // "(ALT F6)"
    inline const char* s__ALT_F7__0055ace8 = reinterpret_cast<const char*>(0x0055ACE8); // "(ALT F7)"
    inline const char* s__ALT_F8__0055addc = reinterpret_cast<const char*>(0x0055ACDC); // "(ALT F8)"

    inline const char* s__ALT_F1__0055acd0 = reinterpret_cast<const char*>(0x0055ACD0); // "(ALT-F1)"
    inline const char* s__ALT_F2__0055acc4 = reinterpret_cast<const char*>(0x0055ACC4); // "(ALT-F2)"
    inline const char* s__ALT_F3__0055acb8 = reinterpret_cast<const char*>(0x0055ACB8); // "(ALT-F3)"
    inline const char* s__ALT_F4__0055acac = reinterpret_cast<const char*>(0x0055ACAC); // "(ALT-F4)"
    inline const char* s__ALT_F5__0055aca0 = reinterpret_cast<const char*>(0x0055ACA0); // "(ALT-F5)"
    inline const char* s__ALT_F6__0055ac94 = reinterpret_cast<const char*>(0x0055AC94); // "(ALT-F6)"
    inline const char* s__ALT_F7__0055ac88 = reinterpret_cast<const char*>(0x0055AC88); // "(ALT-F7)"
    inline const char* s__ALT_F8__0055ac7c = reinterpret_cast<const char*>(0x0055AC7C); // "(ALT-F8)"

    void __cdecl Get_Save_Game_Date_Strings_(char* output_buffer);

    inline void __cdecl Embed_Special_Color_Codes_(char* str, int code) {
        using Fn = void(__cdecl*)(char*, int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00512290 })(str, code);
    }

    inline void __cdecl Remove_Embedded_Special_Codes_(char* str, int max_len = 37) {
        using Fn = void(__cdecl*)(char*, int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00512230 })(str, max_len);
    }

    void __cdecl Draw_Popup_Pic1_();
    void __cdecl Draw_Popup_Pic2_();

    inline void __cdecl Set_Music_For_Game_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005124F0});
        fn();
    }

    inline void __cdecl Set_Sound_For_Game_Popup_(uint8_t param) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00512590});
        fn(param);
    }

    void __cdecl Load_Game_Popup_Pictures_();

    void __cdecl Set_Current_Game_Option_Flags_();

    inline void __cdecl Animate_Game_Popup_Pic_(s_animation_header* pic) {
        using Fn = void(__cdecl*)(s_animation_header*);
        reinterpret_cast<Fn>(0x00511c50)(pic);
    }

    inline void __cdecl Update_Game_Settings_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00511e80)();
    }
}