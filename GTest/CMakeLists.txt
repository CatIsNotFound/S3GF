cmake_minimum_required(VERSION 3.14)
project(MyEngine_GTest)

if (WIN32)
    set(GTEST_LIBS "C:/Program Files (x86)/gtest" CACHE PATH "Google test library directory")
else()
    set(GTEST_LIBS "/usr/local/gtest" CACHE PATH "Google test library directory")
endif()

if (NOT EXISTS ${GTEST_LIB})
    message("Tips: Use '-DHELP=ON' option for more help.")
    message(FATAL_ERROR "The specified path is not exist! Use `-DGTEST_LIB=path/to/gtest` option to set gtest library or use `-DBUILD_GTEST=OFF` option to disable.")
endif()

enable_testing()
set(CMAKE_PREFIX_PATH ${GTEST_LIB})
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        UnitTests/test_window.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PARENT_PROJECT_NAME}
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
        GTest::gtest_main
        GTest::gmock
)

add_test(NAME SimpleWindow COMMAND ${CMAKE_BUILD_TYPE}/test_simple_window)
add_test(NAME RenderGraphics COMMAND ${CMAKE_BUILD_TYPE}/test_render_graphics)
