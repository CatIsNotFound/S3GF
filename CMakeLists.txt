cmake_minimum_required(VERSION 3.14)
project(MyEngine LANGUAGES C CXX VERSION 0.3.0)

include(CMakePackageConfigHelpers)
include(CMakeFindDependencyMacro)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_definitions(MYENGINE_FULL_VERSION="v${PROJECT_VERSION}")
add_compile_definitions(MYENGINE_MAJOR_VERSION=${PROJECT_VERSION_MAJOR})
add_compile_definitions(MYENGINE_MINOR_VERSION=${PROJECT_VERSION_MINOR})
add_compile_definitions(MYENGINE_PATCH_VERSION=${PROJECT_VERSION_PATCH})

function(SHOW_USAGE)
    message("\nUsage: cmake <MyEngine project path> -B <Your build path> -D<OPTIONS>[=VALUE] ....")
    message("\nAvailable options:")
    message("-DCMAKE_BUILD_TYPE         Set \"Debug\" or \"Release\" mode, default value: \"Release\"")
    message("-DCMAKE_INSTALL_PREFIX     Set install path for installing library after building project\n")
    message("-DBUILD_TEST               A simple test project (For testing project), default value: 'ON'")
    message("-DBUILD_SHARED_LIBS_ONLY   Is it allowed to only build shared libraries, without guaranteeing direct use. ")
    message("                           If the value is 'OFF', only static libraries will be built, default value: 'OFF'")
    message("-DUSED_SRC_LIBS            Import from source code reposities in `libs` directory.")
    message("                           p.s: Need using `git submodule` command")
    message("-DSDL3_LIB                 Set SDL3 shared library path [REQUIRED]")
    message("-DSDL3_IMAGE_LIB           Set SDL3_image shared library path [REQUIRED]")
    message("-DSDL3_MIXER_LIB           Set SDL3_mixer shared library path [REQUIRED]")
    message("-DSDL3_TTF_LIB             Set SDL3_ttf shared library path [REQUIRED]")
    message("\nOptional options:")
    message("-DFMT_LIB                  Set FMT library path [p.s: APPLE REQUIRED]")
    message("-DDATE_LIB                 Set Date library path [p.s: APPLE REQUIRED, Windows is not recommend to set]")
    message("-DCATCH2_LIB               Set Catch2 library path [while `BUILD_TEST` option is ON]")
    message(FATAL_ERROR "Configure stopped! To disable show help information, please use '-DHELP=OFF'. Otherwise it will be shown again!")
endfunction()

set(HELP OFF CACHE BOOL "Display the configuration help information")
if(HELP)
    SHOW_USAGE()
endif()

set(USED_SRC_LIBS OFF CACHE BOOL "Import from source code reposities in `libs` directory.")

message("-- Project ${PROJECT_NAME} is configuring...")

if(WIN32)
        set(SDL3_LIB "C:/Program Files (x86)/" CACHE PATH "SDL3 Libs Path")
        set(SDL3_IMAGE_LIB "C:/Program Files (x86)/" CACHE PATH "SDL3 Image Libs Path")
        set(SDL3_MIXER_LIB "C:/Program Files (x86)/" CACHE PATH "SDL3 Mixer Libs Path")
        set(SDL3_TTF_LIB "C:/Program Files (x86)/" CACHE PATH "SDL3 TTF Libs Path")
else()
        set(SDL3_LIB "/usr/local/" CACHE PATH "SDL3 Libs")
        set(SDL3_IMAGE_LIB "/usr/local/" CACHE PATH "SDL3 Image Libs Path")
        set(SDL3_MIXER_LIB "/usr/local/" CACHE PATH "SDL3 Mixer Libs Path")
        set(SDL3_TTF_LIB "/usr/local/" CACHE PATH "SDL3 TTF Libs Path")
endif()

set(BUILD_TEST ON CACHE BOOL "Build Test Modules (For testing project)")
set(BUILD_SHARED_LIBS_ONLY OFF CACHE BOOL "Build Shared Libraries Only")
if (${USED_SRC_LIBS})
    option(ENABLE_FMT "Enabled using FMT library" OFF)
    option(ENABLE_DATE "Enabled using Howard Hinnant Date library" OFF)
    message("-- Importing from source code libraries, it may takes a long time...")
    add_subdirectory(libs/SDL3)
    add_subdirectory(libs/SDL3_image)
    add_subdirectory(libs/SDL3_mixer)
    add_subdirectory(libs/SDL3_ttf)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(ENABLE_FMT ON)
        set(ENABLE_DATE ON)
    endif()
    if (${ENABLE_FMT})
        add_subdirectory(libs/fmt)
        add_compile_definitions(__USED_FMT_LIB__)
    endif()
    if (${ENABLE_DATE})
        add_subdirectory(libs/date)
        # set(USE_SYSTEM_TZ_DB ON CACHE BOOL "Use system timezone database")
        # set(BUILD_TZ_LIB ON CACHE BOOL "Enable building timezone library")
        # set(ENABLE_CURL OFF CACHE BOOL "Enable using curl library")
        add_compile_definitions(__USED_DATE_LIB__)
    endif()
