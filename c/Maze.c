#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define FULL 100
#define COMPORT "COM9"
#define BAUDRATE CBR_9600

/*--------------------------------------------------------------
// Function: initSio
// Description: intializes the parameters as Baudrate, Bytesize,
//           Stopbits, Parity and Timeoutparameters of
//           the COM port
//--------------------------------------------------------------
*/

void initSio(HANDLE hSerial){

    COMMTIMEOUTS timeouts ={0};
    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        /*error getting state*/
        printf("error getting state \n");
    }

    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams)){
        /*error setting serial port state*/
        printf("error setting state \n");
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if(!SetCommTimeouts(hSerial, &timeouts)){
    /*error occureed. Inform user*/
        printf("error setting timeout state \n");
    }
}

/*--------------------------------------------------------------
// Function: readByte
// Description: reads a single byte from the COM port into
//              buffer buffRead
//--------------------------------------------------------------
*/
char readByte(HANDLE hSerial) {

    DWORD dwBytesRead = 0;
    char *buff;
    buff = calloc('i', sizeof(char)); //initializing array to avoid reading error. 
    if (!ReadFile(hSerial, buff, 1, &dwBytesRead, NULL))
    {
        printf("error reading byte from input buffer \n");
    }
    return(buff[0]);
}


/*--------------------------------------------------------------
// Function: writeByte
// Description: writes a single byte stored in buffRead to
//              the COM port
//--------------------------------------------------------------
*/
int writeByte(HANDLE hSerial, char *buffWrite){

    DWORD dwBytesWritten = 0;

    if (!WriteFile(hSerial, buffWrite, 1, &dwBytesWritten, NULL))
    {
        printf("error writing byte to output buffer \n");
    }
    printf("Byte written to write buffer is: %c \n", buffWrite[0]);

    return(0);
}



int maze[13][13];
int RouteCount;  


//defining structs for easy point representation. 
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
//creating queue for the BFS algorithm.
struct queue
{
    int count;
    node *front;
    node *rear;
};
typedef struct queue queue;
//Initializing queue.
void initialize(queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}
//Function to check if the queue is empty. 
int isempty(queue *q)
{
    return (q->rear == NULL);
}
//function to add elements to queue.
void enqueue(queue *q, Pos point, int value)
{
    if (q->count < FULL)
    {
        //Creating new node, and adding it to the back of the queue.
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
    // removing the front node in the queue, then assigning to front pointer to the next node.
    if (q->front == NULL){return false;}
    node *tmp = q->front;
    
    q->front = q->front->next;
    if(q->front == NULL){ q->rear = NULL;}
    q->count--;
    free(tmp);
    return true;
}
    // function to print out the queue.
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
// check is the target is reached.
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
// 
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

//function to add mine if one is detected,
bool addMine(Pos mine)
{
    maze[mine.x][mine.y] = -1;
    return true;
}

//switch function to turn the base numbers in actual cordinates.
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
// creating a array representation of the maze.
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
          maze[i][j]  = 0;
        }
    }
}

// Creating vertical Paths.
for (i = 4; i < 9 ; i++){
    if (i == 4 || i == 6 || i == 8){
        for (j = 0; j < 13; j++){
          maze[i][j]  = 0;
        }
    }
}

// Creating In between paths
for (i = 2; i < 11 ; i++){
    if (i == 2 || i == 10){
        for (j = 2; j < 11; j++){
          maze[i][j]  = 0;
        }
    }
}

for (i = 2; i < 11 ; i++){
    if (i == 2 ||i == 10){
        for (j = 2; j < 11; j++){
          maze[i][j]  = 0;
        }
    }
}
}
// checking if cell is within the walls of the maze.
bool CheckCell(int row, int col){

   return (row >= 0) && (row < 13) &&
           (col >= 0) && (col < 13);

}
//function to print the maze.
void PrintMaze(){
     for (int i = 0; i < 13; i++){
        for (int j = 0; j < 13; j++){
             printf( "  %d \t",maze[i][j]);
        }
        printf("\n"); 
        printf("\n");
    }
}
//BFS algorithm.
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
                //When cell is valid and not visited yet, add to queue and mark with
                //distance of current cell+1.

                visited[row][col] = true;
                maze[row][col] = dist+1;
                Pos Neighbour = {row,col};
                enqueue(q, Neighbour, dist+1);
            } 
       }
    } return false;
}

