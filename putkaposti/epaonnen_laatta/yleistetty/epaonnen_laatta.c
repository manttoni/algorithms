#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

int SIZE;
int AREA;
const char ASCII_START = '!';

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
    char biggest = ASCII_START;
    for (int i = 0; i < AREA; ++i)
        biggest = max(laatta[i], biggest);
    return biggest - ASCII_START + 1;
}

void print_sizes(char *laatta)
{
    for (int i = 0; i < pieces_l(laatta); ++i)
    {
        int index = strchr(laatta, ASCII_START + i) - laatta;
        printf("%d ", area_l(laatta, index));
    }
    printf("\n");
}

void print_l(char *laatta)
{
    // print_sizes(laatta);
    for(int i = 0; i < AREA; ++i)
    {
        if (i != 0 && i % SIZE == 0)
            printf("\n");
        printf("%c", laatta[i]);
    }
    // printf("\nPieces: %d\n", pieces_l(laatta));
    printf("\n\n");
}

int has_area(char *laatta, int area)
{
    for (int i = 0; i < pieces_l(laatta); ++i)
    {
        if (area_l(laatta, strchr(laatta, ASCII_START + i) - laatta) == area)
            return 1;
    }
    return 0;
}

char *init_l()
{
    // printf("Size: %d\nArea: %d\n", SIZE, AREA);
    // printf("Size: %d\n", SIZE);
    char *laatta = malloc(AREA + 1);
    memset(laatta, ASCII_START, AREA);
    laatta[AREA] = '\0';
    assert(area_l(laatta, 0) == AREA);
    assert(pieces_l(laatta) == 1);
    assert(x_l(laatta, 0) == SIZE);
    assert(y_l(laatta, 0) == SIZE);
    assert(has_area(laatta, AREA) == 1);
    assert(has_area(laatta, AREA - SIZE) == 0);
    return laatta;
}

void color_area(char *laatta, int index, int height, int width)
{
    print_l(laatta);
    index = strchr(laatta, laatta[index]) - laatta;
    char c = ASCII_START + pieces_l(laatta);
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
    index = strchr(laatta, laatta[index]) - laatta;
    int x = x_l(laatta, index);
    int y_max = y_l(laatta, index);
    
    for (int y = 1; y < y_max; ++y)
    {
        // check if splitting here would result in duplicate areas
        if (y == y_max - y || has_area(laatta, y * x) || has_area(laatta, (y_max - y) * x))
            continue;
        color_area(laatta, index, y, x);
        return 1;
    }
    return 0;
}

int split_vertically(char *laatta, int index)
{
    index = strchr(laatta, laatta[index]) - laatta;
    int x_max = x_l(laatta, index);
    int y = y_l(laatta, index);
    
    for (int x = 1; x < x_max; ++x)
    {
        // check if splitting here would result in duplicate areas
        if (x == x_max - x || has_area(laatta, y * x) || has_area(laatta, y * (x_max - x)))
            continue;
        color_area(laatta, index, y, x);
        return 1;
    }
    return 0;
}

int get_piece(char *laatta, char c)
{
    return strchr(laatta, c) - laatta;
}

int find_largest(char *laatta, int limit)
{
    int larg_indx = 0;
    int larg_area = 0;
    for (int i = 0; i < pieces_l(laatta); ++i)
    {
        int index = get_piece(laatta, ASCII_START + i);
        int area = area_l(laatta, index);
        if (area > larg_area && area < limit)
        {
            larg_area = area;
            larg_indx = index;
        }
    }
    return larg_indx;
}

int find_smallest(char *laatta, int limit)
{
    int smal_indx = 0;
    int smal_area = INT_MAX;
    for (int i = 0; i < pieces_l(laatta); ++i)
    {
        int index = get_piece(laatta, ASCII_START + i);
        int area = area_l(laatta, index);
        if (area < smal_area && area > limit)
        {
            smal_area = area;
            smal_indx = index;
        }
    }
    return smal_indx;
}

int better_result(char *laatta, int prev_result)
{
    for (int limit = AREA + 1; limit > 0; limit--)
    {
        int piece_i = find_largest(laatta, limit);
        int width = x_l(laatta, piece_i);
        int height = y_l(laatta, piece_i);
        if (width > height)
        {
            split_vertically(laatta, piece_i);
        }
        else
        {
            split_horizontally(laatta, piece_i);
        }
    }
    return prev_result < pieces_l(laatta);
}

int main(void)
{
    int result = 0;
    for (int n = 50; n <= 50; ++n)
    {
        SIZE = n;
        AREA = n * n;
        char *laatta = init_l();

        while (better_result(laatta, pieces_l(laatta)));
        
        print_l(laatta);

        result += pieces_l(laatta);

        free(laatta);
    }   
    printf("%d\n", result);
    return 0;
}