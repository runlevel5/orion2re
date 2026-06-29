#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colors;
struct s_mouse_list;
struct s_ship_data;
struct s_ship_design;
struct s_ship_weapons;

namespace COLBLDG {
    static inline bool Colony_Production_Is_Queued_Ship_(int16_t production_id) {
        return production_id <= COLONY_PRODUCTION_QUEUED_SHIP_BASE;
    }

    static inline bool Colony_Production_Is_Ship_Design_(int16_t production_id) {
        return production_id > COLONY_PRODUCTION_QUEUED_SHIP_BASE &&
               production_id <= COLONY_PRODUCTION_SHIP_DESIGN_BASE;
    }

    static inline int16_t Colony_Production_Ship_Index_(int16_t production_id) {
        return (int16_t)(-production_id - 100);
    }

    static inline int16_t Colony_Production_Ship_Design_Index_(int16_t production_id) {
        return (int16_t)(COLONY_PRODUCTION_SHIP_DESIGN_BASE - production_id);
    }

    static inline int16_t Colony_Production_From_Ship_Index_(int16_t ship_idx) {
        return (int16_t)(COLONY_PRODUCTION_QUEUED_SHIP_BASE - ship_idx);
    }

    static inline int16_t Colony_Production_From_Ship_Design_Index_(int16_t design_idx) {
        return (int16_t)(COLONY_PRODUCTION_SHIP_DESIGN_BASE - design_idx);
    }

    extern int16_t _building_indexes[54];
    extern int16_t _n_building_indexes;
    // Size: short[54]
    extern int16_t _building_fields[54];

    int __cdecl qsort_building_by_name_(const void* a, const void* b);

    int16_t __cdecl Calc_Font_Off_();

    void __cdecl Get_Font_Off_For_Buildings_(int16_t* offset);

    void __cdecl Get_Font_Off_For_Military_(int16_t* font_offset);

    void __cdecl Calculate_Building_Array_();

    void __cdecl Calculate_Military_Array_();

    int16_t __cdecl Star_N_Uncolonized_Worlds_(int16_t star_index);

    int16_t __cdecl N_Prods_In_Queue_(int16_t product_id);

    int16_t __cdecl Get_Prod_Queue_Len_();

    bool __cdecl Has_Prod_In_Queue_(int16_t building_id);

    void __cdecl Draw_Icon_(s_animation_header* anim);

    extern int16_t _n_military_indexes;
    extern int16_t _military_indexes[54];
    extern int16_t _military_fields[54];

    void __cdecl Build_Arrays_();

