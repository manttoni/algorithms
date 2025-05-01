#include "colors.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE 13
#define AREA (SIZE * SIZE)

int x_l(char *laatta, int i)
{
    char c = laatta[i];
    int uple = strchr(laatta, c) - laatta;
    int lori = strrchr(laatta, c) - laatta;
    int uplex = uple % SIZE;
    int lorix = lori % SIZE;
    return lorix - uplex + 1;
}

int y_l(char *laatta, int i)
{
    char c = laatta[i];
    int uple = strchr(laatta, c) - laatta;
    int lori = strrchr(laatta, c) - laatta;
    int upley = uple / SIZE;
    int loriy = lori / SIZE;
    return loriy - upley + 1;
}

int area_l(char *laatta, int i)
{
    char c = laatta[i];
    int uple = strchr(laatta, c) - laatta;
    int lori = strrchr(laatta, c) - laatta;
    int uplex = uple % SIZE;
    int lorix = lori % SIZE;
    int upley = uple / SIZE;
    int loriy = lori / SIZE;
    return (lorix - uplex + 1) * (loriy - upley + 1);
}

int pieces_l(char *laatta)
{
    char biggest = 'A';
    for (int i = 0; i < AREA; ++i)
        biggest = max(laatta[i], biggest);
    return biggest - 'A' + 1;
}

void print_l(char *laatta)
{
    printf("Pieces: %d\n", pieces_l(laatta));
    for(int i = 0; i < AREA; ++i)
    {
        if (i != 0 && i % SIZE == 0)
            printf("\n");
        printf("%s", colors[laatta[i] - 'A']);
        printf("%c", laatta[i]);
    }
    printf("%s\n", RESET);
}

int has_area(char *laatta, int area)
{
    for (int i = 0; i < pieces_l(laatta); ++i)
    {
        if (area_l(laatta, strchr(laatta, 'A' + i) - laatta) == area)
            return 1;
    }
    return 0;
}

char *init_l()
{
    printf("Size: %d\nArea: %d\n", SIZE, AREA);
    char *laatta = malloc(AREA + 1);
    memset(laatta, 'A', AREA);
    laatta[AREA] = '\0';
    assert(area_l(laatta, 0) == AREA);
    assert(pieces_l(laatta) == 1);
    assert(x_l(laatta, 0) == 13);
    assert(y_l(laatta, 0) == 13);
    assert(has_area(laatta, AREA) == 1);
    assert(has_area(laatta, AREA - SIZE) == 0);
    return laatta;
}

void color_area(char *laatta, int index, int height, int width)
{
    char c = 'A' + pieces_l(laatta);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int co = index + y * SIZE + x;
            laatta[co] = c;
        }
    }
}

int split_horizontally(char *laatta, int index)
{
    int x = x_l(laatta, index);
    int y = y_l(laatta, index);
    
    for (int i = 1; i < y; ++i)
    {
        // check if splitting here would result in duplicate areas
        if (i == y - i || has_area(laatta, i * x) || has_area(laatta, (y - i) * x))
            continue;
        color_area(laatta, index, i, x);
        return 1;
    }
    return 0;
}

int split_vertically(char *laatta, int index)
{
    int x = x_l(laatta, index);
    int y = y_l(laatta, index);
    
    for (int i = 1; i < x; ++i)
    {
        // check if splitting here would result in duplicate areas
        if (i == x - i || has_area(laatta, i * y) || has_area(laatta, (x - i) * y))
            continue;
        color_area(laatta, index, y, i);
        return 1;
    }
    return 0;
}

int main(void)
{
    char *laatta = init_l();

    for (int i = 0; i < AREA; ++i)
    {
        split_horizontally(laatta, i);
        split_vertically(laatta, i);
    }

    print_l(laatta);


    free(laatta);
    return 0;
}