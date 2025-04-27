#define _GNU_SOURCE
#include <sys/prctl.h>
#include <sys/wait.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>

#define MAX_RUN 14 // how many processes can run simultaneously
#define PROC_MAX 99 // how many processes can be inputted inside file
#define PROC_LEN 16
#define TARGET 54

const char op[] = "OHJELMOINTIPUTKA";
const int moves[] = {1, -1, 4, -4}; // right, left, down, up
const char dirs[] = "OVAY";

char result[TARGET + 1];
int max_depth;
int best_depth; // if multiple 0 found, this is where it is least deep

int ties;
int solved;

// K O T I | O H J E
// P I H O | L M O I
// J E N M | N T I P
// U L T A | U T K A

typedef struct s_branch
{
    int value; // value of the end leaf
    int move; // move from the current saved position
    int lowest; // value of lowest value along branch
    int highest;
    int sum; // sum of all values along branch
    int len; // length of branch
    int average; // average value of branch
}   t_branch;

void print_result()
{
    printf("**********************************************\n");
    printf("%ld moves, depth of %d\n%s\n", strlen(result), max_depth, result);
    printf("**********************************************\n");
}

int dist(const char *sq, const char *a, const char *b)
{
    int ida = a - sq;
    int idb = b - sq;
    int x_a = ida % 4;
    int x_b = idb % 4;
    int y_a = ida / 4;
    int y_b = idb / 4;

    return abs(x_a - x_b) + abs(y_a - y_b);
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

// returns the optimal distance to the correct place
int shortest_dist(char *sq, int i)
{
    char c = sq[i];

    // check if sq[i] is a unique char
    if (strchr("KPHJENMULA", c))
        return dist(sq, sq + i, sq + (strchr(op, c) - op));

    // from now on there is 2 of everything

    // get pointers to current position and the other one
    char *pos1 = sq + i;
    char *pos2 = strchr(pos1 + 1, c);
    if (pos2 == NULL)
        pos2 = strchr(sq, c);

    // pointers in sq range pointing to the correct indices
    char *corpos1 = sq + (strchr(op, c) - op);
    char *corpos2 = sq + (strrchr(op, c) - op);

    // figure out where each of them goes
    int pos1corpos1 = dist(sq, pos1, corpos1);
    int pos1corpos2 = dist(sq, pos1, corpos2);
    int dist1 = pos1corpos1 + dist(sq, pos2, corpos2);
    int dist2 = pos1corpos2 + dist(sq, pos2, corpos1);
    if (dist1 <= dist2)
        return pos1corpos1;
    return pos1corpos2;
}

int is_correct(char *sq, int i)
{
    return sq[i] == op[i];
}

int check(char *sq, int multi, const char *indices)
{
    const char hd[] = "0123456789ABCDEF";
    while (*indices != '\0')
    {
        int i = strchr(hd, *indices) - hd;
        if (!is_correct(sq, i))
            return multi;
        indices++;
    }
    return 1;
}

int value(char *sq)
{
    int multi = 0;
    int v = 0;
    for (int i = 0; i < 16; ++i)
    {
        if (sq[i] == 'A')
            continue;
        int s;
        s = shortest_dist(sq, i);
        v += s * s;
        // if (s > 0)
        //     v += 1;
    }

    multi = check(sq, 2, "014") * min(check(sq, 2, "8CD"), check(sq, 2, "237"));

    // // 0 1 2 3
    // // 4 5 6 7
    // // 8 9 A B
    // // C D E F

    return v * multi;
}

void ps(char *sq, double secs, int best_value, t_branch *best)
{
    printf("                                         \r");
    for(int i = 0; i < 16; ++i)
    {
        if (i % 4 == 0)
        {
            printf("| ");
            switch(i / 4)
            {
                case 0:
                    printf("%d",      ties < 1000 ? ties : ties < 1000000 ? ties / 1000 : ties < 1000000000 ? ties / 1000000 : ties / 1000000000);
                    printf("%s ties", ties < 1000 ? ""   : ties < 1000000 ? "K"         : ties < 1000000000 ? "M"            : "B");
                    break;
                case 1:
                    printf("%d",      solved < 1000 ? solved : solved < 1000000 ? solved / 1000 : solved < 1000000000 ? solved / 1000000 : solved / 1000000000);
                    printf("%s solved", solved < 1000 ? ""   : solved < 1000000 ? "K"         : solved < 1000000000 ? "M"            : "B");
                    break;
                case 2:
                    printf("%d lowest", best_value);
                    break;
                case 3:
                    printf("%c move", dirs[best->move]);
                    break;
                default:
                    printf("\t");
                    break;
            }
            printf("\t");
        }

        // color
        if (sq[i] == 'A')
            printf("\033[33m");
        else if (is_correct(sq, i))
            printf("\033[32m");
        else
            printf("\033[31m");
        
        // char
        printf("%c", sq[i]);
        
        // reset color
        printf("\033[0m");

        if (i % 4 == 3)
        {
            printf("\t");
            switch(i / 4)
            {
                case 0:
                    printf("%d depth ", max_depth);
                    break;
                case 1:
                    printf("%d value ", value(sq));
                    break;
                case 2:
                    printf("%.2f ", secs < 60 ? secs : secs < 60 * 60 ? secs / 60 : secs / (60 * 60));
                    printf("%s", secs < 60 ? "seconds" : secs < 60 * 60 ? "minutes" : "hours");
                    break;
                case 3:
                    printf("%ld len     ", strlen(result));
                    break;
            }
            printf("\t|\n");
        }
    }
    printf("╞=======================================╡\n");
}

void swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

int valid(int last_move, int i, char *copy, char *a)
{
    if (last_move != -1 && moves[last_move] == -moves[i]) // dont move back to where you came from
        return 0;

    if (dist(copy, a, a + moves[i]) != 1) // check if a move out of left/right side of box would happen aka mystical math teleport
        return 0;

    int ida = a - copy;
    int ida2 = ida + moves[i];
    if (ida2 > 15 || ida2 < 0) // check if a move out of bounds would happen
        return 0;

    return 1;
}

// 0 if best wins, 1 if branch wins
int tiebreaker(t_branch *best, t_branch *branch)
{
    // check if it is a tie
    if (best->value != branch->value)
        return 0;

    ties++;
    solved++;

    // 1. shortest sequence to end. should only happen when it finds 0 early
    if (best->len < branch->len)
        return 0;
    else if (best->len > branch->len)
        return 1;

    if (best->lowest < branch->lowest)
        return 0;
    else if (best->lowest > branch->lowest)
        return 1;

    if (best->average < branch->average)
        return 0;
    else if (best->average > branch->average)
        return 1;

    if (best->sum < branch->sum)
        return 0;
    else if (best->sum > branch->sum)
        return 1;

    if (best->highest < branch->highest)
        return 0;
    else if (best->highest > branch->highest)
        return 1;

    solved--;

    return rand() % 2 == 0;
}

t_branch *init_branch(int val, int move)
{
    t_branch *branch = malloc(sizeof(t_branch));
    if (branch == NULL)
    {
        printf("Malloc failed\n");
        exit(255);
    }
    branch->value = val;
    branch->move = move;
    branch->lowest = val;
    branch->highest = val;
    branch->sum = val;
    branch->len = 1;
    branch->average = val;
    return branch;
}

// typedef struct s_branch
// {
//     int value; // value of the end leaf
//     int move; // move that led to this position
//     int lowest; // value of lowest value along branch
//     int sum; // sum of all values along branch
//     int len; // length of branch
//     int average; // average value of branch
// }   t_branch;

t_branch *best_move(int last_move, int depth, char *sq)
{
    t_branch *best = NULL;

    for (int i = 0; i < 4; ++i)
    {
        char copy[17];
        memmove(copy, sq, 17);
        char *a = strchr(copy, 'A');

        if (!valid(last_move, i, copy, a))
            continue;

        swap(a, a + moves[i]);
        int val = value(copy);

        t_branch *branch;

        if (depth == 1)
            branch = init_branch(val, i); // leaf reached
        else
        {
            branch = best_move(i, depth - 1, copy); // go deeper
            branch->len++;
            branch->sum += val;
            branch->average = branch->sum / branch->len;
            branch->lowest = min(val, branch->lowest);
            branch->highest = max(val, branch->highest);
            branch->move = i;
        }

        if (best == NULL || branch->value < best->value || tiebreaker(best, branch))
        {   
            free(best);
            best = branch;
        }
        else
            free(branch);

        swap(a, a + moves[i]);
    }

    return best;
}

char *anagrammi(char *sq)
{
    clock_t start_time = clock();

    memmove(sq, "KOTIPIHOJENMULTA\0", 17);
    memset(result, 0, TARGET + 1);

    int last_move = -1;
    best_depth = INT_MAX - 1;
    int len = 0;
    t_branch *best = NULL;
    int best_value = INT_MAX;
    while (strcmp(sq, op) != 0 && len < TARGET)
    {
        free(best);
        best = best_move(last_move, min(max_depth, TARGET - len), sq);
        best_value = min(best->value, best_value);
        char *a = strchr(sq, 'A');
        swap(a, a + moves[best->move]);
        last_move = best->move;
        
        strncat(result, &dirs[best->move], 1);
        len++;

        const char *asd = "\\/-";
        int i = len % strlen(asd);
        printf("|   %c\r", asd[i]);
        fflush(stdout);
        ps(sq, (double)(clock() - start_time) / CLOCKS_PER_SEC, best_value, best);
    }

    clock_t end_time = clock();
    double elatime = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    ps(sq, elatime, best_value, best);
    free(best);
}

int start_process(int depth)
{
    char buf[12];
    snprintf(buf, sizeof(buf), "a.out %d", depth);
    prctl(PR_SET_NAME, buf, 0, 0, 0);

    ties = 0;
    solved = 0;
    srand(time(NULL) * getpid());
    max_depth = depth;
    char sq[] ="KOTIPIHOJENMULTA";

    anagrammi(sq);

    if (strcmp(sq, op) == 0 && strlen(result) <= TARGET)
    {
        printf("\n");
        print_result();
        exit(0);
    }

    exit(min(255, value(sq)));
}

int read_procs(char processes[PROC_MAX][PROC_LEN], char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        printf("File read error\n");
        exit(3);
    }

    char buf[PROC_MAX * PROC_LEN + 1];
    memset(buf, 0, sizeof(buf));
    ssize_t bytes = read(fd, buf, PROC_MAX * PROC_LEN);
    close(fd);
    if (bytes < 0)
    {
        printf("File read error\n");
        exit(3);
    }

    char *ptr = buf;
    int i = 0;
    while (*ptr != '\0' && strchr(ptr, '\n') != NULL && i < PROC_MAX && strlen(buf) > 1)
    {
        char *next_nl = strchr(ptr, '\n');
        if (next_nl - ptr > 0)
        {
            memmove(*processes, ptr, next_nl - ptr);
            processes++;
            i++;
        }
        ptr = next_nl + 1;
    }
    return i;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Give a file\n");
        return 1;
    }

    char processes[PROC_MAX][PROC_LEN];
    memset(processes, 0, sizeof(processes));
    int procs = read_procs(processes, argv[1]);

    for (int i = 0; i < procs; ++i)
        printf("Process read: %s\n", processes[i]);
    
    printf("╞=======================================╡\n");
    printf("| Objective:                            |\n");
    printf("|  - Find \"OHJELMOINTIPUTKA\"            |\n");
    printf("|    within %d moves                 \t|\n", TARGET);
    printf("|    using %d processes                \t|\n", procs);
    printf("╞=======================================╡\n");

    int i = 0;
    int running = 0, finished = 0, remaining = procs;
    int avg = -1, mini = INT_MAX, maxi = -1, sum = 0;
    while (remaining > 0)
    {
        while (running < MAX_RUN && i < procs)
        {
            // printf("running: %d finished: %d remaining: %d i: %d\n", running, finished, remaining, i);
            // parse process info
            int depth = atoi(processes[i]);
            // printf("depth %d\n", depth);
            i++;
        
            // spawn a process
            int pid = fork();
            if (pid < 0)
            {
                printf("Fork error\n");
                exit(2);
            }

            // and put it to work
            if (pid == 0)
                start_process(depth);

            usleep(100000);

            running++;
        }

        if (running == 0)
            break;
        
        int status;
        wait(&status);

        finished++;
        remaining--;
        running--;

        if (WIFEXITED(status))
        {
            int exitstatus = WEXITSTATUS(status);

            if (exitstatus == 255)  // resulting value was too high
                continue;           // probably because of low depth or crazy value punishments

            sum += exitstatus;
            avg = sum / finished;

            mini = min(mini, exitstatus);
            maxi = max(maxi, exitstatus);

            printf("|\tMin:%d\tMax:%d\tAvg:%d\t\t|\r", mini, maxi, avg);
            fflush(stdout);

            if (exitstatus == 0)
            {
                printf("                                           \r");
                printf("|\tMin:%d\tMax:%d\tAvg:%d\t\t|\r", mini, maxi, avg);
                printf("\n╞=======================================╡\n");
                printf("Got result!\n");
                kill(0, SIGTERM);
                return 42;
            }
        }
        else if (WIFSIGNALED(status))
            printf("Child was terminated by signal %d\n", WTERMSIG(status));
        else
            printf("not WIFEXITED for some other reason\n");
    }
    printf("|\tMin:%d\tMax:%d\tAvg:%d\t\t|\r", mini, maxi, avg);
    printf("\n╞=======================================╡\n");
    return 0;
}
