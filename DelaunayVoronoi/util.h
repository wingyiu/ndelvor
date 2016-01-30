#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED


double det(double *s, int n);

//circumcenter of n-simplex
void circumcenter(double *c, int d, double *cc);

//squared distance of two point in d-dimension
double sq_distance(const double *pa, const double *pb, int d);

int sign(double v);
#endif // UTIL_H_INCLUDED
