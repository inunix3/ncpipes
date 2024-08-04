/*
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#include <ncpipes/cli.h>
#include <ncpipes/config.h>
#include <ncpipes/direction.h>
#include <ncpipes/pipe.h>
#include <ncpipes/point.h>
#include <ncpipes/util.h>

#include <argtable3/argtable3.h>
#include <notcurses/direct.h>

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static struct ncdirect *_nc;
static config_t _cfg;

static uint64_t gen_pipe_color(void) {
    uint64_t channels = 0;

    if (_cfg.colors == COLOR_SET_RGB) {
        unsigned r = rand_uint(0, 0xff);
        unsigned g = rand_uint(0, 0xff);
        unsigned b = rand_uint(0, 0xff);

        channels = NCCHANNELS_INITIALIZER(r, g, b, 0, 0, 0);
    } else if (_cfg.colors == COLOR_SET_PALETTE) {
        channels = NCCHANNELS_INITIALIZER(0, 0, 0, 0, 0, 0);

        ncchannels_set_fg_palindex(&channels, rand_uint(0, ncdirect_palette_size(_nc) - 1));
    } else if (_cfg.colors == COLOR_SET_NONE) {
        ncchannels_set_fg_default(&channels);
    }

    ncchannels_set_bg_default(&channels);

    return channels;
}

static void main_loop(void) {
    int drawn_chars = 0;

    while (true) {
        point_t pipe_pos = { rand_int(0, (int) ncdirect_dim_x(_nc) - 1), rand_int(0, (int) ncdirect_dim_y(_nc) - 1) };
        direction_t dir = (direction_t) rand_int(0, 3);
        pipe_t pipe = { pipe_pos, dir, rand_int(_cfg.min_pipe_len, _cfg.max_pipe_len), gen_pipe_color() };

        drawn_chars += pipe_draw(_nc, &_cfg, &pipe);

        if (_cfg.max_drawn_chars != 0 && drawn_chars >= _cfg.max_drawn_chars) {
            drawn_chars = 0;

            if (_cfg.clear_screen) {
                ncdirect_clear(_nc);
            }

            sleep_ms(500);
        }
    }
}

static bool _init_graphics(void) {
    _nc = ncdirect_core_init(NULL, stdout, NCDIRECT_OPTION_NO_QUIT_SIGHANDLERS | NCDIRECT_OPTION_DRAIN_INPUT);

    if (!_nc) {
        return NULL;
    }

    ncdirect_cursor_disable(_nc);

    if (_cfg.clear_screen) {
        ncdirect_clear(_nc);
    }

    return true;
}

static void set_signal_handler(int signal, void (*handler)(int signal_id)) {
    struct sigaction action;

    action.sa_handler = handler;

    sigaction(signal, &action, NULL);
}

static void sigint_handler(int signal_id) {
    (void) signal_id;

    ncdirect_clear(_nc);
    ncdirect_stop(_nc);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));

    if (!parse_cli(argc, argv, &_cfg)) {
        return EXIT_FAILURE;
    }

    if (!_init_graphics()) {
        fputs("error: cannot initialize ncdirect\n", stderr);

        return EXIT_FAILURE;
    }

    set_signal_handler(SIGINT, sigint_handler);

    main_loop();

    return EXIT_SUCCESS;
}
