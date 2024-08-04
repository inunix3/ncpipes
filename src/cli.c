/* 
 * Copyright (c) 2023-present, eaxecx.
 * This file is distributed under the MIT license (https://opensource.org/license/mit/)
 */

#include <ncpipes/cli.h>

#include <argtable3/argtable3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_SPECIFIED(_arg_name) (_arg_name->count > 0)
#define SIZE_OF_ARRAY(_array) (sizeof(_array) / sizeof(_array[0]))

/* the extra level of indirections expands the argument (if it is a macro) */
#define STR_(_x) #_x
#define STR(_x)  STR_(_x)

typedef struct args {
    arg_lit_t *disable_clear_screen;
    arg_int_t *max_pipe_len;
    arg_int_t *min_pipe_len;
    arg_int_t *max_drawn_chars;
    arg_str_t *color;
    arg_int_t *frame_rate;
    arg_dbl_t *turning_probability;
    arg_lit_t *version;
    arg_lit_t *help;
    arg_end_t *end;
} args_t;

static bool check_int_arg_is_negative(const arg_int_t *arg, const char *name) {
    if (arg->ival[0] < 0) {
        fprintf(stderr, "%s cannot be negative\n", name);

        return true;
    }

    return false;
}

static bool check_dbl_arg_is_negative(const arg_dbl_t *arg, const char *name) {
    if (ARG_SPECIFIED(arg) && arg->dval[0] < 0) {
        fprintf(stderr, "%s cannot be negative\n", name);

        return true;
    }

    return false;
}

static bool check_arg_values_correctness(const args_t *args) {
    if (check_int_arg_is_negative(args->max_pipe_len,        "maximal pipe length") ||
        check_int_arg_is_negative(args->min_pipe_len,        "minimal pipe length") ||
        check_int_arg_is_negative(args->max_drawn_chars,     "maximum of drawn characters") ||
        check_dbl_arg_is_negative(args->turning_probability, "turning probability")) {

        return false;
    } else if (ARG_SPECIFIED(args->color)) {
        const char *value = args->color->sval[0];

        if (strcmp(value, "rgb") != 0 && strcmp(value, "palette") != 0 && strcmp(value, "none")) {
            fputs("bad color value (available options are: rgb, palette or none)\n", stderr);

            return false;
        }
    } else if (ARG_SPECIFIED(args->frame_rate) && args->frame_rate->ival[0] == 0) {
        fputs("frame rate cannot be zero\n", stderr);

        return false;
    } else if (ARG_SPECIFIED(args->frame_rate) && args->frame_rate->ival[0] < 0) {
        fputs("frame rate cannot be negative\n", stderr);

        return false;
    } else if (ARG_SPECIFIED(args->max_pipe_len) && ARG_SPECIFIED(args->min_pipe_len) &&
               args->max_pipe_len->ival[0] <= args->min_pipe_len->ival[0]) {
        fputs("maximal pipe length cannot be less than or equal to the minimal pipe length\n", stderr);

        return false;
    }

    return true;
}

static color_set_t str_to_color_set(const char *str) {
    if (strcmp(str, "rgb") == 0) {
        return COLOR_SET_RGB;
    } else if (strcmp(str, "palette") == 0) {
        return COLOR_SET_PALETTE;
    }

    return COLOR_SET_NONE;
}

static void map_args_to_config(const args_t *args, config_t *cfg) {
    cfg->colors = ARG_SPECIFIED(args->color) ? str_to_color_set(args->color->sval[0]) : DEFAULT_COLOR_SET;
    cfg->frame_rate   = ARG_SPECIFIED(args->frame_rate) ? args->frame_rate->ival[0] : DEFAULT_FRAME_RATE;
    cfg->clear_screen = !ARG_SPECIFIED(args->disable_clear_screen);
    cfg->max_pipe_len = ARG_SPECIFIED(args->max_pipe_len) ? args->max_pipe_len->ival[0] : DEFAULT_MAX_PIPE_LEN;
    cfg->min_pipe_len = ARG_SPECIFIED(args->min_pipe_len) ? args->min_pipe_len->ival[0] : DEFAULT_MIN_PIPE_LEN;
    cfg->max_drawn_chars = ARG_SPECIFIED(args->max_drawn_chars) ? args->max_drawn_chars->ival[0] : DEFAULT_MAX_CHARS;
    cfg->turning_probability = ARG_SPECIFIED(args->turning_probability) ? args->turning_probability->dval[0] :
        DEFAULT_TURNING_PROBABILITY;
}

bool parse_cli(int argc, char *argv[], config_t *cfg) {
    args_t args = {
        arg_lit0("d", NULL,       "don't clear screen on startup or when the maximum of drawn characters is exceeded"),
        arg_int0("x", NULL, NULL, "the maximal pipe length (default = " STR(DEFAULT_MAX_PIPE_LEN) ")"),
        arg_int0("n", NULL, NULL, "the minimal pipe length (default = " STR(DEFAULT_MIN_PIPE_LEN) ")"),
        arg_int0("c", NULL, NULL, "maximum drawn characters. If the limit is exceeded, the screen is cleared"
                                  "(unless option -d is specified) (default = " STR(DEFAULT_MAX_CHARS) ")"),
        arg_str0("C", NULL, NULL, "what colors use: rgb, palette or none (default = palette)"),
        arg_int0("f", NULL, NULL, "frame rate (default = " STR(DEFAULT_FRAME_RATE) ")"),
        arg_dbl0("p", NULL, NULL, "the turning probability (default = " STR(DEFAULT_TURNING_PROBABILITY) ")"),
        arg_lit0("v", NULL,       "print version and exit"),
        arg_lit0("h", NULL,       "print this message and exit"),
        arg_end(1)
    };

    bool success = false;
    void *argtable[] = {
        args.disable_clear_screen,
        args.max_pipe_len,
        args.min_pipe_len,
        args.max_drawn_chars,
        args.color,
        args.frame_rate,
        args.turning_probability,
        args.version,
        args.help,
        args.end
    };

    if (arg_nullcheck(argtable) != 0) {
        fputs("cannot parse cli: arg table was not successfully allocated\n", stderr);

        return false;
    }

    int err_count = arg_parse(argc, argv, argtable);

    if (err_count > 0) {
        arg_print_errors(stderr, args.end, "ncpipes");
        puts("Type 'ncpipes -h' to see available options");

        goto exit;
    } else if (!check_arg_values_correctness(&args)) {
        goto exit;
    }

    if (ARG_SPECIFIED(args.help)) {
        puts("usage: ncpipes [OPTION]...");
        puts("Terminal 2D version of the famous 3D Pipes Screensaver from Windows. Uses notcurses for graphics.");
        arg_print_glossary(stdout, argtable, "  %-15s %s\n");

        arg_freetable(argtable, SIZE_OF_ARRAY(argtable));
        exit(EXIT_SUCCESS);
    } else if (ARG_SPECIFIED(args.version)) {
        puts("ncpipes v1.0.0");

        arg_freetable(argtable, SIZE_OF_ARRAY(argtable));
        exit(EXIT_SUCCESS);
    }

    map_args_to_config(&args, cfg);

    success = true;
exit:
    arg_freetable(argtable, SIZE_OF_ARRAY(argtable));

    return success;
}
