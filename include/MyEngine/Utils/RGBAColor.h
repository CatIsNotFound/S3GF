#pragma once
#ifndef MYENGINE_UTILS_RGBACOLOR_H
#define MYENGINE_UTILS_RGBACOLOR_H
#include "../Basic.h"

namespace MyEngine {
    /**
     * \if EN
     * @namespace MyEngine::RGBAColor
     * @brief Predefined colors, named in RGBA format.
     * \endif
     * @ref related/RGBAColorMap.md
     */
    namespace RGBAColor {
        constexpr SColor Transparent = {255, 255, 255, 0};
        constexpr SColor HalfTransparent = {255, 255, 255, 128};
        constexpr SColor Black = {0, 0, 0, 255};
        constexpr SColor White = {255, 255, 255, 255};

        constexpr SColor RedPure = {255, 0, 0, 255};
        constexpr SColor RedDark = {153, 0, 0, 255};
        constexpr SColor RedDarker = {102, 0, 0, 255};
        constexpr SColor RedWine = {85, 0, 34, 255};
        constexpr SColor RedRose = {255, 26, 99, 255};
        constexpr SColor RedPink = {255, 105, 180, 255};
        constexpr SColor RedLightPink = {255, 179, 217, 255};
        constexpr SColor RedPalePink = {255, 218, 232, 255};
        constexpr SColor RedCoral = {255, 114, 86, 255};
        constexpr SColor RedOrange = {255, 69, 0, 255};
        constexpr SColor RedBrick = {178, 34, 34, 255};
        constexpr SColor RedRust = {183, 65, 14, 255};
        constexpr SColor RedScarlet = {255, 36, 0, 255};
        constexpr SColor RedMagenta = {255, 0, 255, 255};
        constexpr SColor RedFuchsia = {219, 112, 219, 255};
        constexpr SColor RedRosePink = {255, 52, 179, 255};
        constexpr SColor RedDeepPink = {255, 20, 147, 255};
        constexpr SColor RedPeachPink = {255, 153, 170, 255};
        constexpr SColor RedCherryBlossom = {255, 228, 225, 255};
        constexpr SColor RedRouge = {210, 0, 82, 255};
        constexpr SColor RedVermilion = {227, 38, 54, 255};
        constexpr SColor RedBegonia = {255, 99, 71, 255};
        constexpr SColor RedPomegranate = {204, 0, 0, 255};
        constexpr SColor RedLightTrans = {255, 192, 192, 192};
        constexpr SColor RedPaleTrans = {255, 224, 224, 192};
        constexpr SColor RedDarkTrans = {153, 50, 50, 192};
        constexpr SColor RedRoseTrans = {255, 105, 180, 192};
        constexpr SColor RedMagentaTrans = {255, 153, 255, 192};
        constexpr SColor RedCoralTrans = {255, 160, 122, 192};
        constexpr SColor RedOrangeTrans = {255, 127, 80, 192};
        constexpr SColor RedFuchsiaTrans = {230, 145, 230, 192};
        constexpr SColor RedBrickTrans = {199, 21, 133, 192};

