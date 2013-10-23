#include "fractal.h"

void eval (point *p, point *F, matrix *J) {
  double x = p->x, y = p->y;
  F->x = -sin(x);
  F->y = -sin(y);
  J->a11 = -cos(x);
  J->a21 = 0;
  J->a12 = 0;
  J->a22 = -cos(y);
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
  double n, m;
  int ni, mi;
#ifdef VERBOSE
  printf("p = (%lf,%lf)\n", p->x, p->y);
#endif
  n = p->x/M_PI;
  m = p->y/M_PI;
  ni = round(n);
  mi = round(m);
  if (fabs(n-ni) > opt->dsol || fabs(m-mi) > opt->dsol)
    return 0;
  if (ni%2 == 0 && mi%2 == 0)
    return 1;
  else if (ni%2 == 1 && mi%2 == 1)
    return 2;
  else
    return 3;
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
