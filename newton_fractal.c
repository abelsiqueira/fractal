#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LENX 1920
#define LENY 1080
#define SCALE (LENX*1.0/LENY)
#define EPS 1e-2
#define KMAX 20
#define DSOL 0.8
#define BIG 1e10

typedef struct _point {
  double x;
  double y;
} point;

typedef struct _matrix {
  double a11, a12, a21, a22;
} matrix;

double det (matrix *M) {
  return M->a11*M->a22 - M->a12*M->a21;
}

void solve (matrix *M, point *p) {
  double d = det(M);
  double b1 = p->x, b2 = p->y;
  p->x = (M->a22*b1 - M->a12*b2)/d;
  p->y = (M->a11*b2 - M->a21*b1)/d;
}

void eval (point *p, point *F, matrix *J) {
  double x = p->x, xx = x*x, y = p->y, yy = y*y;
  F->x = x*xx - 3*x*yy - 1;
  F->y = 3*xx*y - y*yy;
  J->a11 = 3*(xx-yy);
  J->a21 = 6*x*y;
  J->a12 = -J->a21;
  J->a22 = J->a11;
}

double norm (point *p) {
  if (fabs(p->x) >= fabs(p->y))
    return fabs(p->x);
  else
    return fabs(p->y);
}

int newton (point *p) {
  int k = 0;
  point F;
  matrix J;

  eval(p, &F, &J);

  while (norm(&F) > EPS) {
    if (norm(p) < EPS || norm(p) > BIG)
      return k;
    solve (&J, &F);
    p->x -= F.x;
    p->y -= F.y;
    k = k + 1;
    if (k >= KMAX)
      return k;
    eval(p, &F, &J);
  }
  return k;
}

int close_to_solution (point *p) {
  point dr;
  dr.x = p->x - 1;
  dr.y = p->y;
  if (norm(&dr) < DSOL)
    return 1;
  dr.x = p->x + 0.5;
  dr.y = p->y - 0.86603;
  if (norm(&dr) < DSOL)
    return 2;
  dr.x = p->x + 0.5;
  dr.y = p->y + 0.86603;
  if (norm(&dr) < DSOL)
    return 3;
  return 0;
}

void print_point (point *p) {
  printf("x = %lf, y = %lf\n", p->x, p->y);
}

void fractal () {
  FILE *f = fopen("fractal.ppm","w");
  int N = LENX-1;
  int M = LENY-1;
  double hx = SCALE*2.0/LENX;
  double hy = 2.0/LENY;
  int i, j, v, k;
  point p;
  int s = 0;
  fpos_t beg_pos;
  int max = 1;

  fgetpos(f, &beg_pos);
  fprintf(f, "P3\n%d %d\n%d\n", LENX, LENY, KMAX);

  for (j = 0; j <= M; j++) {
    for (i = 0; i <= N; i++) {
      p.x = -1*SCALE + i*hx;
      p.y = -1 + j*hy;
      k = newton(&p);
#ifdef VERBOSE
      print_point(&p);
#endif
#ifdef BETTER
      v = KMAX-k;
      if (v <= 0) v = 1;
      else if (v > max) max = v;
#else
      v = 1;
#endif
      s = close_to_solution(&p);
      if (s == 0)
        fprintf(f, "0 0 0\n");
      else if (s == 1)
        fprintf(f, "%d 0 0\n", v);
      else if (s == 2)
        fprintf(f, "0 %d 0\n", v);
      else if (s == 3)
        fprintf(f, "0 0 %d\n", v);
      else
        fprintf(stderr, "FAIL: s=%d", s);
    }
  }

  fsetpos(f, &beg_pos);
  fprintf(f, "P3\n%d %d\n%d\n", LENX, LENY, max);

  fclose(f);
}

int main () {
  fractal();

  return 0;
}
