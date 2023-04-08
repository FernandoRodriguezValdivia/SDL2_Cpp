#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
//#include <iostream>

// #include "RenderWindow.hpp

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	// La ventana que será renderizada
	SDL_Window* window = NULL;

	// La superficie dentro de la ventana
	SDL_Surface* screenSurface = NULL;

	// Iniciamos SDL
	if( SDL_Init( SDL_INIT_VIDEO) < 0)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("SDL no pudo inicializar! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		// Creamos la ventana
		window = SDL_CreateWindow("Tutorial de SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL)
		{
			printf("SDL no pudo inicializar! SDL_Error: %s\n", SDL_GetError());		
		}
		else
		{
			// obtenemos la superficie en la ventana y la ponemos en screenSurface
			screenSurface = SDL_GetWindowSurface( window );

			// Rellenamos la superfice de blanco
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF  ) );

			// Actualizamos la superficie de la ventana
			SDL_UpdateWindowSurface( window );

			// hack para tener la ventana abierto ( esto es temporal )
			SDL_Event e;
			bool quit = false;
			while( quit == false )
			{
				while( SDL_PollEvent( &e ) )
				{ 
					if( e.type == SDL_QUIT ) quit = true;
				}
			}
		}
	}
	// destruimos la ventana
	SDL_DestroyWindow( window );

	// Salir de los subsistemas de SLD
	SDL_Quit();
	
	return 0;	
}