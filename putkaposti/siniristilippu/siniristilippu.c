#include "siniristilippu.h"

int maija = 0, lauri = 0;
int x1, y1, x2, y2;
char *flag;

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

void print_flag(char *flag)
{
    char printflag[PRINT_AREA + 1];
    memset(printflag, ' ', PRINT_AREA + 1);
    printflag[PRINT_AREA] = '\0';
    for (int i = 0; i < PRINT_AREA; ++i)
    {
        int draw_x = i % PRINT_WIDTH;
        int draw_y = i / PRINT_WIDTH;
        
        int source_x = draw_x * WIDTH / PRINT_WIDTH;
        int source_y = draw_y * HEIGHT / PRINT_HEIGHT;
        
        int source_index = source_y * WIDTH + source_x;
        printflag[i] = flag[source_index];
    }
    print(BOT | FLAG, "%s", printflag);
}

void start()
{
    while (strchr(flag, 'E') != NULL)
    {
        maijaput();
        lauriput();
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        print(BOTH, "Give 2 starting points in format x1,y1 x2,y2 where x,y are in range [1:]");
        return 1;
    }

    x1 = atoi(argv[1]);
    y1 = atoi(strchr(argv[1], ',') + 1);
    x2 = atoi(argv[2]);
    y2 = atoi(strchr(argv[2], ',') + 1);

    if (x1 < 1 || y1 < 1 || x2 < 1 || y2 < 1 || (x1 == x2 && y1 == y2))
    {
        print(BOTH, "Invalid input");
        return 1;
    }

    flag = my_malloc(AREA + 1);
    memset(flag, 'E', AREA);

    // for(int i = 0; i < AREA; ++i)
    // {
    //     if (rand() % 3 == 0)
    //         flag[i] = get_color(i);
    // }

    print(BOTH, "Flag created:");
    print(0, "- Width:\t%d", WIDTH);
    print(0, "- Height:\t%d", HEIGHT);
    print(BOT, "- Area:\t\t%d", AREA);
    
    // print_flag(flag);

    start(flag);

    free(flag);

    return 0;
}