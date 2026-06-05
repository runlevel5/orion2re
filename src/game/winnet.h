#pragma once

#include <guiddef.h>

namespace WINNET {
    /**
     * Removes the specified player from the cached player list.
     * @param player_id DirectPlay player identifier to remove.
     */
    void __cdecl Drop_Player_(uint32_t player_id);
    extern int32_t _provider_count;

    extern s_winnet_provider* _providers[9];

    extern clock_t _net_refresh_timer;

    extern GUID _game_guid;

    extern uint32_t _session_count;
    extern int8_t _is_searching_games;
    extern DPSESSIONDESC* _sessions[100];
    extern GUID _app_guid;

    extern uint32_t _player_count;

    extern s_net_player* _player_list[20];

    extern DPID _local_player_id;

    extern int32_t _provider_mode;

    /**
     * Returns the cached network player ID at the requested player-list index.
     * @param player_idx Zero-based index into `_player_list`.
     */
    uint32_t __cdecl Get_Player_Network_Id_(int32_t player_idx);

    /**
     * Caches an enumerated session in sorted session-name order.
     * @param session_desc DirectPlay session descriptor supplied by enumeration.
     * @param context Caller-provided enumeration context.
     * @param timeout Enumeration timeout value supplied by DirectPlay.
     * @param flags Enumeration status flags.
     */
    int __stdcall Refresh_Game_List_Callback_(LPDPSESSIONDESC session_desc, void* context, uint32_t* timeout, uint32_t flags);

    /**
     * Rebuilds the local player cache by enumerating players in a session.
     * @param session_id DirectPlay session identifier to enumerate.
     */
    int32_t __cdecl Refresh_Player_List_(DWORD session_id);

    /**
     * Adds an enumerated player to the local player cache.
     * @param dp_id DirectPlay player identifier.
     * @param short_name Enumerated short player name.
     * @param long_name Enumerated long player name.
     * @param flags Enumeration status flags.
     * @param context Caller-provided enumeration context.
     */
    BOOL __stdcall Player_Enumerate_Callback_(DPID dp_id, char* short_name, char* long_name, DWORD flags, void* context);

    /**
     * Reports a DirectPlay error code and terminates with a user-facing message.
     * @param error_code DirectPlay HRESULT-style error code.
     */
    void __cdecl Exit_With_DPlay_Error_(int32_t error_code);

    /**
     * Appends a player entry to the local player cache.
     * @param player_id DirectPlay player identifier.
     * @param player_name Display name stored for the player.
     * @param event_str Event string stored for the player.
     */
    void __cdecl Register_Player_(uint32_t player_id, char* player_name, char* event_str);

    /**
     * Creates the local DirectPlay player and caches it locally.
     * @param pid Receives the created DirectPlay player identifier.
     * @param friendly_name Friendly player name passed to DirectPlay.
     * @param formal_name Formal player name passed to DirectPlay.
     */
    void __cdecl Create_Player_(DPID* pid, char* friendly_name, char* formal_name);

    /**
     * Processes an incoming create-player packet.
     * @param pack Packet describing the player to add.
     */
    void __cdecl Handle_Create_Player_(s_net_pack_create_player* pack);

    /**
     * Processes an incoming destroy-player packet.
     * @param pack Packet describing the player to remove.
     */
    void __cdecl Handle_Destroy_Player_(s_net_pack_destroy_player* pack);

    /**
     * Enumerates currently available sessions for the supplied application GUID.
     * @param guid Application/session GUID to query.
     */
    uint32_t __cdecl Refresh_Game_List_(GUID guid);

    /**
     * Finds the last provider whose name contains the requested transport string.
     * @param name Lowercase provider-name fragment to search for.
     */
    int8_t __cdecl Find_Provider_(const char* name);

    /**
     * Initializes DirectPlay for the requested provider mode.
     * @param mode Provider mode selector used by this module.
     */
    int32_t __cdecl Init_Connection_(int32_t mode);

    /**
     * Caches an enumerated DirectPlay provider.
     * @param guid Provider GUID returned by DirectPlay.
     * @param name Provider name returned by DirectPlay.
     * @param major_version Provider major version.
     * @param minor_version Provider minor version.
     * @param context Caller-provided enumeration context.
     */
    BOOL __stdcall Enumerate_Providers_(GUID* guid, char* name, DWORD major_version, DWORD minor_version, void* context);

    /**
     * Enumerates available DirectPlay providers into the local cache.
     */
    int32_t __cdecl Init_Providers_();

    /**
     * Enables new players on the active DirectPlay connection.
     */
    void __cdecl Net_Enable_New_Players();

    /**
     * Reports a connection-related HRESULT and terminates with a user-facing message.
     * @param hr HRESULT to translate.
     */
    void __cdecl Exit_With_Error_(HRESULT hr);

    /**
     * Updates the cached keepalive timestamp for the sending player.
     * @param poll_buf Poll packet containing the sender identifier.
     */
    void __cdecl SetPlayerLastKeepAlive_(s_comcore_poll_buf* poll_buf);
}