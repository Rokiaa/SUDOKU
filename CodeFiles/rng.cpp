#include "RNG.h"

QRandomGenerator rng;

int RNG::GetRandomNumber(int min, int max)
{
#if NO_RANDOMIZATION
    return 0;
#endif

    return rng.bounded(min, max);

    //std::random_device dev;
    //std::mt19937 rng(dev());

    //// distribution in range [min, max]
    //std::uniform_int_distribution<std::mt19937::result_type> rand(min, max);

    //return rand(rng);
}

int RNG::GetRandomNumber(int max)
{
    return rng.bounded(max);
    //return GetRandomNumber(0, max);
}
