#pragma once

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

	inline uint32_t& screen_refresh_flag =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B1B4 });

	inline uint32_t*& _mouse_palette =
		*reinterpret_cast<uint32_t**>(std::uintptr_t{ 0x00578AB0 });

	inline uint32_t& _screen_pixel_size =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055b1b0 });

	inline auto& _glass_remap_colors = *reinterpret_cast<uint8_t(*)[16][256]>(std::uintptr_t{ 0x00579998 });

	inline s_glass_color (&_glass_colors)[16] =
		*reinterpret_cast<s_glass_color(*)[16]>(std::uintptr_t{ 0x00579858 });

	// Address 0x579350 is the base of the current palette status array
	inline s_palette_entry* _current_palette =
		reinterpret_cast<s_palette_entry*>(std::uintptr_t{ 0x00579350 });

	inline uint8_t** _palette_list =
		reinterpret_cast<uint8_t**>(std::uintptr_t{ 0x00579750 });

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

	// Functions
	void __cdecl Set_Refresh_Stencil_();
	void Set_Refresh_Full_();

	inline void __cdecl Load_Palette_(uint32_t p1, uint32_t p2, uint32_t p3) {
		using Fn = void(__cdecl*)(uint32_t, uint32_t, uint32_t);
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00525400});
		fn(p1, p2, p3);
	}

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

    inline void __cdecl Add_Line_To_Scan_List_(int32_t start_x, int32_t y, int32_t end_x) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528A30 });
        fn(start_x, y, end_x);
    }

    inline void __cdecl Add_Dot_To_Scan_List_(int32_t x, int32_t y) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528970 });
        fn(x, y);
    }

	void __cdecl Set_Refresh_Differential_();
	uint8_t __cdecl Get_Current_Video_Page_Flag_();

	inline void __cdecl Init_Video_Drivers_() {
		using Fn = void(__cdecl*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526d10});
		fn();
	}
}