else()
    if (NOT EXISTS ${SDL3_LIB})
        message("Tips: Use '-DHELP=ON' option for more help.")
        message(FATAL_ERROR "SDL3 Libs path is not found! Use '-DSDL3_LIB=path/to/sdl3' to set the path.")
    endif()
    if (NOT EXISTS ${SDL3_IMAGE_LIB})
        message("Tips: Use '-DHELP=ON' option for more help.")
        message(FATAL_ERROR "SDL3 Image Libs path is not found! Use '-DSDL3_IMAGE_LIB=path/to/sdl3_image' to set the path.")
    endif()
    if (NOT EXISTS ${SDL3_MIXER_LIB})
        message("Tips: Use '-DHELP=ON' option for more help.")
        message(FATAL_ERROR "SDL3 Mixer Libs path is not found! Use '-DSDL3_MIXER_LIB=path/to/sdl3_mixer' to set the path.")
    endif()
    if (NOT EXISTS ${SDL3_TTF_LIB})
        message("Tips: Use '-DHELP=ON' option for more help.")
        message(FATAL_ERROR "SDL3 TTF Libs path is not found! Use '-DSDL3_TTF_LIB=path/to/sdl3_ttf' to set the path.")
    endif()

    list(APPEND CMAKE_PREFIX_PATH ${SDL3_LIB})
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_IMAGE_LIB})
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_MIXER_LIB})
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_TTF_LIB})

    if (APPLE AND NOT EXISTS ${DATE_LIB})
        message("Tips: Use '-DHELP=ON' option for more help.")
        message(FATAL_ERROR "You are using MacOS, but you have not set Date Libs path! Use '-DDATE_LIB=path/to/date' to set the path.")
    endif()

    if(EXISTS ${DATE_LIB})
        message("-- Using Date Libs: ${DATE_LIB}")
        list(APPEND CMAKE_PREFIX_PATH ${DATE_LIB})
        find_package(date REQUIRED)
        add_compile_definitions(__USED_DATE_LIB__)
    endif()

    if (APPLE AND NOT EXISTS ${FMT_LIB})
        message("Tips: Use '-DHELP=ON' option for more help.")
        message(FATAL_ERROR "You are using MacOS, but your FMT Libs path is not set! Use '-DFMT_LIB=path/to/FMT' to set the path.")
    endif()

    if (EXISTS ${FMT_LIB})
        message("-- Using FMT Libs: ${FMT_LIB}")
        list(APPEND CMAKE_PREFIX_PATH ${FMT_LIB})
        find_package(fmt REQUIRED)
        add_compile_definitions(__USED_FMT_LIB__)
    endif()
    find_package(SDL3 REQUIRED)
    find_package(SDL3_image REQUIRED)
    find_package(SDL3_mixer REQUIRED)
    find_package(SDL3_ttf REQUIRED)
endif()

