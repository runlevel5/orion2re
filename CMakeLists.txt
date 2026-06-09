cmake_minimum_required(VERSION 3.20)

if(MSVC AND WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports 32-bit MSVC Windows builds. Reconfigure with -A Win32.")
endif()

project(orion2re LANGUAGES CXX)

set(ORION2RE_VENDORED_SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/SDL")
set(ORION2RE_VENDORED_SDL3_COMMIT "8e37db5e797b6167f3a00d697d816a684bd259c7")

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
endif()

set(ORION2RE_GAME_DIR "" CACHE PATH "Optional game directory used for runtime output and debugger working directory")

set(ORION2RE_LOCAL_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/cmake/local-user.cmake")
if(EXISTS "${ORION2RE_LOCAL_CONFIG}")
    include("${ORION2RE_LOCAL_CONFIG}")
endif()

option(ORION2RE_ENABLE_ASAN "Enable AddressSanitizer for supported MSVC debug builds" OFF)
option(ORION2RE_RELEASE_DEBUG_INFO "Emit debug info for non-Debug MSVC builds" OFF)

if(NOT EXISTS "${ORION2RE_VENDORED_SDL3_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "This project requires the SDL3 submodule at vendors/SDL. "
        "Run 'git submodule update --init --recursive vendors/SDL' and ensure it is pinned to ${ORION2RE_VENDORED_SDL3_COMMIT}."
    )
endif()

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory("${ORION2RE_VENDORED_SDL3_DIR}" EXCLUDE_FROM_ALL)

if(ORION2RE_ENABLE_ASAN AND NOT MSVC)
    message(FATAL_ERROR "ORION2RE_ENABLE_ASAN is only supported with MSVC builds.")
endif()

if(MSVC AND WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4 AND ORION2RE_GAME_DIR)
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

add_executable(orion2re
    ${ORION2RE_SOURCES}
    ${ORION2RE_HEADERS}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES
    ${ORION2RE_SOURCES}
    ${ORION2RE_HEADERS}
)

target_compile_features(orion2re PRIVATE cxx_std_20)
target_include_directories(orion2re PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_definitions(orion2re PRIVATE _WINDOWS SDL_MAIN_HANDLED)

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
    target_link_options(orion2re PRIVATE /MANIFESTUAC:NO)

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_precompile_headers(orion2re PRIVATE
            "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
        )

        target_compile_definitions(orion2re PRIVATE
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

        if(ORION2RE_ENABLE_ASAN)
            if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
                message(FATAL_ERROR "ORION2RE_ENABLE_ASAN requires a Debug build.")
            endif()

            target_compile_options(orion2re PRIVATE /fsanitize=address)
            target_link_options(orion2re PRIVATE /INFERASANLIBS /fsanitize=address)
        endif()

        set(ORION2RE_IMPORT_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/import-libs")
        set(ORION2RE_LIB_TOOL "${CMAKE_LIB}")
        if(NOT ORION2RE_LIB_TOOL)
            set(ORION2RE_LIB_TOOL lib)
        endif()

        add_custom_command(
            OUTPUT
                "${ORION2RE_IMPORT_LIB_DIR}/smackw32.lib"
                "${ORION2RE_IMPORT_LIB_DIR}/dplayx.lib"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${ORION2RE_IMPORT_LIB_DIR}"
            COMMAND "${ORION2RE_LIB_TOOL}"
                "/def:${CMAKE_CURRENT_SOURCE_DIR}/smackw32.def"
                /machine:x86
                /name:SMACKW32.DLL
                "/out:${ORION2RE_IMPORT_LIB_DIR}/smackw32.lib"
            COMMAND "${ORION2RE_LIB_TOOL}"
                "/def:${CMAKE_CURRENT_SOURCE_DIR}/dplayx.def"
                /machine:x86
                /name:DPLAYX.DLL
                "/out:${ORION2RE_IMPORT_LIB_DIR}/dplayx.lib"
            DEPENDS
                "${CMAKE_CURRENT_SOURCE_DIR}/smackw32.def"
                "${CMAKE_CURRENT_SOURCE_DIR}/dplayx.def"
            VERBATIM
        )

        add_custom_target(orion2re_win32_importlibs
            DEPENDS
                "${ORION2RE_IMPORT_LIB_DIR}/smackw32.lib"
                "${ORION2RE_IMPORT_LIB_DIR}/dplayx.lib"
        )

        add_dependencies(orion2re orion2re_win32_importlibs)

        target_link_libraries(orion2re PRIVATE
            "${ORION2RE_IMPORT_LIB_DIR}/smackw32.lib"
            "${ORION2RE_IMPORT_LIB_DIR}/dplayx.lib"
            winmm
            dsound
            ddraw
            SDL3::SDL3-static
        )

        target_link_options(orion2re PRIVATE
            /DYNAMICBASE
            /NXCOMPAT
            $<$<CONFIG:Release>:/DEBUG>
        )
    endif()
elseif(WIN32)
    message(WARNING "The CMake conversion preserves the original Visual Studio/MSVC-specific Windows build behavior; non-MSVC Windows builds may need additional link setup.")
endif()