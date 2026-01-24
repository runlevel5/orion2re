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
        { 0x00527680, &mouse::Mouse_Button_ },
        { 0x0052d580, &sound::Set_Audio_Buffer_ },
        { 0x00500b20, &allocate::Allocate_Data_Space_ },
        { 0x00525400, &fonts::Load_Palette_ },
        { 0x004251d0, &JIM::Play_Background_Music_ },
        { 0x00528680, &video::Set_Page_Off_ },
        { 0x00483ef0, &MOX2::Set_Mox_Language_ },
        { 0x00527ea0, &platform::Toggle_Pages_Stub_ },
        { 0x0052b220, &timer::Mark_Time_ },
        { 0x00531f80, &key::Keyboard_Status_ },
        { 0x00531f20, &key::Read_Key_ },
        { 0x00425a70, &JIM::Draw_Logos_ },
        { 0x0052e880, &allocate::Allocate_Space_ },
        { 0x0052ebb0, &allocate::Get_Remaining_Block_Space_ },
        { 0x0052e9f0, &allocate::Allocation_Error_ },
        { 0x0052eb30, &allocate::Push_Block_ },
        { 0x0052eb70, &allocate::Pop_Block_ },
        { 0x0052e950, &allocate::Allocate_Next_Block_ },
        { 0x0052ea20, &allocate::Reallocation_Error_ },
        { 0x0052e8e0, &allocate::Allocate_Dos_Space_ },
        { 0x0052e9c0, &allocate::Allocate_Far_Space_ },
        // TODO: Why no games in menu when hooked?
        //{ 0x0052d360, &dos::DIR_ },
        { 0x0051c7e0, &farload::Farload_Library_ },
        { 0x0051c710, &farload::Far_Reload_ },
        { 0x00478fb0, &INITGAME::Write_Game_Settings_ },
        { 0x00478b30, &INITGAME::Init_Ships_Array_ },
        { 0x00478d80, &INITGAME::Init_Colonies_Array_ },
        { 0x00478820, &INITGAME::Init_New_Game_ },
        { 0x00479580, &INITGAME::Init_Leaders_ },
        { 0x004f2610, &HAROLD::Get_Officer_Data_LBX_Name_ },
        { 0x00479000, &INITGAME::Init_Players_ },
        { 0x00478bc0, &INITGAME::Make_System_Monsters_Into_Ships_ },
        { 0x00478d10, &INITGAME::Set_Initial_Colony_Auto_Selects_ },
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