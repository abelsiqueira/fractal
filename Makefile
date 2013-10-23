C=gcc
CXX=g++
VERBOSE=-DVERBOSE
CFLAGS=-Wall -Wextra -ggdb -lm -DDEBUG $(VERBOSE) -DROOTS="$(ROOTS)"
CXXFLAGS=$(CFLAGS)
MODE=
SIMPLE=

OBJS = mandelbrot.o newton_classic.o newton_sin.o newton.o fractal.o
EXECS = mandelbrot newton_classic newton_sin newton

all: $(OBJS) mandelbrot newton_classic newton_sin newton

$(EXECS): %: %.o fractal.o
	$(C) $^ -o $@ $(CFLAGS)

%.o: %.c fractal.h
	$(C) -c $< -o $@ $(CFLAGS) 

run-%: %
	./$< $(ARGS)
	convert fractal.ppm fractal.png
	rm -f *.ppm *.b

clean:
	rm -f $(EXECS) $(OBJS) *.png *.ppm *.b
