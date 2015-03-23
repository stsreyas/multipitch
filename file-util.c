#include "file-util.h"
#include "malloc.h"
#include <stdlib.h>
//#define FLOAT_INPUT

int* readSignalAscii(char *filename, int *numSamples1)
{
  FILE *inFile;
  int *signal, *signal_out;
  int numSamples=0;
  int Itmp;
  int i;
  
  if ((inFile=fopen(filename,"r"))==NULL) {
    printf("Can not open input file %s.\n", filename);
    exit(-1);
  }
  
  signal=(int*)malloc(sizeof(int)*MAX_SAMPLES);

#ifdef FLOAT_INPUT
  while (fscanf(inFile,"%f",&Ftmp)!=EOF ) {
    signal[numSamples] = (int)(Ftmp);
    numSamples+=1;
  }
#else
  while (fscanf(inFile,"%d",&Itmp)!=EOF ) {
    signal[numSamples] = Itmp;
    numSamples+=1;
  }
#endif

  fclose(inFile);
  
  //fprintf(stderr,"processing %d samples...\n",numSamples);
  *numSamples1=numSamples;

  signal_out=(int*)malloc(sizeof(int)*(numSamples+1));
  for (i=0;i<numSamples;i++)
    signal_out[i] = signal[i];

  free(signal);
  return signal_out;
}

int* readSignalAsciiFloat(char *filename, int *numSamples1)
{
  FILE *inFile;
  int *signal, *signal_out;
  int numSamples=0;
  float Ftmp;
  int i;
  
  if ((inFile=fopen(filename,"r"))==NULL) {
    printf("Can not open input file %s.\n", filename);
    exit(-1);
  }
  
  signal=(int*)malloc(sizeof(int)*MAX_SAMPLES);

  while (fscanf(inFile,"%f",&Ftmp)!=EOF ) {
    signal[numSamples] = (int)(Ftmp);
    numSamples+=1;
  }

  fclose(inFile);
  
  fprintf(stderr,"processing %d samples...\n",numSamples);
  *numSamples1=numSamples;

  signal_out=(int*)malloc(sizeof(int)*(numSamples+1));
  for (i=0;i<numSamples;i++)
    signal_out[i] = signal[i];

  free(signal);
  return signal_out;
}

int *readSignalBinary(char *filename, int *numSamples1)
{
  FILE *inFile;
  int *signal;
  int numSamples=0;
  
  if ((inFile=fopen(filename,"r"))==NULL) {
    printf("Can not open input file %s.\n", filename);
    exit(-1);
  }
  
  signal=(int*)malloc(sizeof(int)*MAX_SAMPLES);

  while (fread(&signal[numSamples],sizeof(int),1,inFile)==1) {
    numSamples+=1;
  }
  fclose(inFile);
  
  fprintf(stderr,"processing %d samples...\n",numSamples);
  *numSamples1=numSamples;
  return signal;
}
