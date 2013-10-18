#include <string.h>
#include "fractal.h"

void hsv_to_rgb (double H[3], double C[3]) {
  int h = (int) (H[0]/60);
  double f = H[0]/60.0 - h;
  double p = H[2]*(1 - H[1]);
  double q = H[2]*(1 - f*H[1]);
  double t = H[2]*(1 - (1-f)*H[1]);
  double r, g, b;

  if (h == 6)
    h = 0;

  switch (h) {
    case 0:
      r = H[2]; g = t; b = p; break;
    case 1:
      r = q; g = H[2]; b = p; break;
    case 2:
      r = q; g = H[2]; b = t; break;
    case 3:
      r = p; g = q; b = H[2]; break;
    case 4:
      r = t; g = p; b = H[2]; break;
    case 5:
      r = H[2]; g = p; b = q; break;
    default:
      r = 0; g = 0; b = 0; break;
  }
  C[0] = r; C[1] = g; C[2] = b;
}

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

void initialize_options (options *opt) {
  opt->width = 500;
  opt->height = 500;
  opt->cx = -0.5;
  opt->cy = 0;
  opt->r = 1;
  opt->eps = 1e-12;
  opt->kmax = 1000;
  opt->dsol = 1e-2;
  opt->simple = 0;
}

void writefile (int max, options *opt) {
  FILE *S = fopen("sol.b","r");
  FILE *K = fopen("iters.b","r");
  FILE *f = fopen("fractal.ppm","w");
  int i, j, s, k;
  fprintf(f, "P3\n%d %d\n%d\n", opt->width, opt->height, max);

  for (j = 0; j < opt->height; j++) {
    for (i = 0; i < opt->width; i++) {
      fscanf(S, "%d", &s);
      fscanf(K, "%d", &k);
      method_print(f, s, k, max, opt);
    }
  }

  fclose(f);
  fclose(S);
  fclose(K);
}

void fractal (options *opt) {
  double hx = (2*opt->r)/opt->width;
  double hy = (2*opt->r)/opt->height;
  int i, j, k;
  int s = 0;
  int max = 1;
  double scale = ((double)opt->width)/opt->height;
  FILE *S = fopen("sol.b","w");
  FILE *K = fopen("iters.b","w");
  double rx = opt->r, ry = opt->r;
  point p;

  if (scale > 1) {
    hx *= scale;
    rx *= scale;
  } else if (scale < 1) {
    hy /= scale;
    ry /= scale;
  }

  for (j = 0; j < opt->height; j++) {
    for (i = 0; i < opt->width; i++) {
      p.x = opt->cx - rx + i*hx;
      p.y = opt->cy - ry + j*hy;
      k = iterative_method(&p, opt);
      s = close_to_solution(&p, opt);
      fprintf(S, "%d ", s);
      fprintf(K, "%d ", k);
      if (opt->simple == 0 && fabs(k) > max)
        max = fabs(k);
    }
  }
  fclose(S);
  fclose(K);

  printf("max = %d\n", max);

  writefile(max, opt);
}

void parse_options (int argc, char **argv, options *opt) {
  int i_aux, i;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i],"-w") == 0) {
      i_aux = strtol(argv[++i], NULL, 10);
      opt->width = i_aux;
    } else if (strcmp(argv[i],"-h") == 0) {
      i_aux = strtol(argv[++i], NULL, 10);
      opt->height = i_aux;
    } else if (strcmp(argv[i],"-simple") == 0) {
      opt->simple = 1;
    }
  }
}

int main (int argc, char **argv) {
  options opt;

  initialize_options(&opt);
  parse_options(argc, argv, &opt);
  fractal(&opt);

  return 0;
}
