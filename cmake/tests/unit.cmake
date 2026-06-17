function(orion2re_add_unit_test TARGET_NAME)
    cmake_parse_arguments(ORION2RE_TEST "LINK_SDL" "" "SOURCES;PRODUCTION_SOURCES;FAKE_SOURCES;LIBRARIES" ${ARGN})

    add_executable(${TARGET_NAME}
        tests/test_main.cpp
        ${ORION2RE_TEST_SOURCES}
        ${ORION2RE_TEST_PRODUCTION_SOURCES}
        ${ORION2RE_TEST_FAKE_SOURCES}
        src/pch.cpp
    )

    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
    target_include_directories(${TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${ORION2RE_VENDORED_DOCTEST_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests"
    )

    target_precompile_headers(${TARGET_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
    )

    target_link_libraries(${TARGET_NAME} PRIVATE SDL3::Headers)

    if(ORION2RE_TEST_LINK_SDL)
        target_link_libraries(${TARGET_NAME} PRIVATE SDL3::SDL3-static)
    endif()

    target_link_libraries(${TARGET_NAME} PRIVATE
        ${ORION2RE_TEST_LIBRARIES}
    )

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W3 /permissive-)
        target_compile_definitions(${TARGET_NAME} PRIVATE
            WIN32
            DOCTEST_CONFIG_NO_WINDOWS_SEH
            DOCTEST_CONFIG_NO_POSIX_SIGNALS
            $<$<CONFIG:Debug>:_CRT_SECURE_NO_DEPRECATE>
            $<$<CONFIG:Debug>:_CRT_NONSTDC_NO_DEPRECATE>
        )
    endif()

    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

orion2re_add_unit_test(test_fields_helpers
    SOURCES
        tests/unit/test_fields_helpers.cpp
    FAKE_SOURCES
        tests/fakes/fake_exitm.cpp
)

orion2re_add_unit_test(test_video_scan_bounds
    SOURCES
        tests/unit/test_video_scan_bounds.cpp
    PRODUCTION_SOURCES
        src/game/setvesa.cpp
    FAKE_SOURCES
        tests/fakes/fake_video.cpp
)

orion2re_add_unit_test(test_fields
    SOURCES
        tests/unit/test_fields.cpp
    PRODUCTION_SOURCES
        src/game/fields.cpp
    FAKE_SOURCES
        tests/fakes/fake_animate.cpp
        tests/fakes/fake_exitm.cpp
        tests/fakes/fake_fields_runtime.cpp
        tests/fakes/fake_input.cpp
        tests/fakes/fake_memory.cpp
        tests/fakes/fake_mmio.cpp
        tests/fakes/fake_sound.cpp
        tests/fakes/fake_textbox.cpp
        tests/fakes/fake_video.cpp
    LINK_SDL
)

orion2re_add_unit_test(test_router_protocol
    SOURCES
        tests/unit/test_router_protocol.cpp
    PRODUCTION_SOURCES
        src/network/router_protocol.cpp
    LIBRARIES
        orion2re_lz4
)

orion2re_add_unit_test(test_router_core
    SOURCES
        tests/unit/test_router_core.cpp
    PRODUCTION_SOURCES
        src/network/router_core.cpp
        src/network/router_protocol.cpp
    LIBRARIES
        orion2re_lz4
)

orion2re_add_unit_test(test_router_service
    SOURCES
        tests/unit/test_router_service.cpp
    PRODUCTION_SOURCES
        src/network/router_core.cpp
        src/network/router_protocol.cpp
        src/network/router_service.cpp
    LIBRARIES
        uv_a
        orion2re_lz4
    LINK_SDL
)

orion2re_add_unit_test(test_router_session_browser
    SOURCES
        tests/unit/test_router_session_browser.cpp
    PRODUCTION_SOURCES
        src/network/router_client.cpp
        src/network/router_core.cpp
        src/network/router_protocol.cpp
        src/network/router_session_browser.cpp
        src/network/router_service.cpp
    LIBRARIES
        uv_a
        orion2re_lz4
    LINK_SDL
)

orion2re_add_unit_test(test_netadapter
    SOURCES
        tests/unit/test_netadapter.cpp
    PRODUCTION_SOURCES
        src/game/netadapter.cpp
        src/network/router_client.cpp
        src/network/router_core.cpp
        src/network/router_protocol.cpp
        src/network/router_session_browser.cpp
        src/network/router_service.cpp
    LIBRARIES
        uv_a
        orion2re_lz4
    LINK_SDL
)

orion2re_add_unit_test(test_router_client
    SOURCES
        tests/unit/test_router_client.cpp
    PRODUCTION_SOURCES
        src/network/router_client.cpp
        src/network/router_core.cpp
        src/network/router_protocol.cpp
        src/network/router_service.cpp
    LIBRARIES
        uv_a
        orion2re_lz4
    LINK_SDL
)