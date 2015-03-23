#include "type-def.h"
#include "mem-util.h"
#include "file-util.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

void Init_INT1D(INT1D *f1d, int row)
{
	f1d->row=row;
	f1d->array=(int *)Allocate_Array(row*sizeof(int));
}

void Init_INT1D_Value(INT1D *i1d, int row, int value)
{
	int i;
	
	Init_INT1D(i1d, row);
	for (i=0;i<row;i++)
		i1d->array[i] = value;
}

void Init_INT3D_Value(INT3D *id, int row1, int row2, int col, int value)
{ 
	int i,j,k;
	
	Init_INT3D(id, row1, row2, col);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col;k++)
				id->array[i][j][k] = value;
}

void Init_INT2D_Value(INT2D *i2d, int row, int col, int value)
{
	int i,j;
	
	Init_INT2D(i2d, row, col);
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			i2d->array[i][j] = value;
}

void Init_FLOAT1D(FLOAT1D *f1d, int row)
{
	f1d->row=row;
	f1d->array=(float *)Allocate_Array(row*sizeof(float));
}

void Init_FLOAT1D_Value(FLOAT1D *f1d, int row, float value)
{
	int i;
	
	Init_FLOAT1D(f1d, row);
	for (i=0;i<row;i++)
		f1d->array[i] = value;
}

void Init_DOUBLE1D(DOUBLE1D *dd, int row)
{
	dd->row=row;
	dd->array=(double *)Allocate_Array(row*sizeof(double));
}

void Init_DOUBLE1D_Value(DOUBLE1D *dd, int row, double value)
{
	int i;
	
	Init_DOUBLE1D(dd, row);
	for (i=0;i<row;i++)
		dd->array[i] = value;
}

void Init_CHAR1D(CHAR1D *dd, int row)
{
	dd->row=row;
	dd->array=(unsigned char *)Allocate_Array(row*sizeof(unsigned char));
}

void Init_CHAR1D_Value(CHAR1D *dd, int row, unsigned char value)
{
	int i;
	
	Init_CHAR1D(dd, row);
	for (i=0;i<row;i++)
		dd->array[i] = value;
}

void Init_INT2D(INT2D *i2d, int row, int col)
{
	i2d->row=row;
	i2d->col=col;
	i2d->array=(int **)Allocate_Array2D(row, col*sizeof(int));
}

void Init_INT3D(INT3D *id, int row1, int row2, int col)
{
	id->row1 = row1;
	id->row2 = row2;
	id->col = col;
	id->array=(int ***)Allocate_Array3D(row1, row2, col*sizeof(int));
}

void Init_FLOAT2D(FLOAT2D *f2d, int row, int col)
{
	f2d->row=row;
	f2d->col=col;
	f2d->array=(float **)Allocate_Array2D(row, col*sizeof(float));
}

void Init_FLOAT2D_Value(FLOAT2D *f2d, int row, int col, float value)
{
	int i,j;
	
	Init_FLOAT2D(f2d, row, col);
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			f2d->array[i][j] = value;
}

void Init_DOUBLE2D(DOUBLE2D *dd, int row, int col)
{
	dd->row=row;
	dd->col=col;
	dd->array=(double **)Allocate_Array2D(row, col*sizeof(double));
}

void Init_DOUBLE2D_Value(DOUBLE2D *dd, int row, int col, double value)
{
	int i,j;
	
	Init_DOUBLE2D(dd, row, col);
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			dd->array[i][j] = value;
}

void Init_CHAR2D(CHAR2D *dd, int row, int col)
{
	dd->row=row;
	dd->col=col;
	dd->array=(unsigned char **)Allocate_Array2D(row, col*sizeof(unsigned char));
}

void Init_CHAR2D_Value(CHAR2D *dd, int row, int col, unsigned char value)
{
	int i,j;
	
	Init_CHAR2D(dd, row, col);
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			dd->array[i][j] = value;
}

void Init_FLOAT3D(FLOAT3D *f3d, int row1, int row2, int col)
{
	f3d->row1=row1;
	f3d->row2=row2;
	f3d->col=col;
	f3d->array=(float ***)Allocate_Array3D(row1, row2, col*sizeof(float));
}

void Init_FLOAT3D_Value(FLOAT3D *f3d, int row1, int row2, int col, float value)
{
	int i,j,k;
	
	Init_FLOAT3D(f3d, row1, row2, col);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col;k++)
				f3d->array[i][j][k] = value;
}

void Init_DOUBLE3D(DOUBLE3D *dd, int row1, int row2, int col)
{
	dd->row1=row1;
	dd->row2=row2;
	dd->col=col;
	dd->array=(double ***)Allocate_Array3D(row1, row2, col*sizeof(double));
}

