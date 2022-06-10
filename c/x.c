#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#define FULL 100
#define COMPORT "COM9"
#define BAUDRATE CBR_9600

int maze[13][13] =
{
    {-1,-1,-1,-1,0,-1,0,-1,0,-1,-1,-1,-1}, //1
    {-1,-1,-1,-1,0,-1,0,-1,0,-1,-1,-1,-1}, //2
    {-1,-1, 0, 0,0, 0,0, 0,0, 0, 0,-1,-1}, //3
    {-1,-1, 0,-1,0,-1,0,-1,0,-1, 0,-1,-1}, //4
    { 0, 0, 0, 0,0, 0,0 ,0,0, 0, 0, 0, 0}, //5
    {-1,-1, 0,-1,0,-1,0,-1,0,-1, 0,-1,-1}, //6
    { 0, 0, 0, 0,0, 0,0 ,0,0, 0, 0, 0, 0}, //7
    {-1,-1, 0,-1,0,-1,0,-1,0,-1, 0,-1,-1}, //8
    { 0, 0, 0, 0,0, 0,0 ,0,0, 0, 0, 0, 0}, //9
    {-1,-1, 0,-1,0,-1,0,-1,0,-1, 0,-1,-1}, //10
    {-1,-1, 0, 0,0, 0,0 ,0,0 ,0, 0,-1,-1}, //11
    {-1,-1,-1,-1,0,-1,0,-1,0,-1,-1,-1,-1}, //12
    {-1,-1,-1,-1,0,-1,0,-1,0,-1,-1,-1,-1}, //13
    //1, 2, 3, 4,5, 6,7, 8,9,10,11,12,13
};
int mazecopy[13][13];
int RouteCount;  
int runtimes = 0;

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



//defining structs for easy point representation. 
struct Pos
{
    int x;
    int y;
}; typedef struct Pos Pos;

struct route
{
    char instruction;
    char direction;
    Pos cords;
}; typedef struct route route;
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

// checking if cell is within the walls of the maze.
bool CheckCell(int row, int col){

   return (row >= 0) && (row < 13) &&
           (col >= 0) && (col < 13);

}
void copyMaze()
{
    for (int x=0 ; x < 13; x++)
        {
            for (int y=0 ; y < 13; y++)
                {
                     mazecopy[x][y] = maze[x][y];
                }
        }
}

//function to print the maze.
void PrintMaze(){
     for (int i = 0; i < 13; i++){
        for (int j = 0; j < 13; j++){
             printf( " %d \t",mazecopy[i][j]);
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
    
    copyMaze();

    //Creating visited map
    bool visited[13][13];
    memset(visited, false, sizeof(visited));
    visited[goal.x][goal.y] = true;
    //Marking goal cell with 1.
    mazecopy[goal.x][goal.y] = 1;

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

            if(CheckCell(row,col) && mazecopy[row][col] == 0 && !visited[row][col])
            {
                //When cell is valid and not visited yet, add to queue and mark with
                //distance of current cell+1.

                visited[row][col] = true;
                mazecopy[row][col] = dist+1;
                Pos Neighbour = {row,col};
                enqueue(q, Neighbour, dist+1);
            } 
       }
    } return false;
}

