#ifndef PUPU_H
#define PUPU_H

#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#define PRINT_WIDTH 42
#define PRINT_AREA (PRINT_WIDTH * PRINT_WIDTH)
#define TAB_SIZE 4
#define TOP 1
#define BOT 2
#define BOTH (TOP | BOT)

#define TOP_MASK 1
#define BOT_MASK 2

#define SUCCESS 0
#define FAILURE 1
#define FAILURE_MAX_PROCESS 2

#define RESET "\x1b[0m"

clock_t timer;

void print_fg_color(int c)
{
    printf("\x1b[38;5;%dm", c % 256);
}

void print_bg_color(int c)
{
    printf("\x1b[48;5;%dm", c % 256);
}

void *my_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
        exit(ENOMEM);
    memset(ptr, 0, size);
    return ptr;
}

void start_timer()
{
    timer = clock();
}

double stop_timer()
{
    return (double)(clock() - timer) / CLOCKS_PER_SEC;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

char *find_next_not_of(char *str, char *set)
{
    while (*str != '\0' && strchr(set, *str) != NULL)
        str++;
    return str;
}

static void print_sep(char end)
{
    printf("╞");
    for (int i = 1; i < PRINT_WIDTH + 3; ++i)
        printf("=");
    printf("╡%c", end);
    fflush(stdout);
}

static void print_sides(char end)
{
    printf("|");
    for (int i = 1; i < PRINT_WIDTH + 3; ++i)
        printf(" ");
    printf("|%c", end);
    printf("| ");
    fflush(stdout);
}

static int wordlen(char *word)
{
    int len = 0;
    while (word[len] != '\0' && !isspace(word[len]))
        len++;
    return len;
}

static char *print_line(int setting, char *line)
{
    print_sides('\r');
    size_t line_space = PRINT_WIDTH;
    while (*line != '\0' && line_space > 0)
    {
        line = find_next_not_of(line, "\v\f");
        int word_len = min(wordlen(line), PRINT_WIDTH);
        if (*line == '\n')
        {
            line++;
            break;
        }
        else if (*line == '\t')
        {
            int t = TAB_SIZE - ((PRINT_WIDTH - line_space) % TAB_SIZE);
            for (int i = 0; i < t; ++i)
                printf(" ");
            line++;
            line_space -= t;
        }
        else if (*line == '\r')
        {
            printf("\r| ");
            line++;
            line_space = PRINT_WIDTH;
        }
        else if (*line == ' ')
        {
            printf(" ");
            line++;
            line_space--;
        }
        else if (*line != '\0' && word_len <= line_space)
        {
            printf("%.*s", word_len, line);
            line += word_len;
            line_space -= word_len;
        }
        else
            break;
    }
    printf("\n");
    return line;
}

void print(int setting, char *format, ...)
{
    va_list args;
    va_start(args, format);

    char *text = my_malloc(PRINT_AREA + 1);
    char *ptr = text;
    vsnprintf(text, PRINT_AREA + 1, format, args);

    if (setting & TOP_MASK)
        print_sep('\n');
    while (*text != '\0')
        text = print_line(setting, text);
    if (setting & BOT_MASK)
        print_sep('\n');

    va_end(args);
    free(ptr);
}

int start_process(int *pids, void *data, int (*func)(void *))
{
    // check if maximum processes are already running
    int *pid_ptr = NULL;
    while (pids != NULL)
        if (*pids == 0)
            pid_ptr = pids;
    if (pid_ptr == NULL)
        return FAILURE_MAX_PROCESS;

    *pid_ptr = fork();
    if (*pid_ptr < 0)
        return FAILURE;
    if (*pid_ptr > 0)
        return SUCCESS;

    start_timer();
    func(data);
    print(BOTH, "%.1f s", stop_timer());
    exit(SUCCESS);
}

#endif