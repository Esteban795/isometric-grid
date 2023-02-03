#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

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
        if (chr == ';' && !done) nb_columns++;
        chr = getc(f);
    }
    *columns = nb_columns;
    rewind(f);
    return count;
}


struct vect3D {
    int x;
    int y;
    int z;
};

typedef struct vect3D vect3D;


void print_vect3D(vect3D v){
    printf("X : %d, Y : %d, Z : %d\n",v.x,v.y,v.z);
}


vect3D** read_file(FILE* f,int* rows,int* columns){
    *rows = count_lines_columns(f,columns);
    rewind(f);
    vect3D** coords = malloc(sizeof(vect3D*) * *rows);
    for (int i = 0; i < *rows; i++){
        coords[i] = malloc(sizeof(vect3D) * *columns);
        int temp;
        for (int j = 0;j < *columns;j++){
            fscanf(f,"%d;",&temp);
            coords[i][j].z = temp;
            coords[i][j].x = (SCREEN_WIDTH / (*columns + 8)) * (i + 1);
            coords[i][j].y = (SCREEN_HEIGHT / (*rows + 3)) * (j + 1);
        }
        fscanf(f,"\n");
    }
    return coords;
}

void rotate_grid(vect3D** coords,int rows,int columns,int angle){
    float x,y,z,a,Rx,Ry,Rz;
    int axis = 0;
    switch (angle)
    {
    case 2:
        axis = 1;
        a = 1 * M_PI /180;
        break;
    case -2:
        axis = 1;
        a = -1 * M_PI /180;
        break;
    default:
        break;
    }
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns;j++){
            if (axis == 0){ //we're moving right or left
                x = coords[i][j].x;
                y = coords[i][j].y;
                Rx = x * cos(a) - y * sin(a);
                Ry = x * sin(a) + y * cos(a);
                coords[i][j].x = Rx;
                coords[i][j].y = Ry;
            } else {
                z = coords[i][j].z;
                y = coords[i][j].y;
                Ry = y * cos(a) - z * sin(a);
                Rz = y * sin(a) + z * cos(a);
                coords[i][j].z = Rz;
                coords[i][j].y = Ry;
            }
        }
    }
}


void draw_grid(SDL_Renderer* renderer,vect3D** coords,int rows,int columns){
    float x_incl = 0.8, y_incl = 0.7;
    int x,y,z;
    float xx,yy,XX,YY;

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns;j++){
            x = coords[i][j].x;
            y = coords[i][j].y;
            z = coords[i][j].z;
            xx = x_incl * (x - y);
            yy = (1 - y_incl) * (x + y) - z;
            if (i < rows - 1){
                x = coords[i + 1][j].x;
                y = coords[i + 1][j].y;
                z = coords[i + 1][j].z;
                XX = x_incl * (x - y);
                YY = (1 - y_incl) * (x + y) - z;
                SDL_RenderDrawLine(renderer,xx,yy,XX,YY);
            }
            if (j < columns - 1){
				x = coords[i][j + 1].x;
				y = coords[i][j + 1].y;
				z = coords[i][j + 1].z;
                XX = x_incl * (x - y);
                YY = (1 - y_incl) * (x + y) - z;
                SDL_RenderDrawLine(renderer,xx,yy,XX,YY);
            }
        }
    }
}

int start_SDL(SDL_Window** window,SDL_Renderer** renderer,int width,int height, const char* title){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    *window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if (*window == NULL) return 1;
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) return 1;
    return 0;
}


int main(int argc, char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int rows;
    int columns;
    FILE* f = fopen("test.txt","r");
    vect3D** coords = read_file(f,&rows,&columns);
    printf("Rows : %d, Columns : %d\n",rows,columns);
    fclose(f);
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns; j++){
            print_vect3D(coords[i][j]);
        }
    }
    /*
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    int running = 1;
    SDL_Event e;
    while (running){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_q:
                    running = 0;
                    break;
                case SDLK_w:
                    rotate_grid(coords,rows,columns,2);
                    break;
                case SDLK_a:
                    rotate_grid(coords,rows,columns,1);
                    break;
                case SDLK_s:
                    rotate_grid(coords,rows,columns,-2);
                    break;
                case SDLK_d:
                    rotate_grid(coords,rows,columns,-1);
                    break;
                default:
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        draw_grid(renderer,coords,rows,columns);
        SDL_RenderPresent(renderer);
    }
    */
   if (coords != NULL){
   for (int i = 0; i < rows;i++){
    free(coords[i]);
   }
   free(coords);
   }

    return 0;
}

//gcc main.c -o main -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm