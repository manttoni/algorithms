#ifndef UTILS_H
#define UTILS_H

typedef enum
{
    HOR,
    VER
}   direction;

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

#endif