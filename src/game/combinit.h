#pragma once

namespace COMBINIT {
    constexpr int STARBASE_SPECIAL_SLOT_COUNT = 14;
    constexpr int STARBASE_SPECIAL_ALTERNATIVE_COUNT = 3;

    inline uint8_t starbase_special
        [STARBASE_SPECIAL_SLOT_COUNT]
        [STARBASE_SPECIAL_ALTERNATIVE_COUNT] = {
            {SPECIAL_REINFORCED_HULL, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_SHIELD_CAPACITOR, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_HARD_SHIELDS, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_MULTI_PHASED_SHIELDS, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_WIDE_AREA_JAMMER, SPECIAL_MULTI_WAVE_ECM_JAMMER, SPECIAL_ECM_JAMMER},
            {SPECIAL_WARP_DISSIPATOR, SPECIAL_WARP_DISSIPATOR, SPECIAL_WARP_DISSIPATOR},
            {SPECIAL_SECURITY_STATIONS, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_BATTLE_SCANNER, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_HIGH_ENERGY_FOCUS, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_STRUCTURAL_ANALYZER, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_ACHILLES_TARGETING_UNIT, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_RANGEMASTER_UNIT, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_TRANSPORTERS, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
            {SPECIAL_TRANSPORTERS, SPECIAL_NO_SPECIAL, SPECIAL_NO_SPECIAL},
        };

    struct s_repair_rate {
        uint8_t cyber;
        uint8_t aru;
        uint8_t regen;
    };

    inline s_repair_rate repair_rate = {10, 20, 20};
    inline uint8_t _amoeba_regeneration = 0;
    inline uint8_t _crystal_keep = 0;
    inline uint8_t _planet_damper_enabled = 0;
    inline uint8_t _satellite_capture_enabled = 0;
    inline uint8_t _satellite_warlord_bonus = 0;
    inline uint8_t _satellite_size_class_plus_one = 0;
    inline uint8_t _phasing_cloak_effective_turns = 10;

    inline bool Is_Combat_Satellite_(const s_combat_data* ship) {
        return ship != nullptr &&
               ship != MOX::_combat_data &&
               ship->ship_idx == -1 &&
               ship->ftl_type == DRIVE_NO_DRIVE;
    }

    inline int16_t Combat_Satellite_Hull_Class_(const s_combat_data* ship) {
        return ship->size_class + 4 -
               (_satellite_size_class_plus_one != 0 ? 1 : 0);
    }

    struct s_satellite_capture_state {
        int16_t colony_idx = -1;
        int16_t building_idx = -1;
    };

    inline void Reset_Satellite_Capture_State_(s_satellite_capture_state* state) {
        state->colony_idx = -1;
        state->building_idx = -1;
    }

    inline void Record_Captured_Satellite_State_(
        s_satellite_capture_state* state, int16_t colony_idx, int16_t building_idx) {
        state->colony_idx = colony_idx;
        state->building_idx = building_idx;
    }

    inline int16_t Take_Captured_Satellite_State_(
        s_satellite_capture_state* state, int16_t colony_idx) {
        if (state->colony_idx != colony_idx) {
            return -1;
        }

        const int16_t building_idx = state->building_idx;
        Reset_Satellite_Capture_State_(state);
        return building_idx;
    }

    inline bool Satellite_Was_Captured_By_Attacker_(
        int16_t satellite_owner, int16_t victor_player, int16_t attacker_player) {
        return _satellite_capture_enabled != 0 &&
               victor_player == attacker_player &&
               satellite_owner == attacker_player;
    }

    // Clears any satellite awaiting the outcome of colony capture.
    void __cdecl Reset_Satellite_Capture_();
    // Records a removed satellite that may transfer with its colony.
    void __cdecl Record_Captured_Satellite_(int16_t colony_idx, int16_t building_idx);
    // Returns and clears the satellite recorded for this colony, or -1.
    int16_t __cdecl Take_Captured_Satellite_(int16_t colony_idx);

    // Loads the tactical planet graphics for the specified colony.
    // colony_idx: Colony whose planet art should be loaded.
    void __cdecl Load_Combat_Colony_Picture_(int16_t colony_idx);

    extern const char s_cmbtmisl_lbx_00551234[13];
    extern const char s_sphersfx_lbx_00551218[13];

    // Copies a strategic ship into the temporary display-combat record.
    // ship_idx: Strategic ship index to copy.
    void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx);

