#pragma once

#include "FLAGS.h"
#include <random>
#include <QRandomGenerator>

//TODO: write more efficient code.

class RNG
{

public:
    static int GetRandomNumber(int min, int max);
    static int GetRandomNumber(int max);
};

