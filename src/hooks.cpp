#include "pch.h"
#include <array>

namespace hooks {
    static constexpr hook all_hooks[] = {
        { 0x005422e0, &platform::entry },
        { 0x005276f0, &platform::Parse_Ten_Cmd_Parameter },
        { 0x00527960, &platform::Create_Direct_Draw },
        { 0x00527e60, &platform::Flip_Surface },
        { 0x005282D0, &platform::Lock_Appropriate_Render_Buffer },
        { 0x005281A0, &platform::Unlock_Appropriate_Render_Buffer },
        { 0x00528500, &platform::Unlock_Primary_Surface },
        { 0x00527E10, &platform::Blit_To_Back_Buffer },
        { 0x005272F0, &platform::Draw_Cursor_XY },
        { 0x00526EF0, &platform::Force_Cursor_Pos },
        { 0x005273C0, &platform::Create_Cursor_Surfaces },
        { 0x00527090, &platform::Draw_Auto_Cursor },
        { 0x00528B30, &platform::Restore_Surfaces },
        { 0x00527D30, &platform::Cleanup_Surfaces },
        { 0x00528470, &platform::Lock_Primary_Surface },
        { 0x005277b0, &platform::WinMain_Proxy },
        { 0x0052d310, &exitm::Exit_With_Message },
        { 0x0043ce10, &FILEDEF::Load_Game_Settings_ },
        { 0x0043cdb0, &FILEDEF::Save_Game_Settings_ },
        { 0x0043d300, &FILEDEF::Update_Game_Settings_Version_Number_ },
        { 0x00478e80, &INITGAME::Set_Default_Game_Settings_ },
        { 0x004f24d0, &HAROLD::Get_Font_File_ },
        { 0x00483470, &MOX2::main2_ },
        { 0x005274d0, &mouse::Set_Mouse_List_ },
        { 0x0052d580, &sound::Set_Audio_Buffer_ },
        { 0x00500b20, &allocate::Allocate_Data_Space_ },
        { 0x00525400, &fonts::Load_Palette_ },
        { 0x004251d0, &JIM::Play_Background_Music_ },
    };

#define ASSERT(x, m) if (!(x)) { MessageBoxA(platform::g_main_window, m, "Microprose", MB_OK); __exit(1); }

    void Assert_Types() {
        ASSERT(sizeof(s_settings) == 0x229, "MOX::_settings");
        ASSERT(sizeof(s_player) == 0xea9, "s_player");
        ASSERT(sizeof(s_planet_data) == 0x11, "s_planet_data");
        ASSERT(sizeof(s_colony) == 0x169, "s_colony");
        ASSERT(sizeof(s_star_data) == 0x71, "s_star_data");
        ASSERT(sizeof(s_ship_data) == 0x81, "s_ship_data");
        ASSERT(sizeof(s_leader_data) == 0x3b, "s_leader_data");
    }

    void Install_Hooks()
    {
        for (const auto& h : all_hooks) {
            patch::Create_Jmp(reinterpret_cast<void*>(h.address), h.handler);
        }

        Assert_Types();
    }
};