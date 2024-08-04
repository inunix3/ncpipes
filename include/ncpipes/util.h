/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/* these two functions generate random numbers in the range [min; max] */
int rand_int(int min, int max);
unsigned rand_uint(unsigned min, unsigned max);
bool rand_bool(void);

/* checks if value is in the range [min; max] */
bool int_fits_range(int value, int min, int max);
void sleep_ms(long ms);
