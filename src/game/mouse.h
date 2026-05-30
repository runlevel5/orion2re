#pragma once

namespace mouse {
    extern uint16_t _mouse_enabled;

    extern uint16_t _mouse_state;

    extern int16_t _current_mouse_list_count;

    extern uint32_t _num;

    extern int16_t _mouse_buffer_button;

    extern int16_t _mouse_buffer_x;

    extern int16_t _mouse_buffer_y;

    extern s_mouse_list* _current_mouse_list;

    void __cdecl Check_Mouse_Shape_();

    void __cdecl Check_Mouse_Buffer_();

    void __cdecl User_Mouse_Handler_(int16_t mouse_x, int16_t mouse_y, uint16_t mouse_buttons);

    extern int32_t _in_mouse_handler;

    extern int16_t _mouse_overide_flag;

    extern int16_t _mouse_overide_right_button;

    extern int16_t _mouse_overide_left_button;

    extern int16_t _mouse_buttons;

    extern int16_t _mouse_buffer_flag;

    extern uint16_t _mouse_buffer_flag2;

    int16_t __cdecl Mouse_Buffer_();

    uint16_t __cdecl Mouse_Buffer2_();

    /**
     * Returns the current mouse button state, allowing for button overrides.
     * Usually, the return value represents a bitmask or sum of button states:
     * 1 = Left, 2 = Right, 3 = Both.
     */
    int16_t __cdecl Mouse_Button_();

    void __cdecl Set_Mouse_List_(s_mouse_list* list, int16_t count);

    int16_t Pointer_X_();

    int16_t Pointer_Y_();

    void __cdecl Clear_Mouse_XY_();

    int __cdecl Get_Pointer_Offset_();

    int __cdecl Get_Pointer_Image_Number_();

    void __cdecl Save_Mouse_State_();

    void __cdecl Restore_Mouse_State_();

    int16_t __cdecl Mouse_Buffer_Button_();

    int16_t __cdecl Mouse_Buffer_X_();

    int16_t __cdecl Mouse_Buffer_Y_();

    void __cdecl Disable_Mouse_();

    void __cdecl Enable_Mouse_();
}