void Init_DOUBLE3D_Value(DOUBLE3D *dd, int row1, int row2, int col, 
						 double value)
{
	int i,j,k;
	
	Init_DOUBLE3D(dd, row1, row2, col);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col;k++)
				dd->array[i][j][k] = value;
}

void Init_CHAR3D(CHAR3D *dd, int row1, int row2, int col)
{
	dd->row1=row1;
	dd->row2=row2;
	dd->col=col;
	dd->array=(unsigned char ***)Allocate_Array3D(row1, row2, col*sizeof(char));
}

void Init_CHAR3D_Value(CHAR3D *dd, int row1, int row2, int col, 
					   unsigned char value)
{
	int i,j,k;
	
	Init_CHAR3D(dd, row1, row2, col);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col;k++)
				dd->array[i][j][k] = value;
}

void Init_FLOAT4D(FLOAT4D *f4d, int row1, int row2, int col1, int col2)
{
	f4d->row1=row1;
	f4d->row2=row2;
	f4d->col1=col1;
	f4d->col2=col2;
	f4d->array=(float ****)Allocate_Array4D(row1, row2, col1, 
		col2*sizeof(float));
}

void Init_DOUBLE4D(DOUBLE4D *dd, int row1, int row2, int col1, int col2)
{
	dd->row1=row1;
	dd->row2=row2;
	dd->col1=col1;
	dd->col2=col2;
	dd->array=(double ****)Allocate_Array4D(row1, row2, col1, 
		col2*sizeof(double));
}

void Init_DOUBLE4D_Value(DOUBLE4D *dd, int row1, int row2, int col1, int col2,
						 double value)
{
	int i,j,k,l;
	
	Init_DOUBLE4D(dd, row1, row2, col1, col2);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col1;k++)
				for (l=0;l<col2;l++)
					dd->array[i][j][k][l] = value;
}

void Init_CHAR4D(CHAR4D *dd, int row1, int row2, int col1, int col2)
{
	dd->row1=row1;
	dd->row2=row2;
	dd->col1=col1;
	dd->col2=col2;
	dd->array=(unsigned char ****)Allocate_Array4D(row1, row2, col1, 
		col2*sizeof(char));
}

void Init_CHAR4D_Value(CHAR4D *dd, int row1, int row2, int col1, int col2,
					   unsigned char value)
{
	int i,j,k,l;
	
	Init_CHAR4D(dd, row1, row2, col1, col2);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col1;k++)
				for (l=0;l<col2;l++)
					dd->array[i][j][k][l] = value;
}

void Init_INT4D(INT4D *dd, int row1, int row2, int col1, int col2)
{
	dd->row1=row1;
	dd->row2=row2;
	dd->col1=col1;
	dd->col2=col2;
	dd->array=(int ****)Allocate_Array4D(row1, row2, col1, col2*sizeof(int));
}

void Init_INT4D_Value(INT4D *dd, int row1, int row2, int col1, int col2,
					  int value)
{
	int i,j,k,l;
	
	Init_INT4D(dd, row1, row2, col1, col2);
	for (i=0;i<row1;i++)
		for (j=0;j<row2;j++)
			for (k=0;k<col1;k++)
				for (l=0;l<col2;l++)
					dd->array[i][j][k][l] = value;
}

void Free_INT1D(INT1D *f1d)
{
	Free_Array((void *)(f1d->array));
}

void Free_INT2D(INT2D *i2d)
{
	Free_Array2D((void *)(i2d->array), i2d->row);
}

void Free_INT3D(INT3D *id)
{
	Free_Array3D((void *)(id->array), id->row1, id->row2);
}

void Free_INT4D(INT4D *dd)
{
	Free_Array4D((void ****)(dd->array), dd->row1, dd->row2, dd->col1);
}

void Free_FLOAT1D(FLOAT1D *f1d)
{
	Free_Array((void *)(f1d->array));
}

void Free_FLOAT2D(FLOAT2D *f2d)
{
	Free_Array2D((void **)(f2d->array), f2d->row);
}

void Free_FLOAT3D(FLOAT3D *f3d)
{
	Free_Array3D((void ***)(f3d->array), f3d->row1, f3d->row2);
}

void Free_FLOAT4D(FLOAT4D *f4d)
{
	Free_Array4D((void ****)(f4d->array), f4d->row1, f4d->row2, f4d->col1);
}

void Free_DOUBLE1D(DOUBLE1D *dd)
{
	Free_Array((void*)(dd->array));
}

