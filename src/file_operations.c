#include "../include/file_operations.h"

int count_lines_columns(FILE* f,int* columns){
    int count = 1;
    int nb_columns = 1;
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
    rewind(f);
    return count;
}

vect3D** read_file(FILE* f,int* rows,int* columns){
    *rows = count_lines_columns(f,columns);
    rewind(f);
    vect3D** coords = malloc(sizeof(vect3D*) * *rows);
    for (int i = 0; i < *rows; i++){
        coords[i] = malloc(sizeof(vect3D) * *columns);
        float temp;
        for (int j = 0;j < *columns;j++){
            fscanf(f,"%f ",&temp);
            coords[i][j].z = temp;
            coords[i][j].x = (SCREEN_WIDTH / (*columns + 8)) * (i + 1);
            coords[i][j].y = (SCREEN_HEIGHT / (*rows + 3)) * (j + 1);
        }
        fscanf(f,"\n");
    }
    return coords;
}