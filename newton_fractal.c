#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define USENORM2
#define SIMPLE

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
#define EPS 1e-4
#define KMAX 20
#define DSOL 0.01
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

void writefile (int max) {
  FILE *S = fopen("sol.b","r");
  FILE *K = fopen("iters.b","r");
  FILE *f = fopen("fractal.ppm","w");
  int i, j, s, k;
  double p;
  fprintf(f, "P3\n%d %d\n%d\n", LENX, LENY, max);

  for (j = 0; j < LENY; j++) {
    for (i = 0; i < LENX; i++) {
      fscanf(S, "%d", &s);
      fscanf(K, "%d", &k);
#ifdef SIMPLE
      p = 1;
#else
      p = ((double) (max-k))/max;
      p = p*p*p*max;
#endif
      if (s == 0)
        fprintf(f, "0 0 0\n");
      else if (s == 1)
        fprintf(f, "%d 0 0\n", (int)p);
      else if (s == 2)
        fprintf(f, "0 %d 0\n", (int)p);
      else if (s == 3)
        fprintf(f, "0 0 %d\n", (int)p);
      else
        fprintf(f, "0 0 0\n");
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
      k = newton(&p);
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
