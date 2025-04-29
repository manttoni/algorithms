#include "siniristilippu.h"

void move_cursor(int dir, size_t len)
{
    if (dir == UP)
    {
        printf("\033[%ldA", len);
    }
    else
    {
        printf("\033[%ldB", len);
    }
}

void *my_malloc(size_t size)
{
    void *ret = malloc(size);
    if (ret == NULL)
    {
        print(BOTH, "Error: malloc failed with size %ld", size);
        exit(ENOMEM);
    }
    memset(ret, 0, size);
    return ret;
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
    for (int i = 1; i < PRINT_WIDTH * 2 + 3; ++i)
        printf("=");
    printf("╡%c", end);
    fflush(stdout);
}

static void print_sides(char end)
{
    printf("|");
    for (int i = 1; i < PRINT_WIDTH * 2 + 3; ++i)
        printf(" ");
    printf("|%c", end);
    printf("| ");
    fflush(stdout);
}

static size_t wordlen(char *word)
{
    int len = 0;
    while (word[len] != '\0' && !isspace(word[len]))
        len++;
    return len;
}

static char *print_line(int setting, char *line)
{
    print_sides('\r');
    size_t line_space = PRINT_WIDTH * 2;
    while (*line != '\0' && line_space > 0)
    {
        line = find_next_not_of(line, "\v\f");
        size_t word_len = min(wordlen(line), PRINT_WIDTH * 2);
        if (*line == '\n')
        {
            line++;
            break;
        }
        else if (*line == '\t')
        {
            int t = TAB_SIZE - ((PRINT_WIDTH * 2 - line_space) % TAB_SIZE);
            for (int i = 0; i < t; ++i)
                write(1, " ", 1);
            line++;
            line_space -= t;
        }
        else if (*line == '\r')
        {
            write(1, "\r| ", 3);
            line++;
            line_space = PRINT_WIDTH * 2;
        }
        else if (*line == ' ')
        {
            write(1, " ", 1);
            line++;
            line_space--;
        }
        else if (*line != '\0' && word_len <= line_space)
        {
            if (setting & FLAG_MASK)
            {
                for (size_t i = 0; i < word_len; ++i)
                {
                    if (*line == 'W')
                        printf("%s", WHITE);
                    else if (*line == 'B')
                        printf("%s", BLUE);
                    else
                        printf("%s", BLACK);
                    fflush(stdout);
                    write(1, FULL_BLOCK, 6);
                    printf("%s", RESET);
                    line++;
                }
            }
            else
            {
                write(1, line, word_len);
                line += word_len;
            }
            line_space -= word_len;
        }
        else
            break;
    }
    write(1, "\n", 1);
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