#pragma once
#ifndef MYENGINE_RANDOM_H
#define MYENGINE_RANDOM_H
#include "../Libs.h"

namespace MyEngine {
    class RandomGenerator {
    public:
        RandomGenerator() = delete;
        RandomGenerator(RandomGenerator &&) = delete;
        RandomGenerator(const RandomGenerator &) = delete;
        RandomGenerator &operator=(RandomGenerator &&) = delete;
        RandomGenerator &operator=(const RandomGenerator &) = delete;
        ~RandomGenerator() = delete;

        static int randInt(int start, int end) {
            std::uniform_int_distribution<int> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static int64_t randBigInt(int64_t start, int64_t end) {
            std::uniform_int_distribution<int64_t> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static uint32_t randUInt(uint32_t start, uint32_t end) {
            std::uniform_int_distribution<uint32_t> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static uint64_t randBigUInt(uint64_t start, uint64_t end) {
            std::uniform_int_distribution<uint64_t> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static float randFloat(float start, float end) {
            std::uniform_real_distribution<float> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static double randDouble(double start, double end) {
            std::uniform_real_distribution<double> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static long double randBigDouble(long double start, long double end) {
            std::uniform_real_distribution<long double> uni(start, end);
            if (!_is_init) {
                _def_rand_eng.seed(time(nullptr));
                _is_init = true;
            }
            return uni(_def_rand_eng);
        }

        static std::random_device _rnd_dev;
        static std::default_random_engine _def_rand_eng;
        static bool _is_init;
    };

    inline std::random_device RandomGenerator::_rnd_dev{};
    inline std::default_random_engine RandomGenerator::_def_rand_eng{};
    inline bool RandomGenerator::_is_init{false};
}


#endif //MYENGINE_RANDOM_H
