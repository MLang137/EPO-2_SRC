#include <stdio.h>

int maze[13][13];
int startdirection;
int BaseToCords(int Base);
void CreateMap(void);
void MazeAlgorithm(int Source,int Target);

int main(){
CreateMap();
MazeAlgorithm(12,9);
        for (int i = 0; i < 13; i++){
            for (int j = 0; j < 13; j++){
               printf( "  %d \t ",maze[i][j]);
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

void MazeAlgorithm(int Source,int Target){
int *MazeEdit = *maze;
int direction; 
int pos;

int TargetOffset = BaseToCords(Target);
int StartOffset = BaseToCords(Source);

int i = 1;
*(MazeEdit + TargetOffset) = i++;


pos = TargetOffset;
printf("pos: %d \n", pos);
printf("\n");
int up = -13; // +y = offset - 14
int dwn = 13; // -y = offset + 14
int r = 1; // +x = offset + 1
int l = -1;// -x = offset - 1

for (int q; q < 40; q++){ {     // While target is not marked keep looking
if( *(MazeEdit + TargetOffset + up) == 0 ){
  pos += up;
  *(MazeEdit + pos) = i++; 
} else 
if( *(MazeEdit + TargetOffset + dwn) == 0 ){
  pos += dwn ;
  *(MazeEdit + pos) = i++; 
} else
if( *(MazeEdit + TargetOffset + l) == 0 ){
  pos += l;
  *(MazeEdit + pos) = i++; 
} else
if( *(MazeEdit + TargetOffset + r) == 0 ){
  pos += r;
  *(MazeEdit + pos) = i++; 
} 
}
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









