#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// API de representación de texturas. Esto brinda una representación basada en hardware rápida y flexible

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Función para iniciar SDL y crear una ventana con su superficie
bool init();

// Funcion para cargar media
bool loadMedia();

// Función para liberar media y cerrar SDL
void close();

// Función que cargará una imagen individual como una textura desde una superificie, toma como argumento el path
SDL_Texture* loadTextureFromSurface( std::string path);

// Función que cargará una imagen individual como una textura sin superficie, toma como argumento el path
SDL_Texture* loadTexture( std::string path);

//La ventana que será renderizada (ahora es global)
SDL_Window* gWindow = NULL;

// El renderizador de la ventana ( cuando usamos texturas se necesita un SDL_Renderer para mostrarlo en pantalla)
SDL_Renderer* gRenderer = NULL;

// La textura actual mostrada
SDL_Texture* gTexture = NULL;

bool init()
{
	bool succes = true;
	if( SDL_Init( SDL_INIT_VIDEO) < 0)
	{
		printf("SDL no pudo inicializar! SDL_Error: %s\n", SDL_GetError());
		succes = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Tutorial de SDL / Imagen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL)
		{
			printf("SDL no pudo inicializar! SDL_Error: %s\n", SDL_GetError());
			succes = false;
		}
		else
		{
			// Creamos el renderizador de la ventana
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if( gRenderer == NULL )
			{
				printf("El renderizador no pudo ser creado! SDL_ERROR: %s\n", SDL_GetError());
			}
			else
			{
				// Inizializar el color del renderizado (Esto controla qué color se utiliza para varias operaciones de renderizado.)
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Inicializar la carga de PNG
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image no pudo inicializar! SDL_image Error: %s\n", IMG_GetError() );
					succes = false;
				}
			}
		}
	}
	return succes;
}

// creamos loadMedia()
bool loadMedia()
{
	bool succes = true;

	// cargar textura de PNG
	gTexture = loadTexture("res/gfx/texture.png");
	if( gTexture == NULL)
	{
		// SDL_GetError() devuelve el ultimo error
		printf("No se pudo cargar la imagen PNG!\n");
		succes = false;
	}

	return succes;
}

void close()
{
	// liberar imagen cargada en textura
	SDL_DestroyTexture ( gTexture );
	gTexture = NULL;
	

	// destruimos la ventana
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	
	// Salir de los subsistemas de SLD
	// Ahora aumentamos IMG
	IMG_Quit();
	SDL_Quit();	
}

// Funcion que carga una textura desde una superficie
SDL_Texture* loadTextureFromSurface( std::string path )
{
	//La textura final optimizada
	SDL_Texture* newTexture = NULL;

	// cargar la imagen de un path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf("No se pudo cargar la imagen %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Crear la textura desde la superficie
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf("No se pudo cargar la imagen optimizada %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Liberamos la imagen cargada
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

// Funcion que carga una textura sin superficie usando la API IMG_LoadTexture
SDL_Texture* loadTexture( std::string path )
{
	//La textura final optimizada
	SDL_Texture* newTexture = NULL;

	// cargar la textura de un path
	newTexture = IMG_LoadTexture( gRenderer, path.c_str() );

	if( newTexture == NULL )
	{
		printf("No se pudo cargar la textura %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	
	return newTexture;
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

				// Limpiamos la pantalla
				SDL_RenderClear(gRenderer);

				// Renderizamos la textura a la pantalla
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL);

				// actualizamos la pantalla
				SDL_RenderPresent( gRenderer );
			}
		}
	}
	// destruimos la ventana y liberamos los recursos
	close();

	printf("Presione una tecla para salir");
	getchar();

	return 0;	
}