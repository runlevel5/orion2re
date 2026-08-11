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
        tests/fakes/fake_fields_helpers_runtime.cpp
        tests/fakes/fake_video.cpp
)

if(ORION2RE_BUILD_CONFIG_TOOL)
    orion2re_add_unit_test(test_config_schema
        SOURCES
            tests/unit/test_config_schema.cpp
        PRODUCTION_SOURCES
            tools/config/file_util.cpp
            tools/config/generate_cpp.cpp
            tools/config/generate_docs.cpp
            tools/config/json5_reader.cpp
            tools/config/schema_model.cpp
            vendors/yyjson/src/yyjson.c
    )
    target_include_directories(test_config_schema PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/tools/config"
        "${ORION2RE_VENDORED_YYJSON_DIR}/src"
    )
    target_compile_definitions(test_config_schema PRIVATE
        CONFIG_SCHEMA_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/data/config/schema"
        CONFIG_MAIN_SCHEMA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/specs/config/main.json5"
    )
endif()

orion2re_add_unit_test(test_cfg_parser
    SOURCES
        tests/unit/test_cfg_parser.cpp
    PRODUCTION_SOURCES
        src/config/cfg_parser.cpp
)

orion2re_add_unit_test(test_config_decode
    SOURCES
        tests/unit/test_config_decode.cpp
    PRODUCTION_SOURCES
        src/config/cfg_parser.cpp
        src/config/config_decode.cpp
        src/config/config_runtime.cpp
        src/config/generated/config.cpp
        src/game/colcalc_config.cpp
        src/game/design_config.cpp
        src/game/ericnet_config.cpp
        src/game/events_config.cpp
        src/game/invasion_config.cpp
        src/game/ship_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_config_runtime.cpp
)

orion2re_add_unit_test(test_config_bindings
    SOURCES
        tests/unit/test_config_bindings.cpp
    PRODUCTION_SOURCES
        src/config/cfg_parser.cpp
        src/config/config_decode.cpp
        src/config/config_runtime.cpp
        src/config/generated/config.cpp
        src/game/colcalc_config.cpp
        src/game/design_config.cpp
        src/game/ericnet_config.cpp
        src/game/events_config.cpp
        src/game/invasion_config.cpp
        src/game/ship_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_config_runtime.cpp
)

orion2re_add_unit_test(test_config_lifecycle
    SOURCES
        tests/unit/test_config_lifecycle.cpp
    PRODUCTION_SOURCES
        src/config/cfg_parser.cpp
        src/config/config_decode.cpp
        src/config/config_lifecycle.cpp
        src/config/config_runtime.cpp
        src/config/generated/config.cpp
        src/game/colcalc_config.cpp
        src/game/design_config.cpp
        src/game/ericnet_config.cpp
        src/game/events_config.cpp
        src/game/invasion_config.cpp
        src/game/ship_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_config_runtime.cpp
)

orion2re_add_unit_test(test_config_loader
    SOURCES
        tests/unit/test_config_loader.cpp
    PRODUCTION_SOURCES
        src/config/cfg_parser.cpp
        src/config/config_decode.cpp
        src/config/config_lifecycle.cpp
        src/config/config_loader.cpp
        src/config/mod_manifest.cpp
        src/config/mod_manager.cpp
        src/config/config_runtime.cpp
        src/config/generated/config.cpp
        src/game/colcalc_config.cpp
        src/game/design_config.cpp
        src/game/ericnet_config.cpp
        src/game/events_config.cpp
        src/game/invasion_config.cpp
        src/game/ship_config.cpp
        src/game/compat.cpp
        src/game/vfs.cpp
        src/game/vfs_file.cpp
        src/game/vfs_lbx.cpp
        src/game/vfs_path.cpp
        src/game/vfs_zip.cpp
        vendors/yyjson/src/yyjson.c
    FAKE_SOURCES
        tests/fakes/fake_config_runtime.cpp
    LIBRARIES
        miniz
    LINK_SDL
)
target_include_directories(test_config_loader PRIVATE
    "${ORION2RE_VENDORED_YYJSON_DIR}/src"
)

