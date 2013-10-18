C=gcc
CXX=g++
CFLAGS=-Wall -Wextra -ggdb -lm
CXXFLAGS=$(CFLAGS)
MODE=
SIMPLE=

OBJS = newton.o fractal.o
EXECS = newton

all: $(OBJS) newton

$(EXECS): %: %.o fractal.o
	$(C) $^ -o $@ $(CFLAGS)

%.o: %.c fractal.h
	$(C) -c $< -o $@ $(CFLAGS) 

run-%: %
	./$<
	convert fractal.ppm fractal.png
	rm -f *.ppm *.b

clean:
	rm -f $(EXECS) $(OBJS) *.png *.ppm *.b
