#include "fractal.h"

void eval (point *p, point *F, matrix *J) {
  double x = p->x, xx = x*x, y = p->y, yy = y*y;
  F->x = x*xx - 3*x*yy - 1;
  F->y = 3*xx*y - y*yy;
  J->a11 = 3*(xx-yy);
  J->a21 = 6*x*y;
  J->a12 = -J->a21;
  J->a22 = J->a11;
}

int iterative_method (point *p, options *opt) {
  int k = 0;
  int exitflag = 0;
  point F;
  matrix J;

  eval(p, &F, &J);

  while (norm(&F) > opt->eps) {
    if (norm(p) < opt->eps || norm(p) > BIG)
      return k;
    exitflag = solve (&J, &F);
    if (exitflag == 1)
      return k;
    p->x -= F.x;
    p->y -= F.y;
    k = k + 1;
    if (k >= opt->kmax)
      return k;
    eval(p, &F, &J);
  }
  return k;
}

int close_to_solution (point *p, options *opt) {
  point dr;
  dr.x = p->x - 1;
  dr.y = p->y;
  if (norm(&dr) < opt->dsol)
    return 1;
  dr.x = p->x + 0.5;
  dr.y = p->y - 0.86603;
  if (norm(&dr) < opt->dsol)
    return 2;
  dr.x = p->x + 0.5;
  dr.y = p->y + 0.86603;
  if (norm(&dr) < opt->dsol)
    return 3;
  return 0;
}

void method_print (FILE *f, int s, int k, int max, options *opt) {
  int C[3] = {0, 0, 0};
  double p;
  if (opt->simple == 1)
    p = 1;
  else {
    p = ((double) (max-k))/max;
    p = p*p*p*max;
  }
  if (s != 0)
    C[s-1] = (int)p;
  fprintf (f, "%d %d %d\n", C[0], C[1], C[2]);
}
