#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Modularizando una textura usando varios colores.
// Funciona como un multiplicador a los colores de la imagen

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Clase de contenedor de textura
class LTexture
{
	public:
		//Inicializamos valores
		LTexture();

		// Liberamos memoria
		~LTexture();

		// Cargamos una imagen de un path
		bool loadFromFile( std::string path );

		// Liberamos textura
		void free();

		// Establecer modulación de color
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		// Renderizamos textura en un punto dado y un rectangulo que define que parte de la textura se renderizara (NULL es si queremos renderizar todo)
		void render( int x, int y, SDL_Rect* clip = NULL);

		// Obtenemos las dimensiones de la imagen
		int getWidth();
		int getHeight();
	
	private:
		// la textura de hardware real
		SDL_Texture* mTexture;

		// Dimension de la imagen
		int mWidth;
		int mHeight;
};

// Función para iniciar SDL y crear una ventana con su superficie
bool init();

// Funcion para cargar media
bool loadMedia();

// Función para liberar media y cerrar SDL
void close();

//La ventana que será renderizada (ahora es global)
SDL_Window* gWindow = NULL;

// El renderizador de la ventana ( cuando usamos texturas se necesita un SDL_Renderer para mostrarlo en pantalla)
SDL_Renderer* gRenderer = NULL;

// sprites de escenas
LTexture gModulatedTexture; // 1 textura

LTexture::LTexture()
{
	// Inicializar
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	// Liberar memoria
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	// Liberar la textura pre existente
	free();

	// La textura final
	SDL_Texture* newTexture = NULL;

	// Cargar la imagen a una superficie
	SDL_Surface* loadedSurface = IMG_Load( path.c_str());
	if( loadedSurface == NULL)
	{
		printf("No se pudo cargar la imagen %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Color de la imagen
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface -> format, 0, 0xFF, 0xFF));

		// Crear la texturra de la supericie de pixel
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if( newTexture == NULL)
		{
			printf("No se pudo crear la textura de %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else
		{
			// obtener las dimensiones de la imagen
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		// Liberamos la superficie cargada
		SDL_FreeSurface( loadedSurface );
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	// Liberamos textura si existe
	if( mTexture != NULL )
	{
		// destruye la textura y reinicializa las variables
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	// Modular textura
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render( int x, int y, SDL_Rect* clip)
{
	// Establecer el espacio de renderizado y renderizar en pantalla
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	// Establecer las dimensiones clip de renderizado
	if( clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// Renderizamos al screen (ahora usamos el clip para tomar unaa parte de la textura)
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

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
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

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

	// Cargamos las sprites de hojas
	if( !gModulatedTexture.loadFromFile("res/gfx/colors.png") )
	{
		printf("No se pudo cargar la textura imagen dots!\n");
		succes = false;
	}

	return succes;
}

void close()
{
	// liberar imagenes cargada en textura
	gModulatedTexture.free();

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

			// Modulation components
			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

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
					else if(e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_q:
							r+=32;
							break;
						case SDLK_w:
							g+=32;
							break;
						case SDLK_e:
							b+=32;
							break;
						case SDLK_r:
							r-=32;
							break;
						case SDLK_t:
							g-=32;
							break;
						case SDLK_y:
							b-=32;
							break;
						default:
							break;
						}
					}
				}

				// Limpiamos la pantalla con el color de fondo
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				// Modularizar y renderizar
				gModulatedTexture.setColor(r,g,b);
				gModulatedTexture.render(0,0);

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