# MyEngine

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg) ![SDL3](https://img.shields.io/badge/SDL-3-blue.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

![cover](./assets/cover.jpg)

MyEngine 是一个轻量级但功能强大的 2D 图形框架，构建于 SDL3 及其扩展之上。它以简洁和灵活为设计理念，提供直观的面向对象 API，使游戏和应用程序开发变得快速而愉悦。利用现代 C++20 特性，MyEngine 提供了模块化架构，允许开发人员以最少的样板代码轻松创建交互式图形应用程序。

## 环境依赖

使用此第三方库需要安装如下依赖：

- [SDL](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)

## 快速开始

### 二进制版本获取

目前暂时未发布正式版本，预计明年早些时候发布。

~~可通过 [Github Release]() 或 [Gitee Release]() 页面获取预编译版本。~~

### 源仓库安装

1. 使用 Git 克隆项目

   ```shell
   git clone https://github.com/CatIsNotFound/MyEngine
   ```
   
   或者使用如下目录克隆项目：

   ```shell
   git clone https://gitee.com/CatIsNotFound/MyEngine
   ```

2. 使用 CMake 配置项目

   ```shell
   cd MyEngine
   mkdir build ; cd build
   cmake .. -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="/path/to/MyEngine" -DSDL3_LIB=/path/to/SDL3 -DSDL3_IMAGE_LIB=/path/to/SDL3_image -DSDL3_MIXER_LIB=/path/to/SDL3_mixer -DSDL3_TTF_LIB=/path/to/SDL3_ttf
   ```
   
   > ❗ 注意：
   > 
   > 需要替换 `CMAKE_INSTALL_PREFIX`, `SDL3_LIB`, `SDL3_IMAGE_LIB`, `SDL3_TTF_LIB`, `SDL3_MIXER_LIB` 的静态库路径。

3. 编译并安装项目

   ```shell
   cmake --build . --config install
   ```
   
### 最小示例测试

1. 新建 `CMakeLists.txt` 文件，并写入如下内容：

   ```cmake
    cmake_minimum_required(VERSION 3.24)
    # Need to modify your project name.
    project(Demo)
    
    set(CMAKE_CXX_STANDARD 20)
    
    # Need to set these paths before cmake configuration.
    set(SDL3_DIR       "/path/to/SDL")
    set(SDL3_IMAGE_DIR "/path/to/SDL3_image")
    set(SDL3_TTF_DIR   "/path/to/SDL3_ttf")
    set(SDL3_MIXER_DIR "/path/to/SDL3_mixer")
    set(MYENGINE_DIR      "/path/to/MyEngine")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_IMAGE_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_TTF_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${SDL3_MIXER_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${MYENGINE_DIR})
    
    find_package(SDL3 REQUIRED)
    find_package(SDL3_image REQUIRED)
    find_package(SDL3_ttf REQUIRED)
    find_package(SDL3_mixer REQUIRED)
    find_package(MyEngine REQUIRED)
    
    add_executable(${PROJECT_NAME}
            main.cpp
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SDL3::SDL3
            SDL3_image::SDL3_image
            SDL3_ttf::SDL3_ttf
            SDL3_mixer::SDL3_mixer
            MyEngine::MyEngine
    )
   
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
   ```

2. 编辑 `main.cpp` 文件：

   ```cpp
   #include <MyEngine/MyEngine>

   using namespace MyEngine;

   int main() {
      Engine engine;
      auto win = new Window(&engine, "Hello world!");
      win->installPaintEvent([](Renderer* r) {
         r->fillBackground(StdColor::DarkBlue);
         r->drawPixelText("Hello world!", {20, 20});		    
      });
      return engine.exec();
   }
   ```

3. 测试并运行整个项目，以命令行为例：

   ```shell
   cd /path/to/YourProject
   mkdir build ; cd build
   cmake ..
   cmake --build .
   ./YourProject
   ```

4. 此时会显示如下图所示的窗口，若能正常显示，说明成功了！
   ![screenshot.png](assets/screenshot.png)

## 许可证

本项目采用 MIT 开源许可证，详见 [LICENSE](LICENSE) 文件。

## 获取帮助

1. 通过[说明文档](https://github.com/CatIsNotFound/MYENGINE_Docs.git)进行查看相关 API 文档。（内容未更新）
2. 通过 [Github Issue](https://github.com/CatIsNotFound/MyEngine/issues) 或 [Gitee Issues](https://gitee.com/CatIsNotFound/MyEngine) 提交遇到的问题
