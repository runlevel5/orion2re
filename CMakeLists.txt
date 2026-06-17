cmake_minimum_required(VERSION 3.20)

project(orion2re LANGUAGES C CXX)

include(CTest)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ORION2RE_TARGET_ARCH x86)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ORION2RE_TARGET_ARCH x64)
    else()
        message(FATAL_ERROR "Linux builds are only supported for x86 and x64 targets.")
    endif()
elseif(APPLE)
    set(ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG -mlittle-endian)
elseif(WIN32)
    set(ORION2RE_WINDOWS_ARCH "" CACHE STRING "Target Windows architecture (x86 or x64)")
    set_property(CACHE ORION2RE_WINDOWS_ARCH PROPERTY STRINGS x86 x64)

    string(TOLOWER "${ORION2RE_WINDOWS_ARCH}" ORION2RE_WINDOWS_ARCH_NORMALIZED)

    if(NOT ORION2RE_WINDOWS_ARCH_NORMALIZED)
        set(ORION2RE_WINDOWS_ARCH_HINT "")

        if(CMAKE_VS_PLATFORM_NAME)
            set(ORION2RE_WINDOWS_ARCH_HINT "${CMAKE_VS_PLATFORM_NAME}")
        elseif(CMAKE_GENERATOR_PLATFORM)
            set(ORION2RE_WINDOWS_ARCH_HINT "${CMAKE_GENERATOR_PLATFORM}")
        elseif(DEFINED ENV{VSCMD_ARG_TGT_ARCH})
            set(ORION2RE_WINDOWS_ARCH_HINT "$ENV{VSCMD_ARG_TGT_ARCH}")
        endif()

        string(TOLOWER "${ORION2RE_WINDOWS_ARCH_HINT}" ORION2RE_WINDOWS_ARCH_HINT)

        if(ORION2RE_WINDOWS_ARCH_HINT STREQUAL "win32" OR ORION2RE_WINDOWS_ARCH_HINT STREQUAL "x86")
            set(ORION2RE_WINDOWS_ARCH_NORMALIZED x86)
        elseif(ORION2RE_WINDOWS_ARCH_HINT STREQUAL "x64" OR ORION2RE_WINDOWS_ARCH_HINT STREQUAL "amd64")
            set(ORION2RE_WINDOWS_ARCH_NORMALIZED x64)
        endif()

        if(ORION2RE_WINDOWS_ARCH_NORMALIZED)
            set(ORION2RE_WINDOWS_ARCH "${ORION2RE_WINDOWS_ARCH_NORMALIZED}" CACHE STRING "Target Windows architecture (x86 or x64)" FORCE)
            set_property(CACHE ORION2RE_WINDOWS_ARCH PROPERTY STRINGS x86 x64)
        endif()
    endif()

    if(NOT ORION2RE_WINDOWS_ARCH_NORMALIZED)
        message(FATAL_ERROR
            "Select a Windows target architecture with -DORION2RE_WINDOWS_ARCH=x86 or x64 "
            "and configure the generator or toolchain for the same target."
        )
    elseif(ORION2RE_WINDOWS_ARCH_NORMALIZED STREQUAL "x86")
        set(ORION2RE_TARGET_ARCH x86)
        set(ORION2RE_WINDOWS_POINTER_SIZE 4)
    elseif(ORION2RE_WINDOWS_ARCH_NORMALIZED STREQUAL "x64")
        set(ORION2RE_TARGET_ARCH x64)
        set(ORION2RE_WINDOWS_POINTER_SIZE 8)
    else()
        message(FATAL_ERROR "Unsupported ORION2RE_WINDOWS_ARCH value '${ORION2RE_WINDOWS_ARCH}'. Use x86 or x64.")
    endif()

    if(CMAKE_SIZEOF_VOID_P AND NOT CMAKE_SIZEOF_VOID_P EQUAL ORION2RE_WINDOWS_POINTER_SIZE)
        if(MSVC)
            message(FATAL_ERROR
                "ORION2RE_WINDOWS_ARCH=${ORION2RE_WINDOWS_ARCH_NORMALIZED} does not match the configured MSVC generator platform. "
                "Use -A Win32 for x86 or -A x64 for x64."
            )
        else()
            message(FATAL_ERROR
                "ORION2RE_WINDOWS_ARCH=${ORION2RE_WINDOWS_ARCH_NORMALIZED} does not match the configured Windows toolchain target."
            )
        endif()
    endif()
endif()

