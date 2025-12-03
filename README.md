# S3GF

[简体中文](README_zh.md)

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg) ![SDL3](https://img.shields.io/badge/SDL-3-blue.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

S3GF (SDL3 Graphics Framework) is a graphics framework library developed based on SDL3 and its derivative libraries.
This library is a third-party library built using modern C++20 features, object-oriented design, and modular architecture.

## Environment Dependencies

To use this third-party library, you need to install the following dependencies:

- [SDL](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)

## Quick Start

### Getting Binary Version

Currently, no official version has been released yet. It is expected to be released early next year.

~~Precompiled versions can be obtained through [Github Release]() or [Gitee Release]() pages.~~

### Source Repository Installation

1. Clone the project using Git

   ```shell
   git clone https://github.com/CatIsNotFound/S3GF
   ```
   
   Or clone the project using the following URL:

   ```shell
   git clone https://gitee.com/CatIsNotFound/S3GF
   ```

2. Configure the project using CMake

   ```shell
   cd S3GF
   mkdir build ; cd build
   cmake .. -DCMAKE_BUILD_TYPE="Release" -DSDL3_LIB=/path/to/SDL3 -DSDL3_IMAGE_LIB=/path/to/SDL3_image -DSDL3_MIXER_LIB=/path/to/SDL3_mixer -DSDL3_TTF_LIB=/path/to/SDL3_ttf
   ```
   
   > ❗ Note:
   > 
   > You need to replace the shared library paths for `SDL3_LIB`, `SDL3_IMAGE_LIB`, `SDL3_TTF_LIB`, and `SDL3_MIXER_LIB`.

3. Compile and install the project

   ```shell
   cmake --build . --config install
   ```
   
### Minimal Example Test

1. Create a new `CMakeLists.txt` file and write the following content:

   ```cmake
    cmake_minimum_required(VERSION 3.31)
    project(Demo)
    
    set(CMAKE_CXX_STANDARD 20)
    
    # Need to set these paths before cmake configuration.
    set(SDL_DIR       "/path/to/SDL3_mingw")
    set(SDL_IMAGE_DIR "/path/to/SDL3_image")
    set(SDL_TTF_DIR   "/path/to/SDL3_ttf")
    set(SDL_MIXER_DIR "/path/to/SDL3_mixer")
    set(S3GF_DIR      "/path/to/S3GF")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    
    list(APPEND CMAKE_PREFIX_PATH ${SDL_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${SDL_IMAGE_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${SDL_TTF_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${SDL_MIXER_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${S3GF_DIR})
    
    find_package(SDL3 REQUIRED)
    find_package(SDL3_image REQUIRED)
    find_package(SDL3_ttf REQUIRED)
    find_package(SDL3_mixer REQUIRED)
    find_package(S3GF REQUIRED)
    
    add_executable(${PROJECT_NAME}
            main.cpp
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SDL3::SDL3
            SDL3_image::SDL3_image
            SDL3_ttf::SDL3_ttf
            SDL3_mixer::SDL3_mixer
            S3GF::S3GF
    )
   ```

2. Edit the `main.cpp` file:

   ```cpp
   #include <S3GF/S3GF>
   using namespace S3GF;
   
   int main() {
       Engine engine("org.example.app", "Hello world", "v1.0.0");
       Window* win = new Window("Hello world!");
       return engine.exec();
   }
   ```

3. Test and run the entire project, for example using the command line:

   ```shell
   cd /path/to/YourProject
   mkdir build ; cd build
   cmake ..
   cmake --build .
   ./YourProject
   ```

4. A completely black window will be displayed. If it displays normally, it means you have succeeded!


## License

This project is licensed under the MIT open source license. See the [LICENSE](LICENSE) file for details.

## Getting Help

1. Check the [documentation](https://github.com/CatIsNotFound/S3GF_Docs.git) for related API documentation. (Content not updated)
2. Submit issues through [Github Issue](https://github.com/CatIsNotFound/S3GF/issues) or [Gitee Issues](https://gitee.com/CatIsNotFound/S3GF)