#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fractal.h"

double det (matrix *M) {
  return M->a11*M->a22 - M->a12*M->a21;
}

int solve (matrix *M, point *p) {
  double d = det(M);
  double b1 = p->x, b2 = p->y;
  if (fabs(d) < TINY)
    return 1;
  p->x = (M->a22*b1 - M->a12*b2)/d;
  p->y = (M->a11*b2 - M->a21*b1)/d;

  return 0;
}

double norm (point *p) {
#ifdef USENORM2
  return sqrt(p->x*p->x + p->y*p->y);
#else 
#ifdef USENORM1
  return fabs(p->x) + fabs(p->y);
#else
  if (fabs(p->x) >= fabs(p->y))
    return fabs(p->x);
  else
    return fabs(p->y);
#endif
#endif
}

void print_point (point *p) {
  printf("x = %lf, y = %lf\n", p->x, p->y);
}

void writefile (int max) {
  FILE *S = fopen("sol.b","r");
  FILE *K = fopen("iters.b","r");
  FILE *f = fopen("fractal.ppm","w");
  int i, j, s, k;
  fprintf(f, "P3\n%d %d\n%d\n", LENX, LENY, max);

  for (j = 0; j < LENY; j++) {
    for (i = 0; i < LENX; i++) {
      fscanf(S, "%d", &s);
      fscanf(K, "%d", &k);
      method_print(f, s, k, max);
    }
  }

  fclose(f);
  fclose(S);
  fclose(K);
}

void fractal () {
  double hx = (2*R)/LENX;
  double hy = (2*R)/LENY;
  int i, j, k;
  point p;
  int s = 0;
  int max = 1;
  FILE *S = fopen("sol.b","w");
  FILE *K = fopen("iters.b","w");
  double rx = R, ry = R;

  if (SCALE > 1) {
    hx *= SCALE;
    rx *= SCALE;
  } else if (SCALE < 1) {
    hy /= SCALE;
    ry /= SCALE;
  }

  for (j = 0; j < LENY; j++) {
    for (i = 0; i < LENX; i++) {
      p.x = CX - rx + i*hx;
      p.y = CY - ry + j*hy;
      k = iterative_method(&p);
      s = close_to_solution(&p);
      fprintf(S, "%d ", s);
      fprintf(K, "%d ", k);
#ifndef SIMPLE
      if (k > max) max = k;
#endif
    }
  }
  fclose(S);
  fclose(K);

  writefile(max);
}

int main () {
  fractal();

  return 0;
}
