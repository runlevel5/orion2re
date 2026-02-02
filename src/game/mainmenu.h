#pragma once
#include <cstdint>

namespace MAINMENU {
#pragma pack(push, 1)
    struct s_field_button {
        int16_t field;
        void* field1_0x2;
        void* field2_0x6;
        void* field3_0xa;
    };

    struct s_main_menu {
        uint8_t field0_0x0[12];
        s_field_button fields[6];
        uint8_t field13_0x60[114];
    };
#pragma pack(pop)

    typedef enum MAIN_MENU_BTN {
        MAIN_MENU_BTN_CONTINUE = 0,
        MAIN_MENU_BTN_LOAD = 1,
        MAIN_MENU_BTN_NEW = 2,
        MAIN_MENU_BTN_MULTIPLAY = 3,
        MAIN_MENU_BTN_HELP = 4,
        MAIN_MENU_BTN_QUIT = 5,
    } MAIN_MENU_BTN;

    inline s_main_menu*& _mainmenu =
        *reinterpret_cast<s_main_menu**>(std::uintptr_t{0x0056ECF0});

    inline uint32_t& _main_menu_background =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0056ECDC });

    inline uint32_t& _mainmenu_first_seg = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00595c58});
    inline void*& __gametype = *reinterpret_cast<void**>(std::uintptr_t{0x0056ece8});
    inline int32_t& _credit_y = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056ed0c});
    inline int32_t& __credit_x = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056ed10});
    inline int16_t& _delay_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056ed18});

    inline const char* const s_MAINMENU_LBX_0055a630 = reinterpret_cast<const char*>(0x0055A630); // "MAINMENU.LBX"
    inline const char* const s_H_005569D4 = reinterpret_cast<const char*>(0x005569D4); // "H"
    inline const char* const s_N_0055765C = reinterpret_cast<const char*>(0x0055765C); // "N"
    inline const char* const s_M_00557ED8 = reinterpret_cast<const char*>(0x00557ED8); // "M"
    inline const char* const s_save_question_gam = reinterpret_cast<const char*>(0x0055A640); // "SAVE?.GAM"
    inline const char* const s_save_percent_d_gam = reinterpret_cast<const char*>(0x0055A64C); // "SAVE%d.GAM"

    void __cdecl Main_Menu_Fade_Function_();
    void __cdecl Add_Main_Menu_Fields_();

    inline void __cdecl Reload_Main_Menu_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e9d50});
        fn();
    }

    inline void __cdecl Main_Menu_Fade_Function2_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e9580});
        fn();
    }

    inline int16_t __cdecl Do_Main_Menu_File_Animation_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e9250});
        return fn();
    }

    inline void __cdecl Draw_Main_Menu_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e95b0});
        fn();
    }

    inline void __cdecl Set_Main_Menu_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041e3a0});
        fn();
    }

    inline uint8_t __cdecl Do_Mainmenu_Load_Screen_() {
        using Fn = uint8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e94d0});
        return fn();
    }

    void __cdecl Main_Menu_Screen_();
}