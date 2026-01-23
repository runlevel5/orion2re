#include "deps.h"

namespace video {
	void Set_Page_Off_() {
		platform::g_render_buffer = 1;
		_copy_to_on_page_flag = 0;
	}
}