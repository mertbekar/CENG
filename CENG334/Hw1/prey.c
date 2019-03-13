#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

typedef struct coordinate{
	int x;
	int y;
} coordinate;

typedef struct server_message {
	coordinate pos;
	coordinate adv_pos;
	int object_count;
	coordinate object_pos[4];
} server_message;

typedef struct ph_message {
coordinate move_request;
} ph_message;

int main(int argc, char *argv[])
{
	int c = 0;
	while(1)
	{
		server_message t1;
		int flag = 1;
		int dist;
		coordinate next;
		next.x = 1;
		next.y = 1;
		read(0, &t1, sizeof(server_message));
		next.x = t1.pos.x;
		next.y = t1.pos.y;
		dist = (abs(t1.adv_pos.x - t1.pos.x) + abs(t1.adv_pos.y - t1.pos.y));
		int heigth = atoi(argv[1]);
		int width = atoi(argv[2]);
		for(int i = 0; i<t1.object_count; i++)
		{
			if((t1.pos.x)-1 == t1.object_pos[i].x && t1.pos.y == t1.object_pos[i].y && t1.pos.x > 0)
			{
				flag = 0;
				break;
			}
		}
		if(flag == 1)
		{
			if(t1.pos.x > 0 && dist < (abs(t1.adv_pos.x - (t1.pos.x-1)) + abs(t1.adv_pos.y - t1.pos.y)))
			{
				dist = (abs(t1.adv_pos.x - (t1.pos.x-1)) + abs(t1.adv_pos.y - t1.pos.y));
				next.x = (t1.pos.x)-1;
				next.y = t1.pos.y;
			}
		}
		flag = 1;
		for(int i = 0; i<t1.object_count; i++)
		{
			if(t1.pos.x == t1.object_pos[i].x && (t1.pos.y)-1 == t1.object_pos[i].y && t1.pos.y > 0)
			{
				flag = 0;
				break;
			}
		}
		if(flag == 1)
		{
			if(t1.pos.y > 0 && dist < (abs(t1.adv_pos.x - t1.pos.x) + abs(t1.adv_pos.y - ((t1.pos.y)-1))))
			{
				dist = (abs(t1.adv_pos.x - t1.pos.x) + abs(t1.adv_pos.y - ((t1.pos.y)-1)));
				next.x = t1.pos.x;
				next.y = (t1.pos.y)-1;
			}
		}

		flag = 1;
		for(int i = 0; i<t1.object_count; i++)
		{
			if((t1.pos.x)+1 == t1.object_pos[i].x && t1.pos.y == t1.object_pos[i].y && (t1.pos.x)+1)
			{
				flag = 0;
				break;
			}
		}
		if(flag == 1)
		{
			if((t1.pos.x)+1 < width && dist < (abs(t1.adv_pos.x - ((t1.pos.x)+1)) + abs(t1.adv_pos.y - t1.pos.y)))
			{
				dist = (abs(t1.adv_pos.x - ((t1.pos.x)+1)) + abs(t1.adv_pos.y - t1.pos.y));
				next.x = (t1.pos.x)+1;
				next.y = t1.pos.y;
			}
		}

		flag = 1;
		for(int i = 0; i<t1.object_count; i++)
		{
			if(t1.pos.x == t1.object_pos[i].x && (t1.pos.y)+1 == t1.object_pos[i].y && (t1.pos.y)+1 < heigth)
			{
				flag = 0;
				break;
			}
		}
		if(flag == 1)
		{
			if((t1.pos.y)+1 < heigth && dist < (abs(t1.adv_pos.x - t1.pos.x) + abs(t1.adv_pos.y - ((t1.pos.y)+1))))
			{
				dist = (abs(t1.adv_pos.x - t1.pos.x) + abs(t1.adv_pos.y - ((t1.pos.y)+1)));
				next.x = t1.pos.x;
				next.y = (t1.pos.y)+1;
			}
		}

		ph_message req;
		req.move_request.x = next.x;
		req.move_request.y = next.y;
		write(1, &req, sizeof(ph_message));
		usleep(10000*(1+rand()%9));
		c++;
	}
	return 0;
}
