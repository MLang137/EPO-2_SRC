#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define FULL 100




int maze[13][13];

struct Pos
{
    int x;
    int y;
}; typedef struct Pos Pos;

struct node
{
    Pos pt;
    int dist;

    struct node *next;
};
typedef struct node node;

struct queue
{
    int count;
    node *front;
    node *rear;
};
typedef struct queue queue;

void initialize(queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

int isempty(queue *q)
{
    return (q->rear == NULL);
}

void enqueue(queue *q, Pos point, int value)
{
    if (q->count < FULL)
    {
        node *tmp;
        tmp = malloc(sizeof(node));
        tmp->dist = value;
        tmp->pt = point;
        tmp->next = NULL;
        if(!isempty(q))
        {
            q->rear->next = tmp;
            q->rear = tmp;
        }
        else
        {
            q->front = q->rear = tmp;
        }
        q->count++;
    }
    else
    {
        printf("List is full\n");
    }
}

bool dequeue(queue *q)
{
    
    if (q->front == NULL){return false;}
    node *tmp = q->front;
    
    q->front = q->front->next;
    if(q->front == NULL){ q->rear = NULL;}
    q->count--;
    free(tmp);
    return true;
}

void display(node *head)
{
    if(head == NULL)
    {
        printf("NULL\n");
    }
    else
    {
        printf("count: %d, x: %d, y:%d\n", head -> dist, head->pt.x, head->pt.y);
        display(head->next);
    }
}

bool TargetReached(Pos Target, Pos pos)
{
    if (pos.x == Target.x && pos.y == Target.y)
    {
        return true;
    }else
    {
        return false;
    }
}

void readInput()
{
int minecount,i;
Pos mine;
char direction;
scanf("%d", &minecount);
int goal,source;

for(i = 0; i < minecount; i++)
{
    scanf("%d %d %c",&mine.x,&mine.y,&direction);
    // Read x and y of intersections and add 2 to place them at the right cords.
    mine.x = mine.x+2;
    mine.y = mine.y+2;
    switch(direction)
    {
        case 'n':
            mine.x--;
            break;
        case 'e':
            mine.y++;
            break;
        case 's':
            mine.x++;
            break;
        case 'w':
            mine.y--;
            break;
        default:
        break;
    }
        maze[mine.x][mine.y] = -1;
}
}



void addMine(Pos mine)
{
    maze[mine.x][mine.y] = -1;
    return;
}



//bocht duurt 4.6
//rechtdoor duurt 3.8.
Pos BasetoCord(int base)
{   
    Pos newCord;
    switch(base)
    {
        case 1:
            newCord.x = 12;
            newCord.y = 4;
            break;
        case 2: 
            newCord.x = 12;
            newCord.y = 6;
            break;
        case 3:
            newCord.x = 12;
            newCord.y = 8;
            break;
        case 4:
            newCord.x = 8;
            newCord.y = 12;
            break;
        case 5:
            newCord.x = 6;
            newCord.y = 12;
            break;
        case 6:
            newCord.x = 4;
            newCord.y = 12;
            break;
        case 7:
            newCord.x = 0;
            newCord.y = 8;
            break;
        case 8:
            newCord.x = 0;
            newCord.y = 6;
            break;
        case 9:
            newCord.x = 0;
            newCord.y = 4;
            break;
        case 10:
            newCord.x = 4;
            newCord.y = 0;
            break;
        case 11:
            newCord.x = 6;
            newCord.y = 0;
            break;
        case 12:
            newCord.x = 8;
            newCord.y = 0;
            break;
        default:
        printf("Please give a basenumber between 1 and 12\n");
    } 
            return newCord;
}

void CreateMap(void){
int i,j;
// Creating Map Full of -1
for (i = 0; i < 13; i++){
    for (j = 0; j < 13; j++){
        maze[i][j] = -1;
    }
}

// Creating horizontal Paths.
for (i = 4; i < 10 ; i++){
    if (i == 4 || i == 6 || i == 8){
        for (j = 0; j < 13; j++){
          maze[i][j] = 0;
        }
    }
}

// Creating vertical Paths.
for (i = 4; i < 9 ; i++){
    if (i == 4 || i == 6 || i == 8){
        for (j = 0; j < 13; j++){
          maze[j][i] = 0;
        }
    }
}

// Creating In between paths
for (i = 2; i < 11 ; i++){
    if (i == 2 || i == 10){
        for (j = 2; j < 11; j++){
          maze[j][i] = 0;
        }
    }
}

for (i = 2; i < 11 ; i++){
    if (i == 2 ||i == 10){
        for (j = 2; j < 11; j++){
          maze[i][j] = 0;
        }
    }
}
}
bool CheckCell(int row, int col){

   return (row >= 0) && (row < 13) &&
           (col >= 0) && (col < 13);

}
void PrintMaze(){
     for (int i = 0; i < 13; i++){
        for (int j = 0; j < 13; j++){
             printf( "  %d \t",maze[i][j]);
        }
        printf("\n"); 
        printf("\n");
    }
}

bool Algorithm(Pos source, Pos goal)
{
    //Creating queue and assigning memory
    queue *q;
    q = malloc(sizeof(queue));
    initialize(q);

    //Initilizing some variables used.
    int p,qu;
    int dist = 1;
    int dix[4] = {-1,0,0,1};
    int diy[4] = {0,-1,1,0};



    //Creating visited map
    bool visited[13][13];
    memset(visited, false, sizeof(visited));
    visited[goal.x][goal.y] = true;
    //Marking goal cell with 1.
    maze[goal.x][goal.y] = 1;

    node *CurrNode;

    enqueue(q,goal,1);


//Expand Function
    while(!isempty(q))
    {
        CurrNode = q->front;
        Pos CurrPos = CurrNode->pt;
        dist = CurrNode->dist;
        if(TargetReached(source,CurrPos)){return true;}
        dequeue(q);

       for(p = 0; p < 4; p++)
       {    
            int row = CurrPos.x + dix[p];
            int col = CurrPos.y + diy[p];

            if(CheckCell(row,col) && maze[row][col] == 0 && !visited[row][col])
            {
                visited[row][col] = true;
                maze[row][col] = dist+1;
                Pos Neighbour = {row,col};
                enqueue(q, Neighbour, dist+1);
            } 
       }
    } return false;
}



bool RoutePlanner(Pos Source, Pos Goal)
{
    int dix[4] = {-1,0,0,1};
    int diy[4] = {0,-1,1,0};
    int p;

    int count = maze[Source.x][Source.y];
    Pos CurrPos = Source;

    Pos Route[] = malloc(sizeof(Pos));

int i = 0;
while(!TargetReached(Goal,CurrPos)){
       for(p = 0; p < 4; p++)
       {     
            int row = CurrPos.x + dix[p];
            int col = CurrPos.y + diy[p];

            if(maze[row][col] == count-1){
                CurrPos.x = row;
                CurrPos.y = col;
                Route[i].x = dix[p];
                Route[i].y = diy[p];
            }
       }
       count--;
       i++;

}
int j;
for(j =0; j < i; j++)
{

    printf("Step %d, dx: %d, dy: %d\n",j,Route[j].x,Route[j].y);


}

}
int main()
{
    Pos Mine1 = {5,2}; 
    Pos Mine2 = {5,5};

    Pos Source = {4,0}; //Base 10
    Pos Goal = {8,0}; //Base 12
    CreateMap();
    addMine(Mine1);
    addMine(Mine2);
    Algorithm(Source, Goal);
    printf("\n");
    PrintMaze();
    RoutePlanner(Source, Goal);
    return 0;
}