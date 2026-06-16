#include "doctest/doctest.h"

#include "network/router_core.h"

#include <cstring>
#include <string>
#include <vector>

using namespace network::router;

namespace {

s_router_message Make_Hello_(const char* name, uint32_t request_id) {
    s_router_message message;
    message.header.kind = ROUTER_MESSAGE_HELLO;
    message.header.request_id = request_id;
    Encode_Hello_Payload(name, "test-build", &message.payload);
    return message;
}

s_router_message Make_Hello_Build_(const char* name, const char* build_string, uint32_t request_id) {
    s_router_message message;
    message.header.kind = ROUTER_MESSAGE_HELLO;
    message.header.request_id = request_id;
    Encode_Hello_Payload(name, build_string, &message.payload);
    return message;
}

s_router_message Make_Register_(uint32_t request_id) {
    s_router_session_registration_payload registration;
    Router_Copy_String(registration.session_name, sizeof(registration.session_name), "Game");
    Router_Copy_String(registration.host_name, sizeof(registration.host_name), "Host");
    Router_Copy_String(registration.build_string, sizeof(registration.build_string), "test-build");
    registration.max_players = 4;

    s_router_message message;
    message.header.kind = ROUTER_MESSAGE_REGISTER_SESSION;
    message.header.request_id = request_id;
    Encode_Session_Registration_Payload(&registration, &message.payload);
    return message;
}

s_router_message Make_Register_Build_(const char* build_string, uint32_t request_id) {
    s_router_session_registration_payload registration;
    Router_Copy_String(registration.session_name, sizeof(registration.session_name), "Game");
    Router_Copy_String(registration.host_name, sizeof(registration.host_name), "Host");
    Router_Copy_String(registration.build_string, sizeof(registration.build_string), build_string);
    registration.max_players = 4;

    s_router_message message;
    message.header.kind = ROUTER_MESSAGE_REGISTER_SESSION;
    message.header.request_id = request_id;
    Encode_Session_Registration_Payload(&registration, &message.payload);
    return message;
}

std::vector<s_router_outbound_message> Handle_Message_(s_router_core_state* core, t_router_connection_id connection_id, const s_router_message& message) {
    std::vector<s_router_outbound_message> output;
    Router_Core_Handle_Message(core, connection_id, &message, &output);
    return output;
}

}

DOCTEST_TEST_SUITE("router core") {

DOCTEST_TEST_CASE("hello assigns stable network ids and register exposes sessions") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);

    std::vector<s_router_outbound_message> output = Handle_Message_(&core, 1, Make_Hello_("host", 10));
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].connection_id == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_WELCOME);
    DOCTEST_CHECK(output[0].message.header.request_id == 10);

    s_router_welcome_payload welcome;
    std::string error;
    DOCTEST_REQUIRE(Decode_Welcome_Payload(&output[0].message.payload, &welcome, &error));
    DOCTEST_CHECK(welcome.network_id == 1);
    DOCTEST_CHECK(std::strcmp(welcome.router_id, "router-test") == 0);

    output = Handle_Message_(&core, 2, Make_Hello_("guest", 11));
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_REQUIRE(Decode_Welcome_Payload(&output[0].message.payload, &welcome, &error));
    DOCTEST_CHECK(welcome.network_id == 2);

    output = Handle_Message_(&core, 1, Make_Register_(12));
    DOCTEST_REQUIRE(output.size() == 2);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_SESSION_REGISTERED);

    s_router_session_info registered;
    DOCTEST_REQUIRE(Decode_Session_Info_Payload(&output[0].message.payload, &registered, &error));
    DOCTEST_CHECK(registered.session_id == 1);
    DOCTEST_CHECK(std::strcmp(registered.router_id, "router-test") == 0);
    DOCTEST_CHECK(std::strcmp(registered.session_name, "Game") == 0);
    DOCTEST_CHECK(std::strcmp(registered.host_name, "Host") == 0);
    DOCTEST_CHECK(registered.current_players == 1);
    DOCTEST_CHECK(registered.max_players == 4);
    DOCTEST_CHECK(registered.open);

    DOCTEST_CHECK(output[1].message.header.kind == ROUTER_MESSAGE_MEMBER_LIST);
    std::vector<s_router_member_info> members;
    DOCTEST_REQUIRE(Decode_Member_List_Payload(&output[1].message.payload, &members, &error));
    DOCTEST_REQUIRE(members.size() == 1);
    DOCTEST_CHECK(members[0].network_id == 1);
    DOCTEST_CHECK(members[0].owner);

    s_router_message list;
    list.header.kind = ROUTER_MESSAGE_LIST_SESSIONS;
    list.header.request_id = 13;
    output = Handle_Message_(&core, 2, list);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_SESSION_LIST);

    std::vector<s_router_session_info> sessions;
    DOCTEST_REQUIRE(Decode_Session_List_Payload(&output[0].message.payload, &sessions, &error));
    DOCTEST_REQUIRE(sessions.size() == 1);
    DOCTEST_CHECK(sessions[0].session_id == 1);
}

