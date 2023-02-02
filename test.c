#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define BUFFER_SIZE 65536


struct vect3D {
    int x;
    int y;
    int z;
};

typedef struct vect3D vect3D;


int count_lines_columns(FILE* f,int* columns){
    int count = 1;
    int nb_columns = 0;
    char chr = getc(f);
    int done = 0;
    while (chr != EOF){
        if (chr == '\n'){ 
            count++;
            done = 1;
        }
        if (chr == ' ' && !done) nb_columns++;
        chr = getc(f);
    }
    *columns = nb_columns;
    return count;
}

void print_vect3D(vect3D v){
    printf("X : %d, Y : %d, Z : %d\n",v.x,v.y,v.z);
}


int main(void){
    FILE* f = fopen("test2.txt","r");
    int columns;
    int rows = count_lines_columns(f,&columns);
    printf("Rows : %d, Columns : %d\n",rows,columns);
    vect3D** coords = malloc(sizeof(vect3D*) * rows);
    rewind(f);
    int temp1,temp2,temp3;
    for (int i = 0; i < rows;i++){
        coords[i] = malloc(columns * sizeof(vect3D));
        for (int j = 0; j < columns;j++){
            fscanf(f,"%d %d %d",&temp1,&temp2,&temp3);
            coords[i][j].x = temp1;
            coords[i][j].y = temp2;
            coords[i][j].z = temp3;
        }
    }
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns; j++){
            print_vect3D(coords[i][j]);
        }
    }
    for (int i = 0; i < rows;i++){
        free(coords[i]);
    }
    free(coords);
    return 0;
}



//gcc test.c -o test -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2