#if !defined(TYPE_DEF_H)
#define TYPE_DEF_H
#include <stdio.h>
#include <math.h>
#include "mem-util.h"

typedef struct {
  int    row, col;
  float  weight;
} NEIGHBOR_TABLE_INDEX;

typedef struct {
  int nneighbor;
  NEIGHBOR_TABLE_INDEX *neighbors;
} NEIGHBORHOOD;

typedef struct {
int row1, row2, col1, col2;
float ****array;
} FLOAT4D;

typedef struct {
int row1, row2, col;
float ***array;
} FLOAT3D;

typedef struct {
int row, col;
float **array;
} FLOAT2D;

typedef struct {
int row;
float *array;
} FLOAT1D;

typedef struct {
  int row1, row2, col1, col2;
  double ****array;
} DOUBLE4D;

typedef struct {
  int row1, row2, col;
  double ***array;
} DOUBLE3D;

typedef struct {
  int row, col;
  double **array;
} DOUBLE2D;

typedef struct {
  int row;
  double *array;
} DOUBLE1D;

typedef struct {
int row;
int *array;
} INT1D;

typedef struct {
  int row, col;
  int **array;
} INT2D;

typedef struct {
  int row1, row2, col;
  int ***array;
} INT3D;

typedef struct {
  int row1, row2, col1, col2;
  int ****array;
} INT4D;

typedef struct {
  int row;
  unsigned char *array;
} CHAR1D;

typedef struct {
  int row, col;
  unsigned char **array;
} CHAR2D;

typedef struct {
  int row1, row2, col;
  unsigned char ***array;
} CHAR3D;

typedef struct {
  int row1, row2, col1, col2;
  unsigned char ****array;
} CHAR4D;

void Init_INT1D(INT1D *i1d, int row);
void Init_INT1D_Value(INT1D *i1d, int row, int value);
void Init_INT2D(INT2D *i2d, int row, int col);
void Init_INT2D_Value(INT2D *i2d, int row, int col, int value);
void Init_INT3D(INT3D *id, int row1, int row2, int col);
void Init_INT3D_Value(INT3D *id, int row1, int row2, int col, int value);
void Init_INT4D(INT4D *id, int row1, int row2, int col1, int col2);
void Init_INT4D_Value(INT4D *id, int row1, int row2, int col1, 
		      int col2, int value);
void Init_CHAR1D(CHAR1D *i1d, int row);
void Init_CHAR1D_Value(CHAR1D *i1d, int row, unsigned char value);
void Init_CHAR2D(CHAR2D *i2d, int row, int col);
void Init_CHAR2D_Value(CHAR2D *i2d, int row, int col, unsigned char value);
void Init_CHAR3D(CHAR3D *id, int row1, int row2, int col);
void Init_CHAR3D_Value(CHAR3D *id, int row1, int row2, int col, 
		       unsigned char value);
void Init_CHAR4D(CHAR4D *id, int row1, int row2, int col1, int col2);
void Init_CHAR4D_Value(CHAR4D *id, int row1, int row2, int col1, 
		      int col2, unsigned char value);
void Init_FLOAT1D(FLOAT1D *f1d, int row);
void Init_FLOAT1D_Value(FLOAT1D *f1d, int row, float value);
void Init_FLOAT2D(FLOAT2D *f2d, int row, int col);
void Init_FLOAT2D_Value(FLOAT2D *f2d, int row, int col, float value);
void Init_FLOAT3D(FLOAT3D *f3d, int row1, int row2, int col);
void Init_FLOAT3D_Value(FLOAT3D *f3d, int row1, int row2, int col, 
			float value);
void Init_FLOAT4D(FLOAT4D *f4d, int row1, int row2, int col1, int col2);
void Init_DOUBLE1D(DOUBLE1D *dd, int row);
void Init_DOUBLE1D_Value(DOUBLE1D *dd, int row, double value);
void Init_DOUBLE2D(DOUBLE2D *dd, int row, int col);
void Init_DOUBLE2D_Value(DOUBLE2D *dd, int row, int col, double value);
void Init_DOUBLE3D(DOUBLE3D *dd, int row1, int row2, int col);
void Init_DOUBLE3D_Value(DOUBLE3D *dd, int row1, int row2, int col, 
			 double value);
void Init_DOUBLE4D(DOUBLE4D *dd, int row1, int row2, int col1, int col2);
void Init_DOUBLE4D_Value(DOUBLE4D *dd, int row1, int row2, int col1, int col2,
			 double value);
void Free_INT1D(INT1D *i1d);
void Free_INT2D(INT2D *i2d);
void Free_INT3D(INT3D *id);
void Free_FLOAT1D(FLOAT1D *f1d);
void Free_FLOAT2D(FLOAT2D *f2d);
void Free_FLOAT3D(FLOAT3D *f3d);
void Free_FLOAT4D(FLOAT4D *f4d);
void Free_DOUBLE1D(DOUBLE1D *dd);
void Free_DOUBLE2D(DOUBLE2D *dd);
void Free_DOUBLE3D(DOUBLE3D *dd);
void Free_DOUBLE4D(DOUBLE4D *dd);
void Free_CHAR2D(CHAR2D *c2d);
void Free_CHAR3D(CHAR3D *c3d);
void Free_CHAR4D(CHAR4D *c4d);
INT2D *Load_INT2D(char *fName);
void Save_INT2D(INT2D *i2d, char *fName);
FLOAT2D *Load_FLOAT2D(char *fName);
void Save_INT3D(INT3D *id, char *fName);
INT3D *Load_INT3D(char *fName);
void Save_FLOAT2D(FLOAT2D *f2d, char *fName);
FLOAT3D *Load_FLOAT3D(char *fName);
void Save_FLOAT3D(FLOAT3D *f3d, char *fName);
FLOAT1D *Load_FLOAT1D_Ascii(char *fname);
FLOAT2D *Load_FLOAT2D_Ascii(char *fname);
void Save_Float1D(float *ptr, int n, char *fname);
float *Load_Float1D(char *fname, int *n);
FLOAT1D *Load_FLOAT1D(char *fname);
void Save_FLOAT1D(FLOAT1D *f1d, char *fname);
void Save_DOUBLE1D(DOUBLE1D *dd, char *fname);
DOUBLE1D *Load_DOUBLE1D(char *fname);
void Save_DOUBLE2D(DOUBLE2D *dd, char *fname);
DOUBLE2D *Load_DOUBLE2D(char *fname);
void Save_INT1D(INT1D *i1d, char *fname);
INT1D *Load_INT1D(char *fname);

#endif // TYPE_DEF_H
