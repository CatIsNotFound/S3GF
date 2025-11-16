#pragma once
#ifndef S3GF_RANDOM_H
#define S3GF_RANDOM_H
#include "Libs.h"

namespace S3GF {
    class RandomGenerator {
    public:
        RandomGenerator() = delete;
        RandomGenerator(RandomGenerator &&) = delete;
        RandomGenerator(const RandomGenerator &) = delete;
        RandomGenerator &operator=(RandomGenerator &&) = delete;
        RandomGenerator &operator=(const RandomGenerator &) = delete;
        ~RandomGenerator() = delete;

        static int randInt(int start, int end);
        static int64_t randBigInt(int64_t start, int64_t end);
        static uint32_t randUInt(uint32_t start, uint32_t end);
        static uint64_t randBigUInt(uint64_t start, uint64_t end);
        static float randFloat(float start, float end);
        static double randDouble(double start, double end);
        static time_t time_seed;
    };
}


#endif //S3GF_RANDOM_H
