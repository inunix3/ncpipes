/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#include <ncpipes/util.h>

#include <stdlib.h>
#include <time.h>

#define NANOSLEEP_NS_MAX 999999999
#define MS_TO_NS_MULTIPLIER 1000000
#define MS_TO_NS(_x) ((_x) * MS_TO_NS_MULTIPLIER)

int rand_int(int min, int max) {
    return rand() % (max + 1 - min) + min;
}

unsigned rand_uint(unsigned min, unsigned max) {
    return (unsigned) rand() % (max + 1 - min) + min;
}

bool rand_bool(void) {
    return rand_uint(0, 1);
}

bool int_fits_range(int value, int min, int max) {
    return value >= min && value <= max;
}

void sleep_ms(long ms) {
    long secs = 0;
    long ns = MS_TO_NS(ms);
    
    if (ns >= NANOSLEEP_NS_MAX) {
        secs = ms / 1000;
        ns = MS_TO_NS(ms - secs * 1000);
    }

    struct timespec ts = {
        secs,
        ns
    };

    nanosleep(&ts, NULL);
}