orion2re_add_unit_test(test_pop_helpers
    SOURCES
        tests/unit/test_pop_helpers.cpp
)

orion2re_add_unit_test(test_aidudes
    SOURCES
        tests/unit/test_aidudes.cpp
    PRODUCTION_SOURCES
        src/game/aileader.cpp
        src/game/aidudes.cpp
        src/game/maintain.cpp
    FAKE_SOURCES
        tests/fakes/fake_aidudes_runtime.cpp
)

orion2re_add_unit_test(test_refit_cost
    SOURCES
        tests/unit/test_refit_cost.cpp
    PRODUCTION_SOURCES
        src/game/aibuild.cpp
        src/game/design_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_refit_cost_runtime.cpp
)

orion2re_add_unit_test(test_settler
    SOURCES
        tests/unit/test_settler.cpp
    PRODUCTION_SOURCES
        src/game/settler.cpp
    FAKE_SOURCES
        tests/fakes/fake_settler_runtime.cpp
)

orion2re_add_unit_test(test_colony_production_helpers
    SOURCES
        tests/unit/test_colony_production_helpers.cpp
    PRODUCTION_SOURCES
        src/game/colcalc_base.cpp
        src/game/colcalc_config.cpp
        src/game/plntsum.cpp
    FAKE_SOURCES
        tests/fakes/fake_colcalc_base_runtime.cpp
)

orion2re_add_unit_test(test_colony_production_draw
    SOURCES
        tests/unit/test_colony_production_draw.cpp
    PRODUCTION_SOURCES
        src/game/coldraw.cpp
    FAKE_SOURCES
        tests/fakes/fake_coldraw_runtime.cpp
)

orion2re_add_unit_test(test_colony_pop_growth
    SOURCES
        tests/unit/test_colony_pop_growth.cpp
    PRODUCTION_SOURCES
        src/game/colcalc.cpp
        src/game/colcalc_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_colony_pop_growth_runtime.cpp
)

orion2re_add_unit_test(test_colony_replicators
    SOURCES
        tests/unit/test_colony_replicators.cpp
    PRODUCTION_SOURCES
        src/game/colcalc.cpp
        src/game/colcalc_config.cpp
        src/game/colcalc_main.cpp
    FAKE_SOURCES
        tests/fakes/fake_colony_replicators_runtime.cpp
)

orion2re_add_unit_test(test_build_queue
    SOURCES
        tests/unit/test_build_queue.cpp
    PRODUCTION_SOURCES
        src/game/build_queue.cpp
    FAKE_SOURCES
        tests/fakes/fake_build_queue_runtime.cpp
)

orion2re_add_unit_test(test_colonize
    SOURCES
        tests/unit/test_colonize.cpp
    PRODUCTION_SOURCES
        src/game/colonize.cpp
    FAKE_SOURCES
        tests/fakes/fake_colonize_runtime.cpp
)

orion2re_add_unit_test(test_shipmove
    SOURCES
        tests/unit/test_shipmove.cpp
    PRODUCTION_SOURCES
        src/game/shipmove.cpp
        src/game/ships.cpp
    FAKE_SOURCES
        tests/fakes/fake_shipmove_runtime.cpp
)

