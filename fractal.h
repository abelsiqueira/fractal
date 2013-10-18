#ifndef fractal_h
#define fractal_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define USENORM2

#define BIG 1e10
#define TINY 1e-12

typedef struct _point {
  double x;
  double y;
} point;

typedef struct _matrix {
  double a11, a12, a21, a22;
} matrix;

typedef struct _options {
  int width;
  int height;
  double cx, cy, r;
  double eps;
  double kmax;
  double dsol;
  int simple;
} options;

void hsv_to_rgb (double H[3], double C[3]);
double det (matrix *M);
int solve (matrix *M, point *p);
double norm (point *p);
void print_point (point *p);
void initialize_options (options *opt);
void writefile (int max, options *opt);
void fractal (options *opt);
void parse_options (int argc, char **argv, options *opt);

/*
 * Expected from the file defining the iterative method
 */
int iterative_method (point *p, options *opt);
int close_to_solution (point *p, options *opt);
void method_print (FILE *f, int s, int k, int max, options *opt);

#endif
