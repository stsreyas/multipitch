#if !defined(MEM_UTIL_H)
#define MEM_UTIL_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

// Macros
//#define NEW(A) (A)=(typeof(A))malloc(sizeof(*A))
#define NEW(A) (A)=malloc(sizeof(*A))

//#define ALLOCATE_ARRAY(A,B) (A)=(typeof(A))calloc((size_t)(B),sizeof(*A))
#define ALLOCATE_ARRAY(A,B) (A)=calloc((size_t)(B),sizeof(*A))

#define INIT_CHAIN(A) {NEW(A); NEW(A##_all); (A##_all)->row = 0;}

void *Allocate_Array(int row);
void **Allocate_Array2D(int row, int column);
void ***Allocate_Array3D(int row1, int row2, int column);
void ****Allocate_Array4D(int row1, int row2, int col1, int col2);
void Free_Array(void *dataptr);
void Free_Array2D(void **dataptr,int row);
void Free_Array3D(void ***dataptr,int row1,int row2);
void Free_Array4D(void ****dataptr, int row1, int row2, int col1);
void Save_Array1D(void *dataptr, int row, FILE *fp);
void Save_Array2D(void **dataptr, int row, int col, FILE *fp);
void Save_Array3D(void ***dataptr, int row1, int row2, int col, FILE *fp);
void Load_Array1D(void *dataptr, int row, FILE *fp);
void Load_Array2D(void **dataptr, int row, int col, FILE *fp);
void Load_Array3D(void ***dataptr, int row1, int row2, int col, FILE *fp);

void Save_Float2D_Ascii(float** ptr, int row, int col, char *fName);
void Save_Float1D_Ascii(float* ptr, int col, char *fName);
float *ReadSignalFloatAscii(char *fName, int *col);
void Save_Int1D_Ascii(int* ptr, int row, char *fName);
void Save_Int2D_Ascii(int** ptr, int row, int col, char *fName);
void Save_unsignedInt2D_Ascii(unsigned int **ptr, int row, int col, 
			      char *fName);

#endif // MEM_UTIL_H
