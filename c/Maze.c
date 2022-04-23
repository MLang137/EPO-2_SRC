#include <stdio.h>

int maze[14][14];
int * BaseToCords(int Base);
void CreateMap(void);
void MazeAlgorithm(int SourceNode, 
int Target);


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

    static int BaseCords[2];
    int* CordEdit = BaseCords;
    // Changing Base numbers to actual map co-ordinates
    switch (Base) {
    case 1:
        *CordEdit = 13;
        *(CordEdit + 1) = 5;
        break;
    case 2:
        *CordEdit = 13;
        *(CordEdit + 1) = 7;
        break;
    case 3:
        *CordEdit = 13;
        *(CordEdit + 1) = 9;
        break;
    case 4:
        *CordEdit = 9;
        *(CordEdit + 1) = 13;
        break;
    case 5:
        *CordEdit = 7;
        *(CordEdit + 1) = 13;
        break;
    case 6:
        *CordEdit = 5;
        *(CordEdit + 1) = 13;
        break;
    case 7:
        *CordEdit = 1;
        *(CordEdit + 1) = 9;
        break;
    case 8:
        *CordEdit = 1;
        *(CordEdit + 1) = 7;
        break;
    case 9:
        *CordEdit = 1;
        *(CordEdit + 1) = 9;
        break;
    case 10:
        *CordEdit = 5;
        *(CordEdit + 1) = 1;
        break;
    case 11:
        *CordEdit = 7;
        *(CordEdit + 1) = 1;
        break;
    case 12:
        *CordEdit = 9;
        *(CordEdit + 1) = 1;
        break;

    default:
        printf("Please input a base number between 1 and 12");
    }
    return BaseCords;
}


MazeAlgorithm(){

printf()







}





CreateMap(){
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