DOCTEST_TEST_CASE("join plus direct and broadcast sends route opaque game payloads") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);
    Handle_Message_(&core, 1, Make_Hello_("host", 1));
    Handle_Message_(&core, 2, Make_Hello_("guest", 2));
    Handle_Message_(&core, 1, Make_Register_(3));

    s_router_message join;
    join.header.kind = ROUTER_MESSAGE_JOIN_SESSION;
    join.header.session_id = 1;
    join.header.request_id = 4;
    std::vector<s_router_outbound_message> output = Handle_Message_(&core, 2, join);
    DOCTEST_REQUIRE(output.size() == 3);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_JOINED_SESSION);
    DOCTEST_CHECK(output[1].message.header.kind == ROUTER_MESSAGE_MEMBER_LIST);
    DOCTEST_CHECK(output[2].connection_id == 1);
    DOCTEST_CHECK(output[2].message.header.kind == ROUTER_MESSAGE_MEMBER_JOINED);

    s_router_member_event_payload member_event;
    std::string error;
    DOCTEST_REQUIRE(Decode_Member_Event_Payload(&output[2].message.payload, &member_event, &error));
    DOCTEST_CHECK(member_event.event == ROUTER_MEMBER_JOINED);
    DOCTEST_CHECK(member_event.member.network_id == 2);
    DOCTEST_CHECK(std::strcmp(member_event.member.player_name, "guest") == 0);

    DOCTEST_CHECK(Router_Core_Client_Session_Id(&core, 1) == 1);
    DOCTEST_CHECK(Router_Core_Client_Session_Id(&core, 2) == 1);

    s_router_message direct;
    direct.header.kind = ROUTER_MESSAGE_SEND_TO;
    direct.header.session_id = 999;
    direct.header.sender_network_id = 999;
    direct.header.target_network_id = 1;
    direct.header.request_id = 5;
    const uint8_t direct_payload[] = {9, 8, 7};
    direct.payload.Write_Bytes(direct_payload, sizeof(direct_payload));
    output = Handle_Message_(&core, 2, direct);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].connection_id == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_GAME_DATA);
    DOCTEST_CHECK(output[0].message.header.session_id == 1);
    DOCTEST_CHECK(output[0].message.header.sender_network_id == 2);
    DOCTEST_CHECK(output[0].message.header.target_network_id == 1);
    DOCTEST_CHECK(std::vector<uint8_t>(output[0].message.payload.Data(), output[0].message.payload.Data() + output[0].message.payload.Size()) == std::vector<uint8_t>{9, 8, 7});

    s_router_message broadcast;
    broadcast.header.kind = ROUTER_MESSAGE_SEND_BROADCAST;
    broadcast.header.session_id = 999;
    broadcast.header.sender_network_id = 999;
    broadcast.header.request_id = 6;
    const uint8_t broadcast_payload[] = {1, 2};
    broadcast.payload.Write_Bytes(broadcast_payload, sizeof(broadcast_payload));
    output = Handle_Message_(&core, 1, broadcast);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].connection_id == 2);
    DOCTEST_CHECK(output[0].message.header.session_id == 1);
    DOCTEST_CHECK(output[0].message.header.sender_network_id == 1);
    DOCTEST_CHECK(output[0].message.header.target_network_id == 2);

    broadcast.header.flags = ROUTER_FLAG_BROADCAST_INCLUDE_SENDER;
    output = Handle_Message_(&core, 1, broadcast);
    DOCTEST_REQUIRE(output.size() == 2);
    DOCTEST_CHECK(output[0].connection_id == 1);
    DOCTEST_CHECK(output[1].connection_id == 2);
    DOCTEST_CHECK(output[0].message.header.session_id == 1);
    DOCTEST_CHECK(output[0].message.header.sender_network_id == 1);
    DOCTEST_CHECK(output[1].message.header.session_id == 1);
    DOCTEST_CHECK(output[1].message.header.sender_network_id == 1);
}

