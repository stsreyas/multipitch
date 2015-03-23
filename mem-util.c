#include <stdio.h>
#include "mem-util.h"
#include "file-util.h"

void *Allocate_Array(int row)
{
	void *dataptr;
	dataptr  = (void *) calloc((size_t) (row), sizeof(char));
	if (!dataptr) { 
		printf("\n Error: can't make 1-d array.") ;
		return(NULL); 
	}
	return dataptr;
}

void  **Allocate_Array2D(int row, int column)
{ 
	int i;
	void  **dataptr;
	dataptr  = (void **) calloc((size_t) (row), sizeof(char *));
	if (!dataptr) { printf("\n Error: can't make 2-d array.") ;
	return(NULL);
	}
	/* Allocating memeory for scanptr */
	for (i = 0; i < row; i++)
	{
		/* allocate memory for scanline */
		dataptr[i] = (char *) calloc((size_t) (column), 
			sizeof(char));
		/*Initialize the Data Buffer */
		if (!dataptr[i])
		{
			printf(" Error : can't make row %d in [%dx%d] matrix.\n",
				i,row,column);
			exit(1);
		}
	}
	return dataptr;
}

void  ***Allocate_Array3D(int row1, int row2, int column)
{ 
	int i;
	void  ***dataptr;
	dataptr  = (void ***) calloc((size_t) (row1), 
		sizeof(char **));
	if (!dataptr) { printf("\n Error: can't make 3-d array.") ;
	return(NULL);
	}
	/* Allocating memeory for scanptr */
	for (i = 0; i < row1; i++)
    {
		/* allocate memory for scanline */
		dataptr[i] =  Allocate_Array2D(row2,column);
		/*Initialize the Data Buffer */
		if (!dataptr[i])
        {
			printf(" Error : can't make row %d in [%d x %d x %d] matrix.\n",
				i,row1,row2, column);
			exit(1);
        }
    }
	return dataptr;
}

void  ****Allocate_Array4D(int row1, int row2, int col1, int col2)
{ 
	int i;
	void  ****dataptr;
	dataptr  = (void ****) calloc((size_t) (row1), 
		sizeof(char ***));
	if (!dataptr) { printf("\n Error: can't make 3-d array.") ;
	return(NULL);
	}
	/* Allocating memeory for scanptr */
	for (i = 0; i < row1; i++)
    {
		/* allocate memory for scanline */
		dataptr[i] =  Allocate_Array3D(row2,col1,col2);
		/*Initialize the Data Buffer */
		if (!dataptr[i])
        {
			printf(" Error : can't make row %d in [%d x %d x %d x %d] matrix.\n",
				i,row1,row2, col1, col2);
			exit(1);
        }
    }
	return dataptr;
}

void Free_Array(void *dataptr)
{
	free(dataptr);
}

void Free_Array2D(void **dataptr,int row)
{
	int i;
	for(i=0;i<row; i++)
    { if (dataptr[i])
	free(dataptr[i]);
    }
	free(dataptr);
}

void Free_Array3D(void ***dataptr,int row1,int row2)
{
	int i;
	for (i=0; i<row1; i++)
		if (dataptr[i])
			Free_Array2D(dataptr[i],row2);
		free(dataptr);
}

void Free_Array4D(void ****dataptr,int row1,int row2,int col1)
{
	int i;
	for (i=0; i<row1; i++)
		if (dataptr[i])
			Free_Array3D(dataptr[i],row2,col1);
		free(dataptr);
}

void Save_Array1D(void *dataptr, int row, FILE *fp)
{
	if (fwrite(dataptr, (size_t) 1, row, fp) != (unsigned int) row) { 
		fprintf(stderr, "Error when writing file.\n");
		exit(-1);
	}
}

