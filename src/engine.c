#include "../include/engine.h"
#include <stdbool.h>


void rotate_grid(vect3D** coords,int rows,int columns,int angle){
	float x, y, z;
	float a, Rx, Ry, Rz;
	int axis = 0;
    float Cx = SCREEN_WIDTH/4.0; //to make sure we stay in the screen as much as possible
	float Cy = SCREEN_HEIGHT/3.0;
    switch (angle) {
    case 2:
        axis = 1;
        a = M_PI / 180;
        break;
    case -2:
        axis = 1;
        a = -M_PI / 180;
        break;
    default:
        a = angle * M_PI / 180;
        break;
    }
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			if (axis == 0) { //right or left rotation
				x = coords[i][j].x;
				y = coords[i][j].y;
				Rx = (x - Cx) * cos(a) - (y - Cy) * sin(a) + Cx;
				Ry = (x - Cx) * sin(a) + (y - Cy) * cos(a) + Cy;
				coords[i][j].x = Rx;
				coords[i][j].y = Ry;
			} else {  //top or down rotation
				y = coords[i][j].y;
				z = coords[i][j].z;
				Ry = y * cos(a) - z * sin(a);
				Rz = y * sin(a) + z * cos(a);
				coords[i][j].y = Ry;
				coords[i][j].z = Rz;
			}
		}
    }
}


void draw_grid(SDL_Renderer* renderer,vect3D** coords,int rows,int columns){
	float x, y, z;
	float real_x, real_y, neighbor_x, neighbor_y;
    float x_incl = 0.8, y_incl = 0.7;
	float Cx = SCREEN_WIDTH / 2.2;
	float Cy = SCREEN_HEIGHT / 3.2;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			x = coords[i][j].x;
			y = coords[i][j].y;
			z = coords[i][j].z;
			real_x = (x_incl * (x - y)) + Cx;
			real_y = ((1 - y_incl) * (x + y) - z) + Cy;
			if (i < rows- 1){
                //Drawing a line between our current point and its neighbor according to X coordinate
				x = coords[i + 1][j].x;
				y = coords[i + 1][j].y;
				z = coords[i + 1][j].z;
				neighbor_x = (x_incl * (x - y)) + Cx;
				neighbor_y = (1 - y_incl) * (x + y) - z + Cy;
				SDL_RenderDrawLine(renderer, real_x, real_y, neighbor_x, neighbor_y);
			}
			if (j < columns - 1){
				//Drawing a line between our current point and its neighbor according to X coordinate
				x = coords[i][j + 1].x;
				y = coords[i][j + 1].y;
				z = coords[i][j + 1].z;
				neighbor_y = ((1 - y_incl) * (x + y) - z) + Cy;
				neighbor_x = (x_incl * (x - y)) + Cx;
				SDL_RenderDrawLine(renderer, real_x, real_y, neighbor_x, neighbor_y);
			}
		}
	}
}


int events_handling(camera* cam,vect3D** coords,int rows,int columns){
	SDL_Event e;
	while (SDL_PollEvent(&e)){
		if (e.type == SDL_KEYDOWN){
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				return -1;
			case SDLK_1:
				if (cam->current_scale <= 1) break;
				cam->source.w *= 2;
				cam->source.h *= 2;
				cam->current_scale /= 2;
				break;
			case SDLK_2:
				if (cam->current_scale > 32) break;
				cam->source.w /= 2;
				cam->source.h /= 2;
				cam->current_scale *= 2;
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
			case SDLK_s:
				if (cam->source.y < -2) break;
				cam->source.y -= 3;
				break;
			case SDLK_z:
				if (cam->source.y > cam->source.h * cam->current_scale) break;
				cam->source.y += 3;
				break;
			case SDLK_d:
				if (cam->source.x < -2) break;
				cam->source.x -= 5;
				break;
			case SDLK_q:
				if (cam->source.x > cam->source.w * cam->current_scale) break;
				cam->source.x += 5;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

void main_loop(SDL_Renderer* renderer,vect3D** coords,int rows,int columns){
	SDL_Texture* texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,2 * SCREEN_WIDTH,2 * SCREEN_HEIGHT);
	camera cam = {.source = {SCREEN_WIDTH/4,SCREEN_HEIGHT/4,SCREEN_WIDTH,SCREEN_HEIGHT}, .current_scale = 2}; //initial zoom at 1 scale, centered camera.
	SDL_Rect dest = {10,10,SCREEN_WIDTH - 20,SCREEN_HEIGHT- 20};
	
	SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    while (true){
        SDL_Delay(FRAME_DELAY); //Clears up renderer
		SDL_SetRenderTarget(renderer,texture);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        if (events_handling(&cam,coords,rows,columns) == -1) break; // user wants to close the window.

        //Redraw everything
        SDL_SetRenderDrawColor(renderer,0,255,255,255);
        draw_grid(renderer,coords,rows,columns);


		SDL_SetRenderTarget(renderer,NULL);
		SDL_RenderCopy(renderer,texture,&(cam.source),&dest);
		SDL_RenderPresent(renderer);
    }
	SDL_DestroyTexture(texture);
}	