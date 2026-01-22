#include "pch.h"
#include <array>

namespace hooks {
    static constexpr hook all_hooks[] = {
        // platform
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
        // exitm
        { 0x0052d310, &exitm::Exit_With_Message },
        // FILEDEF
        { 0x0043ce10, &FILEDEF::Load_Game_Settings_ },
        { 0x0043cdb0, &FILEDEF::Save_Game_Settings_ },
        { 0x0043d300, &FILEDEF::Update_Game_Settings_Version_Number_ },
        // MOX2
        { 0x00483470, &MOX2::main2_ },
    };

#define ASSERT(x, m) if (!x) { exitm::Exit_With_Message(m); }

    void Assert_Types() {
        ASSERT(sizeof(MOX::_settings) == 0x229, "MOX::_settings");
    }

    void Install_Hooks()
    {
        for (const auto& h : all_hooks) {
            patch::Create_Jmp(reinterpret_cast<void*>(h.address), h.handler);
        }

        Assert_Types();
    }
};