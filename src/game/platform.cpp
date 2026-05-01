#include "pch.h"

//#define FULLSCREEN

namespace platform {
    LPDIRECTDRAWSURFACE	l_primary_window_surface = NULL;
    LPDIRECTDRAWSURFACE	l_temp_surface = NULL;

    // Color shifts
    int		rshift;
    int		gshift;
    int		bshift;
    // Scaled window
    int scaleX = 640;
    int scaleY = 480;

    int _Get_Mask_Info(DWORD Bitmask, int* lpShift)
    {
        int Precision, Shift;

        Precision = Shift = 0;
        //count the zeros on right hand side
        while (!(Bitmask & 0x01L))
        {
            Bitmask >>= 1;
            Shift++;
        }

        //count the ones on right hand side
        while (Bitmask & 0x01L)
        {
            Bitmask >>= 1;
            Precision++;
        }
        *lpShift = Shift;
        return Precision;
    }

    void Blit_Surface_To_Screen(LPDIRECTDRAWSURFACE lpSurface)
    {
        HRESULT hr;

        PALETTEENTRY	palette[256];

        RECT rt;
        POINT p = { 0, 0 };
        ClientToScreen(g_main_window, &p);
        rt.left = 0 + p.x;
        rt.top = 0 + p.y;
        rt.right = scaleX + p.x;
        rt.bottom = scaleY + p.y;

        // Lock buffers
        DDSURFACEDESC	srcDesc;
        memset(&srcDesc, 0, sizeof(srcDesc));
        srcDesc.dwSize = sizeof(srcDesc);
        DDSURFACEDESC	dstDesc;
        memset(&dstDesc, 0, sizeof(dstDesc));
        dstDesc.dwSize = sizeof(dstDesc);

        DDPIXELFORMAT DDpf;
        memset(&DDpf, 0, sizeof(DDpf));
        int BytesPerPixel;
        DDpf.dwSize = sizeof(DDpf);
        l_temp_surface->GetPixelFormat(&DDpf);
        BytesPerPixel = DDpf.dwRGBBitCount / 8;

        if (g_palette)
            g_palette->GetEntries(0, 0, 256, &palette[0]);

        lpSurface->Lock(NULL, &srcDesc, DDLOCK_READONLY, NULL);
        l_temp_surface->Lock(NULL, &dstDesc, DDLOCK_WRITEONLY, NULL);

        BYTE* srcData = (BYTE*)srcDesc.lpSurface;
        BYTE* dstData = (BYTE*)dstDesc.lpSurface;
        for (DWORD y1 = 0; y1 < srcDesc.dwHeight; y1++)
        {
            BYTE* srcLine = srcData;
            BYTE* dstLine = dstData;

            for (DWORD x1 = 0; x1 < srcDesc.dwWidth; ++x1)
            {
                tagPALETTEENTRY& pal = palette[*(srcLine++)];

                DWORD	finalColor = 0;
                finalColor |= ((pal.peRed << rshift) & DDpf.dwRBitMask);
                finalColor |= ((pal.peGreen << gshift) & DDpf.dwGBitMask);
                finalColor |= ((pal.peBlue << bshift) & DDpf.dwBBitMask);

                BYTE* byteBuf = (BYTE*)&finalColor;

                for (int i = 0; i < BytesPerPixel; ++i)
                    *(dstLine++) = byteBuf[i];
            }

            srcData += srcDesc.lPitch;
            dstData += dstDesc.lPitch;
        }

        l_temp_surface->Unlock(dstDesc.lpSurface);
        lpSurface->Unlock(srcDesc.lpSurface);

        do
        {
            hr = l_primary_window_surface->Blt(&rt, l_temp_surface, NULL, DDBLT_WAIT, NULL);

            if (hr == DDERR_SURFACELOST)
                hr = Restore_Surfaces();
        } while (hr == DDERR_WASSTILLDRAWING);

        if (lpSurface != g_primary_surface)
        {
            do
            {
                hr = g_primary_surface->Blt(NULL, lpSurface, NULL, DDBLT_WAIT, NULL);

                if (hr == DDERR_SURFACELOST)
                    hr = Restore_Surfaces();
            } while (hr == DDERR_WASSTILLDRAWING);
        }
    }

    // Hooks
    void __cdecl entry() {
        uint32_t version_info = GetVersion();

        g_os_ver = version_info;
        g_win_minor = (version_info >> 8) & 0xFF;
        g_win_major = version_info & 0xFF;
        g_win_ver = (g_win_major << 8) | g_win_minor;

        // Original code overwrites g_os_ver with the build number (high word)
        g_os_ver >>= 16;

        if (!__heap_init()) {
            __amsg_exit(28); // _RT_HEAP
        }

        if (!__mtinit()) {
            __amsg_exit(16); // _RT_THREAD
        }

        __ioinit();
        ___initmbctable();

        g_command_line = GetCommandLineA();
        g_env_strings = ___crtGetEnvironmentStringsA();

        if (g_env_strings == nullptr || g_command_line == nullptr) {
            __exit(-1);
        }

        __setargv();
        __setenvp();
        __cinit();

        // Skip the executable name in the command line
        char* cmd_args = g_command_line;
        if (cmd_args[0] == '"') {
            cmd_args++;
            while (*cmd_args != '\0' && *cmd_args != '"') {
                if (__ismbblead(static_cast<uint8_t>(*cmd_args))) {
                    cmd_args++;
                }
                if (*cmd_args != '\0') {
                    cmd_args++;
                }
            }
            if (*cmd_args == '"') {
                cmd_args++;
            }
        }
        else {
            while (*cmd_args > ' ') {
                cmd_args++;
            }
        }

        // Skip whitespace leading into the first argument
        while (*cmd_args != '\0' && static_cast<uint8_t>(*cmd_args) <= 0x20) {
            cmd_args++;
        }

        STARTUPINFOA si;
        GetStartupInfoA(&si);

        uint32_t show_cmd = SW_SHOWDEFAULT;
        if (si.dwFlags & STARTF_USESHOWWINDOW) {
            show_cmd = si.wShowWindow;
        }

        HINSTANCE h_instance = GetModuleHandleA(nullptr);
        int return_code = WinMain_Proxy(h_instance, nullptr, cmd_args, static_cast<int>(show_cmd));

        __exit(return_code);
    }