    // Copies a strategic ship into a combat-data slot.
    // ship_idx: Strategic ship index to load.
    // combat_idx: Combat-data slot to populate.
    void __cdecl Load_Combat_Ship_(int16_t ship_idx, int16_t combat_idx);

    // Returns the maximum shield strength for a loaded combat ship.
    // ship_idx: Combat-data ship index to inspect.
    int16_t __cdecl Calc_Combat_Shield_Max_(int16_t ship_idx);

    // Returns whether an undamaged special device is present on a combat ship.
    // ship_idx: Combat-data ship index to inspect.
    // special_id: Special-device bit index to test.
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id);

    // Returns whether the colony has any tactical defensive installation.
    // colony_idx: Colony index to inspect.
    int8_t __cdecl Colony_Has_Defenses_(int16_t colony_idx);

    // Loads the scripted Antaran defensive fleet into combat data.
    void __cdecl Load_Antaran_Defense_Fleet_();

    // Loads one Antaran ship template by hull size into a combat-data slot.
    // ship_idx: Combat-data slot to populate.
    // ship_size: Antaran hull-size template index.
    void __cdecl Load_Combat_Antaran_Ship_(int16_t ship_idx, int16_t ship_size);

    // Loads the Antaran star fortress combat unit.
    void __cdecl Load_Antaran_Star_Fortress_();
    // Loads the Antaran planet combat object and its graphics.
    void __cdecl Load_Antaran_Planet_();
    // Loads the colony and any orbiting defense platform into combat data.
    // colony_idx: Colony index whose defenses should be loaded.
    void __cdecl Load_Colony_Defense_(int16_t colony_idx);

    // Loads the colony combat object and optionally its artwork.
    // colony_idx: Colony index to load.
    // load_picture: Nonzero to load the colony picture assets.
    void __cdecl Load_Tactical_Colony_(int16_t colony_idx, int16_t load_picture);

    // Returns the combat station type present at the colony, or -1 if none.
    // colony_idx: Colony index to inspect.
    int16_t __cdecl Type_Of_Space_Station_(int16_t colony_idx);

    // Loads one orbital defense platform or station into combat data.
    // station_type: Station hull/type selector.
    void __cdecl Load_Combat_Satelite_(int16_t station_type);

    // Installs the defender's researched orbital-defense specials in priority order.
    void __cdecl Load_Combat_Satellite_Specials_(
        s_combat_data* satellite, const s_player* defender);

    // Loads and applies the combat palette for one participant.
    // player_idx: Player or monster owner index.
    void __cdecl Load_Combat_Ship_Palette_(int16_t player_idx);

    // Loads per-ship art assets needed by one participant.
    // player_idx: Player or monster owner index.
    void __cdecl Load_Individual_Ship_Pictures_(int16_t player_idx);

    // Resets per-combat special-device state for one loaded ship.
    // combat_ship_idx: Combat-data ship index to initialize.
    void __cdecl Init_Special_Devices_(int16_t combat_ship_idx);

    // Places loaded combat units onto the tactical grid.
    void __cdecl Deploy_Ships_();

    // Returns the deployment spacing offset for a hull size.
    // size: Combat hull-size class.
    int16_t __cdecl Ship_Size_Offset_(int16_t size);

    // Returns whether a ship can be deployed at a tactical grid cell.
    // x: Candidate combat-grid X coordinate.
    // y: Candidate combat-grid Y coordinate.
    // ship_idx: Optional blocking ship index, or -1.
    int8_t __cdecl Ship_Can_Deploy_At_(int16_t x, int16_t y, int16_t ship_idx);

    // Compares two combat-data entries by hull size for qsort, largest first.
    // a: Pointer to the first combat-data entry.
    // b: Pointer to the second combat-data entry.
    int __cdecl qsort_combat_data_by_size_(const void* a, const void* b);

    // Centers the visible combat map near a target grid position.
    // target_x: Combat-grid X coordinate to center on.
    // target_y: Combat-grid Y coordinate to center on.
    void __cdecl Snap_Center_Combat_Screen_(int16_t target_x, int16_t target_y);

    // Scrolls the combat camera while the pointer is at the screen edge.
    void __cdecl Scroll_Combat_Screen_();

    // Applies end-of-phase plasma web damage to one side.
    // owner_idx: Owner whose ships should take web damage.
    void __cdecl Plasma_Web_Damage_(int16_t owner_idx);
    // Advances black-hole-generator countdown effects for one side.
    // player_idx: Owner whose ships should be processed.
    void __cdecl Increment_Bhg_(int16_t player_idx);
    // Recharges shields for all eligible combat ships.
    void __cdecl Recharge_Shields_();
    // Applies end-of-turn repairs to all eligible combat ships.
    void __cdecl Repair_All_Combat_Ships_();
    // Runs end-of-turn tactical combat maintenance.
    void __cdecl End_Of_Turn_Bookeeping_();
    // Resolves post-combat cleanup and strategic side effects.
    // star_idx: Star index for the battle, or _NUM_STARS for Antares.
    // colony_pop_lost: Population lost during the battle.
    // combat_winner: Tactical combat result code.
    void __cdecl End_Of_Combat_(int16_t star_idx, int16_t colony_pop_lost, int16_t combat_winner);

    // Runs a tactical combat encounter and returns the result code.
    // plr1_ships: Attacker ship index array.
    // plr1_ship_count: Number of attacker entries.
    // plr2_ships: Defender ship index array.
    // plr2_ship_count: Number of defender entries.
    // star_idx: Star index for the battle, or _NUM_STARS for Antares.
    // colony_idx: Defending colony index, or -1.
    // retreat_flag: Initial retreat-control flag.
    int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int16_t retreat_flag);

    // Converts subsystem hit-point loss to a strategic damage percentage.
    inline int16_t Damage_Percent_From_Hits_(int16_t maximum_hits, int16_t current_hits) {
        if (maximum_hits <= 0) {
            return 0;
        }

        return (100 * (maximum_hits - current_hits)) / maximum_hits;
    }

    // Reconstructs surviving units from a slot's aggregate hit points.
    inline int8_t Surviving_Weapon_Count_From_Hits_(int16_t current_hits, int16_t hits_per_unit) {
        if (current_hits <= 0 || hits_per_unit <= 0) {
            return 0;
        }

        return (int8_t)((current_hits + hits_per_unit - 1) / hits_per_unit);
    }

    // Writes tactical combat damage state back to the strategic ship record.
    // combat_ship_idx: Combat-data ship index to unload.
    void __cdecl Unload_Combat_Ship_(int16_t combat_ship_idx);

    // Applies diplomatic penalties for bio-weapon use.
    // player_idx: Attacking player responsible for the penalty.
    // star_idx: Star index where the combat occurred.
    // colony_pop_lost: Population lost during the battle.
    void __cdecl Apply_Bio_Weapon_Penalty_(int16_t player_idx, int16_t star_idx, int16_t colony_pop_lost);

    extern uint8_t _real_attack_flag;
    // Compares two ship indices by hull size for qsort, largest first.
    // a: Pointer to the first ship index.
    // b: Pointer to the second ship index.
    int __cdecl qsort_ship_array_by_size_(const void* a, const void* b);

    // Returns the pixel-space center offset for a combat object.
    // ship_idx: Combat-data ship index, or 0 for the colony/planet object.
    // out_x: Receives the X offset.
    // out_y: Receives the Y offset.
    void __cdecl Ship_Center_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y);
}