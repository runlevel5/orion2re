#include "pch.h"

namespace WINNET {

    int8_t __cdecl Find_Provider_(const char* name) {
        int8_t found_index = -1;

        for (int32_t i = 0; i < _provider_count; ++i) {
            if (strstr(_providers[i]->name, name) != nullptr) {
                found_index = static_cast<int8_t>(i);
            }
        }

        return found_index;
    }

    int32_t __cdecl Init_Connection_(int32_t mode) {
        int32_t provider_idx = -1;
        const char* provider_name = nullptr;

        if (mode == 0) {
            provider_name = s_modem_005607dc;
        } else if (mode == 1) {
            provider_name = s_ipx_005607e4;
        } else if (mode == 2) {
            provider_name = s_tcp_005607e8;
        }

        if (provider_name != nullptr) {
            provider_idx = Find_Provider_(provider_name);
        }

        if (provider_idx == -1) {
            return 0;
        }

        if (netcode::_dplay != nullptr) {
            netcode::_dplay->Release();
            netcode::_dplay = nullptr;
        }

        HRESULT hr = DPLAY_DLL::Ordinal_1(&_providers[provider_idx]->guid, &netcode::_dplay, nullptr);
        if (hr == 0) {
            netcode::_connection_type = mode;
            return 1;
        }

        return 0;
    }

    uint32_t __cdecl Refresh_Game_List_(GUID guid) {
        _app_guid = guid;

        if (_session_count != 0) {
            for (uint32_t i = 0; i < _session_count; ++i) {
                if (_sessions[i] != nullptr) {
                    ::_free(_sessions[i]);
                }
            }
        }

        _session_count = 0;

        DPSESSIONDESC session_desc;
        memset(&session_desc, 0, sizeof(session_desc));
        session_desc.dwSize = sizeof(DPSESSIONDESC);
        session_desc.guidSession = guid;

        if (netcode::_connection_type == 0) {
            _is_searching_games = 1;

            HRESULT hr = netcode::_dplay->EnumSessions(
                &session_desc,
                1000,
                reinterpret_cast<LPDPENUMSESSIONSCALLBACK>(Refresh_Game_List_Callback_),
                platform::g_main_window,
                1 /* DPENUMSESSIONS_AVAILABLE */
            );

            if (hr != 0) {
                if (hr == static_cast<HRESULT>(0x88770082)) { // DPERR_INVALIDOBJECT
                    exitm::Exit_With_Message(s_Error_Enum_Current_Games_Invalid_0055c028);
                }
                if (hr == static_cast<HRESULT>(0x80070057)) { // E_INVALIDARG / DPERR_INVALIDPARAMS
                    exitm::Exit_With_Message(s_Error_Enum_Current_Games_Invalid_0055c000);
                }
            }
        }

        return _session_count;
    }

    int __stdcall Refresh_Game_List_Callback_(LPDPSESSIONDESC session_desc, void* context, uint32_t* timeout, uint32_t flags) {
        if ((flags & 1) != 0 || _session_count == 100) {
            _is_searching_games = 0;
            return 0;
        }

        uint32_t insert_idx = _session_count;

        for (uint32_t i = 0; i < _session_count; ++i) {
            if (__strcmpi(session_desc->szSessionName, _sessions[i]->szSessionName) < 0) {
                insert_idx = i;
                _sessions[_session_count] = reinterpret_cast<LPDPSESSIONDESC>(_malloc(sizeof(DPSESSIONDESC)));

                for (uint32_t j = _session_count; j > i; --j) {
                    _memcpy(_sessions[j], _sessions[j - 1], sizeof(DPSESSIONDESC));
                }

                _memcpy(_sessions[i], session_desc, sizeof(DPSESSIONDESC));
                break;
            }
        }

        if (insert_idx == _session_count) {
            _sessions[_session_count] = reinterpret_cast<LPDPSESSIONDESC>(_malloc(sizeof(DPSESSIONDESC)));
            _memcpy(_sessions[_session_count], session_desc, sizeof(DPSESSIONDESC));
        }

        _session_count++;
        return 1;
    }

    int32_t __cdecl Init_Providers_() {
        void* callback = reinterpret_cast<void*>(std::uintptr_t{0x005385c0}); /* WINNET::Enumerate_Providers_ */
        DPLAY_DLL::Ordinal_2(callback, platform::g_main_window);
        return 0;
    }

    BOOL __stdcall Enumerate_Providers_(GUID* guid, char* name, DWORD major_version, DWORD minor_version, void* context) {
        if (_provider_count < 9) {
            s_winnet_provider* provider = static_cast<s_winnet_provider*>(_malloc(sizeof(s_winnet_provider)));
            _providers[_provider_count] = provider;

            provider->guid = *guid;
            _memcpy(provider->name, name, sizeof(provider->name));
            provider->major_version = major_version;
            provider->minor_version = minor_version;

            _strlwr(provider->name);

            _provider_count++;
            return 1;
        }
        return 0;
    }

    void __cdecl Create_Player_(DPID* pid, char* friendly_name, char* formal_name) {
        HRESULT hr = netcode::_dplay->CreatePlayer(pid, friendly_name, formal_name, nullptr);

        if (hr != 0) {
            *pid = 0;
            Exit_With_DPlay_Error_(hr);
            return;
        }

        Register_Player_(*pid, friendly_name, formal_name);
    }