    void __cdecl Parse_Ten_Cmd_Parameter(const char* input_str) {
        if (input_str == nullptr) {
            return;
        }

        uint32_t input_len = 0;
        while (input_str[input_len] != '\0') {
            input_len++;
        }

        if (input_len == 0) {
            return;
        }

        char* copy_buffer = static_cast<char*>(_malloc(input_len + 4));
        if (copy_buffer == nullptr) {
            return;
        }

        for (uint32_t i = 0; i <= input_len; ++i) {
            copy_buffer[i] = input_str[i];
        }

        __strlwr(copy_buffer);

        // Search for the tag "/ten" in the lowercased buffer
        char* match_ptr = strstr(copy_buffer, s_slash_ten);
        if (match_ptr != nullptr) {
            uint32_t needle_len = 0;
            while (s_slash_ten[needle_len] != '\0') {
                needle_len++;
            }

            char* target = match_ptr + needle_len;
            target[1] = '\0';

            int parsed_val = _atoi(target);
            g_parsed_value = static_cast<uint8_t>(parsed_val);
        }

        _free(copy_buffer);
    }

    HWND cleanup() {
        Prepare_Exit_();
        if (g_main_window != nullptr) {
            DestroyWindow(g_main_window);
            g_main_window = nullptr;
        }
        return nullptr;
    }

