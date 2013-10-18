#ifndef fractal_h
#define fractal_h

#include <stdio.h>

#define USENORM2
/*#define SIMPLE*/

#ifdef WIDE
#define LENX 1920
#define LENY 1080
#else
#ifdef CELL
#define LENX 1024
#define LENY 768
#else
#define LENX 1000
#define LENY 1000
#endif
#endif

#define CX 0.0
#define CY 0.0
#define R 1.0
#define SCALE (LENX*1.0/LENY)
#define EPS 1e-15
#define KMAX 40
#define DSOL 0.01
#define BIG 1e10
#define TINY 1e-12

typedef struct _point {
  double x;
  double y;
} point;

typedef struct _matrix {
  double a11, a12, a21, a22;
} matrix;

double det (matrix *M);
int solve (matrix *M, point *p);
double norm (point *p);
void print_point (point *p);
void writefile (int max);
void fractal ();

/*
 * Expected from the file defining the iterative method
 */
int iterative_method (point *p);
int close_to_solution (point *p);
void method_print (FILE *f, int s, int k, int max);

#endif
