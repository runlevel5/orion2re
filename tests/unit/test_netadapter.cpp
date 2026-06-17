#include "doctest/doctest.h"

#include "game/netadapter.h"
#include "network/router_protocol.h"
#include "network/router_service.h"

#include <chrono>
#include <cstring>
#include <thread>

using namespace network::router;

namespace {

struct Service_Thread_Guard_ {
    s_router_service* service = nullptr;
    std::thread* service_thread = nullptr;

    ~Service_Thread_Guard_() {
        if (service != nullptr && service_thread != nullptr && service_thread->joinable()) {
            Router_Service_Request_Stop(service);
            service_thread->join();
        }
    }
};

bool Start_Test_Service_(s_router_service* service, std::thread* service_thread, int* run_result) {
    *run_result = -1;
    *service_thread = std::thread([&]() {
        *run_result = Router_Service_Run(service);
    });

    for (int i = 0; i < 100 && !Router_Service_Is_Running(service); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    for (int i = 0; i < 100 && Router_Service_Bound_Tcp_Port(service) == 0 && Router_Service_Is_Running(service); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return Router_Service_Is_Running(service) && Router_Service_Bound_Tcp_Port(service) != 0;
}

netadapter::s_config Make_Config_(uint16_t port, const char* name) {
    netadapter::s_config config;
    Router_Copy_String(config.host, sizeof(config.host), "127.0.0.1");
    config.tcp_port = port;
    Router_Copy_String(config.client_name, sizeof(config.client_name), name);
    Router_Copy_String(config.build_string, sizeof(config.build_string), "test-build");
    return config;
}

void Pump_For_(netadapter::s_adapter* adapter, std::chrono::milliseconds duration) {
    const auto deadline = std::chrono::steady_clock::now() + duration;
    while (std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool Wait_For_Local_Id_(netadapter::s_adapter* adapter) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (netadapter::Local_Network_Id(adapter) == 0 && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return netadapter::Local_Network_Id(adapter) != 0;
}

bool Wait_For_Game_Count_(netadapter::s_adapter* adapter, int32_t expected_count) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (netadapter::Game_Count(adapter) < expected_count && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return netadapter::Game_Count(adapter) >= expected_count;
}

bool Wait_For_Game_List_Revision_(netadapter::s_adapter* adapter, uint32_t previous_revision) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (netadapter::Game_List_Revision(adapter) == previous_revision && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return netadapter::Game_List_Revision(adapter) != previous_revision;
}

bool Wait_For_Session_(netadapter::s_adapter* adapter) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (netadapter::Session_Id(adapter) == 0 && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return netadapter::Session_Id(adapter) != 0;
}

bool Wait_For_Users_(netadapter::s_adapter* adapter, int32_t expected_count) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (netadapter::Get_Users(adapter, nullptr, 0) < expected_count && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return netadapter::Get_Users(adapter, nullptr, 0) >= expected_count;
}

bool Wait_For_Packet_(netadapter::s_adapter* adapter, netadapter::s_game_packet* packet) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (!netadapter::Poll_Game_Packet(adapter, packet) && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return packet->message_type != 0;
}

bool Wait_For_Event_(netadapter::s_adapter* adapter, netadapter::s_event* event) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (!netadapter::Poll_Event(adapter, event) && std::chrono::steady_clock::now() < deadline) {
        netadapter::Pump(adapter);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return event->kind != netadapter::EVENT_ERROR || event->error.code != ROUTER_ERROR_INVALID_MESSAGE;
}

}

DOCTEST_TEST_SUITE("netadapter") {

DOCTEST_TEST_CASE("adapter creates and joins router sessions and routes game packets") {
    s_router_config service_config;
    Router_Copy_String(service_config.listen_host, sizeof(service_config.listen_host), "127.0.0.1");
    service_config.tcp_port = 0;
    service_config.enable_discovery = false;
    service_config.enable_signal_handlers = false;

    s_router_service service;
    Router_Service_Init(&service, &service_config);
    int run_result = -1;
    std::thread service_thread;
    Service_Thread_Guard_ service_guard{&service, &service_thread};
    DOCTEST_REQUIRE(Start_Test_Service_(&service, &service_thread, &run_result));

    netadapter::s_adapter host;
    netadapter::s_adapter guest;
    netadapter::s_config host_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "host");
    netadapter::s_config guest_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "guest");
    netadapter::Init(&host, &host_config);
    netadapter::Init(&guest, &guest_config);

    DOCTEST_REQUIRE(netadapter::Connect(&host));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&host));
    DOCTEST_CHECK(netadapter::Local_Network_Id(&host) == 1);

    DOCTEST_REQUIRE(netadapter::Create_Game(&host, "Adapter Game", 4, "Host"));
    DOCTEST_REQUIRE(Wait_For_Session_(&host));
    DOCTEST_REQUIRE(Wait_For_Users_(&host, 1));

    DOCTEST_REQUIRE(netadapter::Connect(&guest));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&guest));
    DOCTEST_CHECK(netadapter::Local_Network_Id(&guest) == 2);

    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&guest));
    DOCTEST_REQUIRE(Wait_For_Game_Count_(&guest, 1));
    DOCTEST_CHECK(std::strcmp(netadapter::Game_Name(&guest, 0), "Adapter Game") == 0);
    DOCTEST_CHECK(netadapter::Game_Session_Id(&guest, 0) == 1);

    DOCTEST_REQUIRE(netadapter::Join_Game(&guest, 0, "Guest"));
    DOCTEST_REQUIRE(Wait_For_Session_(&guest));
    DOCTEST_REQUIRE(Wait_For_Users_(&guest, 2));
    DOCTEST_REQUIRE(Wait_For_Users_(&host, 2));

    netadapter::s_event event;
    DOCTEST_REQUIRE(Wait_For_Event_(&host, &event));
    DOCTEST_CHECK(event.kind == netadapter::EVENT_MEMBER_JOINED);
    DOCTEST_CHECK(event.network_id == 2);

    uint32_t users[4]{};
    DOCTEST_CHECK(netadapter::Get_Users(&host, users, 4) == 2);
    DOCTEST_CHECK(users[0] == 1);
    DOCTEST_CHECK(users[1] == 2);

    const uint8_t direct_payload[] = {1, 2, 3};
    DOCTEST_REQUIRE(netadapter::Send_Game_Message(&guest, netadapter::Local_Network_Id(&host), 100, direct_payload, sizeof(direct_payload)));
    netadapter::s_game_packet packet;
    DOCTEST_REQUIRE(Wait_For_Packet_(&host, &packet));
    DOCTEST_CHECK(packet.sender_network_id == netadapter::Local_Network_Id(&guest));
    DOCTEST_CHECK(packet.target_network_id == netadapter::Local_Network_Id(&host));
    DOCTEST_CHECK(packet.message_type == 100);
    DOCTEST_CHECK(packet.payload == std::vector<uint8_t>{1, 2, 3});

    const uint8_t broadcast_payload[] = {9, 8};
    DOCTEST_REQUIRE(netadapter::Send_Game_Message(&host, netadapter::NETADAPTER_BROADCAST_INCLUDING_SELF, 101, broadcast_payload, sizeof(broadcast_payload)));
    packet = {};
    DOCTEST_REQUIRE(Wait_For_Packet_(&host, &packet));
    DOCTEST_CHECK(packet.message_type == 101);
    DOCTEST_CHECK(packet.payload == std::vector<uint8_t>{9, 8});
    packet = {};
    DOCTEST_REQUIRE(Wait_For_Packet_(&guest, &packet));
    DOCTEST_CHECK(packet.message_type == 101);
    DOCTEST_CHECK(packet.payload == std::vector<uint8_t>{9, 8});

    netadapter::Shutdown(&guest);
    DOCTEST_REQUIRE(Wait_For_Event_(&host, &event));
    DOCTEST_CHECK(event.kind == netadapter::EVENT_MEMBER_LEFT);
    DOCTEST_CHECK(event.network_id == 2);

    netadapter::Shutdown(&host);
    Router_Service_Request_Stop(&service);
    service_thread.join();
    service_guard.service = nullptr;
    service_guard.service_thread = nullptr;
    DOCTEST_CHECK(run_result == 0);
}

