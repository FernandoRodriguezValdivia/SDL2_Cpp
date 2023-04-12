# OBJS especifica qué archivos se deben compilar como parte del proyecto
OBJS = src/*.cpp

# CC especifica el compilador que estamos usando
CC = g++

# C Especifica el tipo de compilación, en este caso compilador a objetos
C = -c

# COMPILER_FLAGS
COMPILER_FLAGS = -std=c++14 -g -Wall -m64

# INCLUDE_PATHS especifica las rutas de inclusión adicionales que necesitamos
INCLUDE_PATHS = -I include -I C:/SDL2-w64/include

OBJ = *.o

# OBJ_NAME especifica el nombre de nuestro ejecutable
OBJ_NAME = bin/debug/main

# LIBRARY_PATHS especifica las rutas de biblioteca adicionales que necesitamos
LIBRARY_PATHS = -L C:/SDL2-w64/lib

# LINKER_FLAGS especifica las bibliotecas con las que estamos enlazando
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# Este es el objetivo que compila nuestro ejecutable
all:
	$(CC) $(C) $(OBJS) $(COMPILER_FLAGS) $(INCLUDE_PATHS) && $(CC) $(OBJ) -o $(OBJ_NAME) $(LIBRARY_PATHS) $(LINKER_FLAGS) && start $(OBJ_NAME)

#all:
# g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main