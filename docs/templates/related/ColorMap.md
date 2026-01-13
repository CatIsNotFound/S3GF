# Color Namespace Definition Table

This table lists the color names, hex codes, and RGBA values for the standard colors defined in the `MyEngine::StdColor` namespace.

For more details, please refer to the [RGBA Color Namespace Definition Table](RGBAColorMap.md).

## Hex code conversion to RGBA color

1. Include the header file `<MyEngine/Utils/RGBAColor.h>`. **If `<MyEngine/MyEngine>` has already been included in the header file, there is no need to include it again.**

2. Hex code conversion to RGBA color

   Convert hexadecimal codes to RGBA colors by using the `RGBAColor::RGBA2HexCode()` function.

   ```cpp
   SColor color = RGBAColor::RGBA2HexCode(0xff0000); // Red
   ```

   For hexadecimal codes that contain an alpha channel, you just need to set the second parameter to `true` when calling the `RGBAColor::RGBA2HexCode()` function in order to convert it.

   ```cpp
   SColor color = RGBAColor::RGBA2HexCode(0xff0000C0, true); // RedTransport
   ```

## StdColor

| Color Name                                                     | Hex code (without Alpha) | Hex code | RGBA (SDL_Color) |
| ------------------------------------------------------------ | ------------------------------------------- | ------------------------- | ---------------------------- |
| <div style="width: 32px; height: 32px; background-color: #000000"></div>Black | `0x0`                                       | `0xff`                    | `SColor(0, 0, 0, 255)`             |
| <div style="width: 32px; height: 32px; background-color: #FFFFFF"></div>White | `0xffffff`                                  | `0xffffffff`              | `SColor(255, 255, 255, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #FF0000"></div>Red | `0xff0000`                                  | `0xff0000ff`              | `SColor(255, 0, 0, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #00FF00"></div>Green | `0x00ff00`                                  | `0x00ff00ff`              | `SColor(0, 255, 0, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #0000FF"></div>Blue | `0x0000ff`                                  | `0x0000ffff`              | `SColor(0, 0, 255, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #FFFF00"></div>Yellow | `0xffff00`                                  | `0xffff00ff`              | `SColor(255, 255, 0, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #00FFFF"></div>Cyan | `0x00ffff`                                  | `0x00ffffff`              | `SColor(0, 255, 255, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FF00FF"></div>Magenta | `0xff00ff`                                  | `0xff00ffff`              | `SColor(255, 0, 255, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #800000"></div>DarkRed | `0x800000`                                  | `0x800000ff`              | `SColor(128, 0, 0, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #C80000"></div>FireRed | `0xc80000`                                  | `0xc80000ff`              | `SColor(200, 0, 0, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #FF3232"></div>BrightRed | `0xff3232`                                  | `0xff3232ff`              | `SColor(255, 50, 50, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FF7F50"></div>Coral | `0xff7f50`                                  | `0xff7f50ff`              | `SColor(255, 127, 80, 255)`        |
| <div style="width: 32px; height: 32px; background-color: #FA8072"></div>Salmon | `0xfa8072`                                  | `0xfa8072ff`              | `SColor(250, 128, 114, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #FFAEB9"></div>LightPink | `0xffaeb9`                                  | `0xffaeb9ff`              | `SColor(255, 174, 185, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #008000"></div>DarkGreen | `0x008000`                                  | `0x008000ff`              | `SColor(0, 128, 0, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #228B22"></div>ForestGreen | `0x228b22`                                  | `0x228b22ff`              | `SColor(34, 139, 34, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #32CD32"></div>LimeGreen | `0x32cd32`                                  | `0x32cd32ff`              | `SColor(50, 205, 50, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #BDFCC9"></div>Mint | `0xbdfcc9`                                  | `0xbdfcc9ff`              | `SColor(189, 252, 201, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #90EE90"></div>LightGreen | `0x90ee90`                                  | `0x90ee90ff`              | `SColor(144, 238, 144, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #808000"></div>Olive | `0x808000`                                  | `0x808000ff`              | `SColor(128, 128, 0, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #000080"></div>DarkBlue | `0x000080`                                  | `0x000080ff`              | `SColor(0, 0, 128, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #002040"></div>Navy | `0x002040`                                  | `0x002040ff`              | `SColor(0, 32, 64, 255)`           |
| <div style="width: 32px; height: 32px; background-color: #87CEEB"></div>SkyBlue | `0x87ceeb`                                  | `0x87ceebff`              | `SColor(135, 206, 235, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #ADD8E6"></div>LightBlue | `0xadd8e6`                                  | `0xadd8e6ff`              | `SColor(173, 216, 230, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #4169E1"></div>RoyalBlue | `0x4169e1`                                  | `0x4169e1ff`              | `SColor(65, 105, 225, 255)`        |
| <div style="width: 32px; height: 32px; background-color: #4B0082"></div>Indigo | `0x4b0082`                                  | `0x4b0082ff`              | `SColor(75, 0, 130, 255)`          |
| <div style="width: 32px; height: 32px; background-color: #FFD700"></div>Gold | `0xffd700`                                  | `0xffd700ff`              | `SColor(255, 215, 0, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FFA500"></div>Orange | `0xffa500`                                  | `0xffa500ff`              | `SColor(255, 165, 0, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FF8C00"></div>DarkOrange | `0xff8c00`                                  | `0xff8c00ff`              | `SColor(255, 140, 0, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FFDAB9"></div>Peach | `0xffdab9`                                  | `0xffdab9ff`              | `SColor(255, 218, 185, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #FFC125"></div>Amber | `0xffc125`                                  | `0xffc125ff`              | `SColor(255, 193, 37, 255)`        |
| <div style="width: 32px; height: 32px; background-color: #F0E68C"></div>Khaki | `0xf0e68c`                                  | `0xf0e68cff`              | `SColor(240, 230, 140, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #800080"></div>Purple | `0x800080`                                  | `0x800080ff`              | `SColor(128, 0, 128, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #483D8B"></div>DarkPurple | `0x483d8b`                                  | `0x483d8bff`              | `SColor(72, 61, 139, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #9400D3"></div>Violet | `0x9400d3`                                  | `0x9400d3ff`              | `SColor(148, 0, 211, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FF00FF"></div>Fuchsia | `0xff00ff`                                  | `0xff00ffff`              | `SColor(255, 0, 255, 255)`         |
| <div style="width: 32px; height: 32px; background-color: #FFC0CB"></div>Pink | `0xffc0cb`                                  | `0xffc0cbff`              | `SColor(255, 192, 203, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #E6E6FA"></div>Lavender | `0xe6e6fa`                                  | `0xe6e6faff`              | `SColor(230, 230, 250, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #808080"></div>Gray | `0x808080`                                  | `0x808080ff`              | `SColor(128, 128, 128, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #C8C8C8"></div>LightGray | `0xc8c8c8`                                  | `0xc8c8c8ff`              | `SColor(200, 200, 200, 255)`       |
| <div style="width: 32px; height: 32px; background-color: #404040"></div>DarkGray | `0x404040`                                  | `0x404040ff`              | `SColor(64, 64, 64, 255)`          |