DOCTEST_TEST_CASE("adapter refresh updates cached session list when sessions disappear") {
    s_router_config service_config;
    Router_Copy_String(service_config.listen_host, sizeof(service_config.listen_host), "127.0.0.1");
    service_config.tcp_port = 0;
    service_config.enable_discovery = false;
    service_config.enable_signal_handlers = false;

    s_router_service service;
    Router_Service_Init(&service, &service_config);
    int run_result = -1;
    std::thread service_thread;
    Service_Thread_Guard_ service_guard{&service, &service_thread};
    DOCTEST_REQUIRE(Start_Test_Service_(&service, &service_thread, &run_result));

    netadapter::s_adapter host;
    netadapter::s_adapter browser;
    netadapter::s_config host_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "host");
    netadapter::s_config browser_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "browser");
    netadapter::Init(&host, &host_config);
    netadapter::Init(&browser, &browser_config);

    DOCTEST_REQUIRE(netadapter::Connect(&host));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&host));
    DOCTEST_REQUIRE(netadapter::Create_Game(&host, "Temporary", 4, "Host"));
    DOCTEST_REQUIRE(Wait_For_Session_(&host));

    DOCTEST_REQUIRE(netadapter::Connect(&browser));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&browser));

    uint32_t revision = netadapter::Game_List_Revision(&browser);
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_List_Revision_(&browser, revision));
    DOCTEST_CHECK(netadapter::Game_Count(&browser) == 1);

    netadapter::Shutdown(&host);
    revision = netadapter::Game_List_Revision(&browser);
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_List_Revision_(&browser, revision));
    DOCTEST_CHECK(netadapter::Game_Count(&browser) == 0);

    netadapter::Shutdown(&browser);
    Router_Service_Request_Stop(&service);
    service_thread.join();
    service_guard.service = nullptr;
    service_guard.service_thread = nullptr;
    DOCTEST_CHECK(run_result == 0);
}

