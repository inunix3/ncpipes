/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#pragma once

#include "config.h"

#include <stdbool.h>

bool parse_cli(int argc, char *argv[], config_t *cfg);
