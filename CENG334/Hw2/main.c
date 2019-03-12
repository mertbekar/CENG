#include "do_not_submit.h"
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t antmutexs[GRIDSIZE][GRIDSIZE];
ant ants[GRIDSIZE*GRIDSIZE];

int c = 0;
pthread_mutex_t lock;
pthread_t tid[GRIDSIZE*GRIDSIZE];

typedef struct ant {
    int x;
    int y;
} ant;

ant command(int x, int y, int rnd)
{
    ant move;
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

bool border(int x, int y)
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

bool occupy(int x, int y)
{
    bool b0, b1, b2, b3, b4, b5, b6, b7, res;
    res = false;
    b0 = border(x-1, y+1);
    b1 = border(x-1, y);
    b2 = border(x-1, y-1);
    b3 = border(x, y+1);
    b4 = border(x, y-1);
    b5 = border(x+1, y+1);
    b6 = border(x+1, y);
    b7 = border(x+1, y-1);
    if(b0 && lookCharAt(x-1, y+1) == '-')
    {
        return true;
    }
    if(b1 && lookCharAt(x-1, y) == '-')
    {
        return true;
    }
    if(b2 && lookCharAt(x-1, y-1) == '-')
    {
        return true;
    }
    if(b3 && lookCharAt(x, y+1) == '-')
    {
        return true;
    }
    if(b4 && lookCharAt(x, y-1) == '-')
    {
        return true;
    }
    if(b5 && lookCharAt(x+1, y+1) == '-')
    {
        return true;
    }
    if(b6 && lookCharAt(x+1, y) == '-')
    {
        return true;
    }
    if(b7 && lookCharAt(x+1, y+-1) == '-')
    {
        return true;
    }
    return false;
}

void* counter(int index)
{
    int x = ants[index].x;
    int y = ants[index].y;
    int rnd;
    bool is;
    bool occupy;
    pthread_mutex_lock(&antmutexs[x][y]);
    if(lookCharAt(x, y) == '1')
    {
        pthread_mutex_lock(&antmutexs[x-1][y+1]);
        is = border(x-1, y+1);
        if(is && lookCharAt(x-1, y+1) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x-1, y+1, 'P');
            pthread_mutex_unlock(&antmutexs[x-1][y+1]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x-1][y+1]);
        pthread_mutex_lock(&antmutexs[x-1][y]);
        is = border(x-1, y);
        else if(is && lookCharAt(x-1, y) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x-1, y, 'P');
            pthread_mutex_unlock(&antmutexs[x-1][y]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x-1][y]);
        pthread_mutex_lock(&antmutexs[x-1][y-1]);
        is = border(x-1, y-1);
        else if(is && lookCharAt(x-1, y-1) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x-1, y-1, 'P');
            pthread_mutex_unlock(&antmutexs[x-1][y-1]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x-1][y-1]);
        pthread_mutex_lock(&antmutexs[x][y+1]);
        is = border(x, y+1);
        else if(is && lookCharAt(x, y+1) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x, y+1, 'P');
            pthread_mutex_unlock(&antmutexs[x][y+1]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x][y+1]);
        pthread_mutex_lock(&antmutexs[x][y-1]);
        is = border(x, y-1);
        else if(is && lookCharAt(x, y-1) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x, y-1, 'P');
            pthread_mutex_unlock(&antmutexs[x][y-1]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x][y-1]);
        pthread_mutex_lock(&antmutexs[x+1][y+1]);
        is = border(x+1, y+1);
        else if(is && lookCharAt(x+1, y+1) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x+1, y+1, 'P');
            pthread_mutex_unlock(&antmutexs[x+1][y+1]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x+1][y+1]);
        pthread_mutex_lock(&antmutexs[x+1][y]);
        is = border(x+1, y);
        else if(is && lookCharAt(x+1, y) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x+1, y, 'P');
            pthread_mutex_unlock(&antmutexs[x+1][y]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x+1][y]);
        pthread_mutex_lock(&antmutexs[x+1][y-1]);
        is = border(x+1, y-1);
        else if(is && lookCharAt(x+1, y-1) == 'o')
        {
            putCharTo(x, y, '-');
            putCharTo(x+1, y-1, 'P');
            pthread_mutex_unlock(&antmutexs[x+1][y-1]);
            pthread_mutex_unlock(&antmutexs[x][y]);
            return;
        }
        pthread_mutex_unlock(&antmutexs[x+1][y-1]);
        else
        {
            occupy = occupy(x, y);
            for(int k = 0; k<8; k++)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                is = border(move.x, move.y);
                if(is && lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(x, y, '-');
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    pthread_mutex_unlock(&antmutexs[x][y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
    }
    else if(lookCharAt(x, y) == 'P')
    {
        if(lookCharAt(x-1, y+1) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x-1, y) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x-1, y-1) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x, y+1) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x, y-1) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x+1, y+1) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x+1, y) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else if(lookCharAt(x+1, y-1) == 'o')
        {
        	//pthread_mutex_lock(antmutexs[x][y]);
            putCharTo(x, y, 'o');
            pthread_mutex_unlock(&antmutexs[x][y]);
            while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
        else
        {
        	while(1)
            {
                rnd = rand() % 8;
                ant move;
                move = command(x, y, rnd);
                pthread_mutex_lock(&antmutexs[move.x][move.y]);
                if(lookCharAt(move.x, move.y) == '-')
                {
                	pthread_mutex_lock(&antmutexs[x][y]);
            		putCharTo(x, y, '-');
            		pthread_mutex_unlock(&antmutexs[x][y]);
                    putCharTo(move.x, move.y, '1');
                    pthread_mutex_unlock(&antmutexs[move.x][move.y]);
                    pthread_mutex_unlock(&antmutexs[x][y]);
                    break;
                }
                pthread_mutex_unlock(&antmutexs[move.x][move.y]);
            }
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int err;
    int ants, foods, times;
    number_of_ants = atoi(argv[0]);
    number_of_foods = atoi(argv[1]);
    times = atoi(argv[2]);
    int i,j;
    for (i = 0; i < GRIDSIZE; i++) {
        for (j = 0; j < GRIDSIZE; j++) {
            putCharTo(i, j, '-');
        }
    }
    int a,b;
    /*for (i = 0; i < 5; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b)!= '-');
        putCharTo(a, b, 'P');
    }*/
    for (i = 0; i < number_of_ants; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) != '-');
        putCharTo(a, b, '1');
        ants[i].x = a;
        ants[i].y = b;
    }
    for (i = 0; i < foods; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) != '-');
        putCharTo(a, b, 'o');
    }
    /*for (i = 0; i < 5; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) != '-');
        putCharTo(a, b, 'S');
    }
    for (i = 0; i < 5; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) != '-');
        putCharTo(a, b, '$');
    }*/
    //////////////////////////////

    // you have to have following command to initialize ncurses.
    startCurses();
    
    // You can use following loop in your program. But pay attention to 
    // the function calls, they do not have any access control, you 
    // have to ensure that.
    char c;
    while (TRUE) {
        drawWindow();
        
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
        
        // each ant thread have to sleep with code similar to this
        //usleep(getDelay() * 1000 + (rand() % 5000));
    }
    for(int i = 0; i<number_of_ants; i++)
    {
        err = pthread_create(&(tid[i]), NULL, &counter, i);
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    // do not forget freeing the resources you get
    endCurses();
    
    return 0;
}
