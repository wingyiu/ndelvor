#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define eps (1e-10)
#define fabs(x) ((x)>0?(x):-(x))
#define zero(x) (fabs(x)<1e-10)

double det(double *s, int n);

//circumcenter of n-simplex
void circumcenter(double *c, int d, double *cc);

//squared distance of two point in d-dimension
double sq_distance(const double *pa, const double *pb, int d);

int sign(double v);
#endif // UTIL_H_INCLUDED
