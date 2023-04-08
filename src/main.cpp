#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Ahora veamos como manejar los eventos de key press

// dimensiones de la ventana
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// declaramos una enumeracion que nos ayudara a identificar las superficies usando keys
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// Función para iniciar SDL y crear una ventana con su superficie
bool init();

// Funcion para cargar media
bool loadMedia();

// Función para liberar media y cerrar SDL
void close();

// Función que cargará una imagen individual, toma como argumento el path
SDL_Surface* loadSurface( std::string path);

//La ventana que será renderizada (ahora es global)
SDL_Window* gWindow = NULL;

// La superficie dentro de la superficie de la ventana (ahora es global)
SDL_Surface* gScreenSurface = NULL;

// Un array de imagenes que corresponden a las keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

// La imagen actual mostrada
SDL_Surface* gCurrentSurface = NULL;


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

	// cargar imagen por default
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface("res/gfx/press.bmp");
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen por default!");
		succes = false;
	}

	// cargar imagen por default
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface("res/gfx/up.bmp");
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen up!");
		succes = false;
	}

	// cargar imagen por default
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface("res/gfx/down.bmp");
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen down!");
		succes = false;
	}

	// cargar imagen por default
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface("res/gfx/left.bmp");
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen left!");
		succes = false;
	}

	// cargar imagen por default
	gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface("res/gfx/right.bmp");
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen right!");
		succes = false;
	}

	return succes;
}

void close()
{
	// liberar superficie de imagen de todo el array
	for( int i = 0; i<KEY_PRESS_SURFACE_TOTAL; i++)
	{
		SDL_FreeSurface( gKeyPressSurfaces[i] );
		gKeyPressSurfaces[i] = NULL;
	}
	

	// destruimos la ventana
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gScreenSurface = NULL;
	
	// Salir de los subsistemas de SLD
	SDL_Quit();	
}

// Funcion que carga una imagen
SDL_Surface* loadSurface( std::string path )
{
	// cargar la imagen de un path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf("No se pudo cargar la imagen %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}

	return loadedSurface;
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
			// Explicando el bucle de eventos

			// Declaramos el evento, que es cuando hacemos una accion con un periferic de entrada
			SDL_Event e;

			// la condicion de salida
			bool quit = false;

			// bucle de la aplicacion
			while( quit == false )
			{

				// bucle de eventos, SDL_PollEvent(& e) devuelve 1 mientras hay un evento en la cola de eventos
				while( SDL_PollEvent( &e ) )
				{ 
					// Si el evento es de SDL_QUIT que es cuando se cierra la ultima ventana se sale
					if( e.type == SDL_QUIT ) quit = true;
					else if( e.type == SDL_KEYDOWN)
					{
						// Cuando tenemos el type SDL_KEDOWN el tipo de tecla lo tenemos en keysym.sym
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
								gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
								break;
							case SDLK_DOWN:
								gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
								break;
							case SDLK_LEFT:
								gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
								break;
							case SDLK_RIGHT:
								gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
								break;
							default:
								gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
								break;
						}
					}
				}
				// proyectamos la imagen actual en la superficie de la ventana
				SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );

				// actualizamos la superficie
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}
	// destruimos la ventana y liberamos los recursos
	close();

	return 0;	
}