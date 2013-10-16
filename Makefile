C=gcc
CXX=g++
CFLAGS=-Wall -Wextra -ggdb -lm
CXXFLAGS=$(CFLAGS)

newton_fractal: newton_fractal.c
	$(C) $< -o $@ $(CFLAGS)

run:
	./newton_fractal
	convert fractal.ppm fractal.png

clean:
	rm -f newton_fractal *.png *.ppm *.b
