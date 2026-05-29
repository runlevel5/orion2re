#pragma once

namespace mouse {
    inline uint16_t& _mouse_enabled =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055B178 });

    inline uint16_t& _mouse_state =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578DE4 });

    inline int16_t& _current_mouse_list_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578A90 });

    inline uint32_t& _num =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00578AC0 });

    inline int16_t& _mouse_buffer_button =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578AC8 });

    inline int16_t& _mouse_buffer_x =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578B44 });

    inline int16_t& _mouse_buffer_y =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578B48 });

    inline s_mouse_list*& _current_mouse_list =
        *reinterpret_cast<s_mouse_list**>(std::uintptr_t{ 0x00578A98 });

    void __cdecl Check_Mouse_Shape_();

    void __cdecl Check_Mouse_Buffer_();

    void __cdecl User_Mouse_Handler_(int16_t mouse_x, int16_t mouse_y, uint16_t mouse_buttons);

    inline int32_t& _in_mouse_handler =
        *reinterpret_cast<int32_t*>(std::uintptr_t{ 0x0055B19C });

    inline int16_t& _mouse_overide_flag =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B180 });

    inline int16_t& _mouse_overide_right_button =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B184 });

    inline int16_t& _mouse_overide_left_button =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B188 });

    inline int16_t& _mouse_buttons =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00578DE0});

    inline int16_t& _mouse_buffer_flag =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DD8 });

    inline uint16_t& _mouse_buffer_flag2 =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578B50 });

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