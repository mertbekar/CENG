#include "do_not_submit.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

typedef struct ant {
    int x;
    int y;
    char type;
} ant;

typedef struct coordinate {
    int x;
    int y;
} coordinate;

sem_t antmutexs[GRIDSIZE][GRIDSIZE];
ant* ants;
int count_of_sleeper=0;
int ant_count;
sem_t *semo;
int finisher = 1;
sem_t finish;
int sleeper = 0;

int c = 0;
sem_t lock;
pthread_t tid[GRIDSIZE*GRIDSIZE];
sem_t *ant_lock;

sem_t barrier;
sem_t s_sem;

coordinate command(int x, int y, int rnd)
{
    coordinate move;
    if(rnd == 0)
    {
        move.x = x-1;
        move.y = y+1;
        return move;
    }
    else if(rnd == 1)
    {
        move.x = x-1;
        move.y = y;
        return move;
    }
    else if(rnd == 2)
    {
        move.x = x-1;
        move.y = y-1;
        return move;
    }
    else if(rnd == 3)
    {
        move.x = x;
        move.y = y+1;
        return move;
    }
    else if(rnd == 4)
    {
        move.x = x;
        move.y = y-1;
        return move;
    }
    else if(rnd == 5)
    {
        move.x = x+1;
        move.y = y+1;
        return move;
    }
    else if(rnd == 6)
    {
        move.x = x+1;
        move.y = y;
        return move;
    }
    else if(rnd == 7)
    {
        move.x = x+1;
        move.y = y-1;
        return move;
    }
}

bool border2(int x, int y)
{
    if(x >= 0 && x < GRIDSIZE && y >= 0 && y < GRIDSIZE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int* spaces(int x, int y, int index)
{
    int *space = malloc(sizeof(int)*8);
    bool b0, b1, b2, b3, b4, b5, b6, b7;
    b0 = border2(x-1, y+1);
    b1 = border2(x-1, y);
    b2 = border2(x-1, y-1);
    b3 = border2(x, y+1);
    b4 = border2(x, y-1);
    b5 = border2(x+1, y+1);
    b6 = border2(x+1, y);
    b7 = border2(x+1, y-1);
    int u = 0;
    for(int i = 0; i<8; i++)
    {
        space[i] = -1;
    }
    if(b0)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x-1][y+1]);
        if(lookCharAt(x-1, y+1) == '-')
        {
            space[u] = 0;
            u++;
        }
        sem_post(&antmutexs[x-1][y+1]);
        sem_post(&ant_lock[index]);
    }
    if(b1)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x-1][y]);
        if(lookCharAt(x-1, y) == '-')
        {
            space[u] = 1;
            u++;
        }
        sem_post(&antmutexs[x-1][y]);
        sem_post(&ant_lock[index]);
    }
    if(b2)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x-1][y-1]);
        if(lookCharAt(x-1, y-1) == '-')
        {
            space[u] = 2;
            u++;
        }
        sem_post(&antmutexs[x-1][y-1]);
        sem_post(&ant_lock[index]);
    }
    if(b3)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x][y+1]);
        if(lookCharAt(x, y+1) == '-')
        {
            space[u] = 3;
            u++;
        }
        sem_post(&antmutexs[x][y+1]);
        sem_post(&ant_lock[index]);
    }
    if(b4)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x][y-1]);
        if(lookCharAt(x, y-1) == '-')
        {
            space[u] = 4;
            u++;
        }
        sem_post(&antmutexs[x][y-1]);
        sem_post(&ant_lock[index]);
    }
    if(b5)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x+1][y+1]);
        if(lookCharAt(x+1, y+1) == '-')
        {
            space[u] = 5;
            u++;
        }
        sem_post(&antmutexs[x+1][y+1]);
        sem_post(&ant_lock[index]);
    }
    if(b6)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x+1][y]);
        if(lookCharAt(x+1, y) == '-')
        {
            space[u] = 6;
            u++;
        }
        sem_post(&antmutexs[x+1][y]);
        sem_post(&ant_lock[index]);
    }
    if(b7)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x+1][y-1]);
        if(lookCharAt(x+1, y-1) == '-')
        {
            space[u] = 7;
            u++;
        }
        sem_post(&antmutexs[x+1][y-1]);
        sem_post(&ant_lock[index]);
    }
    return space;
}

