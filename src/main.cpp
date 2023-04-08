#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Mejora de carga de imagen y estrechamiento suave

// dimensiones de la ventana
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

// La imagen actual mostrada
SDL_Surface* gStretchedSurface = NULL;


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

	// cargar superficie (imagen) estirada
	gStretchedSurface = loadSurface("res/gfx/stretch.bmp");
	if( gStretchedSurface == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen stretch!");
		succes = false;
	}

	return succes;
}

void close()
{
	// liberar superficie de imagen
	SDL_FreeSurface( gStretchedSurface );
	gStretchedSurface = NULL;
	

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
	//La imagen final optimizada
	SDL_Surface* optimizedSurface = NULL;

	// cargar la imagen de un path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf("No se pudo cargar la imagen %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		// Convertimos al formato de la pantalla, el cero son banderas opcionales, en este caso no se pasa ninguna
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format,0);
		if( optimizedSurface == NULL )
		{
			printf("No se pudo cargar la imagen optimizada %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Liberamos la imagen cargada
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
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
				}

				// proyectamos la imagen estrechada en la superficie de la ventana, ahora en lugar de usar SDL_BlitSurface usamos SDL_BlitScaled

				// creamos un rectangulo que se usara para estirar la imagen
				SDL_Rect stretchRect;

				// la posicionamos en la esquina superior izquierda y le damos las dimensiones de la ventana
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;

				// Esta funcion ahora estira y copia la superficie de imagen gStretchedSurface en la superficie de pantalla gScreenSurface usando el rectangulo stretchRect como el area de destino. El parametro NULL es un rectangulo opcional que define un area de la imagen a copiar. en NULL significa que se copia toda la imagen
				SDL_BlitScaled( gStretchedSurface, NULL, gScreenSurface, &stretchRect );

				// actualizamos la superficie
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}
	// destruimos la ventana y liberamos los recursos
	close();

	return 0;	
}