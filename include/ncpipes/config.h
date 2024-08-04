/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#pragma once

#include <stdbool.h>

#define DEFAULT_FRAME_RATE   75
#define DEFAULT_COLOR_SET    COLOR_SET_PALETTE
#define DEFAULT_MAX_PIPE_LEN 400
#define DEFAULT_MIN_PIPE_LEN 30
#define DEFAULT_MAX_CHARS    4000
#define DEFAULT_TURNING_PROBABILITY 0.2

typedef enum color_set {
    COLOR_SET_NONE,
    COLOR_SET_PALETTE,
    COLOR_SET_RGB
} color_set_t;

typedef struct config {
    int frame_rate;
    color_set_t colors;
    bool clear_screen;

    int max_pipe_len;
    int min_pipe_len;
    int max_drawn_chars;
    double turning_probability;
} config_t;
