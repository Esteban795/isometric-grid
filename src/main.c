#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600
#define FRAME_DELAY 16


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


struct vect3D {
    float x;
    float y;
    float z;
};

typedef struct vect3D vect3D;


void print_vect3D(vect3D v){
    printf("X : %f, Y : %f, Z : %f\n",v.x,v.y,v.z);
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

void rotate_grid(vect3D** coords,int rows,int columns,int Angle){
	int i, j;
	float x, y, z;
	float a, Rx, Ry, Rz;
	int axis = 0;

	if (Angle == 2){
        axis = 1;
        a = 1 * M_PI / 180;
    } else if (Angle == -2){
        axis = 1;
        a = -1 * M_PI / 180;
    } else {
        a = Angle * M_PI / 180;
    }
		
	float Cx = SCREEN_WIDTH/4.0;
	float Cy = SCREEN_HEIGHT/3.0;

	for (i = 0; i < rows; i++)
		for (j = 0; j < columns; j++)
		{
			if (axis == 0) /* Move left or right */
			{
				x = coords[i][j].x;
				y = coords[i][j].y;
				Rx = (x - Cx) * cos(a) - (y - Cy) * sin(a) + Cx;
				Ry = (x - Cx) * sin(a) + (y - Cy) * cos(a) + Cy;
				coords[i][j].x = Rx;
				coords[i][j].y = Ry;
			}
			else /* Move up or down */
			{
				y = coords[i][j].y;
				z = coords[i][j].z;
				Ry = y * cos(a) - z * sin(a);
				Rz = y * sin(a) + z * cos(a);
				coords[i][j].y = Ry;
				coords[i][j].z = Rz;
			}
		}
}


void draw_grid(SDL_Renderer* renderer,vect3D** coords,int rows,int columns){
	int i, j;
	float x, y, z;
	float xx, yy, XX, YY;
	float x_incl = 0.8, y_incl = 0.7;
	float Cx, Cy;

	Cx = SCREEN_WIDTH / 2.2;
	Cy = SCREEN_HEIGHT / 3.2;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			x = coords[i][j].x;
			y = coords[i][j].y;
			z = coords[i][j].z;
			xx = (x_incl * (x - y)) + Cx;
			yy = ((1 - y_incl) * (x + y) - z) + Cy;

			if (i < rows- 1)
			{
				/* Take out next X coordinate */
				x = coords[i + 1][j].x;
				y = coords[i + 1][j].y;
				z = coords[i + 1][j].z;
				XX = (x_incl * (x - y)) + Cx;
				YY = (1 - y_incl) * (x + y) - z + Cy;
				SDL_RenderDrawLine(renderer, xx, yy, XX, YY);
			}
			if (j < columns - 1)
			{
				/* Take out next Y coordinate */
				x = coords[i][j + 1].x;
				y = coords[i][j + 1].y;
				z = coords[i][j + 1].z;
				YY = ((1 - y_incl) * (x + y) - z) + Cy;
				XX = (x_incl * (x - y)) + Cx;
				SDL_RenderDrawLine(renderer, xx, yy, XX, YY);
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
    FILE* f = fopen("test3.txt","r");
    vect3D** coords = read_file(f,&rows,&columns);
    printf("Rows : %d, Columns : %d\n",rows,columns);
    fclose(f);
    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns; j++){
            print_vect3D(coords[i][j]);
        }
    }
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"test");
    if (status == 1) return EXIT_FAILURE;
    int running = 1;
    SDL_Event e;
    while (running){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_q:
                    running = 0;
                    break;
                case SDLK_UP:
                    rotate_grid(coords,rows,columns,2);
                    break;
                case SDLK_LEFT:
                    rotate_grid(coords,rows,columns,1);
                    break;
                case SDLK_DOWN:
                    rotate_grid(coords,rows,columns,-2);
                    break;
                case SDLK_RIGHT:
                    rotate_grid(coords,rows,columns,-1);
                    break;
                default:
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer,0,255,255,255);
        draw_grid(renderer,coords,rows,columns);
        SDL_RenderPresent(renderer);
        SDL_Delay(FRAME_DELAY);
    }
    for (int i = 0; i < rows;i++){
        free(coords[i]);
    }
    free(coords);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

//gcc main.c -o main -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm