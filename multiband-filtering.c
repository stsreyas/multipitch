#include "multiband-filtering.h"
#include "filter.h"
#include "MeddisRate.h"
#include "common.h"
#include "mem-util.h"
#include "math-util.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

extern channel cochlea[NUM_CHANNEL];

FLOAT2D *Gammatone_Filtering(int* signal, int numSamples)
{
	FLOAT2D *sigFiltered;
	int timeStep, chan;

	NEW(sigFiltered);
	Init_FLOAT2D(sigFiltered, NUM_CHANNEL, numSamples);
   
	// init Gammatone filters
	initOuterMiddleEar();
	initChannels(LOWER_CF, UPPER_CF, NUM_CHANNEL);
	initHairCells();
  
	printf("Gammatone filtering.\n");

	for (chan=0;chan<NUM_CHANNEL;chan++) {
		blip();
		for (timeStep=0; timeStep<numSamples; timeStep++)
		sigFiltered->array[chan][timeStep] = 
		updateCochleaWithoutTransduction(chan, 
			(float)signal[timeStep], timeStep);
	}

	printf("Gammatone filtering done.\n");

	return sigFiltered;
}

void Cal_Envelope(FLOAT2D *sigFiltered)
{
	int timeStep,chan;
	float a[4], b[4];
	float *tmp_y, *p_tmp;
	FLOAT1D *fir_b;

	fprintf(stderr, "\nComputing envelop.\n");

	//800Hz
	b[0] = 2.8982e-3; b[1] = 8.6946e-3; b[2] = 8.6946e-3; b[3] = 2.8982e-3;
	a[0] = 1.0; a[1] = -2.3741; a[2] = 1.9294; a[3] = -0.5321;

	// order of 256 (257 samples) FIR highpass cutoff = 64Hz a(56) = 0
	// a(64) = 0.5 a(72) = 1
	fir_b = Load_FLOAT1D_Ascii("data\\important_data\\FIR.dat");

	ALLOCATE_ARRAY(tmp_y, sigFiltered->col);
  
	for (chan=0;chan<NUM_CHANNEL;chan++) {
		blip();
		if (cochlea[chan].cf>HIGH_CHANNEL_FREQUENCY) {
			tmp_y[0] = 0.0; 
			tmp_y[sigFiltered->col-1] = 0.0; 
			for (timeStep=1;timeStep<sigFiltered->col-1;timeStep++)
				tmp_y[timeStep] = sqr(sigFiltered->array[chan][timeStep]) -
					sigFiltered->array[chan][timeStep-1]*sigFiltered->array[chan][timeStep+1];
			p_tmp = tmp_y;
			tmp_y = sigFiltered->array[chan];
			sigFiltered->array[chan] = p_tmp;

			// 3rd Butterworth filter low pass at 250Hz
			filter(b, 4, a, 4, sigFiltered->array[chan], sigFiltered->col, tmp_y);
			p_tmp = tmp_y;
			tmp_y = sigFiltered->array[chan];
			sigFiltered->array[chan] = p_tmp;
      
		}
		
		// high pass filtering
		FIR_Filter(fir_b->array, fir_b->row, sigFiltered->array[chan], 
			sigFiltered->col, tmp_y);
		p_tmp = tmp_y;
		tmp_y = sigFiltered->array[chan];
		sigFiltered->array[chan] = p_tmp;
    
	}

	fprintf(stderr, "\nDone computing envelop.\n");
}

FLOAT2D *Realign_Multiband_Sig(FLOAT2D *sigFiltered)
{
	FLOAT2D *sigRealigned;
	int max_realignment=0, i, chan;
	INT1D *realignment;

	fprintf(stderr, "\nCompensating delays.\n");

	realignment = Load_INT1D(GAMMATONE_DELAY_FILE);

	for (i=0;i<realignment->row;i++)
		if (max_realignment<realignment->array[i])
			max_realignment=realignment->array[i];

	NEW(sigRealigned);
	Init_FLOAT2D(sigRealigned, sigFiltered->row, 
	       START_POINT + sigFiltered->col - max_realignment);

	for (chan=0;chan<sigFiltered->row;chan++)
		for (i=0;i<START_POINT;i++)
			sigRealigned->array[chan][i] = 0.0;

	for (chan=0;chan<sigFiltered->row;chan++) {
		blip();
		for (i=START_POINT-realignment->array[chan];i<sigRealigned->col;i++)
			sigRealigned->array[chan][i] = 
				sigFiltered->array[chan][i-START_POINT+realignment->array[chan]];
	}

	Free_FLOAT2D(sigFiltered);

	fprintf(stderr, "\nDone compensating delays.\n");

	return sigRealigned;
}

