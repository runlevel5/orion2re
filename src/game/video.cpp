#include "deps.h"

namespace video {
	void Set_Page_Off_() {
		platform::g_render_buffer = 1;
		_copy_to_on_page_flag = 0;
	}

	void Set_Refresh_Stencil_() {
		screen_refresh_flag = 3;
		screen_dump_flag = 0;
	}

	void Set_Refresh_Full_() {
		screen_refresh_flag = 0;
		screen_dump_flag = 0;
	}
}