DOCTEST_TEST_CASE("adapter falls back to a valid name for unnamed hosted sessions") {
    s_router_config service_config;
    Router_Copy_String(service_config.listen_host, sizeof(service_config.listen_host), "127.0.0.1");
    service_config.tcp_port = 0;
    service_config.enable_discovery = false;
    service_config.enable_signal_handlers = false;

    s_router_service service;
    Router_Service_Init(&service, &service_config);
    int run_result = -1;
    std::thread service_thread;
    Service_Thread_Guard_ service_guard{&service, &service_thread};
    DOCTEST_REQUIRE(Start_Test_Service_(&service, &service_thread, &run_result));

    netadapter::s_adapter host;
    netadapter::s_adapter browser;
    netadapter::s_config host_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "host");
    netadapter::s_config browser_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "browser");
    netadapter::Init(&host, &host_config);
    netadapter::Init(&browser, &browser_config);

    DOCTEST_REQUIRE(netadapter::Connect(&host));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&host));
    DOCTEST_REQUIRE(netadapter::Create_Game(&host, "", 4, "Host"));
    DOCTEST_REQUIRE(Wait_For_Session_(&host));

    DOCTEST_REQUIRE(netadapter::Connect(&browser));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&browser));
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_Count_(&browser, 1));
    DOCTEST_CHECK(std::strcmp(netadapter::Game_Name(&browser, 0), "Game") == 0);

    netadapter::Shutdown(&browser);
    netadapter::Shutdown(&host);
    Router_Service_Request_Stop(&service);
    service_thread.join();
    service_guard.service = nullptr;
    service_guard.service_thread = nullptr;
    DOCTEST_CHECK(run_result == 0);
}

DOCTEST_TEST_CASE("adapter refresh hides games closed to new players") {
    s_router_config service_config;
    Router_Copy_String(service_config.listen_host, sizeof(service_config.listen_host), "127.0.0.1");
    service_config.tcp_port = 0;
    service_config.enable_discovery = false;
    service_config.enable_signal_handlers = false;

    s_router_service service;
    Router_Service_Init(&service, &service_config);
    int run_result = -1;
    std::thread service_thread;
    Service_Thread_Guard_ service_guard{&service, &service_thread};
    DOCTEST_REQUIRE(Start_Test_Service_(&service, &service_thread, &run_result));

    netadapter::s_adapter host;
    netadapter::s_adapter browser;
    netadapter::s_config host_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "host");
    netadapter::s_config browser_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "browser");
    netadapter::Init(&host, &host_config);
    netadapter::Init(&browser, &browser_config);

    DOCTEST_REQUIRE(netadapter::Connect(&host));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&host));
    DOCTEST_REQUIRE(netadapter::Create_Game(&host, "Started", 4, "Host"));
    DOCTEST_REQUIRE(Wait_For_Session_(&host));

    DOCTEST_REQUIRE(netadapter::Connect(&browser));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&browser));
    uint32_t revision = netadapter::Game_List_Revision(&browser);
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_List_Revision_(&browser, revision));
    DOCTEST_CHECK(netadapter::Game_Count(&browser) == 1);

    DOCTEST_REQUIRE(netadapter::Set_New_Players_Enabled(&host, false));
    Pump_For_(&host, std::chrono::milliseconds(50));
    revision = netadapter::Game_List_Revision(&browser);
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_List_Revision_(&browser, revision));
    DOCTEST_CHECK(netadapter::Game_Count(&browser) == 0);

    netadapter::Shutdown(&browser);
    netadapter::Shutdown(&host);
    Router_Service_Request_Stop(&service);
    service_thread.join();
    service_guard.service = nullptr;
    service_guard.service_thread = nullptr;
    DOCTEST_CHECK(run_result == 0);
}