DOCTEST_TEST_CASE("disconnect removes membership and notifies remaining session members") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);
    Handle_Message_(&core, 1, Make_Hello_("host", 1));
    Handle_Message_(&core, 2, Make_Hello_("guest", 2));
    Handle_Message_(&core, 1, Make_Register_(3));

    s_router_message join;
    join.header.kind = ROUTER_MESSAGE_JOIN_SESSION;
    join.header.session_id = 1;
    Handle_Message_(&core, 2, join);

    std::vector<s_router_outbound_message> output;
    Router_Core_Close_Connection(&core, 2, &output);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].connection_id == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_MEMBER_LEFT);
    DOCTEST_CHECK(output[0].message.header.sender_network_id == 2);
    DOCTEST_CHECK(output[0].message.header.target_network_id == 1);

    s_router_member_event_payload member_event;
    std::string error;
    DOCTEST_REQUIRE(Decode_Member_Event_Payload(&output[0].message.payload, &member_event, &error));
    DOCTEST_CHECK(member_event.event == ROUTER_MEMBER_LEFT);
    DOCTEST_CHECK(member_event.member.network_id == 2);
    std::vector<s_router_session_info> sessions;
    Router_Core_List_Sessions(&core, &sessions);
    DOCTEST_REQUIRE(sessions.size() == 1);
    DOCTEST_CHECK(sessions[0].current_players == 1);
}

DOCTEST_TEST_CASE("session owner can disable and re-enable new joins like DirectPlay EnableNewPlayers") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);
    Handle_Message_(&core, 1, Make_Hello_("host", 1));
    Handle_Message_(&core, 2, Make_Hello_("guest", 2));
    Handle_Message_(&core, 1, Make_Register_(3));

    s_router_message close_session;
    close_session.header.kind = ROUTER_MESSAGE_SET_SESSION_OPEN;
    close_session.header.session_id = 1;
    close_session.header.flags = 0;
    close_session.header.request_id = 4;
    std::vector<s_router_outbound_message> output = Handle_Message_(&core, 1, close_session);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_OK);

    s_router_message join;
    join.header.kind = ROUTER_MESSAGE_JOIN_SESSION;
    join.header.session_id = 1;
    join.header.request_id = 5;
    output = Handle_Message_(&core, 2, join);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_ERROR);
    s_router_error_payload closed_error;
    std::string closed_parse_error;
    DOCTEST_REQUIRE(Decode_Error_Payload(&output[0].message.payload, &closed_error, &closed_parse_error));
    DOCTEST_CHECK(closed_error.code == ROUTER_ERROR_SESSION_CLOSED);

    close_session.header.flags = ROUTER_FLAG_SESSION_OPEN;
    close_session.header.request_id = 6;
    output = Handle_Message_(&core, 1, close_session);
    DOCTEST_REQUIRE(output.size() == 1);
    output = Handle_Message_(&core, 2, join);
    DOCTEST_REQUIRE(output.size() == 3);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_JOINED_SESSION);
}

DOCTEST_TEST_CASE("join rejects mismatched client builds") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);
    Handle_Message_(&core, 1, Make_Hello_Build_("host", "build-a", 1));
    Handle_Message_(&core, 2, Make_Hello_Build_("guest", "build-b", 2));
    Handle_Message_(&core, 1, Make_Register_Build_("build-a", 3));

    s_router_message join;
    join.header.kind = ROUTER_MESSAGE_JOIN_SESSION;
    join.header.session_id = 1;
    join.header.request_id = 4;
    std::vector<s_router_outbound_message> output = Handle_Message_(&core, 2, join);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_ERROR);

    s_router_error_payload error_payload;
    std::string error;
    DOCTEST_REQUIRE(Decode_Error_Payload(&output[0].message.payload, &error_payload, &error));
    DOCTEST_CHECK(error_payload.code == ROUTER_ERROR_BUILD_MISMATCH);
}

