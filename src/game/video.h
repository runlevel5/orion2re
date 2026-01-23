#pragma once

namespace video {
	// Variables
	inline uint32_t& screen_refresh_flag =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B1B4 });

	inline uint32_t*& _mouse_palette =
		*reinterpret_cast<uint32_t**>(std::uintptr_t{ 0x00578AB0 });

	inline uint32_t* _current_palette =
		reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00579350 });

	// Functions
	inline void __cdecl Set_Refresh_Stencil() {
		using Fn = void(__cdecl*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527CD0 });
		fn();
	}

	inline void Set_Refresh_Full_() {
		using Fn = void(*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527C80 });
		fn();
	}
	inline void Set_Page_Off_() {
		using Fn = void(*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528680 });
		fn();
	}
}