cmake_minimum_required(VERSION 3.20)

project(orion2re LANGUAGES CXX)

file(GLOB_RECURSE ORION2RE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(orion2re
    ${ORION2RE_SOURCES}
)

target_compile_features(orion2re PRIVATE cxx_std_20)
target_include_directories(orion2re PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_definitions(orion2re PRIVATE _WINDOWS)

if(WIN32)
    set_target_properties(orion2re PROPERTIES WIN32_EXECUTABLE YES)
endif()

if(MSVC)
    target_compile_options(orion2re PRIVATE /W3 /permissive-)
    target_link_options(orion2re PRIVATE /MANIFESTUAC:NO)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_precompile_headers(orion2re PRIVATE
            "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<CONFIG:Debug>>>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
        )
    else()
        target_precompile_headers(orion2re PRIVATE
            "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
        )
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set_property(TARGET orion2re PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

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
            $<$<CONFIG:Release>:/GS->
        )

        set_target_properties(orion2re PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../moo2"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../moo2"
        )

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
            COMMAND "${ORION2RE_LIB_TOOL}" /def:"${CMAKE_CURRENT_SOURCE_DIR}/smackw32.def" /machine:x86 /name:SMACKW32.DLL /out:"${ORION2RE_IMPORT_LIB_DIR}/smackw32.lib"
            COMMAND "${ORION2RE_LIB_TOOL}" /def:"${CMAKE_CURRENT_SOURCE_DIR}/dplayx.def" /machine:x86 /name:DPLAYX.DLL /out:"${ORION2RE_IMPORT_LIB_DIR}/dplayx.lib"
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
        )

        target_link_options(orion2re PRIVATE
            $<$<CONFIG:Debug>:/DYNAMICBASE:NO>
            $<$<CONFIG:Debug>:/NXCOMPAT:NO>
            $<$<CONFIG:Release>:/DEBUG>
        )
    elseif(WIN32)
        target_link_options(orion2re PRIVATE $<$<CONFIG:Release>:/DEBUG>)
    endif()
elseif(WIN32)
    message(WARNING "The CMake conversion preserves the original Visual Studio/MSVC-specific Windows build behavior; non-MSVC Windows builds may need additional link setup.")
endif()