#include "stats-model-log.h"
#include "math-util.h"
#include <string.h>
#include "common.h"
#include "multiband-filtering.h"
#include "mem-util.h"
#include "MeddisRate.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define FACTOR 4.0 // 200.0

#ifdef ZERO_POINT
#undef ZERO_POINT
#endif
#define ZERO_POINT (MAX_DELAY_PITCH)

static float offPkLogPrb1[NUM_CHANNEL][ZERO_POINT*2];
static float offPkLogPrb2[NUM_CHANNEL][ZERO_POINT*2];
static double noSigPrb[NUM_CHANNEL];
static float a1[NUM_CHANNEL], a2[NUM_CHANNEL];

#define getOffPkLogPrb1(CHAN,OFFSET) offPkLogPrb1[CHAN][ZERO_POINT+OFFSET]
#define getOffPkLogPrb2(CHAN,OFFSET) offPkLogPrb2[CHAN][ZERO_POINT+OFFSET]

extern channel cochlea[NUM_CHANNEL];

void Init_Log_Prob(void)
{
	FLOAT1D *center_f;
	int chan, i;
	float k;
	double prob_tmp1, prob_tmp2;
	float q11 = 0.0164, q12 = 0.0161, q21 = 0.0626, q22 = 0.1080; // ICASSP
	
	// init channels for the center frequency
	initOuterMiddleEar();
	initChannels(LOWER_CF, UPPER_CF, NUM_CHANNEL);
	initHairCells();
	NEW(center_f);
	Init_FLOAT1D(center_f,NUM_CHANNEL);

	for (chan=0;chan<NUM_CHANNEL;chan++)
		center_f->array[chan] = cochlea[chan].cf;
	
	// init a[chan], T[chan] normalized laplacian w/ + uniform estimation
	k = 1.0;
	for (chan=0;chan<55;chan++) {
		a1[chan] = -0.0105*(float)chan+1.1950; // ICASSP Training set only
		a2[chan] = -0.0178*(float)chan+1.5433; // ICASSP
		
		noSigPrb[chan] = 1.0/(float)(SAMPLING_FREQUENCY/center_f->array[chan]);
		prob_tmp1 = 0.0; prob_tmp2 = 0.0;
		for (i=0;i<ZERO_POINT*2;i++) {
			prob_tmp1 += pow(exp(-(double)(abs(ZERO_POINT-i))
				/a1[chan]),k)/a1[chan];
			prob_tmp2 += pow(exp(-(double)(abs(ZERO_POINT-i))
				/a2[chan]),k)/a2[chan];
		}
		for (i=0;i<ZERO_POINT*2;i++) {
			offPkLogPrb1[chan][i] = 
				log((1.0-q11)*pow(exp(-(double)(abs(ZERO_POINT-i))/a1[chan]),k)
				/a1[chan]/prob_tmp1 + q11*noSigPrb[chan]);
			offPkLogPrb2[chan][i] = 
				log((1.0-q12)*pow(exp(-(double)(abs(ZERO_POINT-i))/a2[chan]),k)
				/a2[chan]/prob_tmp2 + q12*noSigPrb[chan]);
		}
	}
	
	for (chan=55;chan<NUM_CHANNEL;chan++) {
		a1[chan] = -0.0084*(float)chan+2.5943; // ICASSP 
		a2[chan] = -0.0162*(float)chan+3.5661; // ICASSP
		
		noSigPrb[chan] = 1.0/(float)(MAX_DELAY_PITCH-MIN_DELAY);
		prob_tmp1 = 0.0; prob_tmp2 = 0.0;
		for (i=0;i<ZERO_POINT*2;i++) {
			prob_tmp1 += pow(exp(-(double)(abs(ZERO_POINT-i))
				/a1[chan]),k)/a1[chan];
			prob_tmp2 += pow(exp(-(double)(abs(ZERO_POINT-i))
				/a2[chan]),k)/a2[chan];
		}
		for (i=0;i<ZERO_POINT*2;i++) {
			offPkLogPrb1[chan][i] = 
				log((1.0-q21)*pow(exp(-(double)(abs(ZERO_POINT-i))/a1[chan]),k)
				/a1[chan]/prob_tmp1 + q21*noSigPrb[chan]);
			offPkLogPrb2[chan][i] = 
				log((1.0-q22)*pow(exp(-(double)(abs(ZERO_POINT-i))/a2[chan]),k)
				/a2[chan]/prob_tmp2 + q22*noSigPrb[chan]);
		}
	}
	
	fprintf(stderr, "big = %f, small = %f.\n", getOffPkLogPrb1(10,0), 
		getOffPkLogPrb1(10,180));
}

