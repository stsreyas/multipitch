#if !defined(FILTER_H)
#define FILTER_H

void One_Pole_Filter(float *x, int nx, double a);

void FIR_Filter(float *b, int nb, float *x, int nx, float *y);
// y[n] = b[0]*x(n) + b[1]*x[n-1] + ... + b[nb-1]*x[n-nb+1]

void filter(float *b, int nb, float *a, int na, float *x, int nx, float *y);
// y[n] = b[0]*x(n) + b[1]*x[n-1] + ... + b[nb-1]*x[n-nb+1]
// - a[1]*y[n-1] - ... - a[na-1]*y[n-na+1]
// where a[0] == 1.0, normally the order of the filter is nb+1 and na+1
// and the formula is
// y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
// - a(2)*y(n-1) - ... - a(na+1)*y(n-na)


#endif // FILTER_H
