#include "fractal.h"

int iterative_method (point *p, options *opt) {
  point c, pp;
  int k = 0;
  double xx = p->x*p->x, yy = p->y*p->y;
  c.x = p->x;
  c.y = p->y;

  while (xx + yy < 4) {
    pp.x = xx - yy + c.x;
    pp.y = 2*p->x*p->y + c.y;
    p->x = pp.x;
    p->y = pp.y;
    xx = pp.x*pp.x;
    yy = pp.y*pp.y;
    k++;
    if (k >= opt->kmax)
      return 0;
  }

  return k;
}

int close_to_solution (point *p, options *opt) {
  (void) p;
  (void) opt;
  return 0;
}

void method_print (FILE *f, int s, int k, int max, options *opt) {
  double p = ((double)fabs(k))/max;
  double C[3] = {0, 0, 0}, H[3] = {0, 0, 0};
  (void) s;
  (void) opt;
  int r, g, b;

  if (k > 0) {
    H[0] = p*360; 
    H[1] = 1;
/*    H[2] = 1-pow(2*(p-0.5),N);*/
    if (p > 0.95)
      H[2] = 1.0*(1 - (p - 0.95)/(1 - 0.95));
    else if (p > 0.05)
      H[2] = 1;
    else
      H[2] = 1.0*p/0.05;
    hsv_to_rgb(H, C);
  }

  r = max*C[0];
  g = max*C[1];
  b = max*C[2];

/*  if (k > 0)*/
    fprintf(f, "%d %d %d\n", r, g, b);
/*  else*/
/*    fprintf(f, "0 0 0\n");*/
}
