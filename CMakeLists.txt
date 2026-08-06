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
set(ORION2RE_VENDORED_LZ4_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/lz4")
set(ORION2RE_VENDORED_LZ4_COMMIT "0774d05537f9762f838f7ab541b7765f1a729cb5")
set(ORION2RE_VENDORED_MINIZ_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/miniz")
set(ORION2RE_VENDORED_MINIZ_COMMIT "77d0dce8627735138c51770d1799a1ef48f2117d")
set(ORION2RE_VENDORED_LIBSMACKER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/libsmacker")
set(ORION2RE_VENDORED_YYJSON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/yyjson")
set(ORION2RE_VENDORED_YYJSON_COMMIT "8b4a38dc994a110abaec8a400615567bd996105f")

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
endif()

set(ORION2RE_GAME_DIR "" CACHE PATH "Optional game directory used for runtime output and debugger working directory")
set(ORION2RE_SUPPORTED_MODS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/mods")
set(ORION2RE_PACKAGED_MODS_DIR "${CMAKE_CURRENT_BINARY_DIR}/data/mods")

set(ORION2RE_LOCAL_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/cmake/local-user.cmake")
if(EXISTS "${ORION2RE_LOCAL_CONFIG}")
    include("${ORION2RE_LOCAL_CONFIG}")
endif()

option(ORION2RE_ENABLE_ASAN "Enable AddressSanitizer for supported debug builds" OFF)
option(ORION2RE_RELEASE_DEBUG_INFO "Emit debug info for non-Debug MSVC builds" OFF)
option(ORION2RE_BUILD_GAME "Build the SDL game executable" ON)
option(ORION2RE_BUILD_TESTS "Build tests" OFF)
set(ORION2RE_BUILD_CONFIG_TOOL_DEFAULT OFF)
if(NOT CMAKE_CROSSCOMPILING AND
   EXISTS "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.h" AND
   EXISTS "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.c")
    set(ORION2RE_BUILD_CONFIG_TOOL_DEFAULT ON)
endif()
option(ORION2RE_BUILD_CONFIG_TOOL
       "Build configuration tools and regenerate bindings when their inputs change"
       ${ORION2RE_BUILD_CONFIG_TOOL_DEFAULT})
option(ORION2RE_ENFORCE_ORIGINAL_STRUCT_SIZES "Require structures to retain their original game sizes" OFF)
option(ORION2RE_EXCLUDE_RUNTIME_FROM_ALL "Exclude runtime executables from the default all target" ${ORION2RE_BUILD_TESTS})

if(ORION2RE_BUILD_CONFIG_TOOL AND
   (NOT EXISTS "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.h" OR
    NOT EXISTS "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.c"))
    message(FATAL_ERROR
        "Configuration tooling requires the yyjson submodule at vendors/yyjson. "
        "Run 'git submodule update --init vendors/yyjson' and ensure it is pinned to ${ORION2RE_VENDORED_YYJSON_COMMIT}."
    )
endif()

if((ORION2RE_BUILD_GAME OR ORION2RE_BUILD_TESTS) AND
   (NOT EXISTS "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.h" OR
    NOT EXISTS "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.c"))
    message(FATAL_ERROR
        "Runtime mod loading requires the yyjson submodule at vendors/yyjson. "
        "Run 'git submodule update --init vendors/yyjson' and ensure it is pinned to ${ORION2RE_VENDORED_YYJSON_COMMIT}."
    )
endif()

if(ORION2RE_ENFORCE_ORIGINAL_STRUCT_SIZES)
    add_compile_definitions(ORION2RE_ENFORCE_ORIGINAL_STRUCT_SIZES=1)
endif()

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

if(ORION2RE_BUILD_TESTS AND NOT ORION2RE_BUILD_GAME)
    message(FATAL_ERROR "ORION2RE_BUILD_TESTS currently requires ORION2RE_BUILD_GAME=ON because the test targets depend on SDL.")
endif()

if((ORION2RE_BUILD_GAME OR ORION2RE_BUILD_TESTS) AND NOT EXISTS "${ORION2RE_VENDORED_SDL3_DIR}/CMakeLists.txt")
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

if(NOT EXISTS "${ORION2RE_VENDORED_LZ4_DIR}/lib/lz4.h" OR NOT EXISTS "${ORION2RE_VENDORED_LZ4_DIR}/lib/lz4.c")
    message(FATAL_ERROR
        "The router protocol requires the lz4 submodule at vendors/lz4. "
        "Run 'git submodule update --init --recursive vendors/lz4' and ensure it is pinned to ${ORION2RE_VENDORED_LZ4_COMMIT}."
    )
endif()

if((ORION2RE_BUILD_GAME OR ORION2RE_BUILD_TESTS) AND
   (NOT EXISTS "${ORION2RE_VENDORED_MINIZ_DIR}/CMakeLists.txt" OR
    NOT EXISTS "${ORION2RE_VENDORED_MINIZ_DIR}/miniz.h" OR
    NOT EXISTS "${ORION2RE_VENDORED_MINIZ_DIR}/miniz.c"))
    message(FATAL_ERROR
        "VFS zip support requires the miniz submodule at vendors/miniz. "
        "Run 'git submodule update --init --recursive vendors/miniz' and ensure it is pinned to ${ORION2RE_VENDORED_MINIZ_COMMIT}."
    )
endif()

if(NOT EXISTS "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smacker.c")
    message(FATAL_ERROR
        "This project requires libsmacker sources at vendors/libsmacker."
    )
endif()

if(ORION2RE_BUILD_GAME OR ORION2RE_BUILD_TESTS)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)

    add_subdirectory("${ORION2RE_VENDORED_SDL3_DIR}" EXCLUDE_FROM_ALL)

    if(UNIX AND NOT APPLE AND SDL_UNIX_CONSOLE_BUILD AND ORION2RE_BUILD_GAME)
        message(WARNING
            "SDL_UNIX_CONSOLE_BUILD is enabled, so the orion2re game target is being built without desktop video backends and is not intended for gameplay. "
            "Use -DORION2RE_BUILD_GAME=OFF to build only orion2re-router."
        )
    endif()

    add_subdirectory("${ORION2RE_VENDORED_MINIZ_DIR}" EXCLUDE_FROM_ALL)
endif()

set(LIBUV_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(LIBUV_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LIBUV_BUILD_BENCH OFF CACHE BOOL "" FORCE)

add_subdirectory("${ORION2RE_VENDORED_LIBUV_DIR}" EXCLUDE_FROM_ALL)

add_library(orion2re_lz4 STATIC
    "${ORION2RE_VENDORED_LZ4_DIR}/lib/lz4.c"
)

set(ORION2RE_CONFIG_TOOL_SOURCES
    tools/config/config_tool.cpp
    tools/config/file_util.cpp
    tools/config/file_util.h
    tools/config/generate_cpp.cpp
    tools/config/generate_cpp.h
    tools/config/generate_docs.cpp
    tools/config/generate_docs.h
    tools/config/json5_reader.cpp
    tools/config/json5_reader.h
    tools/config/schema_model.cpp
    tools/config/schema_model.h
)

set(ORION2RE_CONFIG_SCHEMA "${CMAKE_CURRENT_SOURCE_DIR}/specs/config/main.json5")
set(ORION2RE_CONFIG_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/config/generated")
set(ORION2RE_CONFIG_GENERATED_FILES
    "${ORION2RE_CONFIG_GENERATED_DIR}/config.cpp"
)

if(ORION2RE_BUILD_CONFIG_TOOL)
    add_executable(config-tool EXCLUDE_FROM_ALL
        ${ORION2RE_CONFIG_TOOL_SOURCES}
        "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.c"
    )
    target_compile_features(config-tool PRIVATE cxx_std_20)
    target_include_directories(config-tool PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/tools/config"
        "${ORION2RE_VENDORED_YYJSON_DIR}/src"
    )

    add_custom_target(config-schema-validate
        COMMAND $<TARGET_FILE:config-tool> validate "${ORION2RE_CONFIG_SCHEMA}"
        DEPENDS config-tool "${ORION2RE_CONFIG_SCHEMA}"
        VERBATIM
    )

    set(ORION2RE_CONFIG_GENERATED_STAMP "${CMAKE_CURRENT_BINARY_DIR}/config-generated.stamp")
    add_custom_command(
        OUTPUT "${ORION2RE_CONFIG_GENERATED_STAMP}"
        BYPRODUCTS ${ORION2RE_CONFIG_GENERATED_FILES}
        COMMAND $<TARGET_FILE:config-tool> generate
            "${ORION2RE_CONFIG_SCHEMA}"
            "${ORION2RE_CONFIG_GENERATED_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${ORION2RE_CONFIG_GENERATED_STAMP}"
        DEPENDS
            config-tool
            "${ORION2RE_CONFIG_SCHEMA}"
            ${ORION2RE_CONFIG_TOOL_SOURCES}
        VERBATIM
    )

    add_custom_target(config-regenerate
        ALL
        DEPENDS "${ORION2RE_CONFIG_GENERATED_STAMP}"
    )

    set_source_files_properties(${ORION2RE_CONFIG_GENERATED_FILES} PROPERTIES
        GENERATED TRUE
        OBJECT_DEPENDS "${ORION2RE_CONFIG_GENERATED_STAMP}"
    )

    add_custom_target(config-generated-check
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_CURRENT_BINARY_DIR}/generated-check"
        COMMAND $<TARGET_FILE:config-tool> check
            "${ORION2RE_CONFIG_SCHEMA}"
            "${ORION2RE_CONFIG_GENERATED_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/generated-check"
        DEPENDS config-tool "${ORION2RE_CONFIG_SCHEMA}" ${ORION2RE_CONFIG_TOOL_SOURCES}
        VERBATIM
    )

    set(ORION2RE_CONFIG_DOCUMENTATION "${CMAKE_CURRENT_BINARY_DIR}/config-reference.html")
    add_custom_command(
        OUTPUT "${ORION2RE_CONFIG_DOCUMENTATION}"
        COMMAND $<TARGET_FILE:config-tool> docs
            "${ORION2RE_CONFIG_SCHEMA}"
            "${ORION2RE_CONFIG_DOCUMENTATION}"
        DEPENDS config-tool "${ORION2RE_CONFIG_SCHEMA}" ${ORION2RE_CONFIG_TOOL_SOURCES}
        VERBATIM
    )
    add_custom_target(config-docs ALL
        DEPENDS "${ORION2RE_CONFIG_DOCUMENTATION}"
    )
    install(FILES "${ORION2RE_CONFIG_DOCUMENTATION}"
        DESTINATION "docs"
    )
elseif((ORION2RE_BUILD_GAME OR ORION2RE_BUILD_TESTS) AND
       NOT EXISTS "${ORION2RE_CONFIG_GENERATED_DIR}/config.cpp")
    message(FATAL_ERROR
        "Building the game or tests requires generated configuration bindings. "
        "Enable ORION2RE_BUILD_CONFIG_TOOL and use a native build to generate them."
    )
endif()
target_include_directories(orion2re_lz4 PUBLIC
    "${ORION2RE_VENDORED_LZ4_DIR}/lib"
)

if(ORION2RE_BUILD_GAME)
    file(GLOB_RECURSE ORION2RE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    )

    file(GLOB_RECURSE ORION2RE_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    )

    list(REMOVE_ITEM ORION2RE_SOURCES ${ORION2RE_CONFIG_GENERATED_FILES})
    list(APPEND ORION2RE_SOURCES ${ORION2RE_CONFIG_GENERATED_FILES})
endif()

set(ORION2RE_SUPPORTED_MOD_ARCHIVES)
file(GLOB ORION2RE_SUPPORTED_MOD_IDS CONFIGURE_DEPENDS
    RELATIVE "${ORION2RE_SUPPORTED_MODS_DIR}"
    "${ORION2RE_SUPPORTED_MODS_DIR}/*"
)
foreach(ORION2RE_SUPPORTED_MOD_ID IN LISTS ORION2RE_SUPPORTED_MOD_IDS)
    set(ORION2RE_SUPPORTED_MOD_SOURCE
        "${ORION2RE_SUPPORTED_MODS_DIR}/${ORION2RE_SUPPORTED_MOD_ID}")
    if(IS_DIRECTORY "${ORION2RE_SUPPORTED_MOD_SOURCE}")
        set(ORION2RE_SUPPORTED_MOD_ARCHIVE
            "${ORION2RE_PACKAGED_MODS_DIR}/${ORION2RE_SUPPORTED_MOD_ID}.zip")
        file(GLOB_RECURSE ORION2RE_SUPPORTED_MOD_FILES CONFIGURE_DEPENDS
            "${ORION2RE_SUPPORTED_MOD_SOURCE}/*"
        )
        add_custom_command(
            OUTPUT "${ORION2RE_SUPPORTED_MOD_ARCHIVE}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${ORION2RE_PACKAGED_MODS_DIR}"
            COMMAND ${CMAKE_COMMAND} -E tar cf "${ORION2RE_SUPPORTED_MOD_ARCHIVE}"
                --format=zip -- .
            WORKING_DIRECTORY "${ORION2RE_SUPPORTED_MOD_SOURCE}"
            DEPENDS ${ORION2RE_SUPPORTED_MOD_FILES}
            VERBATIM
        )
        list(APPEND ORION2RE_SUPPORTED_MOD_ARCHIVES
            "${ORION2RE_SUPPORTED_MOD_ARCHIVE}")
    endif()
endforeach()
add_custom_target(package-supported-mods
    DEPENDS ${ORION2RE_SUPPORTED_MOD_ARCHIVES}
)

if(ORION2RE_GAME_DIR)
    add_custom_target(deploy-supported-mods
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${ORION2RE_GAME_DIR}/data/mods"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ORION2RE_SUPPORTED_MOD_ARCHIVES}
            "${ORION2RE_GAME_DIR}/data/mods"
        DEPENDS package-supported-mods
        VERBATIM
    )
endif()

set(ORION2RE_ROUTER_SOURCES
    src/network/router_cli.cpp
    src/network/router_core.cpp
    src/network/router_protocol.cpp
    src/network/router_service.cpp
)

if(ORION2RE_BUILD_GAME)
    add_executable(orion2re
        ${ORION2RE_SOURCES}
        ${ORION2RE_HEADERS}
        "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smacker.c"
        "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smk_bitstream.c"
        "${ORION2RE_VENDORED_LIBSMACKER_DIR}/smk_hufftree.c"
        "${ORION2RE_VENDORED_YYJSON_DIR}/src/yyjson.c"
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
        "${ORION2RE_VENDORED_YYJSON_DIR}/src"
    )

    target_precompile_headers(orion2re PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
    )

    target_link_libraries(orion2re PRIVATE
        SDL3::SDL3-static
        uv_a
        orion2re_lz4
        miniz
    )

    if(ORION2RE_GAME_DIR)
        add_custom_command(TARGET orion2re POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:orion2re>
                "${ORION2RE_GAME_DIR}/$<TARGET_FILE_NAME:orion2re>"
        )

        if(MSVC)
            add_custom_command(TARGET orion2re POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_PDB_FILE:orion2re>
                    "${ORION2RE_GAME_DIR}/$<TARGET_PDB_FILE_NAME:orion2re>"
                COMMAND_EXPAND_LISTS
            )
        endif()
    endif()
endif()

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
    orion2re_lz4
)

if(ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG)
    if(TARGET orion2re)
        target_compile_options(orion2re PRIVATE "${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG}")
    endif()
    target_compile_options(orion2re-router PRIVATE "${ORION2RE_LITTLE_ENDIAN_COMPILE_FLAG}")
endif()

if(WIN32 AND TARGET orion2re)
    set_target_properties(orion2re PROPERTIES WIN32_EXECUTABLE YES)
endif()

if(TARGET orion2re)
    target_compile_definitions(orion2re PRIVATE
        $<$<CONFIG:Debug>:ORION2RE_DEBUG_BUILD=1>
    )
endif()

if(ORION2RE_GAME_DIR AND TARGET orion2re)
    set_target_properties(orion2re PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${ORION2RE_GAME_DIR}"
    )
endif()

if(MSVC)
    if(TARGET orion2re)
        target_compile_options(orion2re PRIVATE /W3 /permissive-)
    endif()
    target_compile_options(orion2re-router PRIVATE /W3 /permissive-)
    if(TARGET config-tool)
        target_compile_options(config-tool PRIVATE /W3 /permissive-)
    endif()
    if(TARGET orion2re)
        target_link_options(orion2re PRIVATE /MANIFESTUAC:NO)
    endif()

    if(TARGET orion2re)
        target_compile_definitions(orion2re PRIVATE
            WIN32
            $<$<CONFIG:Debug>:_CRT_SECURE_NO_DEPRECATE>
            $<$<CONFIG:Debug>:_CRT_NONSTDC_NO_DEPRECATE>
        )
    endif()

    target_compile_definitions(orion2re-router PRIVATE
        WIN32
        $<$<CONFIG:Debug>:_CRT_SECURE_NO_DEPRECATE>
        $<$<CONFIG:Debug>:_CRT_NONSTDC_NO_DEPRECATE>
    )

    if(TARGET orion2re)
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
endif()

if(ORION2RE_ENABLE_ASAN)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "ORION2RE_ENABLE_ASAN requires a Debug build.")
    endif()

    if(TARGET orion2re)
        target_compile_definitions(orion2re PRIVATE ORION2RE_ASAN_BUILD=1)
    endif()

    if(MSVC)
        if(TARGET orion2re)
            target_compile_options(orion2re PRIVATE /fsanitize=address)
        endif()
        target_compile_options(orion2re-router PRIVATE /fsanitize=address)
        if(TARGET orion2re)
            target_link_options(orion2re PRIVATE /INFERASANLIBS /fsanitize=address)
        endif()
        target_link_options(orion2re-router PRIVATE /INFERASANLIBS /fsanitize=address)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        if(TARGET orion2re)
            target_compile_options(orion2re PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        endif()
        target_compile_options(orion2re-router PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        if(TARGET orion2re)
            target_link_options(orion2re PRIVATE -fsanitize=address)
        endif()
        target_link_options(orion2re-router PRIVATE -fsanitize=address)
    else()
        message(FATAL_ERROR "ORION2RE_ENABLE_ASAN is unsupported for the current compiler.")
    endif()
endif()

if(ORION2RE_BUILD_TESTS)
    include("${ORION2RE_TESTS_DIR}/tests.cmake")
endif()