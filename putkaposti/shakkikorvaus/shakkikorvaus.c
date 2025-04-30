#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 8
#define AREA (SIZE * SIZE)
#define TARGET 24
#define PRINT 0

int count_sormustin(char lauta[AREA])
{
    int sormustin = 0;
    for (int i = 0; i < AREA; ++i)
    {
        if (lauta[i] == 'o')
            sormustin++;
    }
    return sormustin;
}

void print_lauta(char lauta[AREA])
{
    printf("Sormustin: %d\n", count_sormustin(lauta));
    for(int i = 0; i < AREA; ++i)
    {
        if (i != 0 && i % SIZE == 0)
            printf("\n");
        printf("%c", lauta[i]);
    }
    printf("\n--------------------\n");
}

int forms_rect(char lauta[AREA], int add)
{
    int row = add / SIZE;
    int col = add % SIZE;
    for (int i = row * SIZE; i < row * SIZE + SIZE; ++i)
    {
        if (add == i || lauta[i] == '.')
            continue;

        for (int j = col; j < AREA; j += SIZE)
        {
            if (add == j || lauta[j] == '.')
                continue;

            // now we have a side and a top/bot
            if (lauta[(i % SIZE) + (j / SIZE) * SIZE] == 'o')
                return 1;
        }
    }
    return 0;
}

int sormustin_in_line(char lauta[AREA])
{
    for (int i = 0; i < SIZE; ++i)
    {
        int row = 0;
        for (int j = 0; j < SIZE; ++j)
        {
            if (lauta[i * SIZE + j] == 'o')
                row++;
            if (row > limit)
                return 1;
        }
    }
}

int add_sormustin(char lauta[AREA], int last_add, int sormustin)
{
    for (int i = last_add + 1; i < AREA; ++i)
    {
        if (sormustin == 0)
        {
            printf("%.1f%%\r", ((i - last_add - 1) * 100) / ((double)AREA - last_add));
            fflush(stdout);
        }
        int space_left = AREA - i;
        int space_needed = TARGET - sormustin;
        if (space_left < space_needed)
            return 0;
        if (forms_rect(lauta, i))
            continue;

        lauta[i] = 'o';
        if (PRINT)
            print_lauta(lauta);
        if (sormustin + 1 >= TARGET)
        {
            printf("Last move: %d\n", i);
            return 1;
        }
        if (add_sormustin(lauta, i, sormustin + 1) == 1)
            return 1;
        lauta[i] = '.';
    }
    return 0;
}

int main(void)
{
    srand(time(NULL));
    char lauta[AREA];
    memset(lauta, '.', AREA);
    
    printf("result found: %d\n", add_sormustin(lauta, -1, 0));
    print_lauta(lauta);

    return 0;
}