DOCTEST_TEST_CASE("register requires authoritative hello build string") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Handle_Message_(&core, 1, Make_Hello_Build_("host", "", 1));

    std::vector<s_router_outbound_message> output = Handle_Message_(&core, 1, Make_Register_Build_("claimed-build", 2));
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_ERROR);

    s_router_error_payload error_payload;
    std::string error;
    DOCTEST_REQUIRE(Decode_Error_Payload(&output[0].message.payload, &error_payload, &error));
    DOCTEST_CHECK(error_payload.code == ROUTER_ERROR_BUILD_MISMATCH);
}

DOCTEST_TEST_CASE("unregister sends dedicated session closed notice") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);
    Handle_Message_(&core, 1, Make_Hello_("host", 1));
    Handle_Message_(&core, 2, Make_Hello_("guest", 2));
    Handle_Message_(&core, 1, Make_Register_(3));

    s_router_message join;
    join.header.kind = ROUTER_MESSAGE_JOIN_SESSION;
    join.header.session_id = 1;
    Handle_Message_(&core, 2, join);

    s_router_message unregister;
    unregister.header.kind = ROUTER_MESSAGE_UNREGISTER_SESSION;
    unregister.header.session_id = 1;
    unregister.header.request_id = 4;
    std::vector<s_router_outbound_message> output = Handle_Message_(&core, 1, unregister);
    DOCTEST_REQUIRE(output.size() == 2);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_OK);
    DOCTEST_CHECK(output[1].connection_id == 2);
    DOCTEST_CHECK(output[1].message.header.kind == ROUTER_MESSAGE_SESSION_CLOSED);
    DOCTEST_CHECK(Router_Core_Client_Session_Id(&core, 1) == 0);
    DOCTEST_CHECK(Router_Core_Client_Session_Id(&core, 2) == 0);
}

DOCTEST_TEST_CASE("session owner disconnect leaves session alive without promoting a new owner") {
    s_router_core_state core;
    Router_Core_Init(&core, "router-test");
    Router_Core_Open_Connection(&core, 1);
    Router_Core_Open_Connection(&core, 2);
    Router_Core_Open_Connection(&core, 3);
    Handle_Message_(&core, 1, Make_Hello_("host", 1));
    Handle_Message_(&core, 2, Make_Hello_("guest-a", 2));
    Handle_Message_(&core, 3, Make_Hello_("guest-b", 3));
    Handle_Message_(&core, 1, Make_Register_(4));

    s_router_message join;
    join.header.kind = ROUTER_MESSAGE_JOIN_SESSION;
    join.header.session_id = 1;
    Handle_Message_(&core, 2, join);
    Handle_Message_(&core, 3, join);

    std::vector<s_router_outbound_message> output;
    Router_Core_Close_Connection(&core, 1, &output);
    DOCTEST_REQUIRE(output.size() == 2);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_MEMBER_LEFT);
    DOCTEST_CHECK(output[1].message.header.kind == ROUTER_MESSAGE_MEMBER_LEFT);
    DOCTEST_CHECK(Router_Core_Client_Session_Id(&core, 2) == 1);
    DOCTEST_CHECK(Router_Core_Client_Session_Id(&core, 3) == 1);

    std::vector<s_router_session_info> sessions;
    Router_Core_List_Sessions(&core, &sessions);
    DOCTEST_REQUIRE(sessions.size() == 1);
    DOCTEST_CHECK(sessions[0].session_id == 1);
    DOCTEST_CHECK(sessions[0].current_players == 2);
    DOCTEST_CHECK_FALSE(sessions[0].open);

    s_router_message unregister;
    unregister.header.kind = ROUTER_MESSAGE_UNREGISTER_SESSION;
    unregister.header.session_id = 1;
    unregister.header.request_id = 5;
    output = Handle_Message_(&core, 2, unregister);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_ERROR);
    s_router_error_payload error_payload;
    std::string error;
    DOCTEST_REQUIRE(Decode_Error_Payload(&output[0].message.payload, &error_payload, &error));
    DOCTEST_CHECK(error_payload.code == ROUTER_ERROR_PERMISSION_DENIED);

    Router_Core_Close_Connection(&core, 2, &output);
    DOCTEST_REQUIRE(output.size() == 1);
    DOCTEST_CHECK(output[0].message.header.kind == ROUTER_MESSAGE_MEMBER_LEFT);
    Router_Core_Close_Connection(&core, 3, &output);
    DOCTEST_CHECK(output.empty());
    Router_Core_List_Sessions(&core, &sessions);
    DOCTEST_CHECK(sessions.empty());
}

}