FLOAT3D *Cal_Correlogram(FLOAT2D *sigRealigned)
{
	int timeFrame, chan, delay, numTimeFrame;
	FLOAT3D *corr;
	int timeStep;
	double tmp_corr, tmp_corr1, tmp_corr2;
	
	fprintf(stderr, "\nComputing scalar product.\n");
	
	numTimeFrame=floor((sigRealigned->col-START_POINT)/TIME_FRAME)+1;
	NEW(corr);
	Init_FLOAT3D(corr, numTimeFrame, NUM_CHANNEL, MAX_DELAY);
	
	for (chan=0;chan<NUM_CHANNEL;chan++) {
		blip();
		for (timeFrame=0;timeFrame<numTimeFrame;timeFrame++) {
			tmp_corr1 = 0.; 
			for (timeStep=-NEW_WINDOW_HALF;timeStep<NEW_WINDOW_HALF;timeStep++)
				tmp_corr1 += sqr(sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME+
				timeStep]);
			tmp_corr1 = sqrt(tmp_corr1);
			for (delay=0;delay<MAX_DELAY;delay++) {
				tmp_corr = 0.; tmp_corr2 = 0.0;
				for (timeStep=-NEW_WINDOW_HALF;timeStep<NEW_WINDOW_HALF;timeStep++) {
					tmp_corr += 
						sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME+timeStep]*
						sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME-
						delay+timeStep];
					tmp_corr2 += 
						sqr(sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME-
						delay+timeStep]);
				}
				if (REALZEROP(tmp_corr1) || REALZEROP(tmp_corr2))
					corr->array[timeFrame][chan][delay] = 0.0;
				else
					corr->array[timeFrame][chan][delay] = tmp_corr/sqrt(tmp_corr2)/tmp_corr1;
			}
		}
	}
	
	fprintf(stderr, "\nDone computing scalar product.\n");
	
	return corr;
}

FLOAT3D *Cal_Correlogram_Large_Window(FLOAT2D *sigRealigned)
{
	int timeFrame, chan, delay, numTimeFrame;
	FLOAT3D *corr;
	int timeStep;
	double tmp_corr, tmp_corr1, tmp_corr2;
	int starting_chan = 0;
	
	numTimeFrame=floor((sigRealigned->col-START_POINT)/TIME_FRAME)+1;
	NEW(corr);
	Init_FLOAT3D(corr, numTimeFrame, NUM_CHANNEL, MAX_DELAY);
	
	for (chan=1;chan<NUM_CHANNEL;chan++)
		if (cochlea[chan].cf>HIGH_CHANNEL_FREQUENCY && 
			cochlea[chan-1].cf<=HIGH_CHANNEL_FREQUENCY) 
			starting_chan = chan;
		
		printf("Computing scalar product w/ larger window.\n");
		
		for (chan=starting_chan;chan<NUM_CHANNEL;chan++) {
			blip();
			for (timeFrame=0;timeFrame<numTimeFrame;timeFrame++) {
				tmp_corr1 = 0.; 
				for (timeStep=-NEW_LARGE_WINDOW_HALF;timeStep<NEW_LARGE_WINDOW_HALF;timeStep++)
					tmp_corr1 += sqr(sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME+
					timeStep]);
				tmp_corr1 = sqrt(tmp_corr1);
				for (delay=0;delay<MAX_DELAY;delay++) {
					tmp_corr = 0.; tmp_corr2 = 0.0;
					for (timeStep=-NEW_LARGE_WINDOW_HALF;timeStep<NEW_LARGE_WINDOW_HALF;
					timeStep++) {
						tmp_corr += 
							sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME+timeStep]*
							sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME-
							delay+timeStep];
						tmp_corr2 += 
							sqr(sigRealigned->array[chan][START_POINT+timeFrame*TIME_FRAME-
							delay+timeStep]);
					}
					if (REALZEROP(tmp_corr1) || REALZEROP(tmp_corr2))
						corr->array[timeFrame][chan][delay] = 0.0;
					else
						corr->array[timeFrame][chan][delay] = tmp_corr/sqrt(tmp_corr2)/tmp_corr1;
				}
			}
		}
		
		printf("Done Computing scalar product w/ larger window.\n");
		
		return corr;
}