void Save_Array2D(void **dataptr, int row, int col, FILE *fp)
{
	int i, j;
	for (i=0; i<row; i++) {
		j = fwrite(dataptr[i], (size_t) 1, col, fp);
		if (j!=col) {
			fprintf(stderr, "Error when writing file.\n");
			return;
		}
	}
}

void Save_Array3D(void ***dataptr, int row1, int row2, int col, FILE *fp)
{
	int i;
	for (i=0;i<row1;i++)
		Save_Array2D(dataptr[i], row2, col, fp);
}

void Load_Array1D(void *dataptr, int row, FILE *fp)
{
	if (fread(dataptr, (size_t) 1, row, fp) != (unsigned int) row) {
		fprintf(stderr, "Error when writing file.\n");
		exit(-1);
	}
}

void Load_Array2D(void **dataptr, int row, int col, FILE *fp)
{
	int i, j;
	for (i=0; i<row; i++) {
		j = fread(dataptr[i], (size_t) 1, col, fp);
		if (j!=col) {
			fprintf(stderr, "Error when writing file.\n");
			return;
		}
	}
}

void Load_Array3D(void ***dataptr, int row1, int row2, int col, FILE *fp)
{
	int i;
	
	for (i=0;i<row1;i++)
		Load_Array2D(dataptr[i], row2, col, fp);
}

void Save_Float2D_Ascii(float **ptr, int row, int col, char *fName)
{
	int i, j;
	FILE *fp;
	
	OPEN_FOR_WRITE_ASCII(fp,fName);
	
	for (i=0; i<row; i++) {
		for (j=0; j<col; j++)
			fprintf(fp, "%f ", ptr[i][j]);
		fprintf(fp,"\n");
	}
	fflush(fp);
	fclose(fp);
}

void Save_Float1D_Ascii(float *ptr, int col, char *fName)
{
	int j;
	FILE *fp;
	
	OPEN_FOR_WRITE_ASCII(fp,fName);
	
	for (j=0; j<col; j++)
		fprintf(fp, "%f ", ptr[j]);
	fprintf(fp,"\n");
	fflush(fp);
	fclose(fp);
}

float *ReadSignalFloatAscii(char *fName, int *numSamples1)
{
	FILE *fp;
	int numSamples=0;
	float *signal, Ftmp;
	
	OPEN_FOR_READ_ASCII(fp,fName);
	
	signal=(float*)malloc(sizeof(float)*MAX_SAMPLES);
	
	while (fscanf(fp,"%f",&Ftmp)!=EOF ) {
		signal[numSamples] = Ftmp;
		numSamples+=1;
	}
	
	fclose(fp);
	
	//fprintf(stderr,"processing %d samples...\n",numSamples);
	*numSamples1=numSamples;
	return signal;
}

void Save_Int1D_Ascii(int *ptr, int col, char *fName)
{
	int j;
	FILE *fp;
	
	OPEN_FOR_WRITE_ASCII(fp,fName);
	
	for (j=0; j<col; j++)
		fprintf(fp, "%d ", ptr[j]);
	fprintf(fp,"\n");
	fflush(fp);
	fclose(fp);
}

void Save_Int2D_Ascii(int **ptr, int row, int col, char *fName)
{
	int i, j;
	FILE *fp;
	
	OPEN_FOR_WRITE_ASCII(fp,fName);
	
	for (i=0; i<row; i++) {
		for (j=0; j<col; j++)
			fprintf(fp, "%d ", ptr[i][j]);
		fprintf(fp,"\n");
	}
	fflush(fp);
	fclose(fp);
}

void Save_unsignedInt2D_Ascii(unsigned int **ptr, int row, int col, 
							  char *fName)
{
	int i, j;
	FILE *fp;
	
	OPEN_FOR_WRITE_ASCII(fp,fName);
	
	for (i=0; i<row; i++) {
		for (j=0; j<col; j++)
			fprintf(fp, "%d ", ptr[i][j]);
		fprintf(fp,"\n");
	}
	fflush(fp);
	fclose(fp);
}


