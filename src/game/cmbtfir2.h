#pragma once

#include <cstdint>

namespace CMBTFIR2 {
    extern int16_t _artemis_net_chance[6];

    /** Returns the helmsman combat bonus granted by the specified leader. */
    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx);

    /** Returns the weaponry combat bonus granted by the specified leader. */
    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx);

    /** Returns the engineer repair bonus granted by the specified leader. */
    int16_t __cdecl Engineer_Bonus_(int16_t leader_idx);

    /** Returns the fighter-pilot combat bonus granted by the specified leader. */
    int16_t __cdecl Fighter_Pilot_Bonus_(int16_t leader_idx);

    /** Returns the galactic-lore combat bonus granted by the specified leader. */
    int16_t __cdecl Galactic_Lore_Bonus_(int16_t leader_idx);

    /** Recomputes ordnance bonuses for the current combat participants. */
    void __cdecl Set_Combat_Ordnance_Values_();

    /** Returns the ordnance bonus granted by the specified leader. */
    int16_t __cdecl Leader_Ordnance_Bonus_(int16_t leader_idx);

    /** Returns the highest ordnance bonus among active ships owned by a player. */
    int16_t __cdecl Get_Fleet_Ordnance_Bonus_(int16_t owner);

    /** Applies the Artemis Net effect to eligible combat units. */
    bool __cdecl Apply_Combat_Artemis_Net_(char strategic);

    /** Captures the synchronized random seed used by subsequent Artemis Net combats. */
    void __cdecl Save_Artemis_Random_Seed_();

    int16_t __cdecl Used_Teleporter_(int16_t ship_idx);

    void __cdecl Set_Teleporter_Used_(int16_t ship_idx);

    void __cdecl Set_Boarded_Flag_(int16_t ship_idx);

    int16_t __cdecl Commando_Bonus_(int16_t leader_idx);

    int16_t __cdecl Get_Fleet_Commando_Bonus_(int16_t player_idx);

    int16_t __cdecl Crew_Boarding_Combat_Bonus_(int16_t ship_idx);

    int16_t __cdecl Security_Bonus_(int16_t leader_idx);

    int16_t __cdecl Get_Fleet_Security_Bonus_(int16_t player_idx);

    void __cdecl Select_Raid_Internal_(int16_t ship_idx, int16_t* raid_type, int16_t* component_idx);

    int16_t __cdecl Get_Combat_Ship_Special_Id_(int16_t ship_idx, int16_t occurrence);

    void __cdecl Resolve_Raid_(int16_t attacker_idx, int16_t target_idx, int16_t* raiders);

    /** Resolves a boarding capture attempt between two combat ships. */
    void __cdecl Resolve_Capture_(int16_t attacker_idx, int16_t defender_idx, int16_t attacker_marines, int16_t target_x, int16_t target_y);

    void __cdecl Capture_Ship_(int16_t ship_idx, int16_t x, int16_t y);

    int16_t __cdecl Already_Boarded_(int16_t ship_idx);

    void __cdecl Set_Combat_Ship_Done_(int16_t ship_idx);

    int16_t __cdecl Max_Defensive_Fire_Range_(int16_t ship_idx);

    void __cdecl Init_Internal_Space_(s_combat_data* ship);

    int16_t __cdecl Max_Total_Internal_Space_(int16_t ship_idx);

    int16_t __cdecl Total_Internal_Space_(int16_t ship_idx);

    int16_t __cdecl Crew_Defense_Bonus_(int16_t combat_idx);

    int16_t __cdecl Fleet_Defensive_Bonus_(int16_t player_idx, int16_t mode, int16_t* bonus_val_ptr);

    int16_t __cdecl Fleet_Offensive_Bonus_(int16_t player_idx);

    int16_t __cdecl Get_Fleet_Pilot_Bonus_(int16_t owner_idx);

    int16_t __cdecl Ship_Specials_Defensive_Bonus_(int16_t ship_idx, int16_t target_type, int16_t* field_bonus_out);

    int16_t __cdecl Ship_Specials_Offensive_Bonus_(int16_t ship_idx);

    int16_t __cdecl Defensive_Combat_Bonus_(int16_t combat_unit_idx, int16_t target_type);

    int16_t __cdecl Offensive_Combat_Bonus_(int16_t ship_idx);

    int16_t __cdecl Computer_Bonus_(int16_t ship_idx);

    int16_t __cdecl Crew_Attack_Bonus_(int16_t ship_idx);

    int16_t __cdecl Get_Fleet_Engineer_Bonus_(int16_t player_idx);

    void __cdecl Repair_Combat_Ship_(int16_t ship_idx, int16_t amount);

    void __cdecl Clear_Internal_Space_(s_combat_data* combat_data);

    int16_t __cdecl Being_Captured_(int16_t ship_idx);

    int16_t __cdecl Combat_Ship_Done_(int16_t ship_idx);

    void __cdecl Set_Capturing_Flag_(int16_t unit_idx);

    /** Opens the boarding popup and returns the selected boarding action. */
    int16_t __cdecl Init_Boarding_Action_(int16_t unit_idx, int16_t* boarding_choice_out);

    void __cdecl Defensive_Fire_Check_(int16_t target_idx, int16_t target_x, int16_t target_y, int16_t dest_x, int16_t dest_y);

    void __cdecl Final_Fire_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx);

    void __cdecl Kill_Crew_(int16_t combat_data_idx, int16_t kill_count);

    void __cdecl Select_Internal_(int16_t ship_idx, int16_t* hit_type, int16_t* target_idx, int16_t heavy_hit, int16_t direct_hit, int16_t armor_piercing, int16_t internal_only);

    void __cdecl Apply_Internal_Damage_(int16_t target_ship_idx, int16_t* remaining_damage, int16_t sys_index, int16_t count, int16_t flag);

    void __cdecl Clear_Capturing_Flag_(int16_t ship_idx);

    int16_t __cdecl Damage_From_Ship_Exploding_(int16_t ship_idx);
}