cmake_minimum_required(VERSION 3.14)

enable_testing()

if (MSVC)
    add_compile_options(/EHsc /W4 /D_DEBUG /MDd)
else()
    add_compile_options(-Wall -g )
endif()

add_compile_definitions(APP_NAME="MyEngine Test")
add_compile_definitions(APP_ID="test.MyEngine.app")
add_compile_definitions(APP_VERSION="v1.0.1")

# Copy assets folders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/assets ${CMAKE_BINARY_DIR}/${OUT}/assets
)


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
        examples/Graphics/test_image.cpp
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

        add_test(${MODULE} COMMAND ${MODULE})
    endforeach ()
endif()

