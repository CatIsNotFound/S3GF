cmake_minimum_required(VERSION 3.14)
project(S3GF LANGUAGES C CXX)

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
            src/Basic.h
            src/Basic.cpp
            src/Libs.h
            src/Core.cpp
            src/Core.h
            src/Components.cpp
            src/Components.h
            src/MultiThread/Components.cpp
            src/MultiThread/Components.h
            src/Utils/Random.cpp
            src/Utils/Random.h
            src/Algorithm/Draw.h
            src/Utils/DateTime.h
    )
else ()
    add_library(${PROJECT_NAME} STATIC
            src/Basic.h
            src/Basic.cpp
            src/Libs.h
            src/Core.cpp
            src/Core.h
            src/Components.cpp
            src/Components.h
            src/MultiThread/Components.cpp
            src/MultiThread/Components.h
            src/Utils/Random.cpp
            src/Utils/Random.h
            src/Algorithm/Draw.h
            src/Utils/DateTime.h
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

install(FILES src/S3GF.h DESTINATION include)

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