    int32_t __cdecl Refresh_Player_List_(DWORD session_id) {
        for (uint32_t i = 0; i < _player_count; ++i) {
            if (_player_list[i] != nullptr) {
                ::_free(_player_list[i]);
            }
        }

        _player_count = 0;

        netcode::_dplay->EnumPlayers(
            session_id,
            Player_Enumerate_Callback_,
            reinterpret_cast<LPVOID>(platform::g_main_window),
            0x80 /* DPENUMPLAYERS_SESSION */
        );


        return 1;
    }

    uint32_t __cdecl Get_Player_Network_Id_(int32_t player_idx) {
        return _player_list[player_idx]->player_id;
    }

    BOOL __stdcall Player_Enumerate_Callback_(DPID dp_id, char* short_name, char* long_name, DWORD flags, void* context) {
        Register_Player_(dp_id, short_name, long_name);
        return 1;
    }

    void __cdecl Register_Player_(uint32_t player_id, char* player_name, char* event_str) {
        s_net_player* player = reinterpret_cast<s_net_player*>(_malloc(sizeof(s_net_player)));

        _player_list[_player_count] = player;
        player->player_id = player_id;

        strcpy(player->event_string, event_str);
        strcpy(player->player_name, player_name);

        player->join_time = time_::_time();

        _player_count++;
    }

    void __cdecl Exit_With_DPlay_Error_(int32_t error_code) {
        char buffer[200];
        char* err_str;

        strcpy(buffer, s_DirectPlay_ERR_00560a30);

        switch (error_code) {
        case 0x80070057:
            err_str = s_Invalid_Params_0055bff0;
            break;
        case 0x80004005:
            err_str = s_Generic_0055bfe8;
            break;
        case 0x8877001E:
            err_str = s_Buffer_To_Small_00560a14;
            break;
        case 0x88770082:
            err_str = s_Invalid_Object_0055bf8c;
            break;
        case 0x887700BE:
            err_str = s_No_Messages_00560a24;
            break;
        default:
            err_str = s_Unknown_Err_00560a08;
            break;
        }

        strcat(buffer, err_str);
        exitm::Exit_With_Message(buffer);
    }

    void __cdecl Net_Enable_New_Players() {
        if (netcode::_dplay != nullptr) {
            netcode::_dplay->EnableNewPlayers(1);
        }
    }

    void __cdecl Handle_Create_Player_(s_net_pack_create_player* pack) {
        if (pack->player_type == 1) {
            Register_Player_(pack->player_id, pack->player_name, pack->event_str);
        }
    }

    void __cdecl Handle_Destroy_Player_(s_net_pack_destroy_player* packet) {
        Drop_Player_(packet->player_id);
    }

    void __cdecl Drop_Player_(uint32_t player_id) {
        uint32_t index = 0xFFFFFFFF;
        bool found = false;

        if (_player_count > 0) {
            for (uint32_t i = 0; i < _player_count; i++) {
                if (_player_list[i]->player_id == player_id) {
                    index = i;
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            if (index < _player_count - 1) {
                for (uint32_t j = index; j < _player_count - 1; j++) {
                    _player_list[j]->player_id = _player_list[j + 1]->player_id;
                    _player_list[j]->join_time = _player_list[j + 1]->join_time;
                    strcpy(_player_list[j]->event_string, _player_list[j + 1]->event_string);
                    strcpy(_player_list[j]->player_name, _player_list[j + 1]->player_name);
                }
            }

            ::_free(_player_list[_player_count - 1]);
            _player_count--;
        }
    }

    void __cdecl SetPlayerLastKeepAlive_(s_comcore_poll_buf* poll_buf) {
        if (poll_buf->id_from == _local_player_id) {
            return;
        }

        for (uint32_t i = 0; i < _player_count; i++) {
            s_net_player* player = _player_list[i];
            if (player->player_id == poll_buf->id_from) {
                player->join_time = time_::_time();
            }
        }
    }

    void __cdecl Exit_With_Error_(HRESULT hr) {
        char message[100];
        char* error_string;

        switch (hr) {
        case static_cast<int32_t>(0x80004005): /* E_FAIL */
            error_string = s_Generic_0055bfe8;
            break;
        case static_cast<int32_t>(0x80004001): /* E_NOTIMPL */
            error_string = s_Unsupported_0055bfd0;
            break;
        case static_cast<int32_t>(0x88770014): /* DPNERR_ACTIVEPLAYERS */
            error_string = s_Active_Players_0055bfb4;
            break;
        case static_cast<int32_t>(0x80070057): /* E_INVALIDARG */
            error_string = s_Invalid_Params_0055bff0;
            break;
        case static_cast<int32_t>(0x88770082): /* DPNERR_INVALIDOBJECT */
            error_string = s_Invalid_Object_0055bf8c;
            break;
        case static_cast<int32_t>(0x88770096): /* DPNERR_INVALIDPLAYER */
            error_string = s_Invalid_Player_0055bf9c;
            break;
        case static_cast<int32_t>(0x887700FA): /* DPNERR_UNAVAILABLE */
            error_string = s_Unavailable_0055bfdc;
            break;
        case static_cast<int32_t>(0x8877010E): /* DPNERR_UNINITIALIZED */
            error_string = DAT_0055bfac;
            break;
        case static_cast<int32_t>(0x88770118): /* DPNERR_USERCANCEL */
            error_string = s_User_Cancel_0055bfc4;
            break;
        default:
            error_string = s_Error_Unknown_0055bf7c;
            break;
        }

        strcpy(message, error_string);
        exitm::Exit_With_Message(message);
    }
}