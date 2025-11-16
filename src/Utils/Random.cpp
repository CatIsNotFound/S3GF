#include "Random.h"

namespace S3GF {
    time_t RandomGenerator::time_seed{time(0)};
    int RandomGenerator::randInt(int start, int end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<int> r(start, end);
        return r(rand_eng);
    }

    int64_t RandomGenerator::randBigInt(int64_t start, int64_t end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<int64_t> r(start, end);
        return r(rand_eng);
    }

    uint32_t RandomGenerator::randUInt(uint32_t start, uint32_t end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<uint32_t> r(start, end);
        return r(rand_eng);
    }

    uint64_t RandomGenerator::randBigUInt(uint64_t start, uint64_t end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<uint64_t> r(start, end);
        return r(rand_eng);
    }

    float RandomGenerator::randFloat(float start, float end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_real_distribution<float> r(start, end);
        return r(rand_eng);
    }

    double RandomGenerator::randDouble(double start, double end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_real_distribution<double> r(start, end);
        return r(rand_eng);
    }
}