if(MSVC)
    target_compile_options(test_shipmove PRIVATE /Gy)
    target_link_options(test_shipmove PRIVATE /OPT:REF)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_shipmove PRIVATE -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(test_shipmove PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(test_shipmove PRIVATE -Wl,--gc-sections)
    endif()
endif()

orion2re_add_unit_test(test_fleetpop
    SOURCES
        tests/unit/test_fleetpop.cpp
    PRODUCTION_SOURCES
        src/game/fleetpop.cpp
        src/game/sys.cpp
    FAKE_SOURCES
        tests/fakes/fake_fleetpop_runtime.cpp
)

if(MSVC)
    target_compile_options(test_fleetpop PRIVATE /Gy)
    target_link_options(test_fleetpop PRIVATE /OPT:REF)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_fleetpop PRIVATE -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(test_fleetpop PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(test_fleetpop PRIVATE -Wl,--gc-sections)
    endif()
endif()

orion2re_add_unit_test(test_geo
    SOURCES
        tests/unit/test_geo.cpp
    PRODUCTION_SOURCES
        src/game/geo.cpp
    FAKE_SOURCES
        tests/fakes/fake_geo_runtime.cpp
)

orion2re_add_unit_test(test_homegen
    SOURCES
        tests/unit/test_homegen.cpp
    PRODUCTION_SOURCES
        src/game/homegen.cpp
    FAKE_SOURCES
        tests/fakes/fake_homegen_runtime.cpp
)

if(MSVC)
    target_compile_options(test_geo PRIVATE /Gy)
    target_link_options(test_geo PRIVATE /OPT:REF)
    target_compile_options(test_homegen PRIVATE /Gy)
    target_link_options(test_homegen PRIVATE /OPT:REF)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_geo PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_homegen PRIVATE -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(test_geo PRIVATE -Wl,-dead_strip)
        target_link_options(test_homegen PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(test_geo PRIVATE -Wl,--gc-sections)
        target_link_options(test_homegen PRIVATE -Wl,--gc-sections)
    endif()
endif()

orion2re_add_unit_test(test_ground_commando
    SOURCES
        tests/unit/test_ground_commando.cpp
    PRODUCTION_SOURCES
        src/game/invasion.cpp
        src/game/invasion_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_ground_commando_runtime.cpp
)

orion2re_add_unit_test(test_ericnet
    SOURCES
        tests/unit/test_ericnet.cpp
    PRODUCTION_SOURCES
        src/game/ericnet.cpp
        src/game/ericnet_config.cpp
    FAKE_SOURCES
        tests/fakes/fake_ericnet_runtime.cpp
)

orion2re_add_unit_test(test_missile_defense
    SOURCES
        tests/unit/test_missile_defense.cpp
    PRODUCTION_SOURCES
        src/game/cmbtmis.cpp
)

orion2re_add_unit_test(test_combat_stats
    SOURCES
        tests/unit/test_combat_stats.cpp
    PRODUCTION_SOURCES
        src/game/cmbtmov1.cpp
)

orion2re_add_unit_test(test_combat_repair
    SOURCES
        tests/unit/test_combat_repair.cpp
    PRODUCTION_SOURCES
        src/game/combinit.cpp
    FAKE_SOURCES
        tests/fakes/fake_combat_repair_runtime.cpp
)

orion2re_add_unit_test(test_bombardment_ammo
    SOURCES
        tests/unit/test_bombardment_ammo.cpp
    PRODUCTION_SOURCES
        src/game/combat.cpp
)

orion2re_add_unit_test(test_initship
    SOURCES
        tests/unit/test_initship.cpp
    PRODUCTION_SOURCES
        src/game/initship.cpp
)

if(MSVC)
    target_compile_options(test_initship PRIVATE /Gy)
    target_link_options(test_initship PRIVATE /OPT:REF)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_initship PRIVATE
        -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(test_initship PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(test_initship PRIVATE -Wl,--gc-sections)
    endif()
endif()

orion2re_add_unit_test(test_town_zero
    SOURCES
        tests/unit/test_town_zero.cpp
    PRODUCTION_SOURCES
        src/game/bomb.cpp
    FAKE_SOURCES
        tests/fakes/fake_town_zero_runtime.cpp
)

orion2re_add_unit_test(test_animation_frame
    SOURCES
        tests/unit/test_animation_frame.cpp
    PRODUCTION_SOURCES
        src/game/animate.cpp
)

orion2re_add_unit_test(test_absorber_cleanup
    SOURCES
        tests/unit/test_absorber_cleanup.cpp
    PRODUCTION_SOURCES
        src/game/cmbtfire.cpp
        src/game/cmbtfir2.cpp
    FAKE_SOURCES
        tests/fakes/fake_absorber_cleanup_runtime.cpp
)

orion2re_add_unit_test(test_boarding
    SOURCES
        tests/unit/test_boarding.cpp
    PRODUCTION_SOURCES
        src/game/boarding.cpp
        src/game/cmbtfir2.cpp
        src/game/strbuild.cpp
        src/game/struct.cpp
    FAKE_SOURCES
        tests/fakes/fake_boarding_runtime.cpp
    LINK_SDL
)

orion2re_add_unit_test(test_cmbtfire
    SOURCES
        tests/unit/test_cmbtfire.cpp
    PRODUCTION_SOURCES
        src/game/cmbtfire.cpp
    FAKE_SOURCES
        tests/fakes/fake_stasis_runtime.cpp
)

orion2re_add_unit_test(test_stasis_area_effects
    SOURCES
        tests/unit/test_stasis_area_effects.cpp
    PRODUCTION_SOURCES
        src/game/cmbtfir2.cpp
        src/game/combat1.cpp
        src/game/random.cpp
    FAKE_SOURCES
        tests/fakes/fake_stasis_area_effects_runtime.cpp
)

if(MSVC)
    target_compile_options(test_aidudes PRIVATE /Gy)
    target_link_options(test_aidudes PRIVATE /OPT:REF)
    target_compile_options(test_refit_cost PRIVATE /Gy)
    target_link_options(test_refit_cost PRIVATE /OPT:REF)
    target_compile_options(test_colony_production_helpers PRIVATE /Gy)
    target_link_options(test_colony_production_helpers PRIVATE /OPT:REF)
    target_compile_options(test_colony_production_draw PRIVATE /Gy)
    target_link_options(test_colony_production_draw PRIVATE /OPT:REF)
    target_compile_options(test_bombardment_ammo PRIVATE /Gy)
    target_link_options(test_bombardment_ammo PRIVATE /OPT:REF)
    target_compile_options(test_town_zero PRIVATE /Gy)
    target_link_options(test_town_zero PRIVATE /OPT:REF)
    target_compile_options(test_animation_frame PRIVATE /Gy)
    target_compile_options(test_ericnet PRIVATE /Gy)
    target_link_options(test_animation_frame PRIVATE /OPT:REF)
    target_link_options(test_ericnet PRIVATE /OPT:REF)
    target_compile_options(test_missile_defense PRIVATE /Gy)
    target_link_options(test_missile_defense PRIVATE /OPT:REF)
    target_compile_options(test_absorber_cleanup PRIVATE /Gy)
    target_link_options(test_absorber_cleanup PRIVATE /OPT:REF)
    target_compile_options(test_boarding PRIVATE /Gy)
    target_link_options(test_boarding PRIVATE /OPT:REF)
    target_compile_options(test_combat_stats PRIVATE /Gy)
    target_link_options(test_combat_stats PRIVATE /OPT:REF)
    target_compile_options(test_ground_commando PRIVATE /Gy)
    target_link_options(test_ground_commando PRIVATE /OPT:REF)
    target_compile_options(test_combat_repair PRIVATE /Gy)
    target_link_options(test_combat_repair PRIVATE /OPT:REF)
    target_compile_options(test_cmbtfire PRIVATE /Gy)
    target_link_options(test_cmbtfire PRIVATE /OPT:REF)
    target_compile_options(test_stasis_area_effects PRIVATE /Gy)
    target_link_options(test_stasis_area_effects PRIVATE /OPT:REF)
    target_compile_options(test_colony_pop_growth PRIVATE /Gy)
    target_link_options(test_colony_pop_growth PRIVATE /OPT:REF)
    target_compile_options(test_colony_replicators PRIVATE /Gy)
    target_link_options(test_colony_replicators PRIVATE /OPT:REF)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_aidudes PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_refit_cost PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_colony_production_helpers PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_colony_production_draw PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_bombardment_ammo PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_town_zero PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_animation_frame PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_ericnet PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_missile_defense PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_absorber_cleanup PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_boarding PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_combat_stats PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_ground_commando PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_combat_repair PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_cmbtfire PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_stasis_area_effects PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_colony_pop_growth PRIVATE -ffunction-sections -fdata-sections)
    target_compile_options(test_colony_replicators PRIVATE -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(test_aidudes PRIVATE -Wl,-dead_strip)
        target_link_options(test_refit_cost PRIVATE -Wl,-dead_strip)
        target_link_options(test_colony_production_helpers PRIVATE -Wl,-dead_strip)
        target_link_options(test_colony_production_draw PRIVATE -Wl,-dead_strip)
        target_link_options(test_bombardment_ammo PRIVATE -Wl,-dead_strip)
        target_link_options(test_town_zero PRIVATE -Wl,-dead_strip)
        target_link_options(test_animation_frame PRIVATE -Wl,-dead_strip)
        target_link_options(test_ericnet PRIVATE -Wl,-dead_strip)
        target_link_options(test_missile_defense PRIVATE -Wl,-dead_strip)
        target_link_options(test_absorber_cleanup PRIVATE -Wl,-dead_strip)
        target_link_options(test_boarding PRIVATE -Wl,-dead_strip)
        target_link_options(test_combat_stats PRIVATE -Wl,-dead_strip)
        target_link_options(test_ground_commando PRIVATE -Wl,-dead_strip)
        target_link_options(test_combat_repair PRIVATE -Wl,-dead_strip)
        target_link_options(test_cmbtfire PRIVATE -Wl,-dead_strip)
        target_link_options(test_stasis_area_effects PRIVATE -Wl,-dead_strip)
        target_link_options(test_colony_pop_growth PRIVATE -Wl,-dead_strip)
        target_link_options(test_colony_replicators PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(test_aidudes PRIVATE -Wl,--gc-sections)
        target_link_options(test_refit_cost PRIVATE -Wl,--gc-sections)
        target_link_options(test_colony_production_helpers PRIVATE -Wl,--gc-sections)
        target_link_options(test_colony_production_draw PRIVATE -Wl,--gc-sections)
        target_link_options(test_bombardment_ammo PRIVATE -Wl,--gc-sections)
        target_link_options(test_town_zero PRIVATE -Wl,--gc-sections)
        target_link_options(test_animation_frame PRIVATE -Wl,--gc-sections)
        target_link_options(test_ericnet PRIVATE -Wl,--gc-sections)
        target_link_options(test_missile_defense PRIVATE -Wl,--gc-sections)
        target_link_options(test_absorber_cleanup PRIVATE -Wl,--gc-sections)
        target_link_options(test_boarding PRIVATE -Wl,--gc-sections)
        target_link_options(test_combat_stats PRIVATE -Wl,--gc-sections)
        target_link_options(test_ground_commando PRIVATE -Wl,--gc-sections)
        target_link_options(test_combat_repair PRIVATE -Wl,--gc-sections)
        target_link_options(test_cmbtfire PRIVATE -Wl,--gc-sections)
        target_link_options(test_stasis_area_effects PRIVATE -Wl,--gc-sections)
        target_link_options(test_colony_pop_growth PRIVATE -Wl,--gc-sections)
        target_link_options(test_colony_replicators PRIVATE -Wl,--gc-sections)
    endif()
endif()

orion2re_add_unit_test(test_techinit
    SOURCES
        tests/unit/test_techinit.cpp
    PRODUCTION_SOURCES
        src/game/techinit.cpp
    FAKE_SOURCES
        tests/fakes/fake_techinit_runtime.cpp
)

if(MSVC)
    target_compile_options(test_techinit PRIVATE /Gy)
    target_link_options(test_techinit PRIVATE /OPT:REF)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_techinit PRIVATE -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(test_techinit PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(test_techinit PRIVATE -Wl,--gc-sections)
    endif()
endif()

orion2re_add_unit_test(test_map_scale
    SOURCES
        tests/unit/test_map_scale.cpp
)

orion2re_add_unit_test(test_msg_queue
    SOURCES
        tests/unit/test_msg_queue.cpp
    PRODUCTION_SOURCES
        src/game/msg_queue.cpp
)

orion2re_add_unit_test(test_savegame
    SOURCES
        tests/unit/test_savegame.cpp
    PRODUCTION_SOURCES
        src/game/savegame.cpp
        src/game/serialization.cpp
    FAKE_SOURCES
        tests/fakes/fake_savegame_runtime.cpp
)

target_compile_definitions(test_savegame PRIVATE
    SAVEGAME_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/unit"
)

orion2re_add_unit_test(test_netturn_host_selection
    SOURCES
        tests/unit/test_netturn_host_selection.cpp
    PRODUCTION_SOURCES
        src/game/netturn.cpp
        src/game/netmox.cpp
        src/game/netproto.cpp
        src/game/random.cpp
        src/game/serialization.cpp
    FAKE_SOURCES
        tests/fakes/fake_allocate.cpp
        tests/fakes/fake_exitm.cpp
        tests/fakes/fake_fields_helpers_runtime.cpp
        tests/fakes/fake_netturn_runtime.cpp
        tests/fakes/fake_video.cpp
    LINK_SDL
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_netturn_host_selection PRIVATE -ffunction-sections -fdata-sections)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND NOT APPLE)
    target_link_options(test_netturn_host_selection PRIVATE -Wl,--gc-sections)
elseif(APPLE)
    target_link_options(test_netturn_host_selection PRIVATE -Wl,-dead_strip)
endif()

orion2re_add_unit_test(test_netmox_packet_limits
    SOURCES
        tests/unit/test_netmox_packet_limits.cpp
    PRODUCTION_SOURCES
        src/game/netmox.cpp
        src/game/netproto.cpp
        src/game/random.cpp
        src/game/serialization.cpp
    FAKE_SOURCES
        tests/fakes/fake_allocate.cpp
        tests/fakes/fake_exitm.cpp
        tests/fakes/fake_fields_helpers_runtime.cpp
        tests/fakes/fake_netturn_runtime.cpp
        tests/fakes/fake_video.cpp
    LINK_SDL
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test_netmox_packet_limits PRIVATE -ffunction-sections -fdata-sections)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND NOT APPLE)
    target_link_options(test_netmox_packet_limits PRIVATE -Wl,--gc-sections)
elseif(APPLE)
    target_link_options(test_netmox_packet_limits PRIVATE -Wl,-dead_strip)
endif()

orion2re_add_unit_test(test_video_scan_bounds
    SOURCES
        tests/unit/test_video_scan_bounds.cpp
    PRODUCTION_SOURCES
        src/game/setvesa.cpp
    FAKE_SOURCES
        tests/fakes/fake_video.cpp
)

orion2re_add_unit_test(test_vfs
    SOURCES
        tests/unit/test_vfs.cpp
    PRODUCTION_SOURCES
        src/game/compat.cpp
        src/game/vfs.cpp
        src/game/vfs_file.cpp
        src/game/vfs_lbx.cpp
        src/game/vfs_path.cpp
        src/game/vfs_zip.cpp
    LIBRARIES
        miniz
    LINK_SDL
)

orion2re_add_unit_test(test_farload_buffer_vfs
    SOURCES
        tests/unit/test_farload_buffer_vfs.cpp
    PRODUCTION_SOURCES
        src/game/buffer.cpp
        src/game/compat.cpp
        src/game/farload.cpp
        src/game/vfs.cpp
        src/game/vfs_file.cpp
        src/game/vfs_lbx.cpp
        src/game/vfs_path.cpp
        src/game/vfs_zip.cpp
    FAKE_SOURCES
        tests/fakes/fake_allocate.cpp
        tests/fakes/fake_exitm.cpp
        tests/fakes/fake_fields_helpers_runtime.cpp
        tests/fakes/fake_memory.cpp
        tests/fakes/fake_sound.cpp
        tests/fakes/fake_test_state.cpp
        tests/fakes/fake_video.cpp
    LIBRARIES
        miniz
    LINK_SDL
)

orion2re_add_unit_test(test_fields
    SOURCES
        tests/unit/test_fields.cpp
    PRODUCTION_SOURCES
        src/game/exitm.cpp
        src/game/fields.cpp
    FAKE_SOURCES
        tests/fakes/fake_animate.cpp
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
        src/game/netcode_bootstrap.cpp
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