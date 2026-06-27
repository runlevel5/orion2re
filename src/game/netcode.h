#pragma once

namespace netcode {
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    extern uint32_t _thisHost;

    extern uint32_t _netInitialized;

    extern int8_t _tpSending;

    extern uint8_t _inComms;

    extern recv_fn _recv_fn;

    extern int32_t _connection_type;

    extern int32_t _in_send_message;

    /** Initializes networking while pumping MOX before and after backend setup.
     * @param mode Connection backend selector.
     * @param recv_fn_ptr Callback invoked for received packets.
     * @param network Network configuration for router-backed modes.
     */
    int32_t __cdecl Do_Net_Init_(uint32_t mode, recv_fn recv_fn_ptr, s_network* network);

    /** Initializes the requested networking backend.
     * @param mode Connection backend selector.
     * @param recv_fn_ptr Callback invoked for received packets.
     * @param network Network configuration for router-backed modes.
     */
    int32_t __cdecl Net_Init_(uint32_t mode, recv_fn recv_fn_ptr, s_network* network);

    /** Returns the number of discovered game sessions. */
    int32_t __cdecl Net_Get_Game_Count_();

    /** Requests a fresh session list from the active router and returns the cached count after pumping for a response. */
    int32_t __cdecl Net_Refresh_Game_List_();

    /** Returns the number of known players in the active session list. */
    int32_t __cdecl Get_Player_Count_();

    /** Returns the display name for a discovered session.
     * @param session_idx Zero-based session index.
     */
    char* __cdecl Net_Get_Game_Name_(uint32_t session_idx);

    /** Returns the router session ID for a discovered session.
     * @param session_idx Zero-based session index.
     */
    uint32_t __cdecl Net_Get_Game_Session_Id_(uint32_t session_idx);

    /** Joins a discovered game session by ID and creates the local player.
     * @param session_id Router session ID to join.
     * @param password Optional session password.
     * @param player_name Local player display name.
     */
    int32_t __cdecl Net_Join_Game_(uint32_t session_id, const char* password, const char* player_name);

    /** Shuts down the active networking backend and releases session state. */
    void __cdecl Net_Uninit_();

    /** Returns whether a transport send is currently in progress. */
    int32_t __cdecl Net_Is_Sending_();

    /** Writes connected user IDs into the caller buffer and returns the count.
     * @param users Output buffer for 16-bit user IDs, or nullptr to query the count only.
     */
    int32_t __cdecl Net_Get_Users_(int16_t* users);

    /** Prevents the active router session from accepting new players. */
    void __cdecl Net_Disable_New_Players_();

    /** Returns the current host network ID for the active backend. */
    uint32_t __cdecl Net_Host_ID_();

    /** Sends a packet through the active networking backend.
     * @param target_id Recipient network ID.
     * @param msg_type Application-defined message type.
     * @param data Packet payload buffer.
     * @param size Packet payload size in bytes.
     */
    uint32_t __cdecl Net_Send_Message_(int32_t target_id, int16_t msg_type, void* data, uint32_t size);

    /** Pumps backend state and pending communication work. */
    void __cdecl Net_Update_();

    /** Returns transport status flags for the active backend. */
    uint32_t __cdecl Net_Status_();

    /** Creates a new game session and local player.
     * @param name Session display name.
     * @param max_players Maximum player count.
     * @param password Optional session password.
     * @param player_name Local player display name.
     */
    int32_t __cdecl Net_Create_Game_(const char* name, int32_t max_players, const char* password, const char* player_name);
}