    /**
     * Creates a queued ship instance from a player design at a colony.
     * @param player_idx Owning player index.
     * @param design_idx Ship design slot to copy.
     * @param colony_idx Colony that supplies the ship location.
     */
    int16_t __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx);

    /** Opens the colony build-queue popup and runs its input loop. */
    void __cdecl Build_Queue_Popup_();

    int16_t __cdecl Colbldg_Create_Ship_(int16_t design_slot);

    extern uint8_t _return_to_build_popup_from_custom_design;
    extern uint8_t _field_mode;
    extern int16_t _active_prod[4];
    extern int16_t _current_item[7];
    extern int16_t _last_app;
    extern int16_t _last_setting;
    extern int16_t _scanned_field;
    extern char _g_msg[800];
    extern int16_t _colony_bldg_saved_screen;
    extern int16_t _scanned_prod;
    extern int16_t _refitting_ship_n;
    extern int16_t _colony_auto_building;
    extern int16_t _fields[11];
    extern s_colors _l_font_color_array;
    extern s_colors _l_font_dim_color_array;
    extern int16_t _queue_fields[7];

    extern uint8_t* _buffer;

    extern s_animation_header* _bldg_bitm;

    extern uint8_t* _desc_record_ptr;

    extern s_animation_header* _anims[7];
    extern s_animation_header* _colsum_pal_anim;

    void __cdecl Load_Build_Anims_();

    void __cdecl Clear_Last_App_Description_String_();

    void __cdecl Add_Build_Queue_Fields_();

    void __cdecl Add_Buildings_Fields_();

    void __cdecl Add_Militarys_Fields_();

    void __cdecl Clear_Last_App_Desc_();

    void __cdecl Update_Fields_();

    void __cdecl Draw_Build_Queue_Popup_();

    void __cdecl Draw_Buildings_List_();

    void __cdecl Draw_Militarys_List_();

    int16_t __cdecl Auto_Design_Type_(int16_t design_code);

    void __cdecl Draw_Build_Queue_();

    void __cdecl Draw_Current_Selection_();

    /**
     * Prints cost and timing lines for the selected production item.
     * @param colony_idx Colony supplying the production context.
     * @param product_idx Building, ship, or special product identifier.
     * @param line_count In/out line counter used for vertical placement.
     */
    void __cdecl Draw_Cost_And_Time_Info_(int16_t colony_idx, int16_t product_idx, int16_t* line_count);

    void __cdecl Draw_Ship_Design_Info2_(s_ship_design* design);

    /**
     * Builds a formatted paragraph describing a ship design.
     * @param design Ship design to describe.
     * @param buffer Destination paragraph buffer.
     */
    void __cdecl Build_Ship_Design_Info_Formatted_Paragraph_(s_ship_design* design, char* buffer);

    /**
     * Builds a formatted paragraph describing a ship and its crew state.
     * @param ship Ship instance to describe.
     * @param buffer Destination paragraph buffer.
     */
    void __cdecl Build_Ship_Info_Formatted_Paragraph_(s_ship_data* ship, char* buffer);

    /**
     * Dispatches to the ship or design paragraph builder for binary-compatibility callers.
     * @param ship_or_design Either a ship pointer or a design pointer.
     * @param buffer Destination paragraph buffer.
     * @param include_crew_details Nonzero when ship-specific crew details should be printed.
     */
    void __cdecl Build_Ship_Info_Formatted_Paragraph_Binary_(void* ship_or_design, char* buffer, char include_crew_details);

    /**
     * Formats a weapon entry for ship-description output.
     * @param weapon Weapon instance to describe.
     * @param dest Destination string buffer.
     * @param include_postfix_modifiers Nonzero to append trailing modifier text.
     * @param weapon_count Number of copies mounted for pluralization and count output.
     */
    void __cdecl Build_Weapon_String_(s_ship_weapons* weapon, char* dest, uint8_t include_postfix_modifiers, uint8_t weapon_count);

    int16_t __cdecl First_Prod_Colony_Queue_Slot_(int16_t colony_idx, int16_t product_id);

    void __cdecl Delete_From_Colony_Queue_(int16_t colony_idx, int16_t queue_idx);

    void __cdecl Delete_Prod_From_Colony_Queue_(int16_t colony_idx, int16_t product_id);

    void __cdecl Delete_Ship_(int16_t ship_index);

    void __cdecl Squish_Out_Colony_Queue_Slot_(int16_t colony_idx, int16_t queue_idx);

    void __cdecl Clear_Colony_Queue_(int16_t colony_idx);

    void __cdecl Print_App_Description_(uint16_t x, int16_t y, uint16_t width, int16_t app_id);

    void __cdecl Print_Setting_Description_(int16_t x, int16_t y, int16_t width, int16_t setting_id);

    void __cdecl Draw_Ship_Design_Info_(int16_t id);

    void __cdecl Draw_Building_Centered_In_Window_(int16_t building_id);

    void __cdecl Get_Setting_Description_String_(char* buffer, int16_t setting_id);

    void __cdecl Get_App_Description_String_(char* dest_str, int16_t app_id);

    void __cdecl Draw_Building_Info_(int16_t building_idx);

    void __cdecl Draw_Ship_Info_(int16_t item);

    void __cdecl Draw_Active_Prod_();

    char* __cdecl Selection_Name_(int16_t id);

    extern s_mouse_list _mouse_list_to[4];
    extern s_mouse_list _mouse_list_what;
    extern s_mouse_list _mouse_list_which_ship;
    extern uint8_t _hacky_speed_refitted_ship_flag;

    void __cdecl Add_Refited_Ship_To_Queue_();
    int8_t __cdecl Something_Behind_Settting_();
    void __cdecl Squish_Out_Queue_Slot_(int16_t slot_idx);
    void __cdecl Do_Draw_Build_Queue_();
    void __cdecl Do_Exit_Screen_Cleanup_();
    void __cdecl Remove_Ships_(int8_t flag);
    int16_t __cdecl First_Prod_Queue_Slot_(int16_t prod_idx);
    void __cdecl Delete_From_Queue_(int16_t slot_idx);
    int8_t __cdecl Add_To_Queue_(int16_t prod_idx);
    /**
     * Moves the selected queue entry before another queue slot.
     * @param source_slot_idx Slot currently holding the item to move.
     * @param target_slot_idx Slot that should receive the moved item.
     */
    void __cdecl Insert_Before_Queue_Slot_(int16_t source_slot_idx, int16_t target_slot_idx);
    void __cdecl Defragment_Queue_();

    /**
     * Updates build-queue state from the latest field selection and scan result.
     * @param selected_field_id In/out selected field identifier.
     * @param scanned_field_id In/out scanned field identifier.
     */
    void __cdecl Evaluate_Input_(int16_t* selected_field_id, int16_t* scanned_field_id);

    void __cdecl Print_Selection_Centered_(const char* text);

    char* __cdecl Option_String_(int16_t option_id);
}