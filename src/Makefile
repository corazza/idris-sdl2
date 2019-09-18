CC = gcc
CFLAGS = -Wall `idris --include` `pkg-config --cflags SDL2_gfx SDL2_mixer SDL2_image SDL2_ttf` -Umain

sdl2.o: config sdl2.c sdl2.h

.PHONY: config
config:
	echo "%flag C \"`pkg-config --libs SDL2`\"" > Graphics/Config.idr
	echo "%flag C \"`pkg-config --cflags SDL2`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --libs SDL2_gfx`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --cflags SDL2_gfx`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --libs SDL2_image`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --cflags SDL2_image`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --libs SDL2_ttf`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --cflags SDL2_ttf`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --libs SDL2_mixer`\"" >> Graphics/Config.idr
	echo "%flag C \"`pkg-config --cflags SDL2_mixer`\"" >> Graphics/Config.idr
	echo "%flag C \"-Umain\"" >> Graphics/Config.idr

.PHONY: clean
clean:
	rm -f sdl2.o
