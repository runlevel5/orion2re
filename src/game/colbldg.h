#pragma once
#include <cstdint>

namespace COLBLDG {
    inline int16_t* _building_indexes = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AB30});
    inline int16_t& _n_building_indexes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AB9C});
    // Size: short[54]
    inline int16_t* _building_fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F08});

    int __cdecl qsort_building_by_name_(const void* a, const void* b);

    void __cdecl Calculate_Building_Array_();

    void __cdecl Calculate_Military_Array_();

    int16_t __cdecl Star_N_Uncolonized_Worlds_(int16_t star_index);

    int16_t __cdecl N_Prods_In_Queue_(int16_t product_id);

    inline int16_t& _n_military_indexes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC0C});
    inline int16_t* _military_indexes = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055ABA0});
    inline int16_t (&_military_fields)[54] = *reinterpret_cast<int16_t(*)[54]>(std::uintptr_t{0x00571E70});

    void __cdecl Build_Arrays_();

    int16_t __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx);

    void __cdecl Build_Queue_Popup_();

    inline uint8_t& _return_to_build_popup_from_custom_design = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB10});
    inline int16_t& _active_prod = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC10});
    inline int16_t* _current_item = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC18});
    inline int16_t& _last_app = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC28});
    inline int16_t& _last_setting = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC2C});
    inline char (&_g_msg)[800] = *reinterpret_cast<char (*)[800]>(std::uintptr_t{0x00571F98});
    inline int16_t& _colony_bldg_saved_screen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571EDC});
    inline int16_t& _colony_auto_building = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F78});
    inline int16_t* _fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F80});
    inline uint8_t* _l_font_color_array = reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB00});
    inline int16_t (&_queue_fields)[7] = *reinterpret_cast<int16_t(*)[7]>(std::uintptr_t{0x0055AB20});

    inline void*& _buffer = *reinterpret_cast<void**>(std::uintptr_t{0x00595388});

    void __cdecl Clear_Last_App_Description_String_();

    void __cdecl Add_Build_Queue_Fields_();

    void __cdecl Add_Buildings_Fields_();

    void __cdecl Add_Militarys_Fields_();

    void __cdecl Clear_Last_App_Desc_();

    void __cdecl Update_Fields_();
    inline void __cdecl Draw_Build_Queue_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0050DEB0})();
    }

    char* __cdecl Selection_Name_(int16_t id);
}