#pragma once

namespace COMBINIT {
    inline int8_t* s__00579491 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579491});
    inline int8_t* s__00579492 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579492});
    inline int8_t* s__00579493 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579493});
    inline int8_t* s__00579591 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579591});
    inline int8_t* s__00579592 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579592});
    inline int8_t* s__00579593 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579593});
    inline int8_t* s__00579611 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579611});
    inline int8_t* s__00579612 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579612});
    inline int8_t* s__00579613 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579613});

    inline char* s_combat_lbx_00551244 = reinterpret_cast<char*>(std::uintptr_t{0x00551244});
    inline char* s_cmbtmisl_lbx_00551234 = reinterpret_cast<char*>(std::uintptr_t{0x00551234});
    inline char* s_cmbtsfx_lbx_00551228 = reinterpret_cast<char*>(std::uintptr_t{0x00551228});
    inline char* s_sphersfx_lbx_00551218 = reinterpret_cast<char*>(std::uintptr_t{0x00551218});

    inline void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040f660});
        fn(ship_idx);
    }

    inline void __cdecl Play_Dimensional_Portal_Flic_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bb0c0});
        fn();
    }
    inline void __cdecl Clear_Structure_(void* ptr, uint32_t size) {
        using Fn = void(__cdecl*)(void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052d0b7});
        fn(ptr, size);
    }
    inline bool __cdecl Point_Is_In_Some_Nebula_(int16_t x, int16_t y) {
        using Fn = bool(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412b10});
        return fn(x, y);
    }
    inline void __cdecl Load_Combat_Ship_(int16_t ship_idx, int16_t count) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040b460});
        fn(ship_idx, count);
    }
    inline int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t count, int16_t special_id) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040d3c0});
        return fn(count, special_id);
    }
    inline bool __cdecl Player_Has_Artemis_Net_(s_player* player, s_star_data* star) {
        using Fn = bool(__cdecl*)(void*, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f1560});
        return fn(player, star);
    }
    inline bool __cdecl Apply_Combat_Artemis_Net_(int16_t unk) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e88b0});
        return fn(unk);
    }
    inline void __cdecl Refresh_Palette_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b660});
        fn();
    }
    inline void __cdecl Message_Box_(char* buf) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796b0});
        fn(buf);
    }
    inline int8_t __cdecl Colony_Has_Defenses_(int16_t idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004094d0});
        return fn(idx);
    }
    inline void __cdecl Get_Auto_Function_(void** func) {
        using Fn = void(__cdecl*)(void**);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00483c90});
        fn(func);
    }
    inline void __cdecl Reset_Window_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528c90});
        fn();
    }
    inline void __cdecl Play_Combat_Music_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004252a0});
        fn();
    }
    inline void __cdecl Clear_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00521080});
        fn();
    }
    inline void __cdecl Deactivate_Auto_Function_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051ff00});
        fn();
    }
    inline void __cdecl Set_Mouse_List_(int16_t* list, int16_t count) {
        using Fn = void(__cdecl*)(int16_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005274d0});
        fn(list, count);
    }
    inline void __cdecl Set_Reload_Fleet_Box_Palette_Flag_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004cd0d0});
        fn();
    }
    inline void __cdecl Fast_Fade_Out_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba690});
        fn();
    }
    inline void __cdecl Load_Palette_(int16_t a, int16_t b, int16_t c) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00525400});
        fn(a, b, c);
    }
    inline void __cdecl Set_Fade_Percent_(int16_t pct) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba660});
        fn(pct);
    }
    inline uint32_t __cdecl Get_Bitmap_Size_(int16_t w, int16_t h) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052ef80});
        return fn(w, h);
    }
    inline int16_t __cdecl Best_Point_Defense_Weapon_(int16_t plr) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481510});
        return fn(plr);
    }
    inline int16_t __cdecl Best_Bomb_Weapon_(int16_t plr) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004818f0});
        return fn(plr);
    }
    inline void* __cdecl Allocate_Glass_Remap_Next_Block_(void* seg) {
        using Fn = void*(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b6f0});
        return fn(seg);
    }
    inline void __cdecl Save_Glass_Remap_Block_(void* block) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b710});
        fn(block);
    }
    inline void __cdecl Load_Antaran_Defense_Fleet_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040fcf0});
        fn();
    }
    inline void __cdecl Load_Antaran_Planet_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c9b0});
        fn();
    }
    inline void __cdecl Load_Colony_Defense_(int16_t col) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040cb30});
        fn(col);
    }
    inline uint8_t __cdecl Does_Combat_Planet_Have_Defenses_() {
        using Fn = uint8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ae1e0});
        return fn();
    }
    inline void __cdecl Load_Combat_Ship_Palette_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c030});
        fn(plr);
    }
    inline void __cdecl Create_Ship_Darken_Map_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d6380});
        fn();
    }
    inline void __cdecl Load_Combat_Beam_Stuff_(int16_t flag) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420b70});
        fn(flag);
    }
    inline void __cdecl Set_Glass_Remap_Color_(int16_t idx, int16_t r, int16_t g, int16_t b, int16_t c) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b8f0});
        fn(idx, r, g, b, c);
    }
    inline void __cdecl Set_Palette_Gradient_(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f, int16_t g, int16_t h) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b330});
        fn(a, b, c, d, e, f, g, h);
    }
    inline void __cdecl Set_Palette_Changes_(int16_t a, int16_t b) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b280});
        fn(a, b);
    }
    inline void __cdecl Update_Glass_Remap_Colors_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052bc90});
        fn();
    }
    inline void __cdecl Draw_Palette_(void* pal) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00524db0});
        fn(pal);
    }
    inline void __cdecl Set_Combat_Ordnance_Values_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e5ba0});
        fn();
    }
    inline void __cdecl Load_Individual_Ship_Pictures_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c230});
        fn(plr);
    }
    inline void __cdecl Restore_Glass_Remap_Block_(void* block) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b740});
        fn(block);
    }
    inline void __cdecl Init_Special_Devices_(int16_t count) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040f500});
        fn(count);
    }
    inline void __cdecl Deploy_Ships_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040af90});
        fn();
    }
    inline void __cdecl Add_Tactical_Combat_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046a8a0});
        fn();
    }
    inline int16_t __cdecl Check_For_Winner_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004695d0});
        return fn();
    }
    inline void __cdecl Assign_Auto_Function_(void* func, int16_t a) {
        using Fn = void(__cdecl*)(void*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051fe90});
        fn(func, a);
    }

    inline void __cdecl Mox_Send_Message_(int16_t a, int16_t b, void* data, int16_t c) {
        using Fn = void(__cdecl*)(int16_t, int16_t, void*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00515560});
        fn(a, b, data, c);
    }
    inline void __cdecl Wait_Until_Net_Opponent_Finished_(int16_t a) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004706e0});
        fn(a);
    }
    inline int16_t __cdecl Calc_Current_Speed_(int16_t count) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004693d0});
        return fn(count);
    }
    inline int16_t __cdecl Offensive_Combat_Bonus_(int16_t count) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e6410});
        return fn(count);
    }
    inline int16_t __cdecl Defensive_Combat_Bonus_(int16_t count, int16_t flag) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e5920});
        return fn(count, flag);
    }
    inline void __cdecl Do_Combat_Turn_(int16_t a, int16_t b) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00466d10});
        fn(a, b);
    }
    inline void __cdecl Plasma_Web_Damage_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c690});
        fn(plr);
    }
    inline void __cdecl Draw_Super_Fast_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004cd9e0});
        fn();
    }
    inline void __cdecl Increment_Bhg_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c570});
        fn(plr);
    }
    inline void __cdecl End_Of_Turn_Bookeeping_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c620});
        fn();
    }
    inline void __cdecl End_Of_Combat_(int16_t colony_idx, int16_t pop, int16_t winner) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040d4a0});
        fn(colony_idx, pop, winner);
    }
    inline void __cdecl Russ_Event_Monster_Update_(int16_t a, int16_t b, int16_t c) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005187c0});
        fn(a, b, c);
    }
    inline void __cdecl Deactivate_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00520060});
        fn();
    }
    inline void __cdecl Put_Auto_Function_(void** func) {
        using Fn = void(__cdecl*)(void**);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00483ca0});
        fn(func);
    }
    inline void __cdecl Fadeout_Sound_(int16_t a, int16_t b) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052e850});
        fn(a, b);
    }
    inline void __cdecl Play_Background_Music_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004251d0});
        fn();
    }
    inline void __cdecl Update_Ship_Icons_In_Between_Turns_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004cd550});
        fn();
    }

    inline void __cdecl Fade_Into_Screen_(void* func, uint8_t* flag, int16_t a) {
        using Fn = void(__cdecl*)(void*, uint8_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049e630});
        fn(func, flag, a);
    }
    inline void __cdecl Allocate_First_Screen_Seg_(int16_t count) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c4980});
        fn(count);
    }

    int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int32_t retreat_flag);
    int __cdecl qsort_ship_array_by_size_(const int16_t* param_1, const int16_t* param_2);
}