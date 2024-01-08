.ONESHELL:

# Add any project C files here to have them built as part of the project.
PROJECT_SOURCE_FILES ?= main.c missiles.c bullets.c particles.c ship.c asteroid.c collider.c explosion.c title.c starfield.c game.c enemy.c puff.c briefing.c sounds.c

# This is hardcoded to the path that `install-linux.sh` and `install-windows.bat` 
# place emsdk files to avoid needing to set up the environment to build.
CC=../emsdk/emsdk-main/upstream/emscripten/emcc 

# This is the directory within `src` where the finished files will be placed.
DEST=./build

CFLAGS=-I. -I./external 
MAKE=make

OBJ=$(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES)) ./external/libraylib.a

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

index.html: $(OBJ)
	mkdir -p $(DEST)
	$(CC) -o $(DEST)/$@ $^ $(CFLAGS) -s USE_GLFW=3 -s INITIAL_MEMORY=67108864 --preload-file resources --shell-file ./minshell.html --js-library my-library.js

clean:
	rm *.o