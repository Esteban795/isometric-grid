SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(sdl2-config --cflags) -lSDL2 -lm

DEPENDENCIES = $(SRCDIR)/engine.c $(SRCDIR)/file_operations.c $(SRCDIR)/vect3D.c

build:
	gcc $(SRCDIR)/isometric_grid_viewer.c -o ./isometric_grid_viewer $(CFLAGS) $(DEPENDENCIES)

clearscreen:
	clear

run: ./isometric_grid_viewer
	./isometric_grid_viewer $(PATH)
	make clearscreen

clean:
	rm isometric_grid_viewer