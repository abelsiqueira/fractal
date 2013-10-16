C=gcc
CXX=g++
CFLAGS=-Wall -Wextra -ggdb -lm
CXXFLAGS=$(CFLAGS)
MODE=
SIMPLE=

wide: clean
	make newton_fractal MODE=-DWIDE
	make run

newton_fractal: newton_fractal.c
	$(C) $< -o $@ $(CFLAGS) $(MODE) $(SIMPLE)

run:
	./newton_fractal
	convert fractal.ppm fractal.png

clean:
	rm -f newton_fractal *.png *.ppm *.b
