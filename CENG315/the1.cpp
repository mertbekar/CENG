#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

int minDist(vector<int> mlist, vector<bool> blist)
{
	int min = INT_MAX;
	int mindex;
	for(int i = 0; i<mlist.size(); i++)
	{
		if(blist[i] == false && mlist[i] <=min)
		{
			min = mlist[i];
			mindex = i;
		}
	}
	return mindex;
}


struct Node {
	int destination;
	int dist;
	struct Node* next; 
};

struct adList {
	struct Node *head;
};

struct Matrix
{
	int v;
	struct adList* array;
};

Node* addNode(int destination, int dist)
{
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
	node->destination = destination;
	node->dist = dist;
	node->next = NULL;
	return node;
}

Matrix* addMatrix(int v)
{
	struct Matrix* matrix = (struct Matrix*) malloc(sizeof(struct Matrix));
	matrix->v = v;
	matrix->array = (struct adList*) malloc(v * sizeof(struct adList));
	for(int i = 0; i<v; ++i)
	{
		matrix->array[i].head = NULL;
	}
	return matrix;
}

void addRoad(struct Matrix* matrix, int source, int destination, int dist)
{
	struct Node* node = addNode(destination, dist);
	node->next = matrix->array[source].head;
	matrix->array[source].head = node;
	node = addNode(source, dist);
	node->next = matrix->array[destination].head;
	matrix->array[destination].head = node;
}
 

 
// Structure to represent a min heap node
struct heapNode
{
   	int  v;
   	int distance;
};

struct minHeap
{
	int size;
	int capacity;
	int *position;
	struct heapNode **array;
};

struct heapNode* newHeapNode(int v, int distance)
{
	struct heapNode* heap = (struct heapNode*) malloc(sizeof(struct heapNode));
	heap->v = v;
	heap->distance = distance;
	return heap;
}

struct minHeap* conMinHeap(int capacity)
{
	struct minHeap* mHeap = (struct minHeap*) malloc(sizeof(struct minHeap));
	mHeap->position = (int*) malloc(capacity * sizeof(int));
	mHeap->size = 0;
	mHeap->capacity = capacity;
	mHeap->array = (struct heapNode**) malloc(capacity * sizeof(struct heapNode*));
	return mHeap;
}

void swapHeap(struct heapNode** i, struct heapNode** j)
{
	struct heapNode* k = *i;
	*i = *j;
	*j = k;
}

void heapify(struct minHeap* mHeap, int index)
{
	int min, left, right;
	min = index;
	left = 2*index + 1;
	right = 2*index + 2;
	if(left < mHeap->size && mHeap->array[left]->distance < mHeap->array[min]->distance)
	{
		min = left;
	}
	if(right < mHeap->size && mHeap->array[right]->distance < mHeap->array[min]->distance)
	{
		min = right;
	}
	if(min != index)
	{
		heapNode *sNode = mHeap->array[min];
		heapNode *iNode = mHeap->array[index];
		mHeap->position[sNode->v] = index;
		mHeap->position[iNode->v] = min;
		swapHeap(&mHeap->array[min], &mHeap->array[index]);
		heapify(mHeap, min);
	}
}

int isNotFull(struct minHeap* mHeap)
{
	return mHeap->size == 0;
}

struct heapNode* executeMin(struct minHeap* mHeap)
{
	if(isNotFull(mHeap))
	{
		return NULL;
	}
	struct heapNode* root = mHeap->array[0];
	struct heapNode* eNode = mHeap->array[mHeap->size - 1];
	mHeap->array[0] = eNode;
	mHeap->position[root->v] = mHeap->size - 1;
	mHeap->position[eNode->v] = 0;
	--mHeap->size;
	heapify(mHeap, 0);
	return root;
}

void dKey(struct minHeap* mHeap, int v, int dist)
{
	int i = mHeap->position[v];
	mHeap->array[i]->distance = dist;
	while(i && mHeap->array[i]->distance < mHeap->array[(i-1)/2]->distance)
	{
		mHeap->position[mHeap->array[i]->v] = (i-1)/2;
		mHeap->position[mHeap->array[(i-1)/2]->v] = i;
		swapHeap(&mHeap->array[i], &mHeap->array[(i-1)/2]);
		i = (i-1)/2;
	}
}

