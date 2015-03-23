#if !defined(FILE_UTIL_H)
#define FILE_UTIL_H

#include <stdio.h>
#include "common.h"

#define OPEN_FOR_WRITE(fp,fName) \
{if ( ((fp)=fopen(fName,"wb")) == NULL) { \
fprintf(stderr, "can not open file '%s' for write.\n", fName); \
								 exit(-1);}}

#define OPEN_FOR_WRITE_ASCII(fp,fName) \
{if ( ((fp)=fopen(fName,"w")) == NULL) { \
fprintf(stderr, "can not open file '%s' for write.\n", fName); \
								 exit(-1);}}

#define OPEN_FOR_READ(fp,fName) \
{if ( ((fp)=fopen(fName,"rb")) == NULL) { \
fprintf(stderr, "can not open file '%s' for read.\n", fName); \
								exit(-1);}}

#define OPEN_FOR_READ_ASCII(fp,fName) \
{if ( ((fp)=fopen(fName,"r")) == NULL) { \
fprintf(stderr, "can not open file '%s' for read.\n", fName); \
								exit(-1);}}

int* readSignalAscii(char *filename, int *numSamples);

int* readSignalAsciiFloat(char *filename, int *numSamples);

int* readSignalBinary(char *filename, int *numSamples);


#endif // FILE_UTIL_H
