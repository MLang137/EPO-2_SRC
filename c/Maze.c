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
bool readByte(HANDLE hSerial, char *buffRead) {

    DWORD dwBytesRead = 0;

    if (!ReadFile(hSerial, buffRead, 1, &dwBytesRead, NULL))
    {
        printf("error reading byte from input buffer \n");
        return false;
    }
    printf("Byte read from read buffer is: %c \n", buffRead[0]);
    return true;
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

bool addMine(Pos mine)
{
    maze[mine.x][mine.y] = -1;
    return true;
}

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

bool scanMap(Pos source){
    // 120 time to scan the entire map for mines, after 120 sec the bot
    // has to be returned and a new mine will be placed in the map.
    // this mine is the 'treasure', the goal is to find this treasure.

    // t links, p rechts, x vooruit.





}
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
for(j = 1; j < maze[Source.x][Source.y]; j++){
        if (Buffer[j-1] == Buffer[j]){
            RouteLRF[j] = 'x'; 
        }
        else if(Buffer[j] == 'E' && direction == 1){
            RouteLRF[j] = 'p';
            direction = 2;
        }
        else if(Buffer[j] == 'W' && direction == 1){
            RouteLRF[j] = 't';
            direction = 4;
        }
        else if(Buffer[j] == 'S' && direction == 2){
            RouteLRF[j] = 'p';
            direction = 3;
        }
        else if(Buffer[j] == 'N' && direction == 2){
            RouteLRF[j] = 't';
            direction = 1;
        }
        else if(Buffer[j] == 'E' && direction == 3){
            RouteLRF[j] = 't';
            direction = 2;
        }
        else if(Buffer[j] == 'W' && direction == 3){
            RouteLRF[j] = 'p';
            direction = 4;
        }
        else if(Buffer[j] == 'N' && direction == 4){
            RouteLRF[j] = 'p';
            direction = 1;
        }
        else if(Buffer[j] == 'S' && direction == 4){
            RouteLRF[j] = 't';
            direction = 3;
        }
}
free(Buffer);
return RouteLRF;

}
//o is ontvangen

void sendBytes(Pos Source){
    int direction;
    int i;
    HANDLE hSerial;


    char byteBuffer[BUFSIZ+1];

    /*----------------------------------------------------------
    // Open COMPORT for reading and writing
    //----------------------------------------------------------*/
    hSerial = CreateFile(COMPORT,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if(hSerial == INVALID_HANDLE_VALUE){
        if(GetLastError()== ERROR_FILE_NOT_FOUND){
            /*serial port does not exist. Inform user.*/
            printf(" serial port does not exist \n");
        }
        /*some other error occurred. Inform user.*/
        printf(" some other error occured. Inform user.\n");
    }

    /*----------------------------------------------------------
    // Initialize the parameters of the COM port
    //----------------------------------------------------------*/

    initSio(hSerial);

    char *buffer;
    buffer = RoutePlanner(BasetoCord(10),BasetoCord(1),10);

    
    
    for(i = 0; i < maze[Source.x][Source.y]; i++){
        printf("%c ",buffer[i]);
    }
    printf("Press Any Key to Continue\n");
    getchar();
        writeByte(hSerial, (buffer+i));
        printf("Press Any Key to Continue\n");
        getchar();
    	readByte(hSerial, )

}


int main()
{
    int minecount;
    int i;
    int SourceBase = 10;
    ;
    Pos Source = BasetoCord(SourceBase); 
    Pos Goal = BasetoCord(1); 
    CreateMap();
    Pos Mines[5] = {{5,4},{6,3},{7,2},{4,6},{5,5}};
    for(i = 0; i < 5; i++){
        addMine(Mines[i]);
    }
    
    Algorithm(Source, Goal);
    printf("\n");
    PrintMaze();
    sendBytes(Source);
 
    return 0;
}