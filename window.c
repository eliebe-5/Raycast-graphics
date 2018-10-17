#include "window.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gRayScreen = NULL;

int create_window()
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO < 0)){
		printf("SDL could not initialize! SDL_Error %s\n", SDL_GetError());
		success = false;
	}
	else{
		gWindow = SDL_CreateWindow("SDL Tutorial!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be opened! SDL_Error %s\n", SDL_GetError());
			success = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
}

int destroy_window()
{
	SDL_FreeSurface(gRayScreen);
	gRayScreen = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int set_buffer(char* new_buffer, int w, int h)
{
	gRayScreen = SDL_CreateRGBSurfaceFrom(
			(void*)new_buffer,
			w,
			h,
			3*8,
			w*3,
			0x0000FF,
			0x00FF00,
			0xFF0000,
			0
			);
}

void draw_image()
{
	SDL_BlitSurface(gRayScreen, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow);
}

int get_input()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				return -1;
				break;
		}
	}
}
