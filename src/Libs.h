#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <string>
#include <variant>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <memory>
#include <any>
#include <functional>
#include <map>
#include <unordered_map>
#include <utility>
#include <random>
#include <ranges>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include <cstdint>
#include <csignal>
#include <cassert>

#ifdef __USED_DATE_LIB__
#include <date/tz.h>
/// @brief HowardHinnant date libs (Defined the date libs).
/// @since v0.2.1-beta
namespace DATE_TZ = date;
#else
/// @brief Standard chrono library (Undefined the date libs)
/// @since v0.2.1-beta
namespace DATE_TZ = std::chrono;
#endif
#ifdef __USED_FMT_LIB__
#include <fmt/core.h>
#include <fmt/chrono.h>
/// @brief FMT library (Defined the fmt libs)
/// @since v0.2.1-beta
namespace FMT = fmt;
#else
/// @brief Standard format library (Undefined the fmt libs)
/// @since v0.2.1-beta
namespace FMT = std;
#endif

using SRenderer     = SDL_Renderer;
using SSurface      = SDL_Surface;
using STexture      = SDL_Texture;
using SEvent        = SDL_Event;
using SWinEvent     = SDL_WindowEvent;
using SWindow       = SDL_Window;
using SWindowID     = SDL_WindowID;
using SColor        = SDL_Color;
using SCursor       = SDL_Cursor;
using SStdCursor    = SDL_SystemCursor;

using StringList    = std::vector<std::string>;
using BinaryArray   = std::vector<uint8_t>;


