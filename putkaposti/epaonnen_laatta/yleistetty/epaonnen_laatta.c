#include "utils.h"
#include "pupu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <pthread.h>

const char ASCII_START = '!';

int x_l(t_data *data, int i)
{
    char *laatta = data->laatta;
    char c = laatta[i];
    int uple = strchr(laatta, c) - laatta;
    int lori = strrchr(laatta, c) - laatta;
    int uplex = uple % data->size;
    int lorix = lori % data->size;
    return lorix - uplex + 1;
}

int y_l(t_data *data, int i)
{
    char *laatta = data->laatta;
    char c = laatta[i];
    int uple = strchr(laatta, c) - laatta;
    int lori = strrchr(laatta, c) - laatta;
    int upley = uple / data->size;
    int loriy = lori / data->size;
    return loriy - upley + 1;
}

int area_l(t_data *data, int i)
{
    char *laatta = data->laatta;
    char c = laatta[i];
    int uple = strchr(laatta, c) - laatta;
    int lori = strrchr(laatta, c) - laatta;
    int uplex = uple % data->size;
    int lorix = lori % data->size;
    int upley = uple / data->size;
    int loriy = lori / data->size;
    return (lorix - uplex + 1) * (loriy - upley + 1);
}

int pieces_l(t_data *data)
{
    char biggest = ASCII_START;
    for (int i = 0; i < data->area; ++i)
        biggest = max(data->laatta[i], biggest);
    return biggest - ASCII_START + 1;
}

void print_l(t_data *data)
{
    // print_sizes(laatta);
    for(int i = 0; i < data->area; ++i)
    {
        if (i != 0 && i % data->size == 0)
            printf("\n");
        print_fg_color(data->laatta[i] * 2);
        // print_bg_color(data->laatta[i] * 2);
        printf("%c", data->laatta[i]);
        printf("%s", RESET);
    }
    // printf("\nPieces: %d\n", pieces_l(laatta));
    printf("\n\n");
}

int has_area(t_data *data, int area)
{
    for (int i = 0; i < pieces_l(data); ++i)
    {
        if (area_l(data, strchr(data->laatta, ASCII_START + i) - data->laatta) == area)
            return 1;
    }
    return 0;
}

void color_area(t_data *data, int index, int height, int width)
{
    char *laatta = data->laatta;
    index = strchr(laatta, laatta[index]) - laatta;
    char c = ASCII_START + pieces_l(data);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int co = index + y * data->size + x;
            laatta[co] = c;
        }
    }
}

int split_horizontally(t_data *data, int index)
{
    char *laatta = data->laatta;
    index = strchr(laatta, laatta[index]) - laatta;
    int x = x_l(data, index);
    int y_max = y_l(data, index);

    for (int y = 1; y < y_max; ++y)
    {
        // check if splitting here would result in duplicate areas
        if (y == y_max - y || has_area(data, y * x) || has_area(data, (y_max - y) * x))
            continue;
        color_area(data, index, y, x);
        return 1;
    }
    return 0;
}

int split_vertically(t_data *data, int index)
{
    char *laatta = data->laatta;
    index = strchr(laatta, laatta[index]) - laatta;
    int x_max = x_l(data, index);
    int y = y_l(data, index);

    for (int x = 1; x < x_max; ++x)
    {
        // check if splitting here would result in duplicate areas
        if (x == x_max - x || has_area(data, y * x) || has_area(data, y * (x_max - x)))
            continue;
        color_area(data, index, y, x);
        return 1;
    }
    return 0;
}

int get_piece(char *laatta, char c)
{
    return strchr(laatta, c) - laatta;
}

int find_largest(t_data *data, int limit)
{
    int larg_indx = 0;
    int larg_area = 0;
    for (int i = 0; i < pieces_l(data); ++i)
    {
        int index = get_piece(data->laatta, ASCII_START + i);
        int area = area_l(data, index);
        if (area > larg_area && area < limit)
        {
            larg_area = area;
            larg_indx = index;
        }
    }
    return larg_indx;
}

int better_result(t_data *data, int prev_result)
{
    // for (int i = 0; i < data->area; ++i)
    // {
    //     while (i % 2 == 0 && split_vertically(data, i));
    //     while (i % 2 == 1 && split_horizontally(data, i));
    // }
    for (int limit = data->area + 1; limit > 0; limit--)
    {
        int piece_i = find_largest(data, limit);
        int width = x_l(data, piece_i);
        int height = y_l(data, piece_i);
        if (width > height)
        {
            while (split_vertically(data, piece_i));
        }
        else
        {
            while (split_horizontally(data, piece_i));
        }
    }
    return prev_result < pieces_l(data);
}

void *split_laatta(void *arg)
{
    t_data *data = (t_data *) arg;
    while (better_result(data, pieces_l(data)));
    data->pieces = pieces_l(data);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        print(BOTH, "%s <min_size> <max_size>\n", argv[0]);
        return FAILURE;
    }
    int min_size = atoi(argv[1]);
    int max_size = atoi(argv[2]);
    int sizes = max_size - min_size + 1;
    pthread_t threads[sizes];
    t_data data[sizes];

    // start threads
    for (int i = 0; i < sizes; ++i)
    {
        data[i].pieces = 1;
        data[i].size = i + min_size;
        data[i].area = data[i].size * data[i].size;
        data[i].laatta = my_malloc(data[i].area + 1);
        memset(data[i].laatta, ASCII_START, data[i].area);
        if (pthread_create(threads + i, NULL, split_laatta, data + i) != 0)
        {
            print(BOTH, "Error: Failed to create thread!");
            return FAILURE;
        }
    }

    // wait for them to finish
    for (int i = 0; i < sizes; ++i)
        pthread_join(threads[i], NULL);

    // read results
    int result = 0;
    for (int i = 0; i < sizes; ++i)
    {
        print_l(&data[i]);
        free(data[i].laatta);
        result += data[i].pieces;
    }

    print(BOTH, "Result: %d", result);

    return 0;
}