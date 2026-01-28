#pragma once

namespace video {
	// Variables
	inline uintptr_t& _current_video_page = *reinterpret_cast<uintptr_t*>(std::uintptr_t{0x005792F8});

	inline uint32_t& screen_dump_flag =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B1A0 });

	inline uint32_t& screen_refresh_flag =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B1B4 });

	inline uint32_t*& _mouse_palette =
		*reinterpret_cast<uint32_t**>(std::uintptr_t{ 0x00578AB0 });

	inline uint32_t* _current_palette =
		reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00579350 });

	inline uint16_t& _copy_to_on_page_flag =
		*reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578e60 });

	inline int16_t& _screen_window_y1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DF4 });
	inline int16_t& _screen_window_y2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DF8 });
	inline int16_t& _screen_window_x1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DFC });
	inline int16_t& _screen_window_x2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578E00 });

	// Functions
	void Set_Refresh_Stencil_();
	void Set_Refresh_Full_();
	void Set_Page_Off_();

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
}