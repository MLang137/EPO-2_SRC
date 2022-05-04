#include <stdio.h>

int maze[14][14];
int startdirection;
int * BaseToCords(int Base);
void CreateMap(void);
void MazeAlgorithm(int SourceNode, 
int Target;


int main(){
CreateMap();
        for (int i = 1; i <= 13; i++){
            for (int j = 1; j <= 13; j++){
               printf( "%d\t",maze[i][j]);
            }
            printf("\n");
            printf("\n");
        }
}

int* BaseToCords(int Base) {
    int offset;
    // Changing Base numbers to actual map co-ordinates
    switch (Base) {
    case 1:
        offset = 187;
        startdirection = 1;
        break;
    case 2:
        offset = 189;
        startdirection = 1;
        break;
    case 3:
        offset = 191;
        startdirection = 1;
        break;
    case 4:
        offset = 140;
        startdirection = 4;
        break;
    case 5:
        offset = 112;
        startdirection = 4;
        break;
    case 6:
        offset = 84;
        startdirection = 4;
        break;
    case 7:
        offset = 24;
        startdirection = 3;
        break;
    case 8:
        offset = 22;
        startdirection = 3;
        break;
    case 9:
        offset = 20;
        startdirection = 3;
        break;
    case 10:
        offset = 71;
        startdirection = 2;
        break;
    case 11:
        offset = 99;
        startdirection = 2;
        break;
    case 12:
        offset = 127;
        startdirection = 2;
        break;

    default:
        printf("Please input a base number between 1 and 12");
    }
    return offset;
}


}
void MazeAlgorithm(int Target, int Source){
int *MazeEdit = *maze;
bool edge = true;
int direction; 

int TargetOffset = BaseToCords(Target);
int StartOffset = BaseToCords(Source);

*(MazeEdit + TargetOffset) = 1;
int i = 1;

pos = TargetOffset;
int Yup = -14; // +y = offset - 14
int Ydown = 14; // -y = offset + 14
int Xup = 1; // +x = offset + 1
int Xdown = -1;// -x = offset - 1

if (startdirection = 1){
    pos += Yup
    *(MazeEdit + pos) = ++i;
    pos += Yup
    *(MazeEdit + pos) = ++i;  

}
if (startdirection = 2){
    pos += Xdown
    *(MazeEdit + pos) = ++i; 
    pos += Xdown
    *(MazeEdit + pos) = ++i; 
}
if (startdirection = 3){
    pos += Ydown
    *(MazeEdit + pos) = ++i; 
    pos += Ydown
    *(MazeEdit + pos) = ++i; 
}
if (startdirection == 4){
    pos += Xup
    *(MazeEdit + pos) = ++i; 
        pos += Xup
    *(MazeEdit + pos) = ++i; 
}

while(*(MazeEdit + StartOffset) == 0) {
if(*(MazeEdit + TargetOffset + Yup) = 0 ){
  
} 
}


}



void CreateMap(void){
int i,j;
// Creating Map Full of -1
for (i = 1; i <= 13; i++){
    for (j = 1; j <= 13; j++){
        maze[i][j] = -1;
    }
}
// Creating horizontal Paths.
for (i = 5; i <= 10 ; i++){
    if (i == 5 || i == 7 || i == 9){
        for (j = 0; j <= 13; j++){
          maze[i][j] = 0;
        }
    }
}

// Creating vertical Paths.
for (i = 5; i <= 10 ; i++){
    if (i == 5 || i == 7 || i == 9){
        for (j = 1; j <= 13; j++){
          maze[j][i] = 0;
        }
    }
}

// Creating In between paths
for (i = 3; i <= 11 ; i++){
    if (i == 3 || i == 11){
        for (j = 3; j <= 11; j++){
          maze[j][i] = 0;
        }
    }
}

for (i = 3; i <= 11 ; i++){
    if (i == 3 ||i == 11){
        for (j = 3; j <= 11; j++){
          maze[i][j] = 0;
        }
    }
}
}









