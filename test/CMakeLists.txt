cmake_minimum_required(VERSION 3.14)
project(MYENGINE_TEST)

add_executable(${PROJECT_NAME}
        main.cpp
        MyCMD.h
        FilledWin.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PARENT_PROJECT_NAME}
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/assets ${CMAKE_BINARY_DIR}/bin
)
