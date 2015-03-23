#if !defined(MEDDIS_RATE_H)
#define MEDDIS_RATE_H

#include "common.h"

void initOuterMiddleEar();

// function prototypes

void initChannels(int lowerCF, int upperCF, int numChannels);
// initialise filterbank channels

double updateCochlea(int chan_index, float sigval, int time);
// process one sample of the input though the cochlea

double updateCochleaWithoutTransduction(int chan_index, 
					float sigval, int time);
// passing through Gamma tone filter

void initHairCells(void);
// initialise the meddis hair cell parameters

void blip(void);

typedef struct { 
  double cf, bw, criticalRate, z, gain, expCoeff;
  double midEarCoeff;
  double p0, p1, p2, p3, p4;
  double q0, q1, q2, q3, q4;
  double u0, u1; 
  double v0, v1; 
  double c,q,w;
  double rate;
} channel;

channel *getChannels(void);

#endif // MEDDIS_RATE_H
