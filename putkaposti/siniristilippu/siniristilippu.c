#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef WIDTH
    #define WIDTH 18
#endif
#define HEIGHT ((WIDTH / 18) * 11)

int moves[4];

int x(int i)
{
    return (i % WIDTH);
}

int y(int i)
{
    return (i / WIDTH);
}

int get_color(int i)
{
    if ((   x(i) >= 5 * (WIDTH / 18) && x(i) < 8 * (WIDTH / 18)) 
        || (y(i) >= 4 * (HEIGHT / 11) && y(i) < 7 * (HEIGHT / 11)))
        return 0;
    return 1;
}

int mandis(int ida, int idb)
{
    int x_a = ida % WIDTH;
    int x_b = idb % WIDTH;
    int y_a = ida / WIDTH;
    int y_b = idb / WIDTH;
    return abs(x_a - x_b) + abs(y_a - y_b);
}

/* moves indices:
    0: maija blue
    1: maija white
    2: lauri blue
    3: lauri white
   color:
    0: blue
    1: white
*/
void calculate_result(int imaija, int ilauri)
{
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        int m_dist = mandis(i, imaija);
        int l_dist = mandis(i, ilauri);
        int color = get_color(i);
        if (m_dist == l_dist)
        {
            if (color == 0)
                moves[0]++;
            else
                moves[3]++;
        }
        else if (m_dist == 0 || m_dist < l_dist)
            moves[color]++;
        else if (l_dist == 0 || l_dist < m_dist)
            moves[2 + color]++;
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

    memset(moves, 0, 4 * sizeof(int));
    calculate_result(imaija, ilauri);
    printf("%d %d %d %d %d\n", WIDTH, moves[0], moves[1], moves[2], moves[3]);

    return 0;
}