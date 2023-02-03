#ifndef ENGINE_H
#define ENGINE_H


#include "vect3D.h"
#include <math.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600
#define FRAME_DELAY 7


struct Camera {
    SDL_Rect source;
    int current_scale;
};

typedef struct Camera camera;


void rotate_grid(vect3D** coords,int rows,int columns,int angle);

void draw_grid(SDL_Renderer* renderer,vect3D** coords,int rows,int columns);

int events_handling(camera* cam,vect3D** coords,int rows,int columns);

void main_loop(SDL_Renderer* renderer,vect3D** coords,int rows,int columns);
#endif