        constexpr SColor GreenPure = {0, 255, 0, 255};
        constexpr SColor GreenDark = {0, 153, 0, 255};
        constexpr SColor GreenInk = {0, 102, 51, 255};
        constexpr SColor GreenOlive = {107, 142, 35, 255};
        constexpr SColor GreenGrass = {34, 139, 34, 255};
        constexpr SColor GreenLight = {144, 238, 144, 255};
        constexpr SColor GreenPale = {152, 251, 152, 255};
        constexpr SColor GreenMint = {189, 252, 201, 255};
        constexpr SColor GreenEmerald = {0, 201, 87, 255};
        constexpr SColor GreenJade = {50, 205, 50, 255};
        constexpr SColor GreenPine = {2, 94, 47, 255};
        constexpr SColor GreenWillow = {134, 204, 204, 255};
        constexpr SColor GreenBean = {194, 231, 181, 255};
        constexpr SColor GreenMoss = {115, 147, 115, 255};
        constexpr SColor GreenBamboo = {0, 168, 107, 255};
        constexpr SColor GreenLime = {50, 205, 50, 255};
        constexpr SColor GreenApple = {145, 238, 145, 255};
        constexpr SColor GreenForest = {34, 139, 34, 255};
        constexpr SColor GreenIvy = {7, 101, 68, 255};
        constexpr SColor GreenPeacock = {0, 166, 166, 255};
        constexpr SColor GreenCyan = {0, 255, 255, 255};
        constexpr SColor GreenTurquoise = {64, 224, 208, 255};
        constexpr SColor GreenMintCyan = {102, 255, 204, 255};
        constexpr SColor GreenLightTrans = {173, 255, 173, 192};
        constexpr SColor GreenPaleTrans = {204, 255, 204, 192};
        constexpr SColor GreenDarkTrans = {50, 153, 50, 192};
        constexpr SColor GreenOliveTrans = {138, 154, 70, 192};
        constexpr SColor GreenJadeTrans = {102, 255, 102, 192};
        constexpr SColor GreenLimeTrans = {127, 255, 0, 192};
        constexpr SColor GreenCyanTrans = {102, 255, 255, 192};
        constexpr SColor GreenTurquoiseTrans = {128, 240, 224, 192};
        constexpr SColor GreenMossTrans = {143, 188, 143, 192};
        constexpr SColor GreenBambooTrans = {51, 204, 153, 192};

        constexpr SColor BluePure = {0, 0, 255, 255};
        constexpr SColor BlueDark = {0, 0, 153, 255};
        constexpr SColor BlueNavy = {0, 32, 96, 255};
        constexpr SColor BlueSapphire = {0, 102, 204, 255};
        constexpr SColor BlueSky = {135, 206, 235, 255};
        constexpr SColor BlueLight = {173, 216, 230, 255};
        constexpr SColor BluePale = {204, 232, 255, 255};
        constexpr SColor BlueBaby = {176, 224, 230, 255};
        constexpr SColor BlueSea = {28, 134, 238, 255};
        constexpr SColor BlueLake = {0, 191, 255, 255};
        constexpr SColor BlueCobalt = {65, 105, 225, 255};
        constexpr SColor BlueIndigo = {75, 0, 130, 255};
        constexpr SColor BlueViolet = {138, 43, 226, 255};
        constexpr SColor BlueLavender = {191, 211, 254, 255};
        constexpr SColor BlueStone = {100, 149, 237, 255};
        constexpr SColor BlueIce = {240, 248, 255, 255};
        constexpr SColor BlueSteel = {70, 130, 180, 255};
        constexpr SColor BluePrussian = {29, 53, 87, 255};
        constexpr SColor BlueRoyal = {0, 64, 128, 255};
        constexpr SColor BluePeacock = {30, 144, 255, 255};
        constexpr SColor BlueCyan = {0, 255, 255, 255};
        constexpr SColor BlueLilac = {169, 184, 208, 255};
        constexpr SColor BlueDeepSky = {0, 191, 255, 255};
        constexpr SColor BlueLightTrans = {173, 216, 230, 192};
        constexpr SColor BluePaleTrans = {211, 235, 250, 192};
        constexpr SColor BlueDarkTrans = {65, 105, 225, 192};
        constexpr SColor BlueNavyTrans = {30, 144, 255, 192};
        constexpr SColor BlueSapphireTrans = {0, 128, 255, 192};
        constexpr SColor BlueIndigoTrans = {106, 90, 205, 192};
        constexpr SColor BlueVioletTrans = {153, 102, 255, 192};
        constexpr SColor BlueSeaTrans = {64, 170, 255, 192};
        constexpr SColor BlueSteelTrans = {100, 149, 237, 192};