if (BUILD_SHARED_LIBS_ONLY)
    add_library(${PROJECT_NAME} SHARED
            include/MyEngine/MyEngine
            include/MyEngine/Basic.h
            include/MyEngine/Libs.h
            src/Core.cpp
            include/MyEngine/Core.h
            src/Components.cpp
            include/MyEngine/Components.h
            include/MyEngine/MultiThread/All.h
            src/MultiThread/Components.cpp
            include/MyEngine/MultiThread/Components.h
            include/MyEngine/Utils/All.h
            include/MyEngine/Utils/Random.h
            include/MyEngine/Algorithm/All.h
            include/MyEngine/Algorithm/Draw.h
            include/MyEngine/Utils/DateTime.h
            include/MyEngine/Utils/Cursor.h
            src/Utils/Cursor.cpp
            include/MyEngine/Algorithm/Collider.h
            include/MyEngine/MultiThread/ThreadPool.h
            include/MyEngine/Utils/Logger.h
            src/Utils/FileSystem.cpp
            include/MyEngine/Utils/FileSystem.h
            include/MyEngine/Utils/RGBAColor.h
            src/Game/Sprite.cpp
            include/MyEngine/Game/Sprite.h
            src/Game/SpriteSheet.cpp
            include/MyEngine/Game/SpriteSheet.h
            include/MyEngine/Utils/SysMemory.h
            src/Utils/SysMemory.cpp
            src/Game/GObject.cpp
            include/MyEngine/Game/GObject.h
            src/Game/Collider.cpp
            include/MyEngine/Game/Collider.h
            src/Renderer/BaseCommand.cpp
            include/MyEngine/Renderer/BaseCommand.h
            include/MyEngine/Renderer/CommandPool.h
            include/MyEngine/Renderer/CommandFactory.h
            include/MyEngine/Renderer/RCommand.h
            include/MyEngine/Template/Singleton.h
            include/MyEngine/Exception.h
            src/Widgets/AbstractWidget.cpp
            include/MyEngine/Widgets/AbstractWidget.h
            include/MyEngine/Utils/Variant.h
            src/Utils/Variant.cpp
            src/Widgets/Label.cpp
            include/MyEngine/Widgets/Label.h
            src/Widgets/Button.cpp
            include/MyEngine/Widgets/Button.h
            src/Widgets/LineEdit.cpp
            include/MyEngine/Widgets/LineEdit.h
            include/MyEngine/Algorithm/String.h
            src/Widgets/AbstractLayout.cpp
            include/MyEngine/Widgets/AbstractLayout.h
            src/Widgets/VerticalLayout.cpp
            include/MyEngine/Widgets/VerticalLayout.h
            src/Widgets/HorizontalLayout.cpp
            include/MyEngine/Widgets/HorizontalLayout.h
            include/MyEngine/Algorithm/RGBAPixels.h
            include/MyEngine/Widgets/All.h
            include/MyEngine/Config/All.h
            include/MyEngine/Config/Macro.h
            include/MyEngine/Config/Configuration.h
            src/Config/Configuration.cpp
            src/Utils/WavWriter.cpp
            include/MyEngine/Utils/WavWriter.h
            include/MyEngine/UI/All.h
            src/UI/AbstractControl.cpp
            include/MyEngine/UI/AbstractControl.h
            src/UI/Label.cpp
            include/MyEngine/UI/Label.h
            include/MyEngine/Layers/Layer.h
            src/Layers/LayerManager.cpp
            include/MyEngine/Layers/LayerManager.h
    )
else()
    add_library(${PROJECT_NAME} STATIC
            include/MyEngine/MyEngine
            include/MyEngine/Basic.h
            include/MyEngine/Libs.h
            src/Core.cpp
            include/MyEngine/Core.h
            src/Components.cpp
            include/MyEngine/Components.h
            include/MyEngine/MultiThread/All.h
            src/MultiThread/Components.cpp
            include/MyEngine/MultiThread/Components.h
            include/MyEngine/Utils/All.h
            include/MyEngine/Utils/Random.h
            include/MyEngine/Algorithm/All.h
            include/MyEngine/Algorithm/Draw.h
            include/MyEngine/Utils/DateTime.h
            include/MyEngine/Utils/Cursor.h
            src/Utils/Cursor.cpp
            include/MyEngine/Algorithm/Collider.h
            include/MyEngine/MultiThread/ThreadPool.h
            include/MyEngine/Utils/Logger.h
            src/Utils/FileSystem.cpp
            include/MyEngine/Utils/FileSystem.h
            include/MyEngine/Utils/RGBAColor.h
            src/Game/Sprite.cpp
            include/MyEngine/Game/Sprite.h
            src/Game/SpriteSheet.cpp
            include/MyEngine/Game/SpriteSheet.h
            include/MyEngine/Utils/SysMemory.h
            src/Utils/SysMemory.cpp
            src/Game/GObject.cpp
            include/MyEngine/Game/GObject.h
            src/Game/Collider.cpp
            include/MyEngine/Game/Collider.h
            src/Renderer/BaseCommand.cpp
            include/MyEngine/Renderer/BaseCommand.h
            include/MyEngine/Renderer/CommandPool.h
            include/MyEngine/Renderer/CommandFactory.h
            include/MyEngine/Renderer/RCommand.h
            include/MyEngine/Template/Singleton.h
            include/MyEngine/Exception.h
            src/Widgets/AbstractWidget.cpp
            include/MyEngine/Widgets/AbstractWidget.h
            include/MyEngine/Utils/Variant.h
            src/Utils/Variant.cpp
            src/Widgets/Label.cpp
            include/MyEngine/Widgets/Label.h
            src/Widgets/Button.cpp
            include/MyEngine/Widgets/Button.h
            src/Widgets/LineEdit.cpp
            include/MyEngine/Widgets/LineEdit.h
            include/MyEngine/Algorithm/String.h
            src/Widgets/AbstractLayout.cpp
            include/MyEngine/Widgets/AbstractLayout.h
            src/Widgets/VerticalLayout.cpp
            include/MyEngine/Widgets/VerticalLayout.h
            src/Widgets/HorizontalLayout.cpp
            include/MyEngine/Widgets/HorizontalLayout.h
            include/MyEngine/Algorithm/RGBAPixels.h
            include/MyEngine/Widgets/All.h
            include/MyEngine/Config/All.h
            include/MyEngine/Config/Macro.h
            include/MyEngine/Config/Configuration.h
            src/Config/Configuration.cpp
            src/Utils/WavWriter.cpp
            include/MyEngine/Utils/WavWriter.h
            include/MyEngine/UI/All.h
            src/UI/AbstractControl.cpp
            include/MyEngine/UI/AbstractControl.h
            src/UI/Label.cpp
            include/MyEngine/UI/Label.h
            include/MyEngine/Layers/Layer.h
            src/Layers/LayerManager.cpp
            include/MyEngine/Layers/LayerManager.h
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

list(APPEND TARGET_LIBS SDL3::SDL3)
list(APPEND TARGET_LIBS SDL3_image::SDL3_image)
list(APPEND TARGET_LIBS SDL3_mixer::SDL3_mixer)
list(APPEND TARGET_LIBS SDL3_ttf::SDL3_ttf)
if ((EXISTS ${FMT_LIB}) OR (${ENABLE_FMT}))
    list(APPEND TARGET_LIBS fmt::fmt)
endif()
if ((EXISTS ${DATE_LIB}) OR (${ENABLE_DATE}))
    list(APPEND TARGET_LIBS date::date)
    list(APPEND TARGET_LIBS date::date-tz)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${TARGET_LIBS}
)

