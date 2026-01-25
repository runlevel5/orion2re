#pragma once

namespace video {
	// Variables
	inline uint32_t& screen_refresh_flag =
		*reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B1B4 });

	inline uint32_t*& _mouse_palette =
		*reinterpret_cast<uint32_t**>(std::uintptr_t{ 0x00578AB0 });

	inline uint32_t* _current_palette =
		reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00579350 });

	inline uint16_t& _copy_to_on_page_flag =
		*reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578e60 });

	// Functions
	inline void __cdecl Fill_(int x1, int y1, int x2, int y2, uint8_t color) {
		using Fn = void(__cdecl*)(int, int, int, int, uint8_t);
		reinterpret_cast<Fn>(std::uintptr_t{ 0x00528CC0 })(x1, y1, x2, y2, color);
	}

	inline void __cdecl Set_Refresh_Stencil() {
		using Fn = void(__cdecl*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527CD0 });
		fn();
	}

	inline void __cdecl Set_Refresh_Stencil_() {
		using Fn = void(__cdecl*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527CD0 });
		fn();
	}

	inline void Set_Refresh_Full_() {
		using Fn = void(*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527C80 });
		fn();
	}
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