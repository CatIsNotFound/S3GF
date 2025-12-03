# S3GF

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg) ![SDL3](https://img.shields.io/badge/SDL-3-blue.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

S3GF（SDL3 Graphics Framework）是基于 SDL3 以及衍生库开发的图形框架库。
本库基于 C++20 现代特性、面向对象思想、模块化结构等方式构建而成的第三方库。

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
   git clone https://github.com/CatIsNotFound/S3GF
   ```
   
   或者使用如下目录克隆项目：

   ```shell
   git clone https://gitee.com/CatIsNotFound/S3GF
   ```

2. 使用 CMake 配置项目

   ```shell
   cd S3GF
   mkdir build ; cd build
   cmake .. -DCMAKE_BUILD_TYPE="Release" -DSDL3_LIB=/path/to/SDL3 -DSDL3_IMAGE_LIB=/path/to/SDL3_image -DSDL3_MIXER_LIB=/path/to/SDL3_mixer -DSDL3_TTF_LIB=/path/to/SDL3_ttf
   ```
   
   > ❗ 注意：
   > 
   > 需要替换 `SDL3_LIB`, `SDL3_IMAGE_LIB`, `SDL3_TTF_LIB`, `SDL3_MIXER_LIB` 的共享库路径。

3. 编译并安装项目

   ```shell
   cmake --build . --config install
   ```
   
### 最小示例测试

1. 新建 `CMakeLists.txt` 文件，并写入如下内容：

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

2. 编辑 `main.cpp` 文件：

   ```cpp
   #include <S3GF/S3GF>
   using namespace S3GF;
   
   int main() {
       Engine engine("org.example.app", "Hello world", "v1.0.0");
       Window* win = new Window("Hello world!");
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

4. 此时会显示一个完全黑色的窗口，若能正常显示，说明成功了！


## 许可证

本项目采用 MIT 开源许可证，详见 [LICENSE](LICENSE) 文件。

## 获取帮助

1. 通过[说明文档](https://github.com/CatIsNotFound/S3GF_Docs.git)进行查看相关 API 文档。（内容未更新）
2. 通过 [Github Issue](https://github.com/CatIsNotFound/S3GF/issues) 或 [Gitee Issues](https://gitee.com/CatIsNotFound/S3GF) 提交遇到的问题
