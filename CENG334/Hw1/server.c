#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

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

struct Hunter
{
	coordinate coor;
	int energy;
	char status;
};

struct Prey
{
	coordinate coor;
	int energy;
	char status;
};

struct Obstacle
{
	coordinate coor;
};

int main()
{
	int height, width, obstacles, hunters, preys;
	scanf("%d%d", &width, &height);
	char matrix[height][width];
	for(int i = 0; i<height; i++)
	{
		for(int j = 0; j<width; j++)
		{
			matrix[i][j] = ' ';
		}
	}
	scanf("%d", &obstacles);
	struct Obstacle Obstacles[obstacles];
	for(int i = 0; i<obstacles; i++)
	{
		int x, y;
		scanf("%d%d", &y, &x);
		Obstacles[i].coor.x = x;
		Obstacles[i].coor.y = y;
		matrix[y][x] = 'X';
	}
	scanf("%d", &hunters);
	struct Hunter Hunters[hunters];
	for(int i = 0; i<hunters; i++)
	{
		int x, y, energy;
		scanf("%d%d%d", &y, &x, &energy);
		Hunters[i].coor.x = x;
		Hunters[i].coor.y = y;
		Hunters[i].energy = energy;
		Hunters[i].status = 'a';
		matrix[y][x] = 'H';
 	}
 	scanf("%d", &preys);
 	struct Prey Preys[preys];
 	for(int i = 0; i<preys; i++)
 	{
 		int x, y, energy;
 		scanf("%d%d%d", &y, &x, &energy);
 		Preys[i].coor.x = x;
 		Preys[i].coor.y = y;
 		Preys[i].energy = energy;
 		Preys[i].status = 'a';
 		matrix[y][x] = 'P';
 	}

 	printf("+");
 	for(int i = 0; i<width; i++)
 	{
 		printf("-");
 	}
 	printf("+");
 	printf("\n");
 	for(int i = 0; i<height; i++)
 	{
 		printf("|");
 		for(int j = 0; j<width; j++)
 		{
 			printf("%c", matrix[i][j]);
 		}
 		printf("|");
 		printf("\n");
 	}
 	printf("+");
 	for(int i = 0; i<width; i++)
 	{
 		printf("-");
 	}
 	printf("+");
 	printf("\n");
 	fflush(stdout);

 	int fd[hunters+preys][2];
 	int fd2[2];
 	fd_set readfds, actfds;
 	pid_t pid1[hunters+preys];
 	int k = hunters+preys;
 	char str1[12];
 	char str2[12];
 	sprintf(str1, "%d", height);
 	sprintf(str2, "%d", width);
 	char *args[] = {"hunter", str1, str2, NULL};
 	char *args2[] = {"prey", str1, str2, NULL};
 	for(int i = 0; i<hunters; i++)
 	{
 		PIPE(fd[i]);
 		pid1[i] = fork();
 		if(pid1[i] == 0)
 		{
 			close(fd[i][0]);
 			dup2(fd[i][1], 0);
 			dup2(fd[i][1], 1);
 			execv("./hunter", args);
 			exit(0);
 		}
 		struct server_message temp;
 		temp.pos.x = Hunters[i].coor.x;
 		temp.pos.y = Hunters[i].coor.y;
 		temp.adv_pos.x = 0;
 		temp.adv_pos.y = 0;
 		int mh = INT_MAX;
 		for(int k = 0; k<preys; k++)
 		{
 			if(mh > abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y))
 			{
 				mh = abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y);
 				temp.adv_pos.x = Preys[k].coor.x;
 				temp.adv_pos.y = Preys[k].coor.y;
 			}
 		}
 		int a = Hunters[i].coor.x;
 		int b = Hunters[i].coor.y;
 		temp.object_count = 0;
 		int cc = 0;
 		if((a > 0 && matrix[b][a-1] == 'H') || (a > 0 && matrix[b][a-1] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a-1;
 			temp.object_pos[cc].y = b;
 			cc++;
 		}
 		if((b > 0 && matrix[b-1][a] == 'H') || (b > 0 && matrix[b-1][a] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a;
 			temp.object_pos[cc].y = b-1;
 			cc++;
 		}
 		if((a < width-1 && matrix[b][a+1] == 'H') || (a < width-1 && matrix[b][a+1] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a+1;
 			temp.object_pos[cc].y = b;
 			cc++;
 		}
 		if((b < height-1 && matrix[b+1][a] == 'H') || (b < height-1 && matrix[b+1][a] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a;
 			temp.object_pos[cc].y = b+1;
 			cc++;
 		}
 		write(fd[i][0], &temp, sizeof(server_message));
 	}
 	for(int i = hunters; i<preys+hunters; i++)
 	{
 		PIPE(fd[i]);
 		pid1[i] = fork();
 		if(pid1[i] == 0)
 		{
 			close(fd[i][0]);
 			dup2(fd[i][1], 0);
 			dup2(fd[i][1], 1);
 			execv("./prey", args2);
 			exit(0);
 		}
 		struct server_message temp;
 		temp.pos.x = Preys[i-hunters].coor.x;
 		temp.pos.y = Preys[i-hunters].coor.y;
 		temp.adv_pos.x = 0;
 		temp.adv_pos.y = 0;
 		int mh = INT_MAX;
 		for(int k = 0; k<hunters; k++)
 		{
 			if(mh > abs(Preys[i-hunters].coor.x - Hunters[k].coor.x) + abs(Preys[i-hunters].coor.y - Hunters[k].coor.y))
 			{
 				mh = abs(Preys[i-hunters].coor.x - Hunters[k].coor.x) + abs(Preys[i-hunters].coor.y - Hunters[k].coor.y);
 				temp.adv_pos.x = Hunters[k].coor.x;
 				temp.adv_pos.y = Hunters[k].coor.y;
 			}
 		}
 		int a = Preys[i-hunters].coor.x;
 		int b = Preys[i-hunters].coor.y;
 		temp.object_count = 0;
 		int cc = 0;
 		if((a > 0 && matrix[b][a-1] == 'P') || (a > 0 && matrix[b][a-1] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a-1;
 			temp.object_pos[cc].y = b;
 			cc++;
 		}
 		if((b > 0 && matrix[b-1][a] == 'P') || (b > 0 && matrix[b-1][a] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a;
 			temp.object_pos[cc].y = b-1;
 			cc++;
 		}
 		if((a < width-1 && matrix[b][a+1] == 'P') || (a < width-1 && matrix[b][a+1] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a+1;
 			temp.object_pos[cc].y = b;
 			cc++;
 		}
 		if((b < height-1 && matrix[b+1][a] == 'P') || (b < height-1 && matrix[b+1][a] == 'X'))
 		{
 			temp.object_count++;
 			temp.object_pos[cc].x = a;
 			temp.object_pos[cc].y = b+1;
 			cc++;
 		}
 		write(fd[i][0], &temp, sizeof(server_message));
 	}

 	int pp = preys;
 	int hh = hunters;
 	int u = 0;
 	int ct = 0;
 	int r = 0;
 	int t = 0;
 	while(pp && hh)
 	{
	 	FD_ZERO(&readfds);
	 	int nselect = 0;
	 	for(int i = 0; i<hunters+preys; i++)
	 	{
	 		close(fd[i][1]);
	 		if(nselect < fd[i][0])
	 		{
	 			nselect = fd[i][0];
	 		}
	 		if(i < hunters)
	 		{
	 			if(Hunters[i].status == 'a')
	 			{
	 				FD_SET(fd[i][0], &readfds);
	 			}
	 		}
	 		if(i >= hunters)
	 		{
	 			if(Preys[i-hunters].status == 'a')
	 			{
	 				FD_SET(fd[i][0], &readfds);
	 			}
	 		}
	 	}
		select(nselect+1, &readfds, NULL, NULL, NULL);
	 	for(int i = 0; i<hunters+preys; i++)
	 	{
	 		if(FD_ISSET(fd[i][0], &readfds))
	 		{
	 			struct ph_message t2;
	 			t2.move_request.x = 0;
	 			t2.move_request.y = 0;
	 			if(read(fd[i][0], &t2, sizeof(ph_message)) > 0)
	 			{
	 				//printf("xxxx %d\n", i);
	 				if(i < hunters)
	 				{
	 					if(matrix[t2.move_request.y][t2.move_request.x] == ' ')
	 					{
	 						matrix[Hunters[i].coor.y][Hunters[i].coor.x] = ' ';
	 						Hunters[i].coor.x = t2.move_request.x;
	 						Hunters[i].coor.y = t2.move_request.y;
	 						Hunters[i].energy = Hunters[i].energy - 1;
	 						if(Hunters[i].energy == 0)
	 						{
	 							int st;
	 							pid_t pid;
	 							Hunters[i].status = 'd';
	 							hh = hh - 1;
	 							kill(pid1[i], SIGTERM);
	 							close(fd[i][1]);
	 							close(fd[i][0]);
	 							waitpid(pid1[i],&st,0);
	 							printf("+");
							 	for(int i = 0; i<width; i++)
							 	{
							 		printf("-");
							 	}
							 	printf("+");
							 	printf("\n");
							 	for(int i = 0; i<height; i++)
							 	{
							 		printf("|");
							 		for(int j = 0; j<width; j++)
							 		{
							 			printf("%c", matrix[i][j]);
							 		}
							 		printf("|");
							 		printf("\n");
							 	}
							 	printf("+");
							 	for(int i = 0; i<width; i++)
							 	{
							 		printf("-");
							 	}
							 	printf("+");
							 	printf("\n");
							 	fflush(stdout);
							 	if(hh == 0)
	 							{
	 								break;
	 							}
	 						}
	 						else
	 						{
		 						matrix[t2.move_request.y][t2.move_request.x] = 'H';
		 						struct server_message temp;
						 		temp.pos.x = Hunters[i].coor.x;
						 		temp.pos.y = Hunters[i].coor.y;
						 		temp.adv_pos.x = 0;
						 		temp.adv_pos.y = 0;
						 		int mh = INT_MAX;
						 		for(int k = 0; k<preys; k++)
						 		{
						 			if(mh > abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y) && Preys[k].status == 'a')
						 			{
						 				mh = abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y);
						 				temp.adv_pos.x = Preys[k].coor.x;
						 				temp.adv_pos.y = Preys[k].coor.y;
						 			}
						 		}
						 		int a = Hunters[i].coor.x;
						 		int b = Hunters[i].coor.y;
						 		temp.object_count = 0;
						 		int cc = 0;
						 		if((a > 0 && matrix[b][a-1] == 'H') || (a > 0 && matrix[b][a-1] == 'X'))
						 		{
						 			temp.object_count++;
						 			temp.object_pos[cc].x = a-1;
						 			temp.object_pos[cc].y = b;
						 			cc++;
						 		}
						 		if((b > 0 && matrix[b-1][a] == 'H') || (b > 0 && matrix[b-1][a] == 'X'))
						 		{
						 			temp.object_count++;
						 			temp.object_pos[cc].x = a;
						 			temp.object_pos[cc].y = b-1;
						 			cc++;
						 		}
						 		if((a < width-1 && matrix[b][a+1] == 'H') || (a < width-1 && matrix[b][a+1] == 'X'))
						 		{
						 			temp.object_count++;
						 			temp.object_pos[cc].x = a+1;
						 			temp.object_pos[cc].y = b;
						 			cc++;
						 		}
						 		if((b < height-1 && matrix[b+1][a] == 'H') || (b < height-1 && matrix[b+1][a] == 'X'))
						 		{
						 			temp.object_count++;
						 			temp.object_pos[cc].x = a;
						 			temp.object_pos[cc].y = b+1;
						 			cc++;
						 		}
						 		printf("+");
							 	for(int i = 0; i<width; i++)
							 	{
							 		printf("-");
							 	}
							 	printf("+");
							 	printf("\n");
							 	for(int i = 0; i<height; i++)
							 	{
							 		printf("|");
							 		for(int j = 0; j<width; j++)
							 		{
							 			printf("%c", matrix[i][j]);
							 		}
							 		printf("|");
							 		printf("\n");
							 	}
							 	printf("+");
							 	for(int i = 0; i<width; i++)
							 	{
							 		printf("-");
							 	}
							 	printf("+");
							 	printf("\n");
							 	fflush(stdout);
						 		write(fd[i][0], &temp, sizeof(server_message));
						 	}
	 					}
	 					else if(matrix[t2.move_request.y][t2.move_request.x] == 'H' || matrix[t2.move_request.y][t2.move_request.x] == 'X')
	 					{
		 					struct server_message temp;
						 	temp.pos.x = Hunters[i].coor.x;
						 	temp.pos.y = Hunters[i].coor.y;
						 	temp.adv_pos.x = 0;
						 	temp.adv_pos.y = 0;
						 	int mh = INT_MAX;
						 	for(int k = 0; k<preys; k++)
						 	{
						 		if(mh > abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y) && Preys[k].status == 'a')
						 		{
						 			mh = abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y);
						 			temp.adv_pos.x = Preys[k].coor.x;
						 			temp.adv_pos.y = Preys[k].coor.y;
						 		}
						 	}
						 	int a = Hunters[i].coor.x;
						 	int b = Hunters[i].coor.y;
						 	temp.object_count = 0;
						 	int cc = 0;
						 	if((a > 0 && matrix[b][a-1] == 'H') || (a > 0 && matrix[b][a-1] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a-1;
						 		temp.object_pos[cc].y = b;
						 		cc++;
						 	}
						 	if((b > 0 && matrix[b-1][a] == 'H') || (b > 0 && matrix[b-1][a] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a;
						 		temp.object_pos[cc].y = b-1;
						 		cc++;
						 	}
						 	if((a < width-1 && matrix[b][a+1] == 'H') || (a < width-1 && matrix[b][a+1] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a+1;
						 		temp.object_pos[cc].y = b;
						 		cc++;
						 	}
						 	if((b < height-1 && matrix[b+1][a] == 'H') || (b < height-1 && matrix[b+1][a] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a;
						 		temp.object_pos[cc].y = b+1;
						 		cc++;
						 	}
						 	write(fd[i][0], &temp, sizeof(server_message));
	 					}
	 					else if(matrix[t2.move_request.y][t2.move_request.x] == 'P')
	 					{
	 						for(int m = 0; m<preys; m++)
	 						{
	 							if(t2.move_request.x == Preys[m].coor.x && t2.move_request.y == Preys[m].coor.y && Preys[m].status == 'a')
	 							{
	 								u = m;
	 								break;
	 							}
	 						}
	 						int st;
	 						pid_t pid;
	 						pp = pp - 1;
	 						close(fd[u+hunters][1]);
	 						close(fd[u+hunters][0]);
	 						kill(pid1[u+hunters], SIGTERM);
	 						waitpid(pid1[u+hunters],&st,0);
	 						Preys[u].status = 'd';
	 						Hunters[i].energy = Hunters[i].energy + Preys[u].energy;
	 						matrix[Hunters[i].coor.y][Hunters[i].coor.x] = ' ';
	 						matrix[Preys[u].coor.y][Preys[u].coor.x] = 'H';
	 						Hunters[i].coor.x = t2.move_request.x;
	 						Hunters[i].coor.y = t2.move_request.y;
	 						Hunters[i].energy = Hunters[i].energy - 1;
	 						if(pp == 0)
	 						{
							 	printf("+");
								for(int i = 0; i<width; i++)
								{
									printf("-");
								}
								printf("+");
								printf("\n");
								for(int i = 0; i<height; i++)
								{
									printf("|");
									for(int j = 0; j<width; j++)
									{
										printf("%c", matrix[i][j]);
									}
									printf("|");
									printf("\n");
								}
								printf("+");
								for(int i = 0; i<width; i++)
								{
									printf("-");
								}
								printf("+");
								printf("\n");
								fflush(stdout);
		 						break;
	 						}
	 						struct server_message temp;
						 	temp.pos.x = Hunters[i].coor.x;
						 	temp.pos.y = Hunters[i].coor.y;
						 	temp.adv_pos.x = 0;
						 	temp.adv_pos.y = 0;
						 	int mh = INT_MAX;
						 	for(int k = 0; k<preys; k++)
						 	{
						 		if(mh > abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y) && Preys[k].status == 'a')
						 		{
						 			mh = abs(Preys[k].coor.x - Hunters[i].coor.x) + abs(Preys[k].coor.y - Hunters[i].coor.y);
						 			temp.adv_pos.x = Preys[k].coor.x;
						 			temp.adv_pos.y = Preys[k].coor.y;
						 		}
						 	}
						 	int a = Hunters[i].coor.x;
						 	int b = Hunters[i].coor.y;
						 	temp.object_count = 0;
						 	int cc = 0;
						 	if((a > 0 && matrix[b][a-1] == 'H') || (a > 0 && matrix[b][a-1] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a-1;
						 		temp.object_pos[cc].y = b;
						 		cc++;
						 	}
						 	if((b > 0 && matrix[b-1][a] == 'H') || (b > 0 && matrix[b-1][a] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a;
						 		temp.object_pos[cc].y = b-1;
						 		cc++;
						 	}
						 	if((a < width-1 && matrix[b][a+1] == 'H') || (a < width-1 && matrix[b][a+1] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a+1;
						 		temp.object_pos[cc].y = b;
						 		cc++;
						 	}
						 	if((b < height-1 && matrix[b+1][a] == 'H') || (b < height-1 && matrix[b+1][a] == 'X'))
						 	{
						 		temp.object_count++;
						 		temp.object_pos[cc].x = a;
						 		temp.object_pos[cc].y = b+1;
						 		cc++;
						 	}
						 	printf("+");
							for(int i = 0; i<width; i++)
							{
								printf("-");
							}
							printf("+");
							printf("\n");
							for(int i = 0; i<height; i++)
							{
								printf("|");
								for(int j = 0; j<width; j++)
								{
									printf("%c", matrix[i][j]);
								}
								printf("|");
								printf("\n");
							}
							printf("+");
							for(int i = 0; i<width; i++)
							{
								printf("-");
							}
							printf("+");
							printf("\n");
							fflush(stdout);
						 	write(fd[i][0], &temp, sizeof(server_message));
	 					}
	 				}
	 				else if(i >= hunters)
	 				{
	 					if(matrix[t2.move_request.y][t2.move_request.x] == ' ')
	 					{
	 						matrix[Preys[i-hunters].coor.y][Preys[i-hunters].coor.x] = ' ';
	 						Preys[i-hunters].coor.x = t2.move_request.x;
	 						Preys[i-hunters].coor.y = t2.move_request.y;
	 						matrix[Preys[i-hunters].coor.y][Preys[i-hunters].coor.x] = 'P';
	 						struct server_message temp;
					 		temp.pos.x = Preys[i-hunters].coor.x;
					 		temp.pos.y = Preys[i-hunters].coor.y;
					 		temp.adv_pos.x = 0;
					 		temp.adv_pos.y = 0;
					 		int mh = INT_MAX;
					 		for(int k = 0; k<hunters; k++)
					 		{
					 			if((mh > abs(Preys[i-hunters].coor.x - Hunters[k].coor.x) + abs(Preys[i-hunters].coor.y - Hunters[k].coor.y)) && Hunters[k].status == 'a')
					 			{
					 				mh = abs(Preys[i-hunters].coor.x - Hunters[k].coor.x) + abs(Preys[i-hunters].coor.y - Hunters[k].coor.y);
					 				temp.adv_pos.x = Hunters[k].coor.x;
					 				temp.adv_pos.y = Hunters[k].coor.y;
					 			}
					 		}
					 		int a = Preys[i-hunters].coor.x;
					 		int b = Preys[i-hunters].coor.y;
					 		temp.object_count = 0;
					 		int cc = 0;
					 		if((a > 0 && matrix[b][a-1] == 'P') || (a > 0 && matrix[b][a-1] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a-1;
					 			temp.object_pos[cc].y = b;
					 			cc++;
					 		}
					 		if((b > 0 && matrix[b-1][a] == 'P') || (b > 0 && matrix[b-1][a] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a;
					 			temp.object_pos[cc].y = b-1;
					 			cc++;
					 		}
					 		if((a < width-1 && matrix[b][a+1] == 'P') || (a < width-1 && matrix[b][a+1] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a+1;
					 			temp.object_pos[cc].y = b;
					 			cc++;
					 		}
					 		if((b < height-1 && matrix[b+1][a] == 'P') || (b < height-1 && matrix[b+1][a] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a;
					 			temp.object_pos[cc].y = b+1;
					 			cc++;
					 		}
					 		fflush(stdout);
					 		printf("+");
							for(int i = 0; i<width; i++)
							{
								printf("-");
							}
							printf("+");
							printf("\n");
							for(int i = 0; i<height; i++)
							{
								printf("|");
								for(int j = 0; j<width; j++)
								{
									printf("%c", matrix[i][j]);
								}
								printf("|");
								printf("\n");
							}
							printf("+");
							for(int i = 0; i<width; i++)
							{
								printf("-");
							}
							printf("+");
							printf("\n");
	 						write(fd[i][0], &temp, sizeof(server_message));
	 					}
	 					else if(matrix[t2.move_request.y][t2.move_request.x] == 'P' || matrix[t2.move_request.y][t2.move_request.x] == 'X')
	 					{
	 						struct server_message temp;
					 		temp.pos.x = Preys[i-hunters].coor.x;
					 		temp.pos.y = Preys[i-hunters].coor.y;
					 		temp.adv_pos.x = 0;
					 		temp.adv_pos.y = 0;
					 		//printf("burda");
					 		int mh = INT_MAX;
					 		for(int k = 0; k<hunters; k++)
					 		{
					 			if(mh > abs((Preys[i-hunters].coor.x - Hunters[k].coor.x) + abs(Preys[i-hunters].coor.y - Hunters[k].coor.y)) && Hunters[k].status == 'a')
					 			{
					 				mh = abs(Preys[i-hunters].coor.x - Hunters[k].coor.x) + abs(Preys[i-hunters].coor.y - Hunters[k].coor.y);
					 				temp.adv_pos.x = Hunters[k].coor.x;
					 				temp.adv_pos.y = Hunters[k].coor.y;
					 			}
					 		}
					 		int a = Preys[i-hunters].coor.x;
					 		int b = Preys[i-hunters].coor.y;
					 		temp.object_count = 0;
					 		int cc = 0;
					 		if((a > 0 && matrix[b][a-1] == 'P') || (a > 0 && matrix[b][a-1] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a-1;
					 			temp.object_pos[cc].y = b;
					 			cc++;
					 		}
					 		if((b > 0 && matrix[b-1][a] == 'P') || (b > 0 && matrix[b-1][a] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a;
					 			temp.object_pos[cc].y = b-1;
					 			cc++;
					 		}
					 		if((a < width-1 && matrix[b][a+1] == 'P') || (a < width-1 && matrix[b][a+1] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a+1;
					 			temp.object_pos[cc].y = b;
					 			cc++;
					 		}
					 		if((b < height-1 && matrix[b+1][a] == 'P') || (b < height-1 && matrix[b+1][a] == 'X'))
					 		{
					 			temp.object_count++;
					 			temp.object_pos[cc].x = a;
					 			temp.object_pos[cc].y = b+1;
					 			cc++;
					 		}
					 		write(fd[i][0], &temp, sizeof(server_message));
					 	}
					 	else if(matrix[t2.move_request.y][t2.move_request.x] == 'H')
					 	{
					 		int uu = 0;
					 		for(int m = 0; m<hunters; m++)
	 						{
	 							if(t2.move_request.x == Hunters[m].coor.x && t2.move_request.y == Hunters[m].coor.y && Hunters[m].status == 'a')
	 							{
	 								uu = m;
	 								break;
	 							}
	 						}
 							int st;
	 						pid_t pid;
	 						pp = pp - 1;
	 						close(fd[i][1]);
	 						close(fd[i][0]);
	 						kill(pid1[i], SIGTERM);
	 						waitpid(pid1[i],&st,0);
	 						Preys[i-hunters].status = 'd';
	 						Hunters[uu].energy = Hunters[uu].energy + Preys[i-hunters].energy;
	 						matrix[Preys[i-hunters].coor.y][Preys[i-hunters].coor.x] = ' ';
	 						fflush(stdout);
					 		printf("+");
							for(int i = 0; i<width; i++)
							{
								printf("-");
							}
							printf("+");
							printf("\n");
							for(int i = 0; i<height; i++)
							{
								printf("|");
								for(int j = 0; j<width; j++)
								{
									printf("%c", matrix[i][j]);
								}
								printf("|");
								printf("\n");
							}
							printf("+");
							for(int i = 0; i<width; i++)
							{
								printf("-");
							}
							printf("+");
							printf("\n");
							if(pp == 0)
	 						{
	 							break;
	 						}
						}
	 				}
	 			}
	 		}
	 	}
	}


	pid_t pid;
 	int st;
 	for(int i=0; i<preys+hunters; i++)
 	{
		close(fd[i][1]);
		close(fd[i][0]);
		kill(pid1[i],SIGTERM);
		waitpid(pid1[i],&st,0);
	}
	//sleep(20);
	return 0; 
}

