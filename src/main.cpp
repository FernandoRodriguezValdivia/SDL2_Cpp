#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Ahora cambiamos el enfoque, vamos a usar funciones para el ciclo de vida de la app

// Función para iniciar SDL y crear una ventana con su superficie
bool init();

// Funcion para cargar media
bool loadMedia();

// Función para liberar media y cerrar SDL
void close();

//La ventana que será renderizada (ahora es global)
SDL_Window* gWindow = NULL;

// La superficie dentro de la superficie de la ventana (ahora es global)
SDL_Surface* gScreenSurface = NULL;

// La imagen que cargaremos y mostraremos en la superficie ( global)
SDL_Surface* gHelloWorld = NULL;

// dimensiones de la ventana
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// creamos init()
bool init()
{
	bool succes = true;
	if( SDL_Init( SDL_INIT_VIDEO) < 0)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("SDL no pudo inicializar! SDL_Error: %s\n", SDL_GetError());
		succes = false;
	}
	else
	{
		// Creamos la ventana
		gWindow = SDL_CreateWindow("Tutorial de SDL / Imagen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL)
		{
			printf("SDL no pudo inicializar! SDL_Error: %s\n", SDL_GetError());
			succes = false;
		}
		else
		{
			// obtenemos la superficie en la ventana y la ponemos en screenSurface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	return succes;
}

// creamos loadMedia()
bool loadMedia()
{
	bool succes = true;
	// cargar imagen
	gHelloWorld = SDL_LoadBMP("res/gfx/hello_world.bmp");

	if( gHelloWorld == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen! SDL_Error: %s\n", SDL_GetError());
		succes = false;
	}
	return succes;
}

void close()
{
	// liberar superficie de imagen
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	// destruimos la ventana
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gScreenSurface = NULL;
	
	// Salir de los subsistemas de SLD
	SDL_Quit();	
}

int main(int argc, char* args[])
{
	// Iniciamos SDL
	if( !init() )
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo inicializar!\n");
	}
	else
	{
		// cargamos Media
		if( !loadMedia() )
		{
			printf("No se pudo cargar media\n");
		}
		else
		{
			// proyectamos la imagen en la superficie de la ventana
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

			// actualizamos la superficie
			SDL_UpdateWindowSurface( gWindow );
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
	// destruimos la ventana y liberamos los recursos
	close();

	return 0;	
}