set(ORION2RE_VENDORED_SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/SDL")
set(ORION2RE_VENDORED_SDL3_COMMIT "8e37db5e797b6167f3a00d697d816a684bd259c7")
set(ORION2RE_VENDORED_DOCTEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/doctest")
set(ORION2RE_VENDORED_DOCTEST_COMMIT "1da23a3e8119ec5cce4f9388e91b065e20bf06f5")
set(ORION2RE_VENDORED_LIBUV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/libuv")
set(ORION2RE_VENDORED_LIBUV_COMMIT "5152db2cbfeb5582e9c27c5ea1dba2cd9e10759b")
set(ORION2RE_VENDORED_LIBSMACKER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/libsmacker")

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
endif()

set(ORION2RE_GAME_DIR "" CACHE PATH "Optional game directory used for runtime output and debugger working directory")

set(ORION2RE_LOCAL_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/cmake/local-user.cmake")
if(EXISTS "${ORION2RE_LOCAL_CONFIG}")
    include("${ORION2RE_LOCAL_CONFIG}")
endif()

option(ORION2RE_ENABLE_ASAN "Enable AddressSanitizer for supported debug builds" OFF)
option(ORION2RE_RELEASE_DEBUG_INFO "Emit debug info for non-Debug MSVC builds" OFF)
option(ORION2RE_BUILD_TESTS "Build tests" OFF)
option(ORION2RE_EXCLUDE_RUNTIME_FROM_ALL "Exclude runtime executables from the default all target" ${ORION2RE_BUILD_TESTS})

set(ORION2RE_TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests")

if(ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG)
    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)

    check_c_compiler_flag("${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG}" ORION2RE_C_SUPPORTS_LITTLE_ENDIAN_FLAG)
    check_cxx_compiler_flag("${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG}" ORION2RE_CXX_SUPPORTS_LITTLE_ENDIAN_FLAG)

    if(NOT ORION2RE_C_SUPPORTS_LITTLE_ENDIAN_FLAG OR NOT ORION2RE_CXX_SUPPORTS_LITTLE_ENDIAN_FLAG)
        message(FATAL_ERROR "The configured compiler must support ${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG} for macOS builds.")
    endif()
endif()

if(NOT EXISTS "${ORION2RE_VENDORED_SDL3_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "This project requires the SDL3 submodule at vendors/SDL. "
        "Run 'git submodule update --init --recursive vendors/SDL' and ensure it is pinned to ${ORION2RE_VENDORED_SDL3_COMMIT}."
    )
endif()

if(ORION2RE_BUILD_TESTS AND NOT EXISTS "${ORION2RE_VENDORED_DOCTEST_DIR}/doctest/doctest.h")
    message(FATAL_ERROR
        "Tests require the doctest submodule at vendors/doctest. "
        "Run 'git submodule update --init --recursive vendors/doctest' and ensure it is pinned to ${ORION2RE_VENDORED_DOCTEST_COMMIT}."
    )
endif()

if(NOT EXISTS "${ORION2RE_VENDORED_LIBUV_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "The router service requires the libuv submodule at vendors/libuv. "
        "Run 'git submodule update --init --recursive vendors/libuv' and ensure it is pinned to ${ORION2RE_VENDORED_LIBUV_COMMIT}."
    )
endif()

if(NOT EXISTS "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smacker.c")
    message(FATAL_ERROR
        "This project requires libsmacker sources at vendors/libsmacker."
    )
endif()

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory("${ORION2RE_VENDORED_SDL3_DIR}" EXCLUDE_FROM_ALL)

set(LIBUV_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(LIBUV_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LIBUV_BUILD_BENCH OFF CACHE BOOL "" FORCE)

add_subdirectory("${ORION2RE_VENDORED_LIBUV_DIR}" EXCLUDE_FROM_ALL)

if(MSVC AND WIN32 AND ORION2RE_GAME_DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ORION2RE_GAME_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ORION2RE_GAME_DIR}")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_DEBUG "${ORION2RE_GAME_DIR}")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE "${ORION2RE_GAME_DIR}")
    set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG "${ORION2RE_GAME_DIR}")
    set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE "${ORION2RE_GAME_DIR}")
endif()

file(GLOB_RECURSE ORION2RE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE ORION2RE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

list(REMOVE_ITEM ORION2RE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game/comcore.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game/transprt.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game/winnet.cpp"
)

set(ORION2RE_ROUTER_SOURCES
    src/network/router_cli.cpp
    src/network/router_core.cpp
    src/network/router_protocol.cpp
    src/network/router_service.cpp
)

add_executable(orion2re
    ${ORION2RE_SOURCES}
    ${ORION2RE_HEADERS}
    "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smacker.c"
    "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smk_bitstream.c"
    "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smk_hufftree.c"
)

if(ORION2RE_EXCLUDE_RUNTIME_FROM_ALL)
    set_target_properties(orion2re PROPERTIES EXCLUDE_FROM_ALL YES)
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES
    ${ORION2RE_SOURCES}
    ${ORION2RE_HEADERS}
)

target_compile_features(orion2re PRIVATE cxx_std_20)
target_include_directories(orion2re PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${ORION2RE_VENDORED_LIBSMACKER_DIR}"
)

target_precompile_headers(orion2re PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)

target_link_libraries(orion2re PRIVATE
    SDL3::SDL3-static
    uv_a
)

add_executable(orion2re-router
    tools/router_main.cpp
    ${ORION2RE_ROUTER_SOURCES}
)

if(ORION2RE_EXCLUDE_RUNTIME_FROM_ALL)
    set_target_properties(orion2re-router PROPERTIES EXCLUDE_FROM_ALL YES)
endif()

target_compile_features(orion2re-router PRIVATE cxx_std_20)
target_include_directories(orion2re-router PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(orion2re-router PRIVATE
    uv_a
)

if(ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG)
    target_compile_options(orion2re PRIVATE "${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG}")
    target_compile_options(orion2re-router PRIVATE "${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG}")
endif()

if(WIN32)
    set_target_properties(orion2re PROPERTIES WIN32_EXECUTABLE YES)
endif()

if(ORION2RE_GAME_DIR)
    set_target_properties(orion2re PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${ORION2RE_GAME_DIR}"
    )
endif()

if(MSVC)
    target_compile_options(orion2re PRIVATE /W3 /permissive-)
    target_compile_options(orion2re-router PRIVATE /W3 /permissive-)
    target_link_options(orion2re PRIVATE /MANIFESTUAC:NO)

    target_compile_definitions(orion2re PRIVATE
        WIN32
        $<$<CONFIG:Debug>:_CRT_SECURE_NO_DEPRECATE>
        $<$<CONFIG:Debug>:_CRT_NONSTDC_NO_DEPRECATE>
    )

    target_compile_definitions(orion2re-router PRIVATE
        WIN32
        $<$<CONFIG:Debug>:_CRT_SECURE_NO_DEPRECATE>
        $<$<CONFIG:Debug>:_CRT_NONSTDC_NO_DEPRECATE>
    )

    target_compile_options(orion2re PRIVATE
        $<$<CONFIG:Debug>:/RTC1>
        $<$<CONFIG:Debug>:/GS>
        $<$<CONFIG:Release>:/Gy>
        $<$<CONFIG:Release>:/Oi>
        $<$<CONFIG:Release>:/GS>
        $<$<AND:$<CONFIG:Release>,$<BOOL:${ORION2RE_RELEASE_DEBUG_INFO}>>:/Zi>
        $<$<AND:$<CONFIG:Release>,$<BOOL:${ORION2RE_RELEASE_DEBUG_INFO}>>:/Zo>
    )

    target_link_options(orion2re PRIVATE
        /DYNAMICBASE
        /NXCOMPAT
        $<$<CONFIG:Release>:/OPT:REF>
        $<$<CONFIG:Release>:/OPT:ICF>
        $<$<AND:$<CONFIG:Release>,$<BOOL:${ORION2RE_RELEASE_DEBUG_INFO}>>:/DEBUG>
    )
endif()

if(ORION2RE_ENABLE_ASAN)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "ORION2RE_ENABLE_ASAN requires a Debug build.")
    endif()

    if(MSVC)
        target_compile_options(orion2re PRIVATE /fsanitize=address)
        target_compile_options(orion2re-router PRIVATE /fsanitize=address)
        target_link_options(orion2re PRIVATE /INFERASANLIBS /fsanitize=address)
        target_link_options(orion2re-router PRIVATE /INFERASANLIBS /fsanitize=address)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(orion2re PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_compile_options(orion2re-router PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(orion2re PRIVATE -fsanitize=address)
        target_link_options(orion2re-router PRIVATE -fsanitize=address)
    else()
        message(FATAL_ERROR "ORION2RE_ENABLE_ASAN is unsupported for the current compiler.")
    endif()
endif()

if(ORION2RE_BUILD_TESTS)
    include("${ORION2RE_TESTS_DIR}/tests.cmake")
endif()