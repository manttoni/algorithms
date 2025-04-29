#include "siniristilippu.h"

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

int read_input(int argc, char **argv, t_coord *maija, t_coord *lauri)
{
    if (argc != 3)
    {
        printf("Give 2 starting points in format x1,y1 x2,y2 where x,y are in range [1:]. (This exercises coordinates don't start from 0.)\n");
        return 1;
    }

    maija->x = atoi(argv[1]) - 1;
    maija->y = atoi(strchr(argv[1], ',') + 1) - 1;
    lauri->x = atoi(argv[2]) - 1;
    lauri->y = atoi(strchr(argv[2], ',') + 1) - 1;

    if (maija->x < 0 || maija->y < 0 ||
        lauri->x < 0 || lauri->y < 0 ||
        (maija->x == lauri->x && maija->y == lauri->y))
    {
        printf("Invalid input value(s).\n");
        return 2;
    }

    return 0;
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
    t_coord maija, lauri;
    int ret;

    if ((ret = read_input(argc, argv, &maija, &lauri)) != 0)
        return ret;

    char *flag = my_malloc(AREA + 1);
    memset(flag, 'E', AREA);

    int imaija = maija.x + maija.y * WIDTH;
    int ilauri = lauri.x + lauri.y * WIDTH;

    populate_distances(imaija, ilauri);

    int mb = 0, mw = 0, lb = 0, lw = 0;
    
    flag[imaija] = get_color(imaija);
    if (get_color(imaija) == 'W')
        mw++;
    else
        mb++;
    
    flag[ilauri] = get_color(ilauri);
    if (get_color(ilauri) == 'W')
        lw++;
    else
        lb++;

    count_spots(&mb, &mw, &lb, &lw);

    free(flag);

    printf("%d %d %d %d %d\n", WIDTH, mb, mw, lb, lw);

    return 0;
}