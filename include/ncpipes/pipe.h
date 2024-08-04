/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#pragma once

#include "config.h"
#include "direction.h"
#include "point.h"

#include <notcurses/direct.h>

#include <stdint.h>

typedef struct pipe {
    point_t pos;
    direction_t dir;

    int length;

    uint64_t color;
} pipe_t;

int pipe_draw(struct ncdirect *nc, const config_t *cfg, pipe_t *pipe);
