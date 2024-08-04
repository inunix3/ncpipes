/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#include <ncpipes/pipe.h>
#include <ncpipes/util.h>

static const char *_charset[] = { "═", "║", "╔", "╗", "╚", "╝" };

/* maps the last direction and the new direction to charset indices */
static int _pipe_map[4][4] = {
    { 1, 0, 2, 3 },
    { 1, 1, 4, 5 },
    { 5, 3, 0, 0 },
    { 4, 2, 0, 0 },
};

static void advance(point_t *pos, direction_t dir) {
    switch (dir) {
    case DIR_UP:
        --pos->y;

        break;
    case DIR_DOWN:
        ++pos->y;

        break;
    case DIR_RIGHT:
        ++pos->x;

        break;
    case DIR_LEFT:
        --pos->x;

        break;
    }
}

static void put_ch(struct ncdirect *nc, point_t pos, const char *ch, uint64_t color) {
    ncdirect_cursor_move_yx(nc, pos.y, pos.x);
    ncdirect_putegc(nc, color, ch, NULL);
    ncdirect_flush(nc);
}

static void pipe_turn_dir(struct ncdirect *nc, pipe_t *pipe) {
    bool random_choice = rand_bool();
    direction_t new_dir;

    switch (pipe->dir) {
    case DIR_UP:
    case DIR_DOWN:
        new_dir = random_choice ? DIR_RIGHT : DIR_LEFT;

        break;
    case DIR_RIGHT:
    case DIR_LEFT:
        new_dir = random_choice ? DIR_UP : DIR_DOWN;

        break;
    }

    int idx = _pipe_map[pipe->dir][new_dir];
    put_ch(nc, pipe->pos, _charset[idx], pipe->color);

    pipe->dir = new_dir;
}

static double frame_rate_to_ms(int frame_rate) {
    return 1000.0 / (double) frame_rate;
}

int pipe_draw(struct ncdirect *nc, const config_t *cfg, pipe_t *pipe) {
    const char *pipe_ch = _charset[_pipe_map[pipe->dir][pipe->dir]];
    int drawn_chars = 0;

    for (int i = 0; i < pipe->length; ++i) {
        if (!int_fits_range(pipe->pos.x, 0, (int) ncdirect_dim_x(nc) - 1) ||
            !int_fits_range(pipe->pos.y, 0, (int) ncdirect_dim_y(nc) - 1)) {
            break;
        }

        if ((double) rand() / RAND_MAX < cfg->turning_probability) {
            pipe_turn_dir(nc, pipe);
            pipe_ch = _charset[_pipe_map[pipe->dir][pipe->dir]];
        } else {
            put_ch(nc, pipe->pos, pipe_ch, pipe->color);
        }

        ++drawn_chars;

        advance(&pipe->pos, pipe->dir);
        sleep_ms((int) frame_rate_to_ms(cfg->frame_rate));
    }

    return drawn_chars;
}
