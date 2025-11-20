cmake_minimum_required(VERSION 3.14)
project(S3GF LANGUAGES C CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(WIN32)
        set(SDL3_LIB "C:/Program Files (x86)/SDL3" CACHE PATH "SDL3 Libs Path")
        set(SDL3_IMAGE_LIB "C:/Program Files (x86)/SDL3_image" CACHE PATH "SDL3 Image Libs Path")
        set(SDL3_MIXER_LIB "C:/Program Files (x86)/SDL3_mixer" CACHE PATH "SDL3 Mixer Libs Path")
        set(SDL3_TTF_LIB "C:/Program Files (x86)/SDL3_ttf" CACHE PATH "SDL3 TTF Libs Path")
else()
        set(SDL3_LIB "/usr/local/SDL3" CACHE PATH "SDL3 Libs")
        set(SDL3_IMAGE_LIB "/usr/local/SDL3_image" CACHE PATH "SDL3 Image Libs Path")
        set(SDL3_MIXER_LIB "/usr/local/SDL3_mixer" CACHE PATH "SDL3 Mixer Libs Path")
        set(SDL3_TTF_LIB "/usr/local/SDL3_ttf" CACHE PATH "SDL3 TTF Libs Path")
endif()

if (NOT EXISTS ${SDL3_LIB})
    message(FATAL_ERROR "SDL3 Libs path is not found! Use '-DSDL3_LIB=path/to/sdl3' to set the path.")
endif()
if (NOT EXISTS ${SDL3_IMAGE_LIB})
    message(FATAL_ERROR "SDL3 Image Libs path is not found! Use '-DSDL3_IMAGE_LIB=path/to/sdl3_image' to set the path.")
endif()
if (NOT EXISTS ${SDL3_MIXER_LIB})
    message(FATAL_ERROR "SDL3 Mixer Libs path is not found! Use '-DSDL3_MIXER_LIB=path/to/sdl3_mixer' to set the path.")
endif()
if (NOT EXISTS ${SDL3_TTF_LIB})
    message(FATAL_ERROR "SDL3 TTF Libs path is not found! Use '-DSDL3_TTF_LIB=path/to/sdl3_ttf' to set the path.")
endif()

list(APPEND CMAKE_PREFIX_PATH ${SDL3_LIB})
list(APPEND CMAKE_PREFIX_PATH ${SDL3_IMAGE_LIB})
list(APPEND CMAKE_PREFIX_PATH ${SDL3_MIXER_LIB})
list(APPEND CMAKE_PREFIX_PATH ${SDL3_TTF_LIB})

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_library(${PROJECT_NAME} SHARED
            src/S3GF
            src/Basic.h
            src/Libs.h
            src/Core.cpp
            src/Core.h
            src/Components.cpp
            src/Components.h
            src/MultiThread/All
            src/MultiThread/Components.cpp
            src/MultiThread/Components.h
            src/Utils/All
            src/Utils/Random.h
            src/Algorithm/All
            src/Algorithm/Draw.h
            src/Utils/DateTime.h
            src/Utils/Cursor.h
            src/Algorithm/Collider.h
            src/MultiThread/ThreadPool.h
            src/Utils/Logger.h
            src/Controls/Control.h
    )
else ()
    add_library(${PROJECT_NAME} STATIC
            src/S3GF
            src/Basic.h
            src/Libs.h
            src/Core.cpp
            src/Core.h
            src/Components.cpp
            src/Components.h
            src/MultiThread/All
            src/MultiThread/Components.cpp
            src/MultiThread/Components.h
            src/Utils/All
            src/Utils/Random.h
            src/Algorithm/All
            src/Algorithm/Draw.h
            src/Utils/DateTime.h
            src/Utils/Cursor.h
            src/Algorithm/Collider.h
            src/MultiThread/ThreadPool.h
            src/Utils/Logger.h
            src/Controls/Control.h
    )
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES src/S3GF DESTINATION include)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

set(PARENT_PROJECT_NAME ${PROJECT_NAME})
add_subdirectory(test)

message("============================================================")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("Build type: Release")
else()
    message("Build type: Debug")
endif()
message("SDL3 libs  path: ${SDL3_LIB}")
message("SDL3 image path: ${SDL3_IMAGE_LIB}")
message("SDL3 mixer path: ${SDL3_MIXER_LIB}")
message("SDL3 ttf   path: ${SDL3_TTF_LIB}")
message("Install path: ${CMAKE_INSTALL_PREFIX}")
message("Project version: ${PROJECT_VERSION}")
message("============================================================")

if (WIN32)
    set(POST_BUILD_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_LIB}/bin ${CMAKE_BINARY_DIR}/bin
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_MIXER_LIB}/bin ${CMAKE_BINARY_DIR}/bin
    )
    if (MINGW)
        list(APPEND POST_BUILD_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_IMAGE_LIB}/x86_64-w64-mingw32/bin ${CMAKE_BINARY_DIR}/bin
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_TTF_LIB}/x86_64-w64-mingw32/bin ${CMAKE_BINARY_DIR}/bin
        )
    else ()
        list(APPEND POST_BUILD_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_IMAGE_LIB}/bin ${CMAKE_BINARY_DIR}/bin
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_TTF_LIB}/bin ${CMAKE_BINARY_DIR}/bin
        )
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            ${POST_BUILD_COMMANDS}
    )
endif()

