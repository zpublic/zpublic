#pragma once

unsigned int g_seed = 0;

class Random_helper
{
    Random_helper()
    {
        Random_Seed();
    }
};

Random_helper g_random_helper;

void Random_Seed(int seed)
{
    if (!seed)
        g_seed = ::timeGetTime();
    else
        g_seed = seed;
}

int Random_Int(int min, int max)
{
    g_seed = 214013 * g_seed + 2531011;
    return min + (g_seed ^ g_seed >> 15) % (max - min + 1);
}

float Random_Float(float min, float max)
{
    g_seed = 214013 * g_seed + 2531011;
    return min + ( g_seed>>16 ) * (1.0f / 65535.0f) * (max - min);
}
