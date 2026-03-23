#pragma once
#ifndef MYENGINE_UTILS_RANDOM_H
#define MYENGINE_UTILS_RANDOM_H
#include "../Libs.h"

namespace MyEngine {
    /**
     * \if EN
     * @brief Random Generator
     * @details Used for simply generating random values of different numbers
     * @note This class is a static class and does not require obtaining a global singleton.
     * \endif
     */
    class RandomGenerator {
    public:
        RandomGenerator() = delete;
        RandomGenerator(RandomGenerator &&) = delete;
        RandomGenerator(const RandomGenerator &) = delete;
        RandomGenerator &operator=(RandomGenerator &&) = delete;
        RandomGenerator &operator=(const RandomGenerator &) = delete;
        ~RandomGenerator() = delete;

        /**
         * \if EN
         * @brief Generate an integer within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns an integer within the `[start, end]` range
         * \endif
         */
        static int randInt(int start, int end) {
            std::uniform_int_distribution<int> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        /**
         * \if EN
         * @brief Generate a 64-bit integer within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns a 64-bit integer within the `[start, end]` range
         * \endif
         */
        static int64_t randBigInt(int64_t start, int64_t end) {
            std::uniform_int_distribution<int64_t> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        /**
         * \if EN
         * @brief Generate an unsigned integer within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns an unsigned integer within the `[start, end]` range
         * \endif
         */
        static uint32_t randUInt(uint32_t start, uint32_t end) {
            std::uniform_int_distribution<uint32_t> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        /**
         * \if EN
         * @brief Generate an unsigned 64-bit integer within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns an unsigned 64-bit integer within the `[start, end]` range
         * \endif
         */
        static uint64_t randBigUInt(uint64_t start, uint64_t end) {
            std::uniform_int_distribution<uint64_t> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        /**
         * \if EN
         * @brief Generate a single-precision floating-point number within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns a single-precision floating-point number within the `[start, end]` range
         * \endif
         */
        static float randFloat(float start, float end) {
            std::uniform_real_distribution<float> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        /**
         * \if EN
         * @brief Generate a double-precision floating-point number within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns a double-precision floating-point number within the `[start, end]` range
         * \endif
         */
        static double randDouble(double start, double end) {
            std::uniform_real_distribution<double> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        /**
         * \if EN
         * @brief Generate a longer double-precision floating-point number within a specified range randomly
         * @param start     Starting range
         * @param end       Ending range
         * @return Returns a longer double-precision floating-point number within the `[start, end]` range
         * \endif
         */
        static long double randBigDouble(long double start, long double end) {
            std::uniform_real_distribution<long double> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        inline static std::random_device _rnd_dev{};
        inline static std::default_random_engine _def_rand_eng{};
        inline static bool _is_init{};
    };
}


#endif //MYENGINE_UTILS_RANDOM_H