void Free_DOUBLE2D(DOUBLE2D *dd)
{
	Free_Array2D((void **)(dd->array), dd->row);
}

void Free_DOUBLE3D(DOUBLE3D *dd)
{
	Free_Array3D((void ***)(dd->array), dd->row1, dd->row2);
}

void Free_DOUBLE4D(DOUBLE4D *dd)
{
	Free_Array4D((void ****)(dd->array), dd->row1, dd->row2, dd->col1);
}

void Free_CHAR2D(CHAR2D *c2d)
{
	Free_Array2D((void **)(c2d->array), c2d->row);
}

void Free_CHAR3D(CHAR3D *c3d)
{
	Free_Array3D((void ***)(c3d->array), c3d->row1, c3d->row2);
}
void Free_CHAR4D(CHAR4D *c4d)
{
	Free_Array4D((void ****)(c4d->array), c4d->row1, c4d->row2, c4d->col1);
}

INT2D *Load_INT2D(char *fName)
{
	FILE *fp;
	int row, col;
	// char string[64];
	INT2D *i2d;
	
	NEW(i2d);
	OPEN_FOR_READ(fp, fName);
	// fscanf(fp,"%s",string);
	//fscanf(fp,"%d %d\n", &row, &col);
	fscanf(fp,"%d",&row); fscanf(fp,"%d",&col);
	fgetc(fp);
	Init_INT2D(i2d, row, col);
	Load_Array2D((void **)i2d->array, row, col*sizeof(int), fp);
	fclose(fp);
	return(i2d);
}

INT3D *Load_INT3D(char *fName)
{
	FILE *fp;
	int row1, row2, col;
	// char string[64];
	INT3D *id;
	
	NEW(id);
	OPEN_FOR_READ(fp, fName);
	// fscanf(fp,"%s",string);
	//fscanf(fp,"%d %d %d\n", &row1, &row2, &col);
	fscanf(fp,"%d",&row1); fscanf(fp,"%d",&row2); fscanf(fp,"%d",&col);
	fgetc(fp);
	Init_INT3D(id, row1, row2, col);
	Load_Array3D((void ***)id->array, row1, row2, col*sizeof(int), fp);
	fclose(fp);
	return(id);
}

void Save_INT2D(INT2D *i2d, char *fName)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fName);
	fprintf(fp,"%d %d\n", i2d->row, i2d->col);
	Save_Array2D((void **)i2d->array, i2d->row, i2d->col*sizeof(int), fp);
	fclose(fp);
}

FLOAT2D *Load_FLOAT2D(char *fName) 
{
	FILE *fp;
	int row, col;
	//char string[64];
	FLOAT2D *f2d;
	
	NEW(f2d);
	OPEN_FOR_READ(fp, fName);
	//fscanf(fp,"%s",string);
	//fscanf(fp,"%d %d\n", &row, &col);
	fscanf(fp,"%d",&row); fscanf(fp,"%d",&col);
	//sscanf(string,"%d",&row); sscanf(string,"%d",&col);
	fgetc(fp);
	Init_FLOAT2D(f2d, row, col);
	Load_Array2D((void **)f2d->array, row, col*sizeof(float), fp);
	fclose(fp);
	return(f2d);
}

void Save_INT3D(INT3D *id, char *fName)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fName);
	fprintf(fp,"%d %d %d\n", id->row1, id->row2, id->col);
	Save_Array3D((void ***)id->array, id->row1, id->row2, 
	       id->col*sizeof(int), fp);
	fclose(fp);
}

DOUBLE2D *Load_DOUBLE2D(char *fName)
{
	FILE *fp;
	int row, col;
	// char string[64];
	DOUBLE2D *dd;
	
	NEW(dd);
	OPEN_FOR_READ(fp, fName);
	// fscanf(fp,"%s",string);
	//fscanf(fp,"%d %d\n", &row, &col);
	fscanf(fp,"%d",&row); fscanf(fp,"%d",&col);
	fgetc(fp);
	Init_DOUBLE2D(dd, row, col);
	Load_Array2D((void **)dd->array, row, col*sizeof(double), fp);
	fclose(fp);
	return(dd);
}

void Save_FLOAT2D(FLOAT2D *f2d, char *fName)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fName);
	fprintf(fp,"%d %d\n", f2d->row, f2d->col);
	Save_Array2D((void **)f2d->array, f2d->row, f2d->col*sizeof(float), fp);
	fclose(fp);
}

void Save_DOUBLE2D(DOUBLE2D *dd, char *fName)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fName);
	fprintf(fp,"%d %d\n", dd->row, dd->col);
	Save_Array2D((void **)dd->array, dd->row, dd->col*sizeof(double), fp);
	fclose(fp);
}

