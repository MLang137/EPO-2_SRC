#include <stdio.h>
#include <stdbool.h>

int maze[13][13];
void CreateMap(void);
void PrintMaze(void);

int main(){
int i, trow, tcol;
int row,col = 6;
CreateMap();
int dir[4] = {-1,0,1,0}; 
int dil[4] = {0,-1,0,1}; 
maze[6][6] = 5;
row = 6;
col = 6;
for(i = 0; i < 4; i++){
trow = row + dir[i];
tcol = col + dil[i];
printf("row: %d \n", row);
    maze[trow][tcol] = 3;

}


if (!foundBranch ){
    printf("BACKTRACKING\n");
    for(j = 0; j < 4; j++){
        trow = row + dir[j];
        tcol = col + dil[j];
        if (maze[trow][tcol] > 0){
            if (maze[trow][tcol] < i && CheckCell(trow,tcol)){
                row = trow;
                col = tcol;
                i  = maze[trow][tcol];
            } 
         }
    }
}
PrintMaze();


}

void PrintMaze(void){
     for (int i = 0; i < 13; i++){
        for (int j = 0; j < 13; j++){
             printf( " %d \t ",maze[i][j]);
        }
        printf("\n");
        printf("\n");
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