int* foods(int x, int y, int index)
{
    int *space = malloc(sizeof(int)*8);
    bool b0, b1, b2, b3, b4, b5, b6, b7;
    b0 = border2(x-1, y+1);
    b1 = border2(x-1, y);
    b2 = border2(x-1, y-1);
    b3 = border2(x, y+1);
    b4 = border2(x, y-1);
    b5 = border2(x+1, y+1);
    b6 = border2(x+1, y);
    b7 = border2(x+1, y-1);
    int u = 0;
    for(int i = 0; i<8; i++)
    {
        space[i] = -1;
    }
    if(b0)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x-1][y+1]);
        if(lookCharAt(x-1, y+1) == 'o')
        {
            space[u] = 0;
            u++;
        }
        sem_post(&antmutexs[x-1][y+1]);
        sem_post(&ant_lock[index]);
    }
    if(b1)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x-1][y]);
        if(lookCharAt(x-1, y) == 'o')
        {
            space[u] = 1;
            u++;
        }
        sem_post(&antmutexs[x-1][y]);
        sem_post(&ant_lock[index]);
    }
    if(b2)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x-1][y-1]);
        if(lookCharAt(x-1, y-1) == 'o')
        {
            space[u] = 2;
            u++;
        }
        sem_post(&antmutexs[x-1][y-1]);
        sem_post(&ant_lock[index]);
    }
    if(b3)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x][y+1]);
        if(lookCharAt(x, y+1) == 'o')
        {
            space[u] = 3;
            u++;
        }
        sem_post(&antmutexs[x][y+1]);
        sem_post(&ant_lock[index]);
    }
    if(b4)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x][y-1]);
        if(lookCharAt(x, y-1) == 'o')
        {
            space[u] = 4;
            u++;
        }
        sem_post(&antmutexs[x][y-1]);
        sem_post(&ant_lock[index]);
    }
    if(b5)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x+1][y+1]);
        if(lookCharAt(x+1, y+1) == 'o')
        {
            space[u] = 5;
            u++;
        }
        sem_post(&antmutexs[x+1][y+1]);
        sem_post(&ant_lock[index]);
    }
    if(b6)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x+1][y]);
        if(lookCharAt(x+1, y) == 'o')
        {
            space[u] = 6;
            u++;
        }
        sem_post(&antmutexs[x+1][y]);
        sem_post(&ant_lock[index]);
    }
    if(b7)
    {
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x+1][y-1]);
        if(lookCharAt(x+1, y-1) == 'o')
        {
            space[u] = 7;
            u++;
        }
        sem_post(&antmutexs[x+1][y-1]);
        sem_post(&ant_lock[index]);
    }
    return space;
}

