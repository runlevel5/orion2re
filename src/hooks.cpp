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
        { 0x0051a620, &HOMEGEN::Generate_Home_Worlds_ },
        { 0x0051a950, &HOMEGEN::Build_Home_Star_List_ },
        { 0x0051bc90, &HOMEGEN::Set_Up_Home_Gen_Msg_ },
        { 0x0051aca0, &HOMEGEN::Build_Min_Star_Distances_ },
        { 0x0051bbc0, &HOMEGEN::Randomize_Home_Worlds_ },
        { 0x0051a010, &HOMEGEN::Enforce_3_Planet_Minimum_ },
        { 0x004f12d0, &HAROLD::N_Planets_In_System_ },
        { 0x004f0a70, &HAROLD::Habitable_Spectral_Class_ },
        { 0x004b6610, &MAPGEN::Generate_Spectral_Class_ },
        { 0x0051b300, &HOMEGEN::Modify_Home_Worlds_ },
        { 0x004f0fd0, &HAROLD::Is_Colony_Star_ },
        { 0x004f19e0, &HAROLD::Star_Is_Orion_ },
        { 0x0051b190, &HOMEGEN::Is_Added_Food_World_ },
        { 0x0051c3f0, &HOMEGEN::Make_System_Monsters_ },
        { 0x00519ea0, &HOMEGEN::Assign_Home_Star_Names_ },
        { 0x004b5bf0, &MAPGEN::Generate_Food_Per_Farmer_ },
        { 0x0051a170, &HOMEGEN::Enforce_Climate_ },
        { 0x0051bc20, &HOMEGEN::Remove_Specials_ },
        { 0x0051b570, &HOMEGEN::Set_Planet_Size },
        { 0x0051a490, &HOMEGEN::Enforce_Medium_Planet_Size_ },
        { 0x0051a5c0, &HOMEGEN::Enforce_Planet_Type_ },
        { 0x0051a150, &HOMEGEN::Enforce_Planet_Abundant_Mineral_Class },
        { 0x0051a4d0, &HOMEGEN::Enforce_Mineral_Richness_ },
        { 0x0051a410, &HOMEGEN::Enforce_Gravity_ },
        { 0x0051b8c0, &HOMEGEN::Guarantee_A_Planet_At_Min_Dist_ },
        { 0x0051c3b0, &HOMEGEN::compare_star_dist },
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