if (NOT ${USED_SRC_LIBS})
    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
    )

    install(DIRECTORY include/
            DESTINATION include
    )

    install(DIRECTORY assets/
            DESTINATION assets
    )

    install(FILES
            LICENSE
            README.md
            README_zh.md
            DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
    )

    set(CONFIG_PACKAGE_LOCATION lib/cmake/${PROJECT_NAME})

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CONFIG_PACKAGE_LOCATION}
        PATH_VARS CMAKE_INSTALL_PREFIX
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CONFIG_PACKAGE_LOCATION}
    )
endif()

set(PARENT_PROJECT_NAME ${PROJECT_NAME})

if (BUILD_TEST)
    add_subdirectory(test)
endif()

if (BUILD_PYTHON)
    add_subdirectory(Python)
endif()

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

message("================= Configuration Result ======================")
message("Project version: ${PROJECT_VERSION}")
if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    message("Build type: Release")
else()
    message("Build type: Debug")
endif()
message("Build test: ${BUILD_TEST}")
if (${USED_SRC_LIBS})
    message("SDL3 Lib:       ON")
    message("SDL3 image Lib: ON")
    message("SDL3 mixer Lib: ON")
    message("SDL3 ttf Lib:   ON")
    message("FMT Lib:        ${ENABLE_FMT}")
    message("Date Lib:       ${ENABLE_DATE}")
else()
    message("SDL3 libs  path: ${SDL3_LIB}")
    message("SDL3 image path: ${SDL3_IMAGE_LIB}")
    message("SDL3 mixer path: ${SDL3_MIXER_LIB}")
    message("SDL3 ttf   path: ${SDL3_TTF_LIB}")
    if(EXISTS ${FMT_LIB})
        message("FMT  libs  path: ${FMT_LIB}")
    else()
        message("FMT  libs  path: (not set)")
    endif()
    if(EXISTS ${DATE_LIB})
        message("Date libs  path: ${DATE_LIB}")
    else()
        message("Date libs  path: (not set)")
    endif()
    message("Install path: ${CMAKE_INSTALL_PREFIX}")
endif()
message("=============================================================")
message("You can run command as below now: ")
if (${USED_SRC_LIBS})
    message("  cmake --build .")
else()
    message("  cmake --build . --target install")
endif()
message("Have fun! :)")

function(CopyDependencies OUT)
    set(POST_BUILD_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_LIB}/bin ${CMAKE_BINARY_DIR}/${OUT}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_MIXER_LIB}/bin ${CMAKE_BINARY_DIR}/${OUT}
    )
    if (MINGW)
        list(APPEND POST_BUILD_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_IMAGE_LIB}/x86_64-w64-mingw32/bin ${CMAKE_BINARY_DIR}/${OUT}
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_TTF_LIB}/x86_64-w64-mingw32/bin ${CMAKE_BINARY_DIR}/${OUT}
        )
    else ()
        list(APPEND POST_BUILD_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_IMAGE_LIB}/bin ${CMAKE_BINARY_DIR}/${OUT}
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL3_TTF_LIB}/bin ${CMAKE_BINARY_DIR}/${OUT}
        )
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            ${POST_BUILD_COMMANDS}
    )
endfunction()

if (${BUILD_TEST})
    if (WIN32)
        CopyDependencies(bin)
    endif()
    # Copy assets folders
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/assets ${CMAKE_BINARY_DIR}/bin/assets
    )
endif()

