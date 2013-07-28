#ifndef UTILS_H_
#define UTILS_H_

#include <irene_common.h>

namespace NetworkUtils
{
    static uint64_t acquire_next_seesion_id()
    {
        static uint64_t sequence = 0x0000000000000001;
        return sequence++;
    }
}

#endif //UTILS_H_