void* counter(int *ind)
{
    sem_wait(&barrier);
    sem_post(&barrier);
    int index = *ind;
    int rnd;
    int x, y;
    char type;
    int flag = 0;
    int finisher2 = 1;
    while(1)
    {
        sem_wait(&finish);
        if(finisher == 0)
        {
            finisher2 = 0;
        }
        sem_post(&finish);
        if(finisher2 == 0)
        {
            break;
        }
        int* spaces2;
        int *fastfood;
        int fl = 1;
        flag = 0;
        x = ants[index].x;
        y = ants[index].y;
        sem_wait(&s_sem);
        if(index < count_of_sleeper)
        {
            sem_post(&s_sem);
            sem_wait(&ant_lock[index]);
            sem_wait(&antmutexs[x][y]);
            if(lookCharAt(x, y) == '1')
            {
                putCharTo(x, y, 'S');
            }
            if(lookCharAt(x, y) == 'P')
            {
                putCharTo(x, y, '$');
            }
            sem_post(&ant_lock[index]);
            sem_post(&antmutexs[x][y]);
        }
        else
        {
            sem_post(&s_sem);
        }
        sem_wait(&semo[index]);
        sem_post(&semo[index]);
        sem_wait(&s_sem);
        if(index >= count_of_sleeper)
        {
            sem_post(&s_sem);
            sem_wait(&ant_lock[index]);
            sem_wait(&antmutexs[x][y]);
            if(lookCharAt(x, y) == 'S')
            {
                putCharTo(x, y, '1');
            }
            if(lookCharAt(x, y) == '$')
            {
                putCharTo(x, y, 'P');
            }
            sem_post(&antmutexs[x][y]);
            sem_post(&ant_lock[index]);
        }
        else
        {
            sem_post(&s_sem);
        }
        usleep(getDelay() * 1000 + (rand() % 5000));
        type = ants[index].type;
        bool bor7 = border2(x+1, y-1);
        bool bor6 = border2(x+1, y);
        bool bor5 = border2(x+1, y+1);
        bool bor4 = border2(x, y-1);
        bool bor3 = border2(x, y+1);
        bool bor2 = border2(x-1, y-1);
        bool bor1 = border2(x-1, y);
        bool bor0 = border2(x-1, y+1);
        sem_wait(&ant_lock[index]);
        sem_wait(&antmutexs[x][y]);
        //printf("dfdsdf");
        if(lookCharAt(x, y) == '1')
        {
            sem_post(&antmutexs[x][y]);
            sem_post(&ant_lock[index]);
            if(type == 'a')
            {
                while(1)
                {
                    fastfood = foods(x, y, index);
                    int cc = 0;
                    int rd;
                    coordinate move;
                    for(int k = 0; k<8; k++)
                    {
                        if(fastfood[k] == -1)
                        {
                            cc = k-1;
                            break;
                        }
                        cc = k;
                    }
                    if(cc != -1)
                    {
                        rd = (rand() % (cc+1));
                        move = command(x, y, fastfood[rd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == 'o')
                        {
                            putCharTo(move.x, move.y, 'P');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, '-');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                    }
                    else
                    {
                        flag = 1;
                        break;
                    }
                    free(fastfood);
                }
                free(fastfood);
                if(flag == 1)
                {
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, '-');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
            }
            else if(type == 't')
            {
                while(1)
                {
                    spaces2 = spaces(x, y, index);
                    int count = 0;
                    int rnd;
                    coordinate move;
                    for(int k = 0; k<8; k++)
                    {
                        if(spaces2[k] == -1)
                        {
                            count = k-1;
                            break;
                        }
                        count = k;
                    }
                    if(count == -1)
                    {
                        break;
                    }
                    rnd = (rand() % (count+1));
                    move = command(x, y, spaces2[rnd]);
                    sem_wait(&ant_lock[index]);
                    sem_wait(&antmutexs[x][y]);
                    sem_wait(&antmutexs[move.x][move.y]);
                    if(lookCharAt(move.x, move.y) == '-')
                    {
                        putCharTo(move.x, move.y, '1');
                        sem_post(&antmutexs[move.x][move.y]);
                        putCharTo(x, y, '-');
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        ants[index].type = 'a';
                        ants[index].x = move.x;
                        ants[index].y = move.y;
                        break;
                    }
                    sem_post(&antmutexs[move.x][move.y]);
                    sem_post(&antmutexs[x][y]);
                    sem_post(&ant_lock[index]);
                    free(spaces2);
                }
                free(spaces2);
            }
        }
        else if(lookCharAt(x, y) == 'P')
        {
            sem_post(&antmutexs[x][y]);
            sem_post(&ant_lock[index]);
            if(bor0)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x-1][y+1]);
                if(lookCharAt(x-1, y+1) == 'o')
                {
                    sem_post(&antmutexs[x-1][y+1]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x-1][y+1]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor1 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x-1][y]);
                if(lookCharAt(x-1, y) == 'o')
                {
                    sem_post(&antmutexs[x-1][y]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x-1][y]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor2 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x-1][y-1]);
                if(lookCharAt(x-1, y-1) == 'o')
                {
                    sem_post(&antmutexs[x-1][y-1]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x-1][y-1]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor3 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x][y+1]);
                if(lookCharAt(x, y+1) == 'o')
                {
                    sem_post(&antmutexs[x][y+1]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x][y+1]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor4 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x][y-1]);
                if(lookCharAt(x, y-1) == 'o')
                {
                    sem_post(&antmutexs[x][y-1]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x][y-1]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor5 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x+1][y+1]);
                if(lookCharAt(x+1, y+1) == 'o')
                {
                    sem_post(&antmutexs[x+1][y+1]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x+1][y+1]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor6 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x+1][y]);
                if(lookCharAt(x+1, y) == 'o')
                {
                    sem_post(&antmutexs[x+1][y]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x+1][y]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(bor7 && fl)
            {
                sem_wait(&ant_lock[index]);
                sem_wait(&antmutexs[x+1][y-1]);
                if(lookCharAt(x+1, y-1) == 'o')
                {
                    sem_post(&antmutexs[x+1][y-1]);
                    sem_post(&ant_lock[index]);
                    fl = 0;
                    while(1)
                    {
                        spaces2 = spaces(x, y, index);
                        int count = 0;
                        int rnd;
                        coordinate move;
                        for(int k = 0; k<8; k++)
                        {
                            if(spaces2[k] == -1)
                            {
                                count = k-1;
                                break;
                            }
                            count = k;
                        }
                        if(count == -1)
                        {
                            break;
                        }
                        rnd = (rand() % (count+1));
                        move = command(x, y, spaces2[rnd]);
                        sem_wait(&ant_lock[index]);
                        sem_wait(&antmutexs[x][y]);
                        sem_wait(&antmutexs[move.x][move.y]);
                        if(lookCharAt(move.x, move.y) == '-')
                        {
                            putCharTo(move.x, move.y, '1');
                            sem_post(&antmutexs[move.x][move.y]);
                            putCharTo(x, y, 'o');
                            sem_post(&antmutexs[x][y]);
                            sem_post(&ant_lock[index]);
                            ants[index].x = move.x;
                            ants[index].y = move.y;
                            ants[index].type = 't';
                            break;
                        }
                        sem_post(&antmutexs[move.x][move.y]);
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        free(spaces2);
                    }
                    free(spaces2);
                }
                else
                {
                    sem_post(&antmutexs[x+1][y-1]);
                    sem_post(&ant_lock[index]);
                }
            }
            if(fl)
            {
                while(1)
                {
                    spaces2 = spaces(x, y, index);
                    int count = 0;
                    int rnd;
                    coordinate move;
                    for(int k = 0; k<8; k++)
                    {
                        if(spaces2[k] == -1)
                        {
                            count = k-1;
                            break;
                        }
                        count = k;
                    }
                    if(count == -1)
                    {
                        break;
                    }
                    rnd = (rand() % (count+1));
                    move = command(x, y, spaces2[rnd]);
                    sem_wait(&ant_lock[index]);
                    sem_wait(&antmutexs[x][y]);
                    sem_wait(&antmutexs[move.x][move.y]);
                    if(lookCharAt(move.x, move.y) == '-')
                    {
                        putCharTo(move.x, move.y, 'P');
                        sem_post(&antmutexs[move.x][move.y]);
                        putCharTo(x, y, '-');
                        sem_post(&antmutexs[x][y]);
                        sem_post(&ant_lock[index]);
                        ants[index].x = move.x;
                        ants[index].y = move.y;
                        break;
                    }
                    sem_post(&antmutexs[move.x][move.y]);
                    sem_post(&antmutexs[x][y]);
                    sem_post(&ant_lock[index]);
                    free(spaces2);
                }
                free(spaces2);
            }
        }
        else
        {
            sem_post(&antmutexs[x][y]);
            sem_post(&ant_lock[index]);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int err;
    int foods, times;
    int number_of_ants = atoi(argv[1]);
    int number_of_foods = atoi(argv[2]);
    int number_of_time = atoi(argv[3]);
    ant_count = number_of_ants;
    times = atoi(argv[2]);
    int i,j;
    for (i = 0; i < GRIDSIZE; i++) {
        for (j = 0; j < GRIDSIZE; j++) {
            putCharTo(i, j, '-');
        }
    }
    ants = malloc(sizeof(ant)*number_of_ants);
    semo = malloc(sizeof(sem_t)*number_of_ants);
    ant_lock = malloc(sizeof(sem_t)*number_of_ants);
    for(int k = 0; k<GRIDSIZE; k++)
    {
        for(int m = 0; m<GRIDSIZE; m++)
        {
            sem_init(&antmutexs[k][m], 0, 1);
        }
    }
    for(int k = 0; k<number_of_ants; k++)
    {
        sem_init(&ant_lock[k], 0, 1);
        sem_init(&semo[k], 0, 1);
    }
    int a,b;
    sem_init(&barrier, 0, 0);
    sem_init(&finish, 0, 1);
    sem_init(&s_sem, 0, 1);
    for (i = 0; i < number_of_ants; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) == '1');
        putCharTo(a, b, '1');
        ants[i].x = a;
        ants[i].y = b;
        ants[i].type = 'a';
    }
    for (i = 0; i < number_of_foods; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) == 'o' || lookCharAt(a,b) == '1');
        putCharTo(a, b, 'o');
    }
    int *p=malloc(sizeof(int)*number_of_ants);
    for(int i = 0; i<number_of_ants; i++)
    {
        *(p+i) = i;
        err = pthread_create(&(tid[i]), NULL, counter, (p+i));
        //printf("err %d", err);
    }
    startCurses();
    sem_post(&barrier);

    char c;
    time_t t1, t2;
    count_of_sleeper = getSleeperN();
    double taken = number_of_time;
    t1 = time(NULL);
    t2 = time(NULL);
    while (difftime(t2, t1) < taken) {
        //count_of_sleeper = getSleeperN();
        int count2 = getSleeperN();
        sem_wait(&s_sem);
        int temp = count_of_sleeper;
        sem_post(&s_sem);
        if(count2 < temp && count2 < number_of_ants)
        {
            for(int k = count2; k<temp; k++)
            {
                sem_post(&semo[k]);
            }
            sem_wait(&s_sem);
            count_of_sleeper = count2;
            sem_post(&s_sem);
        }
        else if(count2 > temp && count2 <= number_of_ants)
        {
            for(int k = temp; k<count2; k++)
            {
                sem_wait(&semo[k]);
            }
            sem_wait(&s_sem);
            count_of_sleeper = count2;
            sem_post(&s_sem);
        }
        sem_wait(&s_sem);
        count_of_sleeper = count2;
        sem_post(&s_sem);
        for(int u = 0; u<number_of_ants; u++)
        {
            sem_wait(&ant_lock[u]);
            /*for(int o = 0; o<GRIDSIZE; o++)
            {
                sem_wait(&antmutexs[u][o]);
            }*/
        }
        drawWindow();
        for(int u = 0; u<number_of_ants; u++)
        {
            sem_post(&ant_lock[u]);
            /*for(int o = 0; o<GRIDSIZE; o++)
            {
                sem_post(&antmutexs[u][o]);
            }*/
        }
        
        c = 0;
        c = getch();

        if (c == 'q' || c == ESC) break;
        if (c == '+') {
            setDelay(getDelay()+10);
        }
        if (c == '-') {
            setDelay(getDelay()-10);
        }
        if (c == '*') {
            setSleeperN(getSleeperN()+1);
        }
        if (c == '/') {
            setSleeperN(getSleeperN()-1);
        }
        usleep(DRAWDELAY);
        //usleep(getDelay() * 1000 + (rand() % 5000));
        t2 = time(NULL);
    }
    //pthread_mutex_destroy(&lock);
    // do not forget freeing the resources you get
    endCurses();
    sem_wait(&finish);
    finisher = 0;
    sem_post(&finish);
    for(int k = 0; k<number_of_ants; k++)
    {
        sem_post(&semo[k]);
    }
    for(int k = 0; k<number_of_ants; k++)
    {
        pthread_join(tid[k], NULL);
    }
    free(p);
    free(ants);
    free(semo);
    return 0;
}
