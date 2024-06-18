#include <windows.h>
#include <GL/gl.h>
#include "SDL.h"

#define SDL_main main

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char** argv)
{
	window = SDL_CreateWindow("Game", 50, 50, 600, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
	SDL_Quit();
	return 0;
}