DOCTEST_TEST_CASE("adapter LAN discovery does not use localhost fallback") {
    s_router_config service_config;
    Router_Copy_String(service_config.listen_host, sizeof(service_config.listen_host), "127.0.0.1");
    service_config.tcp_port = 0;
    service_config.discovery_port = 0;
    service_config.enable_discovery = true;
    service_config.enable_signal_handlers = false;

    s_router_service service;
    Router_Service_Init(&service, &service_config);
    int run_result = -1;
    std::thread service_thread;
    Service_Thread_Guard_ service_guard{&service, &service_thread};
    DOCTEST_REQUIRE(Start_Test_Service_(&service, &service_thread, &run_result));
    DOCTEST_REQUIRE(Router_Service_Bound_Discovery_Port(&service) != 0);

    netadapter::s_adapter host;
    netadapter::s_config host_config = Make_Config_(Router_Service_Bound_Tcp_Port(&service), "host");
    netadapter::Init(&host, &host_config);
    DOCTEST_REQUIRE(netadapter::Connect(&host));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&host));
    DOCTEST_REQUIRE(netadapter::Create_Game(&host, "Discovered", 4, "Host"));
    DOCTEST_REQUIRE(Wait_For_Session_(&host));

    netadapter::s_adapter browser;
    netadapter::s_config browser_config = Make_Config_(0, "browser");
    browser_config.bootstrap_mode = netadapter::BOOTSTRAP_LAN_DISCOVERY;
    browser_config.discovery_port = Router_Service_Bound_Discovery_Port(&service);
    browser_config.discovery_timeout_ms = 500;
    netadapter::Init(&browser, &browser_config);

    DOCTEST_REQUIRE(netadapter::Connect(&browser));
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_List_Revision_(&browser, 0));
    DOCTEST_CHECK(netadapter::Game_Count(&browser) == 0);
    DOCTEST_CHECK_FALSE(netadapter::Is_Connected(&browser));

    netadapter::Shutdown(&browser);
    netadapter::Shutdown(&host);
    Router_Service_Request_Stop(&service);
    service_thread.join();
    service_guard.service = nullptr;
    service_guard.service_thread = nullptr;
    DOCTEST_CHECK(run_result == 0);
}

DOCTEST_TEST_CASE("adapter LAN discovery without routers succeeds with empty list") {
    netadapter::s_adapter browser;
    netadapter::s_config browser_config = Make_Config_(0, "browser");
    browser_config.bootstrap_mode = netadapter::BOOTSTRAP_LAN_DISCOVERY;
    browser_config.discovery_port = 9;
    browser_config.discovery_timeout_ms = 50;
    netadapter::Init(&browser, &browser_config);

    DOCTEST_REQUIRE(netadapter::Connect(&browser));
    DOCTEST_CHECK_FALSE(netadapter::Is_Connected(&browser));
    DOCTEST_REQUIRE(netadapter::Refresh_Game_List(&browser));
    DOCTEST_REQUIRE(Wait_For_Game_List_Revision_(&browser, 0));
    DOCTEST_CHECK(netadapter::Game_Count(&browser) == 0);

    netadapter::Shutdown(&browser);
}

DOCTEST_TEST_CASE("adapter embedded LAN host starts local router and creates a session") {
    netadapter::s_adapter host;
    netadapter::s_config host_config = Make_Config_(0, "host");
    host_config.bootstrap_mode = netadapter::BOOTSTRAP_EMBEDDED_LAN_HOST;
    host_config.tcp_port = 0;
    host_config.discovery_port = 0;
    host_config.discovery_timeout_ms = 1000;
    netadapter::Init(&host, &host_config);

    DOCTEST_REQUIRE(netadapter::Connect(&host));
    DOCTEST_REQUIRE(Wait_For_Local_Id_(&host));
    DOCTEST_REQUIRE(netadapter::Create_Game(&host, "Embedded", 4, "Host"));
    DOCTEST_REQUIRE(Wait_For_Session_(&host));
    DOCTEST_REQUIRE(Wait_For_Users_(&host, 1));

    netadapter::Shutdown(&host);
}

}