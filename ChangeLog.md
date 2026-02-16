# MyEngine ChangeLog

## v0.2.1-beta (2025/02/16)

### ⚗ Experimental Features
- **Image Processing Algorithms**: Added various image filters and effect processing functions
- **Color System**: Expanded RGBA color constant library with rich preset colors
- **UI Framework**: Improved abstract component system with property and state management support
- **SFX System**: Modified SFX playback mechanism to support batch operations on multiple tracks
- **Game Components**: Enhanced Sprite class functionality with complete transformation and rendering control

### 🔧 Image Processing Improvements
- **Contrast Adjustment**: Improved contrast algorithm using professional-grade contrast formula
- **Negative Effect**: Implemented smooth transition negative effect with intensity control
- **Image Filters**: Added saturation, sepia, gamma correction and other filters
- **Rounded Rectangle**: Implemented high-quality anti-aliased rounded rectangle drawing
- **Pixel Operations**: Provided complete pixel read/write and batch processing interfaces

### 🎨 Color System
- **Expanded Color Library**: Added 50+ preset color constants
- **Color Classification**: Organized colors by color families (red series, blue series, etc.)
- **Transparency Support**: Provided multiple transparency level color variants
- **Color Tools**: Improved color conversion and manipulation utility functions

### 🎮 Game Components
- **Sprite Enhancement**: Support for position, size, rotation, scale, opacity and other properties
- **Texture Management**: Complete texture loading and rendering control
- **Transformation System**: Support for anchor point setting, flip modes and visibility control
- **Color Blending**: Support for color alpha channel blending

### 🖥️ UI Framework
- **Property System**: Implemented generic property management mechanism
- **State Management**: Support for multiple UI states (normal, active, disabled, hover, etc.)
- **Layout System**: Provided horizontal and vertical layout managers
- **Control Enhancement**: Improved basic controls like buttons, labels, input boxes, etc.

### 🐛 Fixes
- **Contrast Algorithm**: Fixed color inversion issue when values are too large
- **Negative Effect**: Fixed issue where effect is not obvious when threshold is too large
- **Type Conversion**: Improved type conversion order in image processing to avoid precision loss
- **Boundary Check**: Strengthened boundary validation and error handling for pixel operations

### 💡 Technical Improvements
- **Code Quality**: Improved algorithm implementation using more professional image processing formulas
- **Performance Optimization**: Optimized pixel operations and batch processing performance
- **Memory Safety**: Strengthened memory management and resource release mechanisms
- **Error Handling**: Improved logging and error reporting system

### 📝 API Changes
- **Image Processing**: Added multiple `apply*Surface` function series
- **Color System**: Expanded `RGBAColor` namespace with many new color constants

### 📝 Notes
- Image processing algorithms are continuously being optimized
- Recommended to verify experimental features in a test environment

-----

## v0.1.2-beta (2025/12/17)

### 🔧 Performance Optimization
- **Render Command Pool**: Refactored command pool system to improve render command reuse efficiency
- **Sub-pool Mechanism**: Implemented dynamic sub-pool allocation based on CPU core count to reduce lock contention
- **Memory Allocation**: Integrated `std::pmr::synchronized_pool_resource` for optimized memory allocation
- **Data Structure**: Replaced `std::deque` with `std::vector` to improve memory locality
- **Atomic Operations**: Adopted lock-free atomic variable polling for sub-pool selection, balancing multi-thread load

### 🐛 Fixes
- **Window Closure Crash**: Fixed CommandPool destructor to resolve crash when closing window
- **Thread Safety**: Enhanced thread safety mechanisms to avoid data races during concurrent access

### 💡 Technical Improvements
- **Command Reuse**: Implemented command object `reset()` method supporting parameterized reset
- **Batch Processing**: Optimized command batch execution mode to improve rendering efficiency
- **Resource Management**: Enhanced resource release mechanism to prevent memory leaks

-----

## v0.1.1-beta (2025/12/10)

### 🎉 New Features
- **Core Engine**: Initial release of MyEngine 2D graphics framework built on SDL3
- **Rendering System**: Command-based renderer with batch rendering support
- **Window Management**: Multi-window support with OpenGL and Vulkan backend options
- **Event System**: Comprehensive input handling for keyboard, mouse, and window events
- **Graphics Primitives**: Support for points, lines, rectangles, triangles, and ellipses
- **Texture System**: Texture loading, rendering, and property management
- **Text Rendering**: TTF font support with pixel-perfect text rendering
- **Audio System**: BGM and SFX support with 3D positioning and mixing
- **Animation System**: GIF animation support with frame-based playback
- **UI Framework**: Basic UI controls with click/hold area detection
- **Game Components**: Sprite and SpriteSheet management for 2D games
- **Collision System**: 2D collision detection with trigger support
- **Threading**: Multi-threading support with thread pool implementation
- **Utilities**: File system, logging, random number generation, and system memory monitoring

### 🔧 Technical Features
- **Modern C++20**: Leverages latest C++ features for clean and efficient code
- **Modular Architecture**: Well-organized module system for easy extension
- **Memory Management**: Smart pointer usage and memory usage monitoring
- **Cross-Platform**: Windows and Linux support with CMake build system
- **CMake Integration**: Easy integration with existing CMake projects

### 📦 Dependencies
- SDL3 and its extensions (SDL_image, SDL_ttf, SDL_mixer)
- CMake 3.14+ for building
- C++20 compatible compiler

### 📝 Notes
- This is a beta release for testing and feedback
- API may change in future versions
- Documentation is under development
