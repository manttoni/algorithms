#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SIZE 6
#define PRINT 0
#define PRINT_NOT 0
#define TARGET 23

#define AREA (SIZE * SIZE)
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

int asci[SIZE];
int desci[SIZE];

void pop_ascidesci()
{
    for (int i = 0; i < SIZE; ++i)
    {
        desci[i] = i * (SIZE + 1);
        asci[i] = (i + 1) * (SIZE - 1);
        if (PRINT)
            printf("%d %d\n", desci[i], asci[i]);
    }
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

int line_guarded(char copy[AREA], int index)
{
    int line = index / SIZE;
    for (int i = line * SIZE; i < (line + 1) * SIZE; ++i)
    {
        if (i == index)
            continue;

        if (copy[i] != 'V')
            return 0;
    }
    return 1;
}

int column_guarded(char copy[AREA], int index)
{
    int column = index % SIZE;
    for (int i = column; i < AREA; i += SIZE)
    {
        if (i == index)
            continue;

        if (copy[i] != 'V')
            return 0;
    }
    return 1;
}

int ascending_guarded(char copy[AREA], int index)
{
    if (index % (SIZE - 1) != 0 || index == 0)
        return 0;
    for (int i = 0; i < SIZE; ++i)
    {
        if (asci[i] == index)
            continue;

        if (copy[asci[i]] != 'V')
            return 0;
    }
    return 1;
}

int descending_guarded(char copy[AREA], int index)
{
    if (index % (SIZE + 1) != 0)
        return 0;
    for (int i = 0; i < SIZE; ++i)
    {
        if (desci[i] == index)
            continue;

        if (copy[desci[i]] != 'V')
            return 0;
    }
    return 1;
}

int guarded_point(char maa[AREA], int i)
{
    if (maa[i] == 'V')
        return 1;
    else if (line_guarded(maa, i))
        return 1;
    else if (column_guarded(maa, i))
        return 1;
    else if (ascending_guarded(maa, i))
        return 1;
    else if (descending_guarded(maa, i))
        return 1;
    return 0;
}

void print_maa(char maa[AREA])
{
    for(int i = 0; i < AREA; ++i)
    {
        if (i % SIZE == 0 && i != 0)
            printf("\n");
        if (maa[i] != 'V')
            printf("%s", RED);
        printf("%c", maa[i]);
        printf("%s", RESET);
    }
    printf("\n-------------\n");
}

int mark_guarded_points(char copy[AREA])
{
    for (int i = 0; i < AREA; ++i)
    {
        if (copy[i] == 'V')
            continue;
        if (guarded_point(copy, i) == 1)
        {
            copy[i] = 'V';
            return 1;
        }
    }
    return 0;
}

int guarded(char maa[AREA])
{
    char copy[AREA];
    memmove(copy, maa, AREA);
    while (mark_guarded_points(copy));
    for (int i = 0; i < AREA; ++i)
    {
        if (guarded_point(copy, i) == 0)
            return 0;
    }
    return 1;
}

int count_guards(char maa[AREA])
{
    int guards = 0;
    for (int i = 0; i < AREA; ++i)
        if (maa[i] == 'V')
            guards++;
    return guards;
}

int remove_extra(char maa[AREA], int last_removed)
{
    for (int i = last_removed + 1; i < AREA; ++i)
    {
        if (maa[i] == '.')
            continue;

        maa[i] = '.';
        if (guarded(maa))
        {
            if (PRINT)
                print_maa(maa);
            if (count_guards(maa) <= TARGET)
                return 1;
            if (remove_extra(maa, i) == 1)
                return 1;
        }
        else if (PRINT_NOT)
        {
            printf("Not guarded...\n");
            print_maa(maa);
        }
        maa[i] = 'V';
    }
    return 0;
}

int main(void)
{    
    pop_ascidesci();
    char maa[AREA];
    memset(maa, 'V', AREA);

    remove_extra(maa, -1);

    printf("\n");
    print_maa(maa);
    printf("Guards: %d\t", count_guards(maa));
    printf("Guarded: %d\n", guarded(maa));
    return 0;
}


// int add_guard(char maa[AREA], int i)
// {
//     // print_maa(maa);
//     for (int j = i; j < AREA; ++j)
//     {
//         if (i == 0)
//         {
//             printf("%f\r", (j * 100) / (double)AREA);
//             fflush(stdout);
//         }
//         if (guarded_point(maa, j) == 0)
//         {
//             maa[j] = 'V';
//             if (guarded(maa) == 1 && count_guards(maa) == 23)
//                 return 1;
//             if (add_guard(maa, j + 1) == 1)
//                 return 1;
//             maa[j] = '.';
//         }
//     }
//     return 0;
// }
