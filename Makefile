C=gcc
CXX=g++
CFLAGS=-Wall -Wextra -ggdb -lm
CXXFLAGS=$(CFLAGS)

newton_fractal: newton_fractal.c
	$(C) $< -o $@ $(CFLAGS)