FLOAT1D *Log_Prob_0_Pitch(FLOAT3D *corr, INT2D *chan_selected,
						  INT3D *valid_peaks)
{
	FLOAT1D *logProb;
	int timeFrame;
	
	NEW(logProb);
	Init_FLOAT1D_Value(logProb, chan_selected->row, -MAXFLOAT);
	
	for (timeFrame=0; timeFrame<chan_selected->row; timeFrame++) 
		logProb->array[timeFrame] = 0.0;
	
	return logProb;
}

FLOAT2D *Log_Prob_1_Pitch(FLOAT3D *corr, INT2D *chan_selected,
						  INT3D *valid_peaks)
{
	FLOAT2D *logProb;
	int timeFrame, chan, delay;
	int delta, indPeak;
	
	NEW(logProb);
	Init_FLOAT2D_Value(logProb, chan_selected->row, MAX_DELAY_PITCH, -MAXFLOAT);
	
	for (timeFrame=0; timeFrame<chan_selected->row; timeFrame++) {    
		for (delay = MIN_DELAY;delay<MAX_DELAY_PITCH;delay++) {
			logProb->array[timeFrame][delay] = 0.0;
			for (chan=0; chan<NUM_CHANNEL; chan++) 
				if (chan_selected->array[timeFrame][chan] > 0) {
					delta = 500;
					for (indPeak=0;indPeak<chan_selected->array[timeFrame][chan];
					indPeak++)
						if (abs(delta) > abs(valid_peaks->array[timeFrame][chan][indPeak] - delay))
							delta = valid_peaks->array[timeFrame][chan][indPeak] - delay;
						logProb->array[timeFrame][delay] += 
							getOffPkLogPrb1(chan,delta) + FACTOR;
				} else
					logProb->array[timeFrame][delay] += getOffPkLogPrb1(chan,180) + FACTOR;
		}
	}
	
	return logProb;
}

FLOAT2D *Log_Prob_2_Pitch_1_Frame(FLOAT3D *corr, INT2D *chan_selected,
								  INT3D *valid_peaks, int frame)
{
	int chan, delay1, delay2, delay;
	FLOAT2D *logProb;
	int timeFrame;
	int indPeak;
	int delta[NUM_CHANNEL][MAX_DELAY_PITCH];
	
	NEW(logProb);
	Init_FLOAT2D_Value(logProb, MAX_DELAY_PITCH, MAX_DELAY_PITCH, -MAXFLOAT);
	
	timeFrame = frame;
	
	for (chan=0;chan<NUM_CHANNEL;chan++)
		if (chan_selected->array[timeFrame][chan] >  0) {
			for (delay=MIN_DELAY;delay<MAX_DELAY_PITCH;delay++) {
				delta[chan][delay] = 500;
				for (indPeak=0;indPeak<chan_selected->array[timeFrame][chan];indPeak++)
					if (abs(delta[chan][delay]) > 
						abs(valid_peaks->array[timeFrame][chan][indPeak] - delay))
						delta[chan][delay] = valid_peaks->array[timeFrame][chan][indPeak] - delay;
			}
		} else {
			for (delay=MIN_DELAY;delay<MAX_DELAY_PITCH;delay++)
				delta[chan][delay] = 500;
		}
		
		for (delay1 = MIN_DELAY;delay1<MAX_DELAY_PITCH;delay1++) {
			for (delay2 = MIN_DELAY;delay2<MAX_DELAY_PITCH;delay2++) {	
				logProb->array[delay1][delay2] = 0.0;
				for (chan=0;chan<NUM_CHANNEL;chan++) {
					if (chan_selected->array[timeFrame][chan] >  0) {				
						if (abs(delta[chan][delay1]) < a2[chan]*Beta) { // delta1 close
							logProb->array[delay1][delay2] += getOffPkLogPrb2(chan,delta[chan][delay1]) 
								+ FACTOR;
						} else
							logProb->array[delay1][delay2] +=
							fmax(getOffPkLogPrb2(chan,delta[chan][delay1]),
							getOffPkLogPrb2(chan,delta[chan][delay2])) + FACTOR;
					} else {
						logProb->array[delay1][delay2] += getOffPkLogPrb2(chan,180) + FACTOR;
					}
				}
			}
		}
		
		
		for (delay2 = MIN_DELAY;delay2<MAX_DELAY_PITCH;delay2++) 
			for (delay1 = MIN_DELAY;delay1<MAX_DELAY_PITCH;delay1++)	
				logProb->array[delay1][delay2] = fmax(logProb->array[delay1][delay2],
				logProb->array[delay2][delay1]);

			return logProb;
}