bool isMinHeap(struct minHeap *mHeap, int v)
{
	if(mHeap->position[v] < mHeap->size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<int> dijkstra(Matrix* matrix, int source)
{
	int v = matrix->v;
	vector<int> dist;
	minHeap* mHeap = conMinHeap(v);
	//cout<<v<<endl;
	for(int i = 0; i<v; i++)
	{
		dist.push_back(INT_MAX);
		//cout<<v<<endl;
		mHeap->array[i] = newHeapNode(i, dist[i]);
		mHeap->position[i] = i;
	}
	mHeap->array[source] = newHeapNode(source, dist[source]);
	mHeap->position[source] = source;
	dist[source] = 0;
	dKey(mHeap, source, dist[source]);
	mHeap->size = v;
	while(!isNotFull(mHeap))
	{
		heapNode* minNode = executeMin(mHeap);
		int u = minNode->v;
		Node* node = matrix->array[u].head;
		while(node != NULL)
		{
			int y = node->destination;
			if(isMinHeap(mHeap, y) && dist[u] != INT_MAX && node->dist + dist[u] < dist[y])
			{
				dist[y] = dist[u] + node->dist;
				dKey(mHeap, y, dist[y]);
			}
			node = node->next;
		}
	}
	return dist;
}


vector<int> dijkstra2(vector<vector<int> >  matrix, int source)
{
	int size = matrix.size();
	vector<int> mlist;
	vector<bool> blist;
	mlist.resize(size);
	blist.resize(size);
	for(int i = 0; i<size; i++)
	{
		mlist[i] = INT_MAX;
		blist[i] = false;
	}
	mlist[source] = 0;
	for(int u = 0; u<size-1; u++)
	{
		int k = minDist(mlist, blist);
		blist[k] = true;
		for(int v = 0; v<size; v++)
		{
			if((!blist[v] && matrix[k][v]) && (mlist[k] != INT_MAX) && (mlist[k]+matrix[k][v] < mlist[v]))
			{
				mlist[v] = mlist[k] + matrix[k][v];
			}
		}
	}
	return mlist;
}


int main()
{
	int city;
	int destination;
	int warehouse1;
	int warehouse2;
	int path1 = 0;
	int count = 0;
	int path2 = 0;
	int dest1 = 0;
	int dest2 = 0;
	int size;
	int t2;
	int maxi = 0;
	int index;
	int x = 0;
	vector<int> mlist;
	vector<int> ww;
	vector<int> temp;
	vector< vector<int> > clist;
	vector<int> mlist2;
	vector< vector<int> > sub;
	vector<int> dest;
	string line;
	string tt;
	vector<int> mm;
	vector< vector<int> > matrix;
	ifstream input;
	input.open("input.txt");
	getline(input, line);
	city = atoi(line.c_str());
	getline(input, line);
	destination = atoi(line.c_str());
	getline(input, line);
	char *dup = strdup(line.c_str());
	char *pch = strtok(dup, " ");
	while(pch!=NULL)
	{
		tt = string(pch);
		warehouse1 = atoi(tt.c_str());
		ww.push_back(warehouse1);
		pch = strtok(NULL, " ");
	}
	warehouse1 = ww[0];
	warehouse2 = ww[1];
	getline(input, line);
	dup = strdup(line.c_str());
	pch = strtok(dup, " ");
	while(pch!=NULL)
	{
		tt = string(pch);
		t2 = atoi(tt.c_str());
		dest.push_back(t2);
		pch = strtok(NULL, " ");
	}
	while(getline(input, line))
	{
		dup = strdup(line.c_str());
		pch = strtok(dup, " ");
		while(pch!=NULL)
		{
			tt = string(pch);
			t2 = atoi(tt.c_str());
			mm.push_back(t2);
			pch = strtok(NULL, " ");
		}
		matrix.push_back(mm);
		mm.clear();
	}
	input.close();
	size = matrix[0].size();
	struct Matrix* matrix2 = addMatrix(size);
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
			if(matrix[i][j] != 0)
			{
				addRoad(matrix2, i, j, matrix[i][j]);
			}
		}
	}
	mlist = dijkstra(matrix2, warehouse1);
	mlist2 = dijkstra(matrix2, warehouse2);
	for(int k = 0; k<dest.size(); k++)
	{
		path1 = mlist[dest[k]];
		path2 = mlist2[dest[k]];
		if(path1-path2>=0)
		{
			temp.push_back(path1-path2);
			temp.push_back(dest[k]);
			sub.push_back(temp);
			temp.clear();
		}
		else
		{
			temp.push_back(path2-path1);
			temp.push_back(dest[k]);
			sub.push_back(temp);
			temp.clear();
		}
	}
	std::sort(sub.begin(), sub.end());
	for(int j = sub.size()-1; j>=0; j--)
	{
			if(mlist[sub[j][1]] < mlist2[sub[j][1]])
			{
				if(dest1 < dest.size()/2)
				{
					count = count + mlist[sub[j][1]];
					temp.push_back(sub[j][1]);
					temp.push_back(warehouse1);
					clist.push_back(temp);
					temp.clear();
					dest1++;
				}
				else
				{
					count = count + mlist2[sub[j][1]];
					temp.push_back(sub[j][1]);
					temp.push_back(warehouse2);
					clist.push_back(temp);
					temp.clear();
					dest2++;
				}
			}
			else
			{
				if(dest2 < dest.size()/2)
				{
					count = count + mlist2[sub[j][1]];
					temp.push_back(sub[j][1]);
					temp.push_back(warehouse2);
					clist.push_back(temp);
					temp.clear();
					dest2++;
				}
				else
				{
					count = count + mlist[sub[j][1]];
					temp.push_back(sub[j][1]);
					temp.push_back(warehouse1);
					clist.push_back(temp);
					temp.clear();
					dest1++;
				}
			}
	}
	std::sort(clist.begin(), clist.end());
	cout<<count<<endl;
	for(int j = 0; j<clist.size(); j++)
	{
		cout<<clist[j][0]<<" "<<clist[j][1]<<endl;
	}
	return 0;
}
