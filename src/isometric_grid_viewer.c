#include <stdbool.h>
#include "../include/engine.h"
#include "../include/file_operations.h"

//Start SDL here
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
    FILE* f = fopen(argv[1],"r");
    vect3D** coords = read_file(f,&rows,&columns);
    fclose(f);
    SDL_Window* window;
    SDL_Renderer* renderer;
    int status = start_SDL(&window,&renderer,SCREEN_WIDTH,SCREEN_HEIGHT,"3D isometric grid viewer, by Esteban795 on Github.");
    if (status == 1) return EXIT_FAILURE;
    main_loop(renderer,coords,rows,columns);

    
    //Free up stuff
    for (int i = 0; i < rows;i++){
        free(coords[i]);
    }
    free(coords);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}