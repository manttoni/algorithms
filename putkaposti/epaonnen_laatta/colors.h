#ifndef COLORS_H
#define COLORS_H

#include <stdlib.h>

#define RESET       "\x1b[0m"

#define RESET           "\x1b[0m"
#define WHITE           "\x1b[38;5;15m"
#define RED             "\x1b[38;5;1m"
#define GREEN           "\x1b[38;5;2m"
#define YELLOW          "\x1b[38;5;3m"
#define BLUE            "\x1b[38;5;4m"
#define MAGENTA         "\x1b[38;5;5m"
#define CYAN            "\x1b[38;5;6m"
#define LIGHT_GRAY      "\x1b[38;5;7m"
#define DARK_GRAY       "\x1b[38;5;8m"
#define BRIGHT_RED      "\x1b[38;5;9m"
#define BRIGHT_GREEN    "\x1b[38;5;10m"
#define BRIGHT_YELLOW   "\x1b[38;5;11m"
#define BRIGHT_BLUE     "\x1b[38;5;12m"
#define BRIGHT_MAGENTA  "\x1b[38;5;13m"
#define BRIGHT_CYAN     "\x1b[38;5;14m"
#define ORANGE          "\x1b[38;5;202m"
#define SKY_BLUE        "\x1b[38;5;39m"  // An extra bright, distinguishable color

const char *colors[17] = {
    WHITE,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    LIGHT_GRAY,
    DARK_GRAY,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    ORANGE,
    SKY_BLUE
};


#endif