FLOAT3D *Load_FLOAT3D(char *fName) 
{
	FILE *fp;
	int row1, row2, col;
	// char string[64];
	FLOAT3D *f3d;
	
	NEW(f3d);
	OPEN_FOR_READ(fp, fName);
	//fscanf(fp,"%s",string);
	//fscanf(fp,"%d %d %d\n", &row1, &row2, &col);
	fscanf(fp,"%d",&row1); fscanf(fp,"%d",&row2); fscanf(fp,"%d",&col);
	fgetc(fp);
	Init_FLOAT3D(f3d, row1, row2, col);
	Load_Array3D((void ***)f3d->array, row1, row2, col*sizeof(float), fp);
	fclose(fp);
	return(f3d);
}

float *Load_Float1D(char *fname, int *n)
{
	FILE *fp;
	float *ptr;
	
	OPEN_FOR_READ(fp, fname);
	fscanf(fp,"%d", &n);
	fgetc(fp);
	ptr = (float*)calloc((size_t) *n, sizeof(float));
	Load_Array1D((void *)ptr, (*n)*sizeof(float), fp);
	fclose(fp);
	return ptr;
}

FLOAT1D *Load_FLOAT1D(char *fname)
{
	FILE *fp;
	FLOAT1D *fd;
	int n;
	
	NEW(fd);
	OPEN_FOR_READ(fp, fname);
	fscanf(fp,"%d", &n);
	fgetc(fp);
	Init_FLOAT1D(fd,n);
	Load_Array1D((void *)fd->array, n*sizeof(float), fp);
	fclose(fp);
	return fd;
}

void Save_FLOAT3D(FLOAT3D *f3d, char *fName)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fName);
	fprintf(fp,"%d %d %d\n", f3d->row1, f3d->row2, f3d->col);
	Save_Array3D((void ***)f3d->array, f3d->row1, f3d->row2, 
	       f3d->col*sizeof(float), fp);
	fclose(fp);
}

void Save_Float1D(float *ptr, int n, char *fname)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fname);
	fprintf(fp,"%d\n", n);
	Save_Array1D((void *)ptr, n*sizeof(float), fp);
	fclose(fp);
}

void Save_INT1D(INT1D *i1d, char *fname)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fname);
	fprintf(fp,"%d\n", i1d->row);
	Save_Array1D((void *)i1d->array, i1d->row*sizeof(int), fp);
	fclose(fp);
}

void Save_DOUBLE1D(DOUBLE1D *dd, char *fname)
{
	FILE *fp;
	
	OPEN_FOR_WRITE(fp, fname);
	fprintf(fp,"%d\n", dd->row);
	Save_Array1D((void *)dd->array, dd->row*sizeof(double), fp);
	fclose(fp);
}

INT1D *Load_INT1D(char *fname)
{
	FILE *fp;
	INT1D *id;
	int n;
	
	NEW(id);
	OPEN_FOR_READ(fp, fname);
	fscanf(fp, "%d", &n);
	fgetc(fp);
	Init_INT1D(id, n);
	Load_Array1D((void *)id->array, n*sizeof(int), fp);
	fclose(fp);
	return id;
}

DOUBLE1D *Load_DOUBLE1D(char *fname)
{
	FILE *fp;
	DOUBLE1D *dd;
	int n;
	
	NEW(dd);
	OPEN_FOR_READ(fp, fname);
	fscanf(fp, "%d", &n);
	fgetc(fp);
	Init_DOUBLE1D(dd, n);
	Load_Array1D((void *)dd->array, n*sizeof(double), fp);
	fclose(fp);
	return dd;
}

void Save_FLOAT1D(FLOAT1D *f1d, char *fname)
{
	Save_Float1D(f1d->array, f1d->row, fname);
}

FLOAT1D *Load_FLOAT1D_Ascii(char *fname)
{
	FLOAT1D *fd;
	
	NEW(fd);
	fd->array = ReadSignalFloatAscii(fname,&(fd->row));
	
	return fd;
}

FLOAT2D *Load_FLOAT2D_Ascii(char *fname)
{
	FILE *fp;
	FLOAT2D *f2d;
	int row, col, i,j;
	
	NEW(f2d);
	OPEN_FOR_READ(fp, fname);
	//fscanf(fp,"%d %d\n", &row, &col);
	fscanf(fp,"%d",&row); fscanf(fp,"%d",&col);
	fgetc(fp);
	Init_FLOAT2D(f2d, row, col);
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			fscanf(fp,"%f",&(f2d->array[i][j]));
		fclose(fp);
		
		return f2d;
}





