#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef WIDTH
    #define WIDTH 18
#endif
#define HEIGHT ((WIDTH / 18) * 11)

int distances[WIDTH * HEIGHT];

int x(int i)
{
    return (i % WIDTH);
}

int y(int i)
{
    return (i / WIDTH);
}

char get_color(int i)
{
    if ((   x(i) >= 5 * (WIDTH / 18) && x(i) < 8 * (WIDTH / 18)) 
        || (y(i) >= 4 * (HEIGHT / 11) && y(i) < 7 * (HEIGHT / 11)))
        return 'B';
    return 'W';
}

int mandis(int ida, int idb)
{
    int x_a = ida % WIDTH;
    int x_b = idb % WIDTH;
    int y_a = ida / WIDTH;
    int y_b = idb / WIDTH;
    return abs(x_a - x_b) + abs(y_a - y_b);
}

// marks the shortest distance. if its lauri makes it negative
void populate_distances(int maija, int lauri)
{
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        int m_dist = mandis(i, maija);
        int l_dist = mandis(i, lauri);
        if (m_dist == l_dist)
        {
            char color = get_color(i);
            if (color == 'W')
                distances[i] = -l_dist;
            else
                distances[i] = m_dist;
        }
        else if (m_dist < l_dist)
            distances[i] = m_dist;
        else
            distances[i] = -l_dist;
    }
}

void count_spots(int *mb, int *mw, int *lb, int *lw)
{
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        int value = distances[i];
        if (value == 0)
            continue;
        char color = get_color(i);
        if (color == 'B')
        {
            if (value > 0)
                (*mb)++;
            if (value < 0)
                (*lb)++;
        }
        else
        {
            if (value > 0)
                (*mw)++;
            if (value < 0)
                (*lw)++;
        }
    }
}

// input starting points as args, compile with flag width
int main(int argc, char **argv)
{
    if (argc != 3)
        return 1;
    int x1 = atoi(argv[1]) - 1;
    int y1 = atoi(strchr(argv[1], ',') + 1) - 1;
    int x2 = atoi(argv[2]) - 1;
    int y2 = atoi(strchr(argv[2], ',') + 1) - 1;
    int imaija = x1 + y1 * WIDTH;
    int ilauri = x2 + y2 * WIDTH;
    int mb = 0, mw = 0, lb = 0, lw = 0;

    populate_distances(imaija, ilauri);

    if (get_color(imaija) == 'W')
        mw++;
    else
        mb++;
    
    if (get_color(ilauri) == 'W')
        lw++;
    else
        lb++;

    count_spots(&mb, &mw, &lb, &lw);
    printf("%d %d %d %d %d\n", WIDTH, mb, mw, lb, lw);

    return 0;
}