        constexpr SColor MixYellow = {255, 255, 0, 255};
        constexpr SColor MixYellowDark = {204, 204, 0, 255};
        constexpr SColor MixYellowLemon = {255, 246, 143, 255};
        constexpr SColor MixOrangeYellow = {255, 165, 0, 255};
        constexpr SColor MixAmber = {255, 193, 37, 255};
        constexpr SColor MixGold = {255, 215, 0, 255};
        constexpr SColor MixPurple = {128, 0, 128, 255};
        constexpr SColor MixPurpleDark = {72, 61, 139, 255};
        constexpr SColor MixPurpleLight = {199, 21, 133, 255};
        constexpr SColor MixPurplePale = {216, 191, 216, 255};
        constexpr SColor MixViolet = {143, 0, 255, 255};
        constexpr SColor MixFuchsia = {255, 0, 255, 255};
        constexpr SColor MixCyan = {0, 255, 255, 255};
        constexpr SColor MixCyanDark = {0, 139, 139, 255};
        constexpr SColor MixCyanLight = {175, 238, 238, 255};
        constexpr SColor MixGray = {128, 128, 128, 255};
        constexpr SColor MixGrayDark = {64, 64, 64, 255};
        constexpr SColor MixGrayLight = {192, 192, 192, 255};
        constexpr SColor MixSilver = {192, 192, 192, 255};
        constexpr SColor MixBrown = {165, 42, 42, 255};
        constexpr SColor MixBrownDark = {101, 67, 33, 255};
        constexpr SColor MixBrownLight = {210, 180, 140, 255};
        constexpr SColor MixKhaki = {240, 230, 140, 255};
        constexpr SColor MixBeige = {245, 245, 220, 255};
        constexpr SColor MixCream = {255, 253, 208, 255};
        constexpr SColor MixYellowTrans = {255, 255, 153, 192};
        constexpr SColor MixPurpleTrans = {204, 153, 255, 192};
        constexpr SColor MixCyanTrans = {153, 255, 255, 192};
        constexpr SColor MixGrayTrans = {192, 192, 192, 192};
        constexpr SColor MixBrownTrans = {205, 133, 63, 192};
        constexpr SColor MixKhakiTrans = {245, 245, 205, 192};
        constexpr SColor MixAmberTrans = {255, 215, 105, 192};

        /**
         * \if EN
         * @brief Convert RGBA color to hexadecimal code
         * @param color             Specified RGBA color
         * @param alpha_enabled     Whether to include a transparent channel (default is `false`)
         * @return Return a converted hexadecimal code
         * \endif
         * @code
         * SDL_Color color = {128, 128, 128, 255};
         * uint64 hex_code = MyEngine::RGBAColor::RGBA2HexCode(color, false);
         * uint64 hex_code_alpha = MyEngine::RGBAColor::RGBA2HexCode(color, true);
         * //       hex_code = 0x808080   (hex)
         * // hex_code_alpha = 0x808080ff (hex with alpha)
         * @endcode
         * @see hexCode2RGBA
         */
        inline uint64_t RGBA2HexCode(const SDL_Color& color, bool alpha_enabled = false) {
            return alpha_enabled ? (color.r * 255 * 255 * 255 + color.g * 255 * 255 + color.b * 255 + color.a) :
                   (color.r * 256 * 256 + color.g * 256 + color.b);
        }

        /**
         * \if EN
         * @brief Convert hexadecimal code to RGBA color
         * @param color_value       Specified hex code
         * @param alpha_enabled     Whether to include a transparent channel (default is `false`)
         * @return Return a converted RGBA color
         * \endif
         * @code
         * SDL_Color color = MyEngine::RGBAColor::HexCode2RGBA(0x808080, false);
         * SDL_Color color_alpha = MyEngine::RGBAColor::HexCode2RGBA(0x808080c0, true)
         * //       color = SDL_Color(128, 128, 128, 255);
         * // color_alpha = SDL_Color(128, 128, 128, 192);
         * @endcode
         * @see RGBA2HexCode
         */
        inline SDL_Color hexCode2RGBA(uint64_t color_value, bool alpha_enabled = false) {
            return (alpha_enabled ? SDL_Color((color_value >> 24) & 255, (color_value >> 16) & 255, (color_value >> 8) & 255, (color_value & 255))
                    : SDL_Color((color_value >> 16) & 255, (color_value >> 8) & 255, (color_value & 255), 255));
        }
    }
}

#endif //MYENGINE_UTILS_RGBACOLOR_H
