#pragma once

namespace LIST {
    using draw_app_fn = void(*)(int16_t, int16_t, uint8_t);

    /**
     * Draws every item on the current page.
     * @param state List pagination and drawing state.
     */
    void __cdecl Display_List_Page_(s_list_state* state);

    /**
     * Positions selected_item at the first entry on the current page.
     * @param state List pagination and selection state.
     */
    void __cdecl Set_List_Page_Pointer_(s_list_state* state);

    /**
     * Draws one list item using its group and application colors.
     * @param state List drawing state.
     * @param item Item to render.
     */
    void __cdecl Display_List_Text_(s_list_state* state, s_list_item* item);

    /**
     * Creates hidden fields for the items on the current page.
     * @param state List state whose visible items should receive fields.
     */
    int16_t __cdecl Add_Fields_To_List_Page_(s_list_state* state);

    /**
     * Updates the up/down page field visibility for the current page.
     * @param state List pagination state.
     * @param up_field_id Field ID for the up-page control.
     * @param down_field_id Field ID for the down-page control.
     */
    void __cdecl Set_List_Up_Down_Field_Drawing_(s_list_state* state, int16_t up_field_id, int16_t down_field_id);

    /**
     * Advances to the next page when one exists.
     * @param state List pagination state.
     */
    int8_t __cdecl Next_List_Page_(s_list_state* state);

    /**
     * Moves back to the previous page when one exists.
     * @param state List pagination state.
     */
    int8_t __cdecl Previous_List_Page_(s_list_state* state);

    /**
     * Marks the entry whose generated field ID matches the requested field.
     * @param state List pagination and selection state.
     * @param target_field_id Hidden-field identifier to select.
     */
    s_list_item* __cdecl Set_Selected_List_Entry_(s_list_state* state, int16_t target_field_id);

    /**
     * Returns the currently marked selectable entry on the active page.
     * @param state List pagination and selection state.
     */
    s_list_item* __cdecl Get_Selected_List_Entry_(s_list_state* state);

    /**
     * Computes item positions and page boundaries for the list.
     * @param state List layout state and item array.
     */
    void __cdecl Init_List_Data_(s_list_state* state);

    /**
     * Draws the selected entry with the active highlight color.
     * @param state List drawing state.
     * @param item Selected item to render.
     */
    void __cdecl Display_Selected_List_Text_(s_list_state* state, s_list_item* item);

    /**
     * Moves the current page so the target item becomes visible.
     * @param state List pagination state.
     * @param target_item Item that should become visible.
     */
    int8_t __cdecl Goto_Page_With_Entry_(s_list_state* state, s_list_item* target_item);

    /**
     * Selects the first entry with the requested group ID and pages to it.
     * @param state List pagination and selection state.
     * @param head_id Group identifier to search for.
     */
    s_list_item* __cdecl Select_And_Page_By_Head_ID_(s_list_state* state, uint8_t head_id);

    /**
     * Draws the selected tech-list entry with the active color.
     * @param list_state Tech-list drawing state.
     * @param item Selected item to render.
     */
    void __cdecl Display_Selected_List_Text_For_Tech_List_(s_list_state* list_state, s_list_item* item);
}