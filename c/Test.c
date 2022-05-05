#include <stdio.h>
#include <stdbool.h>

int maze[13][13];
int BaseToCords(int Base);
void CreateMap(void);
void MazeAlgorithm(int Source,int Target);
void PrintMaze();

int main(){
CreateMap();
MazeAlgorithm(12,4);
PrintMaze();
        
}

void PrintMaze(){
     for (int i = 0; i < 13; i++){
        for (int j = 0; j < 13; j++){
             printf( " %d \t ",maze[i][j]);
        }
        printf("\n");
        printf("\n");
    }
}



int BaseToCords(int Base) {
    int offset;
    // Changing Base numbers to actual map co-ordinates
    switch (Base) {
    case 1:
        offset = 160;

        break;
    case 2:
        offset = 162;
        break;
    case 3:
        offset = 164;
        break;
    case 4:
        offset = 116;
        break;
    case 5:
        offset = 103;
        break;
    case 6:
        offset = 90;
        break;
    case 7:
        offset = 8;
        break;
    case 8:
        offset = 6;
        break;
    case 9:
        offset = 4;
        break;
    case 10:
        offset = 53;
        break;
    case 11:
        offset = 66;
        break;
    case 12:
        offset = 79;
        break;

    default:
        printf("Please input a base number between 1 and 12");
    }
    return offset;
}

bool CheckCell(int row, int col){

   return (row >= 0) && (row < 13) &&
           (col >= 0) && (col < 13);

}


void MazeAlgorithm(int Source,int Target){
int *MazeEdit = *maze; 
int col, row, trow, tcol, nrow, ncol;

int TargetOffset = BaseToCords(Target);
int StartOffset = BaseToCords(Source);

int j;
int i = 1;
int q;
*(MazeEdit + TargetOffset) = i++;

col = TargetOffset % 13;
row = (TargetOffset-col)/13;

int dir[4] = {-1,0,1,0}; 
int dil[4] = {0,-1,0,1}; 

bool foundBranch;

row = 4;
col = 0;
maze[4][0] = 1;

for (q = 0; q < 100; q++){
foundBranch = false;
for(j = 0; j < 4; j++){
trow = row + dir[j];
tcol = col + dil[j];
if(CheckCell(trow,tcol)){
    if (maze[trow][tcol] == 0 ){
        maze[trow][tcol] = i;
        foundBranch = true;
        nrow = trow;
        ncol = tcol;
        printf("Round Check: %d ----> tcol: %d, nCol: %d\n",q,tcol,ncol);

    } 
}
if (j == 3){i++;}
}


if (!foundBranch){
    for(j = 0; j < 4; j++){
        trow = row + dir[j];
        tcol = col + dil[j];
        if (maze[trow][tcol] > 0 && !foundBranch){
            if (maze[trow][tcol] < i){
                row = trow;
                col = tcol;
                i  = maze[trow][tcol];
            } 
         }
    }
}   
printf("Round: %d ----> Row: %d, Col: %d\n",q,row,ncol);
row = nrow;
col = ncol;


}
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









