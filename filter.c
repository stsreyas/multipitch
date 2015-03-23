#include "filter.h"
#include "mem-util.h"
#include <math.h>
#include "math-util.h"

void One_Pole_Filter(float *x, int nx, double a)
{
	int i;
	double y_1 = 0.0, y;
	
	for (i=0;i<nx;i++) {
		y = (double)x[i] + a*y_1;
		y_1 = y;
		x[i] = (float)y;
	}
}

void filter(float *b, int nb, float *a, int na, float *x, int nx, float *y)
/* y[n] = b[0]*x(n) + b[1]*x[n-1] + ... + b[nb-1]*x[n-nb+1]
- a[1]*y[n-1] - ... - a[na-1]*y[n-na+1]
where a[0] == 1.0, normally the order of the filter is nb+1 and na+1
and the formula is
y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
- a(2)*y(n-1) - ... - a(na+1)*y(n-na)
*/

{
	int i,j;
	double *yd;
	
	ALLOCATE_ARRAY(yd, nx);
	
	yd[0] = b[0]*x[0];
	y[0] = yd[0];
	for (i=1;i<nx;i++) {
		yd[i] = 0.0;
		for (j=0;j<min(nb,i+1);j++)
			yd[i] += b[j]*x[i-j];
		for (j=1;j<min(na,i+1);j++)
			yd[i] -= a[j]*yd[i-j];
		y[i] = yd[i];
	}
	
	free(yd);
}

void FIR_Filter(float *b, int nb, float *x, int nx, float *y)
// y[n] = b[0]*x(n) + b[1]*x[n-1] + ... + b[nb-1]*x[n-nb+1]
{
	int i,j;
	double yd;
	
	for (i=0;i<nx;i++) {
		yd = 0.0;
		for (j=0;j<min(nb,i+1);j++)
			yd += b[j]*x[i-j];
		y[i] = yd;
	}
}