    HWND __cdecl Create_Direct_Draw(int16_t video_mode) {
        MEMORYSTATUS mem_status;
        mem_status.dwLength = sizeof(MEMORYSTATUS);
        GlobalMemoryStatus(&mem_status);

        g_video_mode = video_mode;
        g_available_mem = mem_status.dwAvailPhys;
        DAT_00578e20 = reinterpret_cast<uint32_t>(&DAT_00579350);

        if (DirectDrawCreate(nullptr, &g_direct_draw, nullptr) != DD_OK) {
            return cleanup();
        }

#ifdef FULLSCREEN
        // Set cooperative level (DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)
        if (g_direct_draw->SetCooperativeLevel(g_main_window, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK) {
            return cleanup();
        }
#else
        if (g_direct_draw->SetCooperativeLevel(g_main_window, DDSCL_NORMAL) != DD_OK) {
            return cleanup();
        }
#endif

        switch (video_mode) {
        case 0:
            g_screen_res_x = 320;
            g_screen_res_y = 200;
            break;
        case 2:
            g_screen_res_x = 320;
            g_screen_res_y = 240;
            break;
        case 4:
            g_screen_res_x = 640;
            g_screen_res_y = 480;
            break;
        case 5:
            g_screen_res_x = scaleX;
            g_screen_res_y = scaleY;
        default:
            break;
        }

        video::_screen_pixel_size = g_screen_res_x * g_screen_res_y;
        video::Allocate_Refresh_Lists_(video::_screen_pixel_size, g_screen_res_y);
        DAT_0057931c = 0;

        DDSURFACEDESC surface_desc;
        memset(&surface_desc, 0, sizeof(surface_desc));
        surface_desc.dwSize = sizeof(DDSURFACEDESC);

#ifdef FULLSCREEN
        if (g_direct_draw->SetDisplayMode(g_screen_res_x, g_screen_res_y, 8) != DD_OK) {
            cleanup();
            return cleanup();
        }

        surface_desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        surface_desc.dwBackBufferCount = 1;
        surface_desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
#else
        // Create windowed primary surface
        surface_desc.dwFlags = DDSD_CAPS;
        surface_desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        if (g_direct_draw->CreateSurface(&surface_desc, &l_primary_window_surface, NULL) != DD_OK) {
            return cleanup();
        }

        // Create "fake" primary surface for MOO2 with 8 bit palette
        memset(&surface_desc, 0, sizeof(surface_desc));
        surface_desc.dwSize = sizeof(surface_desc);
        surface_desc.dwWidth = g_screen_res_x;
        surface_desc.dwHeight = g_screen_res_y;
        surface_desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        surface_desc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        surface_desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        surface_desc.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8 | DDPF_RGB;
        surface_desc.ddpfPixelFormat.dwRGBBitCount = 8;
#endif

        if (g_direct_draw->CreateSurface(&surface_desc, &g_primary_surface, nullptr) != DD_OK) {
            return cleanup();
        }

#ifndef FULLSCREEN
        // Set clipper for primary surface
        LPDIRECTDRAWCLIPPER	pc_clipper;
        g_direct_draw->CreateClipper(0, &pc_clipper, NULL);
        pc_clipper->SetHWnd(0, g_main_window);
        l_primary_window_surface->SetClipper(pc_clipper);

        // Create additional surface with primary surface properties
        DDSURFACEDESC	desc2;
        memset(&desc2, 0, sizeof(desc2));
        desc2.dwSize = sizeof(desc2);
        desc2.dwWidth = g_screen_res_x;
        desc2.dwHeight = g_screen_res_y;
        desc2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        desc2.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;

        DDPIXELFORMAT	pFormat;
        memset(&pFormat, 0, sizeof(pFormat));
        pFormat.dwSize = sizeof(pFormat);
        l_primary_window_surface->GetPixelFormat(&pFormat);

        desc2.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        desc2.ddpfPixelFormat.dwFlags = DDPF_RGB;
        desc2.ddpfPixelFormat.dwRGBBitCount = pFormat.dwRGBBitCount;
        desc2.ddpfPixelFormat.dwRBitMask = pFormat.dwRBitMask;
        desc2.ddpfPixelFormat.dwGBitMask = pFormat.dwGBitMask;
        desc2.ddpfPixelFormat.dwBBitMask = pFormat.dwBBitMask;

        g_direct_draw->CreateSurface(&desc2, &l_temp_surface, NULL);
        if (l_temp_surface == NULL)
        {
            return cleanup();
        }

        // Get color information
        _Get_Mask_Info(desc2.ddpfPixelFormat.dwRBitMask, &rshift);
        _Get_Mask_Info(desc2.ddpfPixelFormat.dwGBitMask, &gshift);
        _Get_Mask_Info(desc2.ddpfPixelFormat.dwBBitMask, &bshift);
#endif

        // Initialize desc for offscreen surfaces
        memset(&surface_desc, 0, sizeof(surface_desc));
        surface_desc.dwSize = sizeof(DDSURFACEDESC);
        surface_desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        surface_desc.dwWidth = g_screen_res_x;
        surface_desc.dwHeight = g_screen_res_y;
        surface_desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        surface_desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        surface_desc.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8 | DDPF_RGB;
        surface_desc.ddpfPixelFormat.dwRGBBitCount = 8;

#ifdef FULLSCREEN
        DDSCAPS caps;
        caps.dwCaps = DDSCAPS_BACKBUFFER;
        if (g_primary_surface->GetAttachedSurface(&caps, &g_backbuffer_surface) != DD_OK) {
            return cleanup();
        }
#else
        // Create back buffer surface (fake)
        g_direct_draw->CreateSurface(&surface_desc, &g_backbuffer_surface, NULL);
        if (g_backbuffer_surface == NULL)
        {
            return cleanup();
        }
#endif

        if (g_direct_draw->CreateSurface(&surface_desc, &g_offscreen_surface_1, nullptr) != DD_OK || g_offscreen_surface_1 == nullptr) {
            return cleanup();
        }

        if (g_direct_draw->CreateSurface(&surface_desc, &g_offscreen_surface_2, nullptr) != DD_OK || g_offscreen_surface_2 == nullptr) {
            return cleanup();
        }

        if (g_palette != nullptr) {
            g_palette->Release();
            g_palette = nullptr;
        }

        // Create Palette (DDPCAPS_8BIT | DDPCAPS_ALLOW256)
        if (g_direct_draw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, reinterpret_cast<LPPALETTEENTRY>(&g_palette_entries), &g_palette, nullptr) != DD_OK) {
            return cleanup();
        }

#ifdef FULLSCREEN
        if (g_primary_surface->SetPalette(g_palette) != DD_OK) {
            return cleanup();
        }
#endif

        _need_quit = 0;
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);
        video::Set_Page_Back_();
        graphics::Fill_(0, 0, 639, 479, 0);
        video::Clear_Scan_List_();
        video::Clear_Stencil_List_();
        g_frame_count = 0;
        graphics::Reset_Window_();
        return g_main_window;
    }

    HRESULT	Restore_Surfaces()
    {
        HRESULT hr = S_OK;

        if (g_primary_surface != NULL)
            g_primary_surface->Restore();

        if (g_backbuffer_surface != NULL)
            g_backbuffer_surface->Restore();

        if (g_offscreen_surface_1 != NULL)
            g_offscreen_surface_1->Restore();

        if (g_offscreen_surface_2 != NULL)
            g_offscreen_surface_2->Restore();

        if (platform::g_cursor_surface_1 != NULL)
            platform::g_cursor_surface_1->Restore();

        if (g_cursor_surface_2 != NULL)
            hr = g_cursor_surface_2->Restore();

        if (l_primary_window_surface != NULL)
            l_primary_window_surface->Restore();

        if (l_temp_surface != NULL)
            l_temp_surface->Restore();

        g_frame_count = 0;
        if (video::screen_refresh_flag == 3)
        {
            video::Set_Refresh_Stencil_();
        }

        return hr;
    }

    void __cdecl Prepare_Exit_() {
        sound::Destroy_Sound_();
        netcode::Net_Uninit_();
        g_should_exit = 1;
    }

    void Cleanup_Surfaces()
    {
        if (g_direct_draw != NULL)
        {
            if (g_primary_surface != NULL)
            {
                g_primary_surface->Release();
                g_primary_surface = NULL;
            }

            if (g_offscreen_surface_1 != NULL)
            {
                g_offscreen_surface_1->Release();
                g_offscreen_surface_1 = NULL;
            }

            if (g_offscreen_surface_2 != NULL)
            {
                g_offscreen_surface_2->Release();
                g_offscreen_surface_2 = NULL;
            }

            if (g_palette != NULL)
            {
                g_palette->Release();
                g_palette = NULL;
            }

            if (platform::g_cursor_surface_1 != NULL)
            {
                platform::g_cursor_surface_1->Release();
                platform::g_cursor_surface_1 = NULL;
            }

            if (g_cursor_surface_2 != NULL)
            {
                g_cursor_surface_2->Release();
                g_cursor_surface_2 = NULL;
            }

            if (l_primary_window_surface != NULL)
            {
                l_primary_window_surface->Release();
                l_primary_window_surface = NULL;
            }

            if (l_temp_surface != NULL)
            {
                l_temp_surface->Release();
                l_temp_surface = NULL;
            }

            g_direct_draw->Release();
            g_direct_draw = NULL;
        }
    }

    void __cdecl Flip_Surface()
    {
#ifdef FULLSCREEN
        HRESULT hr;

        do
        {
            hr = g_primary_surface->Flip(NULL, 0);
            if (hr == DDERR_SURFACELOST)
                hr = Restore_Surfaces();
        } while (hr == DDERR_WASSTILLDRAWING);
#else
        Blit_Surface_To_Screen(g_backbuffer_surface);
#endif
        g_frame_count++;
    }

    LONG __cdecl Lock_Offscreen_Surface_() {
        platform::DDSURFACEDESC_00578e70.dwSize = sizeof(DDSURFACEDESC);
        platform::DDSURFACEDESC_00578e70.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

        platform::g_offscreen_surface_1->GetSurfaceDesc(&platform::DDSURFACEDESC_00578e70);

        HRESULT result = platform::g_offscreen_surface_2->Lock(
            nullptr,
            &platform::DDSURFACEDESC_00578e70,
            0,
            nullptr);

        if (result == DD_OK) {
            platform::_offscreen_surface = (uint8_t*)platform::_current_locked_page;
            return platform::_surface_lock_flag;
        }

        if (result == E_INVALIDARG ||
            result == E_OUTOFMEMORY ||
            result == DDERR_SURFACELOST ||
            result == DDERR_SURFACEBUSY ||
            result == DDERR_CANTLOCKSURFACE ||
            result == DDERR_WASSTILLDRAWING) {
            return (LONG)result;
        }

        return 0;
    }

    void __cdecl Unlock_Offscreen_Surface_() {
        g_offscreen_surface_2->Unlock(nullptr);
    }

    void __cdecl Lock_Cursor_Surface_() {
        HRESULT lock_result;

        platform::_cursor_surface_desc.dwSize = sizeof(DDSURFACEDESC);
        platform::_cursor_surface_desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
        platform::g_cursor_surface_1->GetSurfaceDesc(&platform::_cursor_surface_desc);

        lock_result = platform::g_cursor_surface_1->Lock(
            nullptr,
            &platform::_cursor_surface_desc,
            DDLOCK_SURFACEMEMORYPTR,
            nullptr);

        if (lock_result == DD_OK) {
            platform::_cursor_surface = platform::_cursor_surface_desc.lpSurface;
        }
    }

    void __cdecl Unlock_Cursor_Surface_() {
        platform::g_cursor_surface_1->Unlock(nullptr);
    }

    uint32_t Lock_Appropriate_Render_Buffer()
    {
        HRESULT hr = S_OK;
        DDSURFACEDESC	desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
        desc.lPitch = 0;

        switch (g_render_buffer)
        {
        case 1:
            g_offscreen_surface_1->GetSurfaceDesc(&desc);
            hr = g_offscreen_surface_1->Lock(NULL, &desc, DDLOCK_WAIT, NULL);
            break;
        case 2:
            g_offscreen_surface_2->GetSurfaceDesc(&desc);
            hr = g_offscreen_surface_2->Lock(NULL, &desc, DDLOCK_WAIT, NULL);
            break;
        case 0:
            g_primary_surface->GetSurfaceDesc(&desc);
            hr = g_primary_surface->Lock(NULL, &desc, DDLOCK_WAIT, NULL);
            break;
        default:
            hr = DDERR_INVALIDMODE;
            break;
        }
        if (SUCCEEDED(hr))
        {
            g_surface_mem = desc.lpSurface;
            return desc.lPitch;
        }
        switch (hr)
        {
        case DDERR_SURFACELOST:
            exitm::Exit_With_Message("lock Surface Lost");
            return DDERR_SURFACELOST;

        case DDERR_SURFACEBUSY:
            exitm::Exit_With_Message("lock Invalid Rect");
            return DDERR_SURFACEBUSY;
        case DDERR_INVALIDOBJECT:
            exitm::Exit_With_Message("lock Invalid Object");
            return DDERR_INVALIDOBJECT;
        case DDERR_INVALIDPARAMS:
            exitm::Exit_With_Message("lock Invalid Params");
            return DDERR_INVALIDPARAMS;
        case DDERR_OUTOFMEMORY:
            exitm::Exit_With_Message("lock Not Locked");
            return DDERR_OUTOFMEMORY;
        case DDERR_WASSTILLDRAWING:
        default:
            exitm::Exit_With_Message("lock Generic");
            return DDERR_WASSTILLDRAWING;
        }
        return 0;
    }


    HRESULT __cdecl	Unlock_Appropriate_Render_Buffer()
    {
        HRESULT hr = S_OK;

        switch (g_render_buffer)
        {
        case 0:
            hr = g_primary_surface->Unlock(NULL);

#ifndef FULLSCREEN
            Blit_Surface_To_Screen(g_primary_surface);
#endif
            break;
        case 1:
            hr = g_offscreen_surface_1->Unlock(NULL);
            break;
        case 2:
            hr = g_offscreen_surface_2->Unlock(NULL);
            break;
        }

        if (SUCCEEDED(hr))
            return hr;

        switch (hr)
        {
        case DDERR_SURFACELOST:
            exitm::Exit_With_Message("Unlock Surface Lost");
            break;
        case DDERR_NOTLOCKED:
            exitm::Exit_With_Message("Unlock Not Locked");
            break;
        case DDERR_INVALIDRECT:
            exitm::Exit_With_Message("Unlock Invalid Rect");
            break;
        case DDERR_INVALIDOBJECT:
            exitm::Exit_With_Message("Unlock Invalid Object");
            break;
        case DDERR_INVALIDPARAMS:
            exitm::Exit_With_Message("Unlock Invalid Params");
            break;
        case DDERR_GENERIC:
            exitm::Exit_With_Message("Unlock Generic");
            break;
        }

        return hr;
    }

    uint32_t Lock_Primary_Surface()
    {
        HRESULT hr = S_OK;
        DDSURFACEDESC	desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

        g_primary_surface->GetSurfaceDesc(&desc);

        hr = g_primary_surface->Lock(NULL, &desc, DDLOCK_SURFACEMEMORYPTR, NULL);

        if (SUCCEEDED(hr)) {
            video::_video_memory = (uint32_t*)desc.lpSurface;
            return desc.lPitch;
        }

        if (hr != DDERR_SURFACELOST && hr != DDERR_WASSTILLDRAWING)
            hr = 0;
        if (hr != DDERR_SURFACEBUSY && hr != DDERR_INVALIDOBJECT)
            hr = 0;
        if (hr != DDERR_INVALIDPARAMS && hr != DDERR_OUTOFMEMORY)
            hr = 0;

        return hr;
    }

    void Unlock_Primary_Surface()
    {
        g_primary_surface->Unlock(NULL);

#ifndef FULLSCREEN
        Blit_Surface_To_Screen(g_primary_surface);
#endif
    }

    void	Blit_To_Back_Buffer()
    {
        RECT	rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = g_screen_res_x;
        rect.bottom = g_screen_res_y;

        while (g_backbuffer_surface->BltFast(0, 0, g_offscreen_surface_1, &rect, DDBLTFAST_WAIT) != DD_OK);
    }

    void	Draw_Cursor_XY(int16_t x, int16_t y)
    {
        if (DAT_578AC4 > 0)
        {
            if (DAT_578AC4 != DAT_578B3C)
            {
                Update_Cursor_Image_();
                DAT_578B3C = DAT_578AC4;
            }

            RECT rect;
            rect.left = 0;
            rect.top = 0;

            if (x + 24 >= g_screen_res_x - 1)
                rect.right = g_screen_res_x - x - 1;
            else
                rect.right = 24;

            if (y + 24 >= g_screen_res_y - 1)
                rect.bottom = g_screen_res_y - y - 1;
            else
                rect.bottom = 24;

            DDCOLORKEY	key;
            key.dwColorSpaceHighValue = 0;
            key.dwColorSpaceLowValue = 0;
            platform::g_cursor_surface_1->SetColorKey(8, &key);

            g_primary_surface->BltFast(x, y, platform::g_cursor_surface_1, &rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

#ifndef FULLSCREEN
            Blit_Surface_To_Screen(g_primary_surface);
#endif
        }
    }

    void __cdecl Save_Mouse_(int16_t mouse_x, int16_t mouse_y) {
        RECT save_rect;
        const LONG max_x = g_screen_res_x - 1;
        const LONG max_y = g_screen_res_y - 1;

        save_rect.left = static_cast<LONG>(mouse_x);
        save_rect.top = static_cast<LONG>(mouse_y);

        save_rect.right = max_x;
        if (save_rect.left + 0x18 < max_x) {
            save_rect.right = save_rect.left + 0x18;
        }

        save_rect.bottom = max_y;
        if (save_rect.top + 0x18 < max_y) {
            save_rect.bottom = save_rect.top + 0x18;
        }

        g_cursor_surface_2->BltFast(
            0,
            0,
            g_primary_surface,
            &save_rect,
            DDBLTFAST_WAIT
        );

        g_cached_mouse_x = mouse_x;
        g_cached_mouse_y = mouse_y;
    }

    void __cdecl Save_Mouse_Offscreen_(int16_t mouse_x, int16_t mouse_y) {
        RECT saved_rect;

        saved_rect.left = static_cast<LONG>(mouse_x);
        saved_rect.top = static_cast<LONG>(mouse_y);

        saved_rect.right = g_screen_res_x - 1;
        if (saved_rect.left + 0x18 < g_screen_res_x - 1) {
            saved_rect.right = saved_rect.left + 0x18;
        }

        saved_rect.bottom = g_screen_res_y - 1;
        if (saved_rect.top + 0x18 < g_screen_res_y - 1) {
            saved_rect.bottom = saved_rect.top + 0x18;
        }

        g_cursor_surface_2->BltFast(
            0,
            0,
            g_offscreen_surface_1,
            &saved_rect,
            DDBLTFAST_WAIT
        );

        g_cached_mouse_x = mouse_x;
        g_cached_mouse_y = mouse_y;
    }

    void __cdecl Draw_Cursor_XY_Offscreen_(int16_t mouse_x, int16_t mouse_y) {
        RECT source_rect;
        DDCOLORKEY color_key;
        int32_t x;
        int32_t y;

        if (video::_current_mouse_shape <= 0) {
            return;
        }

        if (static_cast<int32_t>(video::_current_mouse_shape) != platform::_offscreen_mouse_shape) {
            platform::Update_Cursor_Image_();
            platform::_offscreen_mouse_shape = static_cast<int32_t>(video::_current_mouse_shape);
        }

        source_rect.left = 0;
        source_rect.top = 0;

        x = static_cast<int32_t>(mouse_x);
        if (x + 0x18 < g_screen_res_x - 1) {
            source_rect.right = 0x18;
        } else {
            source_rect.right = g_screen_res_x - x - 1;
        }

        y = static_cast<int32_t>(mouse_y);
        if (y + 0x18 < g_screen_res_y - 1) {
            source_rect.bottom = 0x18;
        } else {
            source_rect.bottom = g_screen_res_y - y - 1;
        }

        color_key.dwColorSpaceLowValue = 0;
        color_key.dwColorSpaceHighValue = 0;

        platform::g_cursor_surface_1->SetColorKey(8, &color_key);
        g_offscreen_surface_1->BltFast(
            static_cast<DWORD>(x),
            static_cast<DWORD>(y),
            platform::g_cursor_surface_1,
            &source_rect,
            DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    }

    void __cdecl Restore_Mouse_Off_Page_() {
        RECT src_rect;
        const int32_t mouse_x = static_cast<int32_t>(g_cached_mouse_x);
        const int32_t mouse_y = static_cast<int32_t>(g_cached_mouse_y);

        src_rect.left = 0;
        src_rect.top = 0;

        if (mouse_x + 0x18 < g_screen_res_x - 1) {
            src_rect.right = 0x18;
        } else {
            src_rect.right = g_screen_res_x - mouse_x - 1;
        }

        if (mouse_y + 0x18 < g_screen_res_y - 1) {
            src_rect.bottom = 0x18;
        } else {
            src_rect.bottom = g_screen_res_y - mouse_y - 1;
        }

        g_offscreen_surface_1->BltFast(
            static_cast<DWORD>(mouse_x),
            static_cast<DWORD>(mouse_y),
            platform::g_cursor_surface_2,
            &src_rect,
            DDBLTFAST_WAIT
        );
    }

    void Force_Cursor_Pos(int16_t x, int16_t y)
    {
        g_mouse_x = x;
        g_mouse_y = y;

#ifndef FULSCREEN
        // Quick hack to ignore window size
        POINT p = { 0, 0 };
        ClientToScreen(g_main_window, &p);

        x += (int16_t)p.x;
        y += (int16_t)p.y;
#endif

        SetCursorPos(x, y);
    }

    void Create_Cursor_Surfaces()
    {
        DDSURFACEDESC	desc2;
        memset(&desc2, 0, sizeof(desc2));
        desc2.dwSize = sizeof(desc2);
        desc2.dwWidth = 24;
        desc2.dwHeight = 24;
        desc2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        desc2.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        desc2.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        desc2.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8 | DDPF_RGB;
        desc2.ddpfPixelFormat.dwRGBBitCount = 8;

        g_direct_draw->CreateSurface(&desc2, &platform::g_cursor_surface_1, NULL);
        g_direct_draw->CreateSurface(&desc2, &g_cursor_surface_2, NULL);
    }

    void Draw_Auto_Cursor()
    {
        RECT rect;
        rect.left = 0;
        rect.top = 0;

        if (g_cached_mouse_x + 24 >= g_screen_res_x - 1)
            rect.right = g_screen_res_x - g_cached_mouse_x - 1;
        else
            rect.right = 24;

        if (g_cached_mouse_y + 24 >= g_screen_res_y - 1)
            rect.bottom = g_screen_res_y - g_cached_mouse_y - 1;
        else
            rect.bottom = 24;

        g_primary_surface->BltFast(g_cached_mouse_x, g_cached_mouse_y, g_cursor_surface_2, &rect, DDBLTFAST_WAIT);

#ifndef FULLSCREEN
        Blit_Surface_To_Screen(g_primary_surface);
#endif
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_ACTIVATEAPP:
            g_window_focus_state = (int32_t)wParam;
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KEYDOWN: {
            uint16_t key_char = 0;
            uint8_t keyboardState[256];
            uint8_t scan_code;

            if (GetKeyboardState(keyboardState)) {
                scan_code = (lParam >> 16) & 0xFF;
                ToAscii((UINT)wParam, scan_code, keyboardState, &key_char, 0);
            }
            else {
                scan_code = keyboardState[0];
            }

            if (key_char == 0) {
                uint32_t iVar7 = 0;
                switch (wParam) {
                case 0x08: iVar7 = 0x0E08; break;
                case 0x09: iVar7 = 0x0F09; break;
                case 0x0D: iVar7 = 0x1C0D; break;
                case 0x21: iVar7 = 0x49E0; break;
                case 0x22: iVar7 = 0x51E0; break;
                case 0x23: iVar7 = 0x4FE0; break;
                case 0x24: iVar7 = 0x47E0; break;
                case 0x25: iVar7 = 0x4BE0; break;
                case 0x26: iVar7 = 0x48E0; break;
                case 0x27: iVar7 = 0x4DE0; break;
                case 0x28: iVar7 = 0x50E0; break;
                case 0x2D: iVar7 = 0x52E0; break;
                case 0x2E: iVar7 = 0x10000; break;
                case 0x6B: iVar7 = 0x0D2B; break;
                case 0x6D: iVar7 = 0x0C2D; break;
                case 0x70: iVar7 = 0x3B00; break;
                case 0x71: iVar7 = 0x3C00; break;
                case 0x72: iVar7 = 0x3D00; break;
                case 0x73: iVar7 = 0x3E00; break;
                case 0x74: iVar7 = 0x3F00; break;
                case 0x75: iVar7 = 0x4000; break;
                case 0x76: iVar7 = 0x4100; break;
                case 0x77: iVar7 = 0x4200; break;
                case 0x78: iVar7 = 0x4300; break;
                case 0x79: iVar7 = 0x4400; break;
                case 0x7A: iVar7 = 0xFFFF8500; break;
                case 0x7B: iVar7 = 0xFFFF8600; break;
                }
                if (iVar7 != 0) {
                    key::_key_pressed = true;
                    key::_last_key_pressed[key::_key_write] = (int32_t)iVar7;
                    key::_key_write = (key::_key_write + 1) % 10;
                    g_last_scan_code = (lParam & 0xFF0000) >> 16;
                }
            }
            else {
                key::_key_pressed = true;
                key_char += (scan_code << 8);
                key::_last_key_pressed[key::_key_write] = (int32_t)key_char;
                key::_key_write = (key::_key_write + 1) % 10;
                g_last_scan_code = (lParam & 0xFF0000) >> 16;
            }
            break;
        }

        case WM_SETCURSOR:
            SetCursor(nullptr);
            return 1;

        case WM_SYSCOMMAND:
            if (wParam == 0xF100) return 0;
            break;

        case WM_SYSKEYDOWN: {
            if ((wParam >= 0x41 && wParam <= 0x5A) || (wParam >= 0x70 && wParam <= 0x79)) {
                uint16_t alt_code = 0;
                switch (wParam) {
                case 0x41: alt_code = 0x1E00; break;
                case 0x42: alt_code = 0x3000; break;
                case 0x43: alt_code = 0x2E00; break;
                case 0x44: alt_code = 0x2000; break;
                case 0x45: alt_code = 0x1200; break;
                case 0x46: alt_code = 0x2100; break;
                case 0x47: alt_code = 0x2200; break;
                case 0x48: alt_code = 0x2300; break;
                case 0x49: alt_code = 0x1700; break;
                case 0x4A: alt_code = 0x2400; break;
                case 0x4B: alt_code = 0x2500; break;
                case 0x4C: alt_code = 0x2600; break;
                case 0x4D: alt_code = 0x3200; break;
                case 0x4E: alt_code = 0x3100; break;
                case 0x4F: alt_code = 0x1800; break;
                case 0x50: alt_code = 0x1900; break;
                case 0x51: alt_code = 0x1000; break;
                case 0x52: alt_code = 0x1300; break;
                case 0x53: alt_code = 0x1F00; break;
                case 0x54: alt_code = 0x1400; break;
                case 0x55: alt_code = 0x1600; break;
                case 0x56: alt_code = 0x2F00; break;
                case 0x57: alt_code = 0x1100; break;
                case 0x58: alt_code = 0x2D00; break;
                case 0x59: alt_code = 0x1500; break;
                case 0x5A: alt_code = 0x2C00; break;
                case 0x70: alt_code = 0x6800; break;
                case 0x71: alt_code = 0x6900; break;
                case 0x72: alt_code = 0x6A00; break;
                case 0x73: alt_code = 0x6B00; break;
                case 0x74: alt_code = 0x6C00; break;
                case 0x75: alt_code = 0x6D00; break;
                case 0x76: alt_code = 0x6E00; break;
                case 0x77: alt_code = 0x6F00; break;
                case 0x78: alt_code = 0x7000; break;
                case 0x79: alt_code = 0x7100; break;
                }
                key::_key_pressed = true;
                key::_last_key_pressed[key::_key_write] = alt_code;
                key::_key_write = (key::_key_write + 1) % 10;
            }
            if (wParam == VK_MENU) return 0;
            break;
        }

        case WM_MOUSEMOVE:
            mouse::User_Mouse_Handler_((int16_t)(lParam & 0xFFFF), (int16_t)((lParam >> 16) & 0xFFFF), (uint16_t)mouse::_mouse_buttons);
            return 1;

        case WM_MENUSELECT:
            return 0;

        case 0x401: // WM_USER + 1
            video::Toggle_Pages_();
            if (_updating_cursor_img == 1) {
                Update_Cursor_Image_();
                _updating_cursor_img = 0;
            }
            return 1;

        case 0x402: // WM_USER + 2
            _updating_cursor_img = 1;
            return 1;

        case WM_LBUTTONDOWN:
            mouse::User_Mouse_Handler_((int16_t)(lParam & 0xFFFF), (int16_t)((lParam >> 16) & 0xFFFF), 1);
            return 1;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            mouse::User_Mouse_Handler_((int16_t)(lParam & 0xFFFF), (int16_t)((lParam >> 16) & 0xFFFF), 0);
            return 1;

        case WM_RBUTTONDOWN:
            mouse::User_Mouse_Handler_((int16_t)(lParam & 0xFFFF), (int16_t)((lParam >> 16) & 0xFFFF), 2);
            return 1;
        }

        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }

    void __cdecl Toggle_Pages_Stub_() {
        g_pending_toggle_pages = true;

        // Send custom toggle message (0x401 = WM_USER + 1)
        SendMessageA(g_main_window, 0x401, 0, 0);

        // Busy wait until the message handler resets the pending flag.
        // Note: The original assembly (0x00527ebc - 0x00527ec3) contains a bug where
        // the variable is not reloaded from memory inside the loop, effectively
        // creating an infinite spin if the flag remains true after SendMessageA returns.
        while (g_pending_toggle_pages) {
            // Spin
        }
    }

    int __stdcall WinMain_Proxy(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd) {
        Parse_Ten_Cmd_Parameter(lpCmdLine);

        WNDCLASSA window_class;
        window_class.cbClsExtra = 0;
        window_class.cbWndExtra = 0;
        window_class.hInstance = hInstance;
        window_class.style = CS_HREDRAW | CS_VREDRAW; // 3
        // Suggested name: main_window_proc
        window_class.lpfnWndProc = &WndProc;
        window_class.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(101)); // 0x65
        window_class.hCursor = LoadCursorA(NULL, IDC_ARROW);              // 0x7F00
        window_class.hbrBackground = nullptr;
        window_class.lpszMenuName = "Microprose";
        window_class.lpszClassName = "Microprose";

        RegisterClassA(&window_class);

        HWND existing_window = FindWindowA("Microprose", "Microprose");
        if (existing_window != nullptr) {
            MessageBoxA(nullptr, "MOO II is already running.", "Microprose", MB_OK | MB_ICONHAND); // 0x10
            __exit(1);
        }

#ifdef FULSCREEN
        int nWidth = GetSystemMetrics(SM_CXSCREEN);
        int nHeight = GetSystemMetrics(SM_CYSCREEN);
        DWORD flags = flags = WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_EX_TOPMOST;
#else
        int nWidth = scaleX + 20;//800;//645;
        int nHeight = scaleY + 40;//600;//500;
        //added| WS_OVERLAPPEDWINDOW | WS_SIZEBOX
        DWORD flags = WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_SIZEBOX;
#endif

        HWND window_handle = CreateWindowExA(
            0,
            "Microprose",
            "Microprose",
            flags,
            0, 0, nWidth, nHeight,
            nullptr, nullptr, hInstance, nullptr
        );

        if (window_handle == nullptr) {
            return 0;
        }

        g_main_window = window_handle;
        UpdateWindow(window_handle);
        SetCursor(nullptr);

        Create_Direct_Draw(4);

        bool has_lost_focus = false;
        g_worker_thread_handle = reinterpret_cast<HANDLE>(begin_thread(MOX2::main2_, 0xFFFF, nullptr));

        do {
            if (!GetMessageA(&g_main_msg, nullptr, 0, 0)) {
                return static_cast<int>(g_main_msg.wParam);
            }

            TranslateMessage(&g_main_msg);
            DispatchMessageA(&g_main_msg);

            // Track focus state transitions to restore surfaces when regained
            if (g_window_focus_state == 0) {
                has_lost_focus = true;
            }

            if (has_lost_focus && g_window_focus_state == 1) {
                has_lost_focus = false;
                Restore_Surfaces();
            }

        } while (g_should_exit != 1);

        Cleanup_Surfaces();
        DestroyWindow(g_main_window);

        return 0;
    }

    void __cdecl Update_Cursor_Image_() {
        if (video::_current_mouse_shape > 0) {
            platform::Lock_Cursor_Surface_();

            uint8_t* const src =
                &video::_mouse_palette[(static_cast<int32_t>(video::_current_mouse_shape) - 1) * 0x240];

            for (int32_t row = 0; row < 0x18; ++row) {
                memcpy(&((uint8_t*)platform::_cursor_surface)[row * 0x18], &src[row * 0x18], 0x18);
            }

            platform::Unlock_Cursor_Surface_();
        }

        platform::Force_Cursor_Pos(platform::g_mouse_x, platform::g_mouse_y);
    }

    void __cdecl Copy_Dwords_(uint32_t* destination, const uint32_t* source, uint32_t dword_count) {
        memcpy(destination, source, dword_count * sizeof(uint32_t));
    }
}
