cmake_minimum_required(VERSION 3.14)

enable_testing()
set(ENABLED_ASAN OFF CACHE BOOL "Enable AddressSanitizer for memory leak detection")
if (ENABLED_ASAN)
    message("-- Enabled option: AddressSanitizer")
    if(MSVC)
        add_compile_options(/EHsc /W4 /D_DEBUG /MDd)
        add_compile_definitions(ADDRESS_SANITIZER)
        add_link_options(/INCREMENTAL:NO)
    else()
        add_compile_options(-Wall -g -fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
else()
    if (MSVC)
        add_compile_options(/EHsc /W4 /D_DEBUG /MDd)
    else()
        add_compile_options(-Wall -g)
    endif()
endif()

if (WIN32 AND MSVC)
    set(ENABLED_VLD OFF CACHE BOOL "Enabled Visual Leak Detector for memory leak detection")
    set(VLD_x64 ON CACHE BOOL "Using Visual Leak Detector x64 library")
    set(VLD_LIB "C:/Program Files (x86)/Visual Leak Detector" CACHE STRING "Defined where the Visual Leak Detector placed")
    if (ENABLED_VLD)
        message("-- Enabled option: Visual Leak Detector")
        if (NOT EXISTS ${VLD_LIB})
            message(FATAL_ERROR "VLD Lib path is not valid! Use '-DVLD_LIB=/path/to/VLD' to set the path.")
        endif()
        if (VLD_x64)
            set(VLD_PLATFORM "Win64")
        else()
            set(VLD_PLATFORM "Win32")
        endif()
        
        # Check for vld.h in different possible locations
        set(VLD_INCLUDE_DIR "")
        if(EXISTS "${VLD_LIB}/include/vld.h")
            set(VLD_INCLUDE_DIR "${VLD_LIB}/include")
        elseif(EXISTS "${VLD_LIB}/include/${VLD_PLATFORM}/vld.h")
            set(VLD_INCLUDE_DIR "${VLD_LIB}/include/${VLD_PLATFORM}")
        elseif(EXISTS "${VLD_LIB}/vld.h")
            set(VLD_INCLUDE_DIR "${VLD_LIB}")
        else()
            message(FATAL_ERROR "VLD header file (vld.h) not found in ${VLD_LIB}. Please check your VLD installation.")
        endif()
        
        include_directories(${VLD_INCLUDE_DIR})
        link_directories(${VLD_LIB}/lib/${VLD_PLATFORM})
        add_compile_definitions(__USED_VLD_LIB__)
        message("-- Found VLD at: ${VLD_LIB} (${VLD_PLATFORM})")
        message("-- VLD include directory: ${VLD_INCLUDE_DIR}")
    endif()
endif()

add_compile_definitions(APP_NAME="MyEngine Test")
add_compile_definitions(APP_ID="test.MyEngine.app")
add_compile_definitions(APP_VERSION="v1.0.1")


############################################
if (EXISTS ${CATCH2_LIB})
    message("-- Using Catch2 Libs: ${CATCH2_LIB}")
    list(APPEND CMAKE_PREFIX_PATH "${CATCH2_LIB}")
    find_package(Catch2 REQUIRED)
    include(Catch)
    function(addC2TestModule MODULE_LIST TEST_NAME)
        add_executable(${TEST_NAME}
                ${ARGN}
        )
        set(${MODULE_LIST} ${${MODULE_LIST}} ${TEST_NAME} PARENT_SCOPE)
    endfunction()

    # TODO: If using Catch2 test framework, please add your target here...
    addC2TestModule(CATCH2_TEST_MODULE_LIST core_engine
            core/engine/test_engine.cpp
    )

    addC2TestModule(CATCH2_TEST_MODULE_LIST core_window
            core/window/test_window.cpp
    )

    addC2TestModule(CATCH2_TEST_MODULE_LIST core_renderer
            core/renderer/test_renderer.cpp
    )

    addC2TestModule(CATCH2_TEST_MODULE_LIST core_event_system
            core/EventSystem/test_event_system.cpp
    )

    addC2TestModule(CATCH2_TEST_MODULE_LIST core_audio_system
            core/AudioSystem/test_audio_system.cpp
    )

    addC2TestModule(CATCH2_TEST_MODULE_LIST utils_configuration
            utils/test_config.cpp
    )

    addC2TestModule(CATCH2_TEST_MODULE_LIST utils_variant
            utils/test_variant.cpp
    )

    # ......

endif()
############################################
function(addModule MODULE_LIST TEST_NAME)
    add_executable(${TEST_NAME}
            ${ARGN}
    )
    set(${MODULE_LIST} ${${MODULE_LIST}} ${TEST_NAME} PARENT_SCOPE)
endfunction()
############################################
# TODO: Added the test module and executable files here...
addModule(TEST_MODULES test_simple
        examples/test_simple/test_simple.cpp
)

addModule(TEST_MODULES test_window
        examples/Window/MyWindow.h
        examples/Window/MyWindow.cpp
        examples/Window/test_window.cpp
)

addModule(TEST_MODULES test_renderer
        examples/Renderer/test_renderer.cpp
)

addModule(TEST_MODULES test_point
        examples/Graphics/test_point.cpp
)

addModule(TEST_MODULES test_line
        examples/Graphics/test_line.cpp
)

addModule(TEST_MODULES test_rect
        examples/Graphics/test_rect.cpp
)

addModule(TEST_MODULES test_ellipse
        examples/Graphics/test_ellipse.cpp
)

addModule(TEST_MODULES test_circle
        examples/Graphics/test_circle.cpp
)

addModule(TEST_MODULES ImageViewer
        examples/Graphics/ImageViewer.cpp
)

addModule(TEST_MODULES test_area
        examples/TriggerArea/MyTriggerArea.h
        examples/TriggerArea/test_area.cpp
)

addModule(TEST_MODULES test_SFX
        examples/SFX/test_SFX.cpp
)

addModule(TEST_MODULES VirtualDrums
        examples/SFX/VirtualDrums.cpp
)

addModule(TEST_MODULES Pong
        examples/Game/Pong.cpp
)

addModule(TEST_MODULES test_config
        examples/Config/test_config.cpp
)

#addModule(TEST_MODULES test_audio_system
#        core/AudioSystem/test_audio_system.cpp
#)

############################################

## For test modules (Example)
foreach (MODULE IN ITEMS ${TEST_MODULES})
    set_target_properties(${MODULE} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )

    target_link_libraries(${MODULE}
            ${TARGET_LIBS}
            ${PARENT_PROJECT_NAME}
    )

    if (WIN32 AND ENABLED_VLD AND EXISTS ${VLD_LIB}/lib/${VLD_PLATFORM}/vld.lib)
        target_link_libraries(${MODULE} ${VLD_LIB}/lib/${VLD_PLATFORM}/vld.lib)
        add_custom_command(TARGET ${MODULE} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${VLD_LIB}/bin/${VLD_PLATFORM} ${CMAKE_BINARY_DIR}/bin
        )
    endif()

    add_test(${MODULE} COMMAND ${MODULE})
endforeach ()

## For test modules (Using Catch2 test framework)
if (EXISTS ${CATCH2_LIB})
    foreach (MODULE IN ITEMS ${CATCH2_TEST_MODULE_LIST})
        set_target_properties(${MODULE} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )

        target_link_libraries(${MODULE}
                ${TARGET_LIBS}
                ${PARENT_PROJECT_NAME}
                Catch2::Catch2WithMain
        )

        if (WIN32 AND ENABLED_VLD AND EXISTS ${VLD_LIB}/lib/${VLD_PLATFORM}/vld.lib)
            target_link_libraries(${MODULE} ${VLD_LIB}/lib/${VLD_PLATFORM}/vld.lib)
            add_custom_command(TARGET ${MODULE} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${VLD_LIB}/bin/${VLD_PLATFORM} ${CMAKE_BINARY_DIR}/bin
            )
        endif()
        add_test(${MODULE} COMMAND ${MODULE})
    endforeach ()
endif()

