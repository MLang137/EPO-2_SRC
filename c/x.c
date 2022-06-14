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
            return 'N';
        } else if (SourceNum == 4||SourceNum == 5 ||SourceNum == 6){
            return 'W';
        } else if( SourceNum == 7||SourceNum == 8|| SourceNum ==9){
            return 'S';
        } else if( SourceNum == 10||SourceNum == 11||SourceNum == 12){
            return 'E';
        }

}
route * RoutePlanner(Pos Source, Pos Goal, char Direction)
{
    int dix[4] = {-1,0,0,1};
    int diy[4] = {0,-1,1,0};
    int p;
    int pdx = 0;
    int pdy = 0;
    //PrintMaze();

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
RouteLRF = (route*)calloc(mazecopy[Source.x][Source.y],sizeof(route));

//a lot of different representations of the route.
for(j =0; j < mazecopy[Source.x][Source.y]; j++)
{


    if(Route[j].x == 1){Buffer[j] = 'S';}
     if(Route[j].x == -1){Buffer[j] = 'N';}
      if(Route[j].y == 1){Buffer[j] = 'E';}
       if(Route[j].y == -1){Buffer[j] = 'W';}
}
if(Buffer[0] == Direction){RouteLRF[0].instruction = 'x';}
else if (Buffer[0] == 'N' && Direction == 'E'){RouteLRF[0].instruction = 't';}
else if (Buffer[0] == 'N' && Direction == 'W'){RouteLRF[0].instruction = 'p';}
else if (Buffer[0] == 'E' && Direction == 'N'){RouteLRF[0].instruction = 'p';}
else if (Buffer[0] == 'E' && Direction == 'S'){RouteLRF[0].instruction = 't';}
else if (Buffer[0] == 'S' && Direction == 'E'){RouteLRF[0].instruction = 'p';}
else if (Buffer[0] == 'S' && Direction == 'W'){RouteLRF[0].instruction = 't';}
else if (Buffer[0] == 'W' && Direction == 'N'){RouteLRF[0].instruction = 't';}
else if (Buffer[0] == 'W' && Direction == 'S'){RouteLRF[0].instruction = 'p';}
RouteLRF[0].direction = Buffer[0];
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
        else if(Buffer[j] == 'E' && RouteLRF[RouteCount-1].direction == 'N'){
            RouteLRF[RouteCount].instruction = 'p';
            RouteLRF[RouteCount].direction = 'E';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;

        }
        else if(Buffer[j] == 'W' && RouteLRF[RouteCount-1].direction  == 'N'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'W';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'S' && RouteLRF[RouteCount-1].direction  == 'E'){
            RouteLRF[RouteCount].instruction = 'p';
            RouteLRF[RouteCount].direction = 'S';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'N' && RouteLRF[RouteCount-1].direction  == 'E'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'N';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'E' && RouteLRF[RouteCount-1].direction  == 'S'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'E';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'W' && RouteLRF[RouteCount-1].direction  == 'S'){
            RouteLRF[RouteCount].instruction = 'p';
            RouteLRF[RouteCount].direction = 'W';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'N' && RouteLRF[RouteCount-1].direction  == 'W'){
            RouteLRF[j].instruction = 'p';
            RouteLRF[RouteCount].direction = 'N';
            RouteLRF[RouteCount].cords.x = CurrPos.x;
            RouteLRF[RouteCount].cords.y = CurrPos.y;
        }
        else if(Buffer[j] == 'S' && RouteLRF[RouteCount-1].direction == 'W'){
            RouteLRF[RouteCount].instruction = 't';
            RouteLRF[RouteCount].direction = 'S';
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
             if(old == 'N'){return 'S';}
        else if(old == 'E'){return 'W';}
        else if(old == 'S'){return 'N';}
        else if(old == 'W'){return 'E';}
}

int sendRoute(Pos Goal,Pos Source, int begin)
{
    static HANDLE hSerial;
    static route *route;
    route = RoutePlanner(Source,Goal,StartDirection(begin));
    bool mineFlag = false;
    int minePos;
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

    /*printf("Press any key to start the navigation.\n");
    getchar();*/
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
        addMine((Pos){(route[i].cords.x),(route[i].cords.y)});
        Algorithm(route[i-1].cords,Goal);
        printf("New route\n");

        char newdirection = flipDirection(route[i].direction);
        Pos lastlocation = route[i-1].cords;
        printf("new direction : %c, newcord : x: %d, y: %d\n",newdirection,lastlocation.x,lastlocation.y);
        route = RoutePlanner(lastlocation, Goal, newdirection);
        i = 1;
        while(readByte(hSerial) == '?')
        {

        }
         writeByte(hSerial,&(route)->instruction);
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


    sendRoute(Goal,Source,begin);

}


int main()
{
    // goTo(6,7); replaced by three stations
    int S_nr;
    int G1_nr;
    int G2_nr;
    int G3_nr;

    printf("Welcome to the \"MAZE ROUTER\" C program for EPO-2 --- challenge A & B!\n");
    printf("Made by group B4.2, April-June 2022\n\n");
	printf("Give source nr, goal 1 nr, goal 2 nr, goal 3 nr:\n");
	scanf("%d %d %d %d", &S_nr, &G1_nr, &G2_nr, &G3_nr);

	Pos Source = BasetoCord(S_nr);
	Pos Goal_1 = BasetoCord(G1_nr);
	Pos Goal_2 = BasetoCord(G2_nr);
	Pos Goal_3 = BasetoCord(G3_nr);

	/******************************************************************************************************************
     * Calculate shortest route to visit three goals
     *****************************************************************************************************************/

    int length_123 = 0, length_132 = 0, length_213 = 0, length_231 = 0, length_312 = 0, length_321 = 0;
    int ordered_goals_nrs[3] = {G1_nr, G2_nr, G3_nr}; // will be adjusted later if necessary

    // Route S->G1->G2->G3
    static route *route;
    Algorithm(Source, Goal_1);
    route = RoutePlanner(Source, Goal_1, StartDirection(S_nr));
    length_123 += mazecopy[Source.x][Source.y];
    Algorithm(Goal_1, Goal_2);
    route = RoutePlanner(Goal_1, Goal_2, StartDirection(G1_nr));
    length_123 += mazecopy[Goal_1.x][Goal_1.y];
    Algorithm(Goal_2, Goal_3);
    route = RoutePlanner(Goal_2, Goal_3, StartDirection(G2_nr));
    length_123 += mazecopy[Goal_2.x][Goal_2.y];

    // Route S->G1->G3->G2
    Algorithm(Source, Goal_1);
    route = RoutePlanner(Source, Goal_1, StartDirection(S_nr));
    length_132 += mazecopy[Source.x][Source.y];
    Algorithm(Goal_1, Goal_3);
    route = RoutePlanner(Goal_1, Goal_3, StartDirection(G1_nr));
    length_132 += mazecopy[Goal_1.x][Goal_1.y];
    Algorithm(Goal_3, Goal_2);
    route = RoutePlanner(Goal_3, Goal_2, StartDirection(G3_nr));
    length_132 += mazecopy[Goal_3.x][Goal_3.y];

    // Route S->G2->G1->G3
    Algorithm(Source, Goal_2);
    route = RoutePlanner(Source, Goal_2, StartDirection(S_nr));
    length_213 += mazecopy[Source.x][Source.y];
    Algorithm(Goal_2, Goal_1);
    route = RoutePlanner(Goal_2, Goal_1, StartDirection(G2_nr));
    length_213 += mazecopy[Goal_2.x][Goal_2.y];
    Algorithm(Goal_1, Goal_3);
    route = RoutePlanner(Goal_1, Goal_3, StartDirection(G1_nr));
    length_213 += mazecopy[Goal_1.x][Goal_1.y];

    // Route S->G2->G3->G1
    Algorithm(Source, Goal_2);
    route = RoutePlanner(Source, Goal_2, StartDirection(S_nr));
    length_231 += mazecopy[Source.x][Source.y];
    Algorithm(Goal_2, Goal_3);
    route = RoutePlanner(Goal_2, Goal_3, StartDirection(G2_nr));
    length_231 += mazecopy[Goal_2.x][Goal_2.y];
    Algorithm(Goal_3, Goal_1);
    route = RoutePlanner(Goal_3, Goal_1, StartDirection(G3_nr));
    length_231 += mazecopy[Goal_3.x][Goal_3.y];

    // Route S->G3->G1->G2
    Algorithm(Source, Goal_3);
    route = RoutePlanner(Source, Goal_3, StartDirection(S_nr));
    length_312 += mazecopy[Source.x][Source.y];
    Algorithm(Goal_3, Goal_1);
    route = RoutePlanner(Goal_3, Goal_1, StartDirection(G3_nr));
    length_312 += mazecopy[Goal_3.x][Goal_3.y];
    Algorithm(Goal_1, Goal_2);
    route = RoutePlanner(Goal_1, Goal_2, StartDirection(G1_nr));
    length_312 += mazecopy[Goal_1.x][Goal_1.y];

    // Route S->G3->G2->G1
    Algorithm(Source, Goal_3);
    route = RoutePlanner(Source, Goal_3, StartDirection(S_nr));
    length_321 += mazecopy[Source.x][Source.y];
    Algorithm(Goal_3, Goal_2);
    route = RoutePlanner(Goal_3, Goal_2, StartDirection(G3_nr));
    length_321 += mazecopy[Goal_3.x][Goal_3.y];
    Algorithm(Goal_2, Goal_1);
    route = RoutePlanner(Goal_2, Goal_1, StartDirection(G2_nr));
    length_321 += mazecopy[Goal_2.x][Goal_2.y];

    /* Print total route lengths
    printf("length123: %d\n", length_123);
    printf("length132: %d\n", length_132);
    printf("length213: %d\n", length_213);
    printf("length231: %d\n", length_231);
    printf("length312: %d\n", length_312);
    printf("length321: %d\n", length_321);
    */

    if (length_132 <= length_123 && length_132 <= length_213 && length_132 <= length_231 &&
        length_132 <= length_312 && length_132 <= length_321)
    {
        ordered_goals_nrs[1] = G3_nr;
        ordered_goals_nrs[2] = G2_nr;
    }
    else if (length_213 <= length_123 && length_213 <= length_132 && length_213 <= length_231 &&
        length_213 <= length_312 && length_213 <= length_321)
    {
        ordered_goals_nrs[0] = G2_nr;
        ordered_goals_nrs[1] = G1_nr;
    }
    else if (length_231 <= length_123 && length_231 <= length_132 && length_231 <= length_213 &&
        length_231 <= length_312 && length_231 <= length_321)
    {
        ordered_goals_nrs[0] = G2_nr;
        ordered_goals_nrs[1] = G3_nr;
        ordered_goals_nrs[2] = G1_nr;
    }
    else if (length_312 <= length_123 && length_312 <= length_132 && length_312 <= length_213 &&
        length_312 <= length_231 && length_312 <= length_321)
    {
        ordered_goals_nrs[0] = G3_nr;
        ordered_goals_nrs[1] = G1_nr;
        ordered_goals_nrs[2] = G2_nr;
    }
    else if (length_321 <= length_123 && length_321 <= length_132 && length_321 <= length_213 &&
        length_321 <= length_231 && length_321 <= length_312)
    {
        ordered_goals_nrs[0] = G3_nr;
        ordered_goals_nrs[2] = G1_nr;
    }
    // Goal nrs are put in the correct order now.
    printf("Fastest route: %d -> %d -> %d -> %d\n",
           S_nr, ordered_goals_nrs[0], ordered_goals_nrs[1], ordered_goals_nrs[2]);
    // Go to the goals in the correct order (and do not wait for any character in between routes).
    printf("Press ENTER to start autonomous navigation!\n");
    getchar(); // apparently twice is necessary
    getchar();
    goTo(S_nr, ordered_goals_nrs[0]);
    PrintMaze();
    goTo(ordered_goals_nrs[0], ordered_goals_nrs[1]);
    PrintMaze();
    goTo(ordered_goals_nrs[1], ordered_goals_nrs[2]);
    PrintMaze();
    // Send a stop command to the robot.
    char *stop;
    stop = "z";
    HANDLE hSerial = CreateFile(COMPORT,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    writeByte(hSerial, stop);
    printf("Challenge complete.");
    return 0;
}
