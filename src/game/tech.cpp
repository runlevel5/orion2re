#include "pch.h"

namespace TECH {

void Tech_Change_() {
    allocate::Allocate_First_Block_(MOX::_screen_seg, 4);
    video::Set_Page_Off_();
    fields::Draw_Visible_Fields_();
    video::Copy_Off_To_Back_();
    TECH::_Tech_Select_(1);
    MOX::_current_screen = MOX::_return_screen;
}

}