INT2D *Peak_Chan_Selection(FLOAT3D *corr1, FLOAT3D *corr2, INT3D *valid_peaks)
{
	INT2D *chan_selected;
	int timeFrame, chan, delay, delay2, max_delay, max_delay2;
	float maxval1, maxval2, max_corr;
	int flag, flag1;
	float th_low = 0.95, th_mid = 0.85, th_mid_high = 0.85; 
	
	NEW(chan_selected);
	Init_INT2D(chan_selected, corr1->row1, corr1->row2);
	
	Init_INT3D(valid_peaks, corr1->row1, corr1->row2, 30);
	
	initOuterMiddleEar();
	initChannels(LOWER_CF, UPPER_CF, NUM_CHANNEL);
	initHairCells();
	
	//fprintf(stderr, "%f\n", cochlea[10].cf);
	for (timeFrame=0; timeFrame<corr1->row1; timeFrame++) {
		for (chan=0; chan<corr1->row2;chan++) {
			if (cochlea[chan].cf<=HIGH_CHANNEL_FREQUENCY) {
				// find peak
				max_corr = -1;
				for (delay=2;delay<MAX_DELAY_PITCH;delay++) { // 50 2
					if (corr1->array[timeFrame][chan][delay] >
						corr1->array[timeFrame][chan][delay-1] &&
						corr1->array[timeFrame][chan][delay] >
						corr1->array[timeFrame][chan][delay+1] &&
						max_corr < corr1->array[timeFrame][chan][delay])
						max_corr = corr1->array[timeFrame][chan][delay];
				}
				
				//if (max_corr>0.97-cochlea[chan].cf/5000.0  //0.97 5000
				if (max_corr>0.945) {
					chan_selected->array[timeFrame][chan] = 0;
					
					for (delay=MIN_DELAY;delay<MAX_DELAY_PITCH;delay++) {
						if (corr1->array[timeFrame][chan][delay] >
							corr1->array[timeFrame][chan][delay-1] &&
							corr1->array[timeFrame][chan][delay] >
							corr1->array[timeFrame][chan][delay+1] &&
							corr1->array[timeFrame][chan][delay] > -10.0) { // 0.6
							valid_peaks->array[timeFrame][chan][chan_selected->array[timeFrame][chan]]
								= delay;
							(chan_selected->array[timeFrame][chan])++;
						}
					}
					//chan_selected->array[timeFrame][chan] = 1;
				} else
					chan_selected->array[timeFrame][chan] = 0;
			} else {
				// High frequency check
				//chan_selected->array[timeFrame][chan] = 1;
				flag = 0; flag1 = 0;
				// check all peaks
				
				for (delay=MIN_DELAY+1;delay<MAX_DELAY_PITCH-1;delay++) {
					//for (delay=MIN_DELAY+1;delay<300;delay++) {
					if (corr1->array[timeFrame][chan][delay] >
						corr1->array[timeFrame][chan][delay-1] &&
						corr1->array[timeFrame][chan][delay] >
						corr1->array[timeFrame][chan][delay+1] && 
						corr1->array[timeFrame][chan][delay] > -10.0) { // 0.1
						flag = 1;
						maxval1 = corr1->array[timeFrame][chan][delay];
						max_delay = delay;
						maxval2 = -MAXFLOAT; max_delay2 = max_delay;
						for (delay2=max_delay-10;delay2<=max_delay+10;delay2++)
							if (corr2->array[timeFrame][chan][delay2] > maxval2) {
								maxval2 = corr2->array[timeFrame][chan][delay2];
								max_delay2 = delay2;
							}
							if (abs(max_delay-max_delay2)>2 // || fabs(maxval1-maxval2)>0.25
								) //2 0.15
								flag1 = 1;
					}
				}
				
				
				if (flag == 0 || flag1 == 1)
					chan_selected->array[timeFrame][chan] = 0;
				else {
					chan_selected->array[timeFrame][chan] = 0;
					for (delay=MIN_DELAY;delay<MAX_DELAY_PITCH;delay++) {
						if (corr1->array[timeFrame][chan][delay] >
							corr1->array[timeFrame][chan][delay-1] &&
							corr1->array[timeFrame][chan][delay] >
							corr1->array[timeFrame][chan][delay+1] &&
							corr1->array[timeFrame][chan][delay] > 0.2) { //0.2 0.0
							
							
							flag = 0;
							max_delay = delay;
							for (delay2=2*max_delay-5;delay2<=2*max_delay+5;delay2++) { //+-5
								if (corr1->array[timeFrame][chan][delay2] >
									corr1->array[timeFrame][chan][delay2-1] &&
									corr1->array[timeFrame][chan][delay2] >
									corr1->array[timeFrame][chan][delay2+1] &&
									corr1->array[timeFrame][chan][delay2] > 0.2) // 0.2 0.0
									flag = 1;
							}
							//flag = 1;
							if (flag == 1) {
								valid_peaks->array[timeFrame][chan][chan_selected->array[timeFrame][chan]]
									= delay;
								(chan_selected->array[timeFrame][chan])++;
							}
						}
					}
					
					if (chan_selected->array[timeFrame][chan] > 0 &&
						corr1->array[timeFrame][chan][valid_peaks->array[timeFrame][chan][0]] > 0.6) // 0.8
						chan_selected->array[timeFrame][chan] = 1;
				}
			}
		}
	}
	
	return chan_selected;
}
