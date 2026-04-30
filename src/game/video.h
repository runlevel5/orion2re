#pragma once

struct IDirectDrawPalette;
struct tagPALETTEENTRY;

namespace video {
	// Variables
	inline uint32_t*& _video_memory = *reinterpret_cast<uint32_t**>(std::uintptr_t{ 0x005792F0 });

	inline int16_t& _current_mouse_list_ptr =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578A94 });

	inline int16_t& _current_mouse_shape =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578AC4 });

	inline uint8_t*& _current_video_page = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005792F8});

	inline uint32_t& screen_dump_flag =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B1A0 });

	struct s_scan_list_entry {
		int16_t min_x;
		int16_t max_x;
	};

	inline int32_t& screen_refresh_flag =
		*reinterpret_cast<int32_t*>(std::uintptr_t{ 0x0055B1B4 });

	inline s_scan_list_entry*& _refresh_scan_list =
		*reinterpret_cast<s_scan_list_entry**>(std::uintptr_t{ 0x00579348 });

	inline s_scan_list_entry*& _refresh_stencil_list =
		*reinterpret_cast<s_scan_list_entry**>(std::uintptr_t{ 0x0057934C });

	inline uint8_t*& _mouse_palette =
		*reinterpret_cast<uint8_t**>(std::uintptr_t{ 0x00578AB0 });

	inline uint32_t& _screen_pixel_size =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055b1b0 });

	inline auto& _glass_remap_colors = *reinterpret_cast<uint8_t(*)[16][256]>(std::uintptr_t{ 0x00579998 });

	inline s_glass_color (&_glass_colors)[16] =
		*reinterpret_cast<s_glass_color(*)[16]>(std::uintptr_t{ 0x00579858 });

	inline IDirectDrawPalette*& g_palette =
		*reinterpret_cast<IDirectDrawPalette**>(std::uintptr_t{ 0x00578E64 });

	inline tagPALETTEENTRY (&g_palette_entries)[256] =
		*reinterpret_cast<tagPALETTEENTRY (*)[256]>(std::uintptr_t{ 0x00578EE8 });

	// Address 0x579350 is the base of the current palette status array
	inline s_palette_entry (&_current_palette)[256] =
		*reinterpret_cast<s_palette_entry(*)[256]>(std::uintptr_t{ 0x00579350 });

	inline uint8_t* (&_palette_list)[65] =
		*reinterpret_cast<uint8_t*(*)[65]>(std::uintptr_t{ 0x00579750 });

	inline uint8_t* _palette_reds =
		reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00579898 });

	inline uint8_t& DAT_00579354 =
		*reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00579354 });

	inline int16_t& _copy_to_on_page_flag =
		*reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578e60 });

	inline int16_t& _screen_window_y1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DF4 });
	inline int16_t& _screen_window_y2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DF8 });
	inline int16_t& _screen_window_x1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DFC });
	inline int16_t& _screen_window_x2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578E00 });

	inline uint8_t* DAT_005A56E0 = reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A56E0 });

	// Functions
	void __cdecl Set_Refresh_Stencil_();
	void __cdecl Set_Refresh_Full_();
	void __cdecl Set_Page_Off_();
	void __cdecl Clip_Off_();
	void __cdecl Clip_On_();

	// TODO: These were re-implemented in platform
	inline void __cdecl Copy_Off_To_Back_() {
		using Fn = void(__cdecl*)();
		reinterpret_cast<Fn>(std::uintptr_t{ 0x005286c0 })();
	}

    inline void __cdecl Set_Page_Back_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005286A0})();
    }

    inline void __cdecl Copy_Back_To_Off_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00528700})();
    }

    void __cdecl Add_Line_To_Scan_List_(int32_t x1, int32_t y, int32_t x2);

	void __cdecl Add_Dot_To_Scan_List_(int32_t x_scaled, int32_t y);

	void __cdecl Set_Refresh_Differential_();
	uint8_t __cdecl Get_Current_Video_Page_Flag_();
	int16_t __cdecl Get_Refresh_Mode_();

	inline void __cdecl Init_Video_Drivers_() {
		using Fn = void(__cdecl*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526d10});
		fn();
	}

    inline void __cdecl _Masked_Screen_Dump() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528030});
        fn();
    }

    inline void __cdecl _Partial_Screen_Dump() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528040});
        fn();
    }

    inline void __cdecl Stencil_Out_Background_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528110});
        fn();
    }

    inline void __cdecl Clear_Stencil_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005287C0});
        fn();
    }

    inline void __cdecl Clear_Scan_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005287A0});
        fn();
    }

    void __cdecl Toggle_Pages_();

    inline void __cdecl Save_Refresh_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527c90 });
        fn();
    }

    inline void __cdecl Set_Refresh_Differential_No_Full_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527d20 });
        fn();
    }

    inline void __cdecl Restore_Refresh_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527cb0 });
        fn();
    }

    inline void __cdecl Clear_Display_And_Release_Palette_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D290 });
        fn();
    }
}