char StartDirection(int SourceNum)
{
        
        if(SourceNum == 1||SourceNum == 2||SourceNum == 3){
            return 'n';
        } else if (SourceNum == 4||SourceNum == 5 ||SourceNum == 6){
            return 'w';
        } else if( SourceNum == 7||SourceNum == 8|| SourceNum ==9){
            return 's';
        } else if( SourceNum == 10||SourceNum == 11||SourceNum == 12){
            return 'e';
        }
    
}
route * RoutePlanner(Pos Source, Pos Goal, char Direction)
{
    int dix[4] = {-1,0,0,1};
    int diy[4] = {0,-1,1,0};
    int p;
    int pdx = 0;
    int pdy = 0;



    int count = mazecopy[Source.x][Source.y];
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

        if(mazecopy[rowlast][collast] == count-1  && i != 0){
                CurrPos.x += pdx;
                CurrPos.y += pdy;
                Route[i].x = pdx;
                Route[i].y = pdy;
        }
        else if (mazecopy[row][col] == count-1)
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
int BufferLength = mazecopy[Source.x][Source.y];
char *Buffer;
Buffer = (char*)calloc(mazecopy[Source.x][Source.y], sizeof(char));
static route *RouteLRF;
RouteLRF = (route*)malloc(mazecopy[Source.x][Source.y]*sizeof(route));

//a lot of different representations of the route.
for(j =0; j < mazecopy[Source.x][Source.y]; j++)
{

    
    if(Route[j].x == 1){Buffer[j] = 'S';}
     if(Route[j].x == -1){Buffer[j] = 'N';}
      if(Route[j].y == 1){Buffer[j] = 'E';}
       if(Route[j].y == -1){Buffer[j] = 'W';}
}

RouteLRF[0].instruction = 'x';
RouteLRF[0].direction = Direction;
RouteLRF[0].cords = Source;
// t links, p rechts, x vooruit.
RouteCount=1;
CurrPos = Source;
CurrPos.x += Route[0].x;
CurrPos.y += Route[0].y;
for(j = 1; j < mazecopy[Source.x][Source.y]; j++){
   // if(j%2 == 0)
   // {    
        if (Buffer[j-1] == Buffer[j]){
            RouteLRF[RouteCount].instruction = 'x'; 
            RouteLRF[RouteCount].direction = RouteLRF[RouteCount-1].direction;
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;

        }
        else if(Buffer[j] == 'E' && RouteLRF[RouteCount-1].direction == 'n'){
            RouteLRF[RouteCount].instruction = 'p';
            RouteLRF[RouteCount].direction = 'e';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;

        }
        else if(Buffer[j] == 'W' && RouteLRF[RouteCount-1].direction  == 'n'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'w';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'S' && RouteLRF[RouteCount-1].direction  == 'e'){
            RouteLRF[RouteCount].instruction = 'p';
            RouteLRF[RouteCount].direction = 's';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'N' && RouteLRF[RouteCount-1].direction  == 'e'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'n';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'E' && RouteLRF[RouteCount-1].direction  == 's'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'e';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'W' && RouteLRF[RouteCount-1].direction  == 's'){
            RouteLRF[RouteCount].instruction = 'p';
            RouteLRF[RouteCount].direction = 'w';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'N' && RouteLRF[RouteCount-1].direction  == 'w'){
            RouteLRF[j].instruction = 'p';
            RouteLRF[RouteCount].direction = 'n';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'S' && RouteLRF[RouteCount-1].direction == 'w'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 's';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        RouteCount++;
  // }
        CurrPos.x += Route[j].x;
        CurrPos.y += Route[j].y;
            
            
}

RouteCount--;
return RouteLRF;

}
//o is ontvangen
char flipDirection(char old)
{
             if(old == 'n'){return 's';}
        else if(old == 'e'){return 'w';}
        else if(old == 's'){return 'n';}
        else if(old == 'w'){return 'e';}
}

int sendRoute(route *firstroute,Pos Goal)
{
    static HANDLE hSerial;
    route *route = firstroute;
    bool mineFlag = false;
    int minePos;
    int dx = 0;
    int dy = 0;
    int i = 0;
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
    //Printing all instructions before sending.
    for(int i = 0; i<RouteCount; i++)
    {
        printf("Instruction %d : %c, direction %c, x: %d, y: %d\n",
        i+1,route[i].instruction,route[i].direction,route[i].cords.x,route[i].cords.y);
    }
         char byteBuffer[BUFSIZ+1];

    printf("Press any key to start the navigation.\n");
    getchar();
    writeByte(hSerial,&route->instruction); 
    Sleep(20);
    writeByte(hSerial,&(route+1)->instruction); 
     while(readByte(hSerial) =='o')
                   {

                   }
    for(i = 2; i < RouteCount; i++)
    {
        Sleep(150);
        writeByte(hSerial,&(route+i)->instruction);
        
        while(readByte(hSerial) == '/')
        {
            
        }  
            if(readByte(hSerial) == '?'){
            printf("Mine Detected\n");
            Sleep(750);
            if(route[i].direction == 'n'){dx = -1;}
        else if(route[i].direction == 'e'){dy = 1;}
        else if(route[i].direction == 's'){dx = 1;}
        else if(route[i].direction == 'w'){dy = -1;}
        
        addMine((Pos){(route[minePos].cords.x+dx),(route[i].cords.y+dy)});
        Algorithm(route[minePos].cords,Goal);
        route =  RoutePlanner( (Pos)route[minePos].cords, Goal, 
                   flipDirection((char)route[minePos].direction));
        for(int i = 0; i<RouteCount; i++)
            {
                printf("Instruction %d : %c, direction %c, x: %d, y: %d\n",
                i+1,route[i].instruction,route[i].direction,route[i].cords.x,route[i].cords.y);
            }
                   continue;
        }   
                   
                   while(readByte(hSerial) =='o')
                   {

                   }
        
        
    }
     

    CloseHandle(hSerial);
    return 0;
}


void goTo(int begin, int end){
    Pos Source =  BasetoCord(begin);
    Pos Goal  = BasetoCord(end);
    Algorithm(Source,Goal);
    static route *route;
    route = RoutePlanner(Source,Goal,StartDirection(begin));

    PrintMaze();

    sendRoute(route,Goal);
  
}


int main()
{
    goTo(7,9);
    return 0;
}