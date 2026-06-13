function(orion2re_add_unit_test TARGET_NAME)
    cmake_parse_arguments(ORION2RE_TEST "" "" "SOURCES;PRODUCTION_SOURCES;FAKE_SOURCES" ${ARGN})

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

    target_link_libraries(${TARGET_NAME} PRIVATE
        SDL3::SDL3-static
    )

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W3 /permissive-)
        target_precompile_headers(${TARGET_NAME} PRIVATE
            "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
        )
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
)