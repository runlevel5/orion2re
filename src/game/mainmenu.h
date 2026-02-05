#pragma once
#include <cstdint>

namespace MAINMENU {
#pragma pack(push, 1)
    struct s_main_menu_button {
        s_animation_header* field0_0x0;
        s_animation_header* field1_0x4;
        s_animation_header* field2_0x8;
        int16_t field;
    };

    struct s_main_menu {
        s_main_menu_button fields[6];
        uint8_t field13_0x54[126];
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

    inline s_animation_header*& _main_menu_background =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x0056ECDC });

    inline s_animation_header*& _mainmenu_first_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00595c58});
    inline s_animation_header*& _mainmenu_last_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00595C5C});
    inline void*& __gametype = *reinterpret_cast<void**>(std::uintptr_t{0x0056ece8});
    inline int16_t& _max_credits = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056ECE0});
    inline int32_t& DAT_0056ece4 = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056ECE4});
    inline uint16_t& DAT_0056ecf4 = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0056ECF4});
    inline uint8_t& _credit_num = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056ECF8});
    inline uint8_t& _credits_font_spacing = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056ECFC});
    inline void*& _bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x0056ED00});
    inline int32_t& _credit_y = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056ed0c});
    inline int32_t& _credit_x = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056ed10});
    inline int16_t& _delay_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056ed18});

    inline const char* const s_MAINMENU_LBX_0055a630 = reinterpret_cast<const char*>(0x0055A630); // "MAINMENU.LBX"
    inline const char* const s_C_00552420 = reinterpret_cast<const char*>(0x00552420); // "C"
    inline const char* const s_S_005569CC = reinterpret_cast<const char*>(0x005569CC); // "S"
    inline const char* const s_L_005560E8 = reinterpret_cast<const char*>(0x005560E8); // "L"
    inline const char* const s_M_00557ED8 = reinterpret_cast<const char*>(0x00557ED8); // "M"
    inline const char* const s_H_005569D4 = reinterpret_cast<const char*>(0x005569D4); // "H"
    inline const char* const s_Q_005598F0 = reinterpret_cast<const char*>(0x005598F0); // "Q"
    inline const char* const s_hotkeys_005510f0 = reinterpret_cast<const char*>(0x005510F0);
    inline const char* const s_empty_00551120 = reinterpret_cast<const char*>(0x00551120);
    inline const char* const s_N_0055765C = reinterpret_cast<const char*>(0x0055765C); // "N"
    inline const char* const s_save_question_gam = reinterpret_cast<const char*>(0x0055A640); // "SAVE?.GAM"
    inline const char* const s_save_percent_d_gam = reinterpret_cast<const char*>(0x0055A64C); // "SAVE%d.GAM"
    inline const char* s_Version_1_31_0055a658 = reinterpret_cast<const char*>(0x0055A658); // "Version 1.31"

    void __cdecl Main_Menu_Fade_Function_();
    void __cdecl Main_Menu_Fade_Function2_();
    void __cdecl Add_Main_Menu_Fields_();
    void __cdecl Reload_Main_Menu_Screen_();

    uint16_t __cdecl Do_Main_Menu_File_Animation_();

    inline void __cdecl Draw_File_Animation_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e95a0});
        fn();
    }

    void __cdecl Fade_Credit_Colors_(int8_t amount, s_colors* colors);

    inline void __cdecl Copy_Credit_String_To_Bitmap_(int16_t index, void* bmp) {
        using Fn = void(__cdecl*)(int16_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004E9A40});
        fn(index, bmp);
    }

    void __cdecl Draw_Credits_();

    void __cdecl Draw_Main_Menu_Screen_();

    void __cdecl Set_Main_Menu_Screen_Help_List_Aux_(void* help_data, int16_t count);
    void __cdecl Set_Main_Menu_Screen_Help_List_();

    int8_t __cdecl Do_Mainmenu_Load_Screen_();

    void __cdecl Load_Mainmenu_Load_Game_Popup_();

    int8_t __cdecl Mainmenu_Load_Game_Popup_(int16_t* save_game_list);

    void __cdecl Main_Menu_Screen_();
}