bool scanMap(Pos source){
    // 120 time to scan the entire map for mines, after 120 sec the bot
    // has to be returned and a new mine will be placed in the map.
    // this mine is the 'treasure', the goal is to find this treasure.

    // t links, p rechts, x vooruit.





}
//Using Lee turn find a route back. 

char * RoutePlanner(Pos Source, Pos Goal, int SourceNum)
{
    int dix[4] = {-1,0,0,1};
    int diy[4] = {0,-1,1,0};
    int p;
    int pdx = 0;
    int pdy = 0;
    int direction;


    
        if(SourceNum == 1||SourceNum == 2||SourceNum == 3){
            direction = 1;
        } else if (SourceNum == 4||SourceNum == 5 ||SourceNum == 6){
            direction = 4;
        } else if( SourceNum == 7||SourceNum == 8|| SourceNum ==9){
            direction = 3;
        } else if( SourceNum == 10||SourceNum == 11||SourceNum == 12){
            direction = 2;
        }
            
    
    int count = maze[Source.x][Source.y];
    Pos CurrPos = Source;

    Pos *Route;
    Route = (Pos*)calloc(count, sizeof(Pos));

int i = 0;
while(!TargetReached(Goal,CurrPos)){
       for(p = 0; p < 4; p++)
       {     
            int row = CurrPos.x + dix[p];
            int col = CurrPos.y + diy[p];

            int rowlast = CurrPos.x + pdx;
            int collast = CurrPos.y + pdy;

        if(maze[rowlast][collast] == count-1  && i != 0){
                CurrPos.x += pdx;
                CurrPos.y += pdy;
                Route[i].x = pdx;
                Route[i].y = pdy;
        }
        else if (maze[row][col] == count-1)
            {
                pdx = dix[p];
                pdy = diy[p];
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
int BufferLength = maze[Source.x][Source.y];
char *Buffer;
Buffer = (char*)calloc(maze[Source.x][Source.y], sizeof(char));
static char *RouteLRF;
RouteLRF = (char*)calloc(maze[Source.x][Source.y], sizeof(char));

//a lot of different representations of the route.
for(j =0; j < maze[Source.x][Source.y]; j++)
{

    
    if(Route[j].x == 1){Buffer[j] = 'S';}
     if(Route[j].x == -1){Buffer[j] = 'N';}
      if(Route[j].y == 1){Buffer[j] = 'E';}
       if(Route[j].y == -1){Buffer[j] = 'W';}
}
free(Route);
RouteLRF[0] = 'x';
// t links, p rechts, x vooruit.
RouteCount=0;
for(j = 1; j < maze[Source.x][Source.y]; j++){
    if(j%2 == 0)
    {    
        if (Buffer[j-1] == Buffer[j]){
            RouteLRF[RouteCount] = 'x'; 
        }
        else if(Buffer[j] == 'E' && direction == 1){
            RouteLRF[RouteCount] = 'p';
            direction = 2;
        }
        else if(Buffer[j] == 'W' && direction == 1){
            RouteLRF[RouteCount] = 't';
            direction = 4;
        }
        else if(Buffer[j] == 'S' && direction == 2){
            RouteLRF[RouteCount] = 'p';
            direction = 3;
        }
        else if(Buffer[j] == 'N' && direction == 2){
            RouteLRF[RouteCount] = 't';
            direction = 1;
        }
        else if(Buffer[j] == 'E' && direction == 3){
            RouteLRF[RouteCount] = 't';
            direction = 2;
        }
        else if(Buffer[j] == 'W' && direction == 3){
            RouteLRF[RouteCount] = 'p';
            direction = 4;
        }
        else if(Buffer[j] == 'N' && direction == 4){
            RouteLRF[j] = 'p';
            direction = 1;
        }
        else if(Buffer[j] == 'S' && direction == 4){
            RouteLRF[RouteCount] = 't';
            direction = 3;
        }
        RouteCount++;
    }
            
            
}
free(Buffer);
return RouteLRF;

}
//o is ontvangen



void goTo(int begin, int end){
    Pos Source = BasetoCord(begin); 
    Pos Goal = BasetoCord(end); 
    Algorithm(Source, Goal);
    char *buffer;
    buffer = RoutePlanner(Source,Goal,begin);
    
    for(int i = 0; i <RouteCount; i++){
        printf("%c ", buffer[i]);
    }
    printf("\n");
    //sendBytes(Source,Goal,begin);
}



int main()
{
    int minecount;
    int i;
    CreateMap();
    printf("\n");
    goTo(10,4);
    PrintMaze();
    
 
    return 0;
}