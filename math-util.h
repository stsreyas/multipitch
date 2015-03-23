#if !defined(MATH_UTIL_H)
#define MATH_UTIL_H

#define sqr(x) ((x)*(x))

#define Heaviside(x) ((x)<0.0?0.0:1.0)

#define EPSILON 1.0e-6

#define EPSILON_DOUBLE 1.0e-10

#define REALZEROP(x) ((x<EPSILON)&&(x>-EPSILON))

#define fmax(x,y) (x>y?x:y)

#define fmin(x,y) (x<y?x:y)

//#define abs(x) (x>=0?x:-x)

#define MAXFLOAT ((float)3.402823e+38)

#define MINFLOAT ((float)1.401298e-45)

#define MAXDOUBLE ((double)1.797693e+308)

#define MINDOUBLE ((double)4.940656e-324)

#define MAXINT 2147483647

#define M_PI 3.1415926535

#endif // MATH_UTIL_H
