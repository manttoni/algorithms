#ifndef SINIRISTILIPPU_H
#define SINIRISTILIPPU_H

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>

#ifndef WIDTH
#define WIDTH 18
#endif

#define HEIGHT ((WIDTH / 18) * 11)
#define AREA (WIDTH * HEIGHT)

#define PRINT_FLAG_WIDTH_STRETCH 2
#define PRINT_FLAG_HEIGHT_STRETCH 2

#define PRINT_WIDTH (18 * PRINT_FLAG_WIDTH_STRETCH)
#define PRINT_HEIGHT (11 * PRINT_FLAG_HEIGHT_STRETCH)
#define PRINT_AREA (PRINT_WIDTH * PRINT_HEIGHT)

#define TAB_SIZE 4
#define TOP 1
#define BOT 2
#define BOTH 3
#define FLAG 4

#define TOP_MASK 0
#define BOT_MASK 1
#define FLAG_MASK 4

#define UP 1
#define DOWN 0

#define STOP_BUTTON ⏹
#define FULL_BLOCK "██"
#define BLACK_SQUARE ■
#define BLUE "\033[38;2;0;47;108m"
#define WHITE "\033[38;2;255;255;255m"
#define BLACK "\033[30m"
#define RESET "\033[0m"

void *my_malloc(size_t size);
void print(int sep, char *format, ...);
void move_cursor(int dir, size_t len);

#endif