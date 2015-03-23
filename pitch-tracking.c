#include "pitch-tracking.h"

#define LOW_CHANNEL 55

#define LIMIT 30

#ifdef ZERO_POINT
#undef ZERO_POINT
#endif
#define ZERO_POINT 200

float ptchDynmOnly[ZERO_POINT*2];
float logPtchDynmOnly[ZERO_POINT*2];
float ptchDynm2dOnly[ZERO_POINT*2][ZERO_POINT*2];
float logPtchDynm2dOnly[ZERO_POINT*2][ZERO_POINT*2];

#define getPtchDynm(OFFSET) ptchDynmOnly[ZERO_POINT+OFFSET]
#define getPtchDynm2d(OFFSET1,OFFSET2)  \
ptchDynm2dOnly[ZERO_POINT+OFFSET1][ZERO_POINT+OFFSET2]
#define getLogPtchDynm2d(OFFSET1,OFFSET2)  \
logPtchDynm2dOnly[ZERO_POINT+OFFSET1][ZERO_POINT+OFFSET2]

// 0 to *
float logPDynm0To0New;
float logPDynm0To1New; 
// 1 to *
float logPDynm1To0New;
FLOAT2D *logPDynm1To1New; 
FLOAT3D *logPDynm1To2New;
// 2 to *
FLOAT3D *logPDynm2To1New;

float logPDynm2To2Para;

#define MAX_SEQ 12000

// Initialize the pitch dynamics probabilities
void Init_Pitch_Tracking(float p0To1, float p1To0, float p1To2,
	float p2To1, float mean, float aa)

{
	int i,j,i1,i2,j1,j2;
	float tmp;

	tmp = 0.0;
	for (j=0;j<ZERO_POINT*2;j++) {
		// Laplacian
		ptchDynmOnly[j] = (float)exp(-fabs((double)(j-ZERO_POINT)-mean)/aa);
		tmp += ptchDynmOnly[j];
	}
	
	for (j=0;j<ZERO_POINT*2;j++) {
		ptchDynmOnly[j] /= tmp;
		logPtchDynmOnly[j] = (float)log(ptchDynmOnly[j]);
	}
  
	tmp = 0.0;
	for (i=0;i<ZERO_POINT*2;i++) 
		for (j=0;j<ZERO_POINT*2;j++) {
			ptchDynm2dOnly[i][j] = ptchDynmOnly[i]*ptchDynmOnly[j];
			tmp += ptchDynm2dOnly[i][j];
		}

	for (i=0;i<ZERO_POINT*2;i++) 
		for (j=0;j<ZERO_POINT*2;j++) {
			ptchDynm2dOnly[i][j] /= tmp;
			logPtchDynm2dOnly[i][j] = (float)log(ptchDynm2dOnly[i][j]);
		}
			
	// 0 to *  
	logPDynm0To0New = (float)log(1.0-p0To1);
	logPDynm0To1New = (float)log(p0To1/(double)(MAX_DELAY_PITCH-MIN_DELAY));
			
	// 1 to *
	logPDynm1To0New = (float)log(p1To0);
			
	NEW(logPDynm1To1New);
	Init_FLOAT2D_Value(logPDynm1To1New, MAX_DELAY_PITCH, MAX_DELAY_PITCH, 0.0);
	for (i=MIN_DELAY;i<MAX_DELAY_PITCH;i++) {
		for (j=MIN_DELAY;j<MAX_DELAY_PITCH;j++) {      
			logPDynm1To1New->array[i][j] = (float)log(getPtchDynm(j-i)*(1.0-p1To0-p1To2));
		}
	}
			
	NEW(logPDynm1To2New);
	Init_FLOAT3D_Value(logPDynm1To2New, MAX_DELAY_PITCH, MAX_DELAY_PITCH, 
			MAX_DELAY_PITCH, 0.0);
	for (i=MIN_DELAY;i<MAX_DELAY_PITCH;i++) {
		for (j1=MIN_DELAY;j1<MAX_DELAY_PITCH;j1++) 
			for (j2=MIN_DELAY;j2<MAX_DELAY_PITCH;j2++) {  
				logPDynm1To2New->array[i][j1][j2] = 
					(float)log(getPtchDynm(j1-i)*p1To2/(double)(MAX_DELAY_PITCH-MIN_DELAY));
			}
	}
			
	// 2 to *
	NEW(logPDynm2To1New);
	Init_FLOAT3D_Value(logPDynm2To1New, MAX_DELAY_PITCH, MAX_DELAY_PITCH, 
		MAX_DELAY_PITCH, 0.0);
	for (i1=MIN_DELAY;i1<MAX_DELAY_PITCH;i1++) 
		for (i2=MIN_DELAY;i2<MAX_DELAY_PITCH;i2++) {
			for (j=MIN_DELAY;j<MAX_DELAY_PITCH;j++) {
				logPDynm2To1New->array[i1][i2][j] = 
					(float)log(fmax(getPtchDynm(j-i1),getPtchDynm(j-i2))*p2To1);
			}
		}    
				
	logPDynm2To2Para = (float)log(1.0-p2To1);
}

// tracking multiple pitch periods
void Track_Pitch(int *signal, int NumSamples, INT1D *track1, INT1D *track2,
	float n_root, float c0_new, float c2_new)
{
	FLOAT3D *corr, *corr2;
	INT2D *chan_selected;
	INT3D *valid_peaks;
	FLOAT2D *sigFiltered, *sigRealigned;

	// prob vars
	int timeFrame, delay, delay1, delay2, delay_1, delay_2;
	FLOAT1D *p0_obs;
	FLOAT2D *p1_obs, *p2_obs_raw;
	float p_max, trans_prob;
	int i, time;
	
	static CHAR3D *old_seqs, *new_seqs, *tmp_seqs, *swap_seqs;
	static FLOAT1D *old_seqs_prob, *new_seqs_prob, *tmp_seqs_prob, *swap_seqs_prob;
	int num_old_seqs, num_new_seqs, old_seqs_index, t;
	int flag;
	int check[MAX_DELAY_PITCH][MAX_DELAY_PITCH];
	char **swap_float_pp;
	float min_value, max_value, selection_value;
	int selection_cnt, max_num_new_seqs = 200; // 200 for neighbor 2, 
	static int first_time = 0;
	
	sigFiltered = Gammatone_Filtering(signal, NumSamples); // Gammatone filtering
	Cal_Envelope(sigFiltered); // Envelope
	sigRealigned = Realign_Multiband_Sig(sigFiltered); // Realigned different channels due to different delays of Gammatone filters
	// Compute correlograms
	corr = Cal_Correlogram(sigRealigned);
	corr2 = Cal_Correlogram_Large_Window(sigRealigned);
	
	Free_FLOAT2D(sigRealigned); free(signal); 
	
	// Peak Channel selection
	NEW(valid_peaks);
	chan_selected = Peak_Chan_Selection(corr, corr2, valid_peaks);
	Free_FLOAT3D(corr2);
	
	// Initialize for observation probabilities computation
	Init_Log_Prob();
	
	p0_obs = Log_Prob_0_Pitch(corr,chan_selected,valid_peaks);
	p1_obs = Log_Prob_1_Pitch(corr,chan_selected,valid_peaks);

	for (timeFrame=0;timeFrame<p1_obs->row;timeFrame++) {
		// p0
		p0_obs->array[timeFrame] = c0_new/n_root;
		// p1 
		for (delay=0;delay<p1_obs->col;delay++)
			p1_obs->array[timeFrame][delay] = 
			p1_obs->array[timeFrame][delay]/n_root;
	}
	
	// init pitch
	Init_INT1D_Value(track1, p0_obs->row, 0);
	Init_INT1D_Value(track2, p0_obs->row, 0);
	

	// pitch tracking using HMM
	if (first_time == 0) {
	// init seqs
	NEW(old_seqs); NEW(new_seqs); NEW(old_seqs_prob); NEW(new_seqs_prob);
	NEW(swap_seqs); NEW(swap_seqs_prob);
	Init_CHAR3D_Value(old_seqs, MAX_SEQ, MAX_TIME_FRAME, 3, (unsigned char)0);
	Init_CHAR3D_Value(new_seqs, MAX_SEQ, MAX_TIME_FRAME, 3, (unsigned char)0);
	Init_FLOAT1D_Value(old_seqs_prob, MAX_SEQ, 0.0);
	Init_FLOAT1D_Value(new_seqs_prob, MAX_SEQ, 0.0);
	Init_CHAR3D_Value(swap_seqs, MAX_SEQ, MAX_TIME_FRAME, 3, (unsigned char)0);
	Init_FLOAT1D_Value(swap_seqs_prob, MAX_SEQ, 0.0);
	first_time = 1;
	}
	num_old_seqs = 0; num_new_seqs = 0;

	for (delay1=MIN_DELAY;delay1<MAX_DELAY_PITCH;delay1++) {
		for (delay2=MIN_DELAY;delay2<MAX_DELAY_PITCH;delay2++) {
			check[delay1][delay2] = 0;
		}
	}
	for (timeFrame=0;timeFrame<p1_obs->row;timeFrame++) {
		if (timeFrame==0) {
			old_seqs->array[0][0][0] = PITCH_0;
			old_seqs->array[0][0][1] = (unsigned char)0;
			old_seqs->array[0][0][2] = (unsigned char)0;
			num_old_seqs = 1;
			old_seqs_prob->array[0] = 0.0;
		} else {
			p2_obs_raw = Log_Prob_2_Pitch_1_Frame(corr,chan_selected,valid_peaks,
				timeFrame);

			for (delay1=0;delay1<p1_obs->col;delay1++) {
				for (delay2=0;delay2<p1_obs->col;delay2++)
					p2_obs_raw->array[delay1][delay2] =
					(p2_obs_raw->array[delay1][delay2]+c2_new)/n_root;
			}

			num_new_seqs = 0;
			
			// 0
			p_max = -MAXFLOAT; old_seqs_index = 0;
			for (i=0;i<num_old_seqs;i++) {
				switch (old_seqs->array[i][timeFrame-1][0]) {
				case PITCH_0:
					trans_prob = old_seqs_prob->array[i]+logPDynm0To0New;
					if (p_max<trans_prob) {
						p_max = trans_prob;
						old_seqs_index = i;
					}
					break;
				case PITCH_1:
					trans_prob = old_seqs_prob->array[i]+logPDynm1To0New;
					if (p_max<trans_prob) {
						p_max = trans_prob;
						old_seqs_index = i;
					}
					break;
				}
			}

			new_seqs->array[num_new_seqs][timeFrame][0] = PITCH_0;
			for (t=0;t<timeFrame;t++) {
				for (i=0;i<3;i++)
					new_seqs->array[num_new_seqs][t][i] = old_seqs->array[old_seqs_index][t][i];
			}

			new_seqs_prob->array[num_new_seqs] = p_max+p0_obs->array[timeFrame];
			num_new_seqs++;
			
			// 1
			for (delay=MIN_DELAY+1;delay<MAX_DELAY_PITCH-1;delay++) {
				p_max = -MAXFLOAT; old_seqs_index = 0;
				for (i=0;i<num_old_seqs;i++) {
					switch (old_seqs->array[i][timeFrame-1][0]) {
					case PITCH_0:
						trans_prob = old_seqs_prob->array[i]+logPDynm0To1New;
						if (p_max<trans_prob) {
							p_max = trans_prob;
							old_seqs_index = i;
						}
						break;
					case PITCH_1:
						delay_1 = (int)old_seqs->array[i][timeFrame-1][1];
						if (abs(delay_1-delay) < LIMIT) {
							trans_prob = old_seqs_prob->array[i]
								+ logPDynm1To1New->array[delay_1][delay];
							if (p_max<trans_prob) {
								p_max = trans_prob;
								old_seqs_index = i;
							}
						}
						break;
					case PITCH_2:
						delay_1 = (int)old_seqs->array[i][timeFrame-1][1];
						delay_2 = (int)old_seqs->array[i][timeFrame-1][2];
						if (abs(delay_1-delay) < LIMIT || abs(delay_2-delay) < LIMIT) {
							trans_prob = old_seqs_prob->array[i]
								+logPDynm2To1New->array[delay_1][delay_2][delay];
							if (p_max<trans_prob) {
								p_max = trans_prob;
								old_seqs_index = i;
							}
						}
						break;
					}
				}
				new_seqs->array[num_new_seqs][timeFrame][0] = PITCH_1;
				new_seqs->array[num_new_seqs][timeFrame][1] = (unsigned char)delay;
				for (t=0;t<timeFrame;t++) {
					for (i=0;i<3;i++)
						new_seqs->array[num_new_seqs][t][i] = old_seqs->array[old_seqs_index][t][i];
				}
				new_seqs_prob->array[num_new_seqs] = p_max+p1_obs->array[timeFrame][delay];
				num_new_seqs++;
			}
			
			// 2
			for (delay1=MIN_DELAY+1;delay1<MAX_DELAY_PITCH-2;delay1++) {
				for (delay2=MIN_DELAY+1;delay2<MAX_DELAY_PITCH-2;delay2++) {
					if (p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1-1][delay2-1] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1][delay2-1] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1+1][delay2-1] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1-1][delay2] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1+1][delay2] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1-1][delay2+1] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1][delay2+1] &&
						p2_obs_raw->array[delay1][delay2] > p2_obs_raw->array[delay1+1][delay2+1]) {
						
						check[delay1][delay2] = 1;

						check[delay1-1][delay2-1] = 1; check[delay1][delay2-1] = 1;
						check[delay1+1][delay2-1] = 1; check[delay1-1][delay2] = 1;
						check[delay1+1][delay2] = 1; check[delay1-1][delay2+1] = 1;
						check[delay1][delay2+1] = 1; check[delay1+1][delay2+1] = 1;

						check[delay1][delay2+2] = 1; check[delay1][delay2-2] = 1; 
						check[delay1+1][delay2+2] = 1; check[delay1+1][delay2-2] = 1; 
						check[delay1-1][delay2+2] = 1; check[delay1-1][delay2-2] = 1;
						check[delay1-2][delay2] = 1; check[delay1+2][delay2] = 1; 
						check[delay1-2][delay2-1] = 1; check[delay1+2][delay2-1] = 1;
						check[delay1-2][delay2+1] = 1; check[delay1+2][delay2+1] = 1;
						
						check[delay1+2][delay2+2] = 1; check[delay1+2][delay2-2] = 1;
						check[delay1-2][delay2+2] = 1; check[delay1-2][delay2-2] = 1;
						
					}
				}
			}
			
			for (delay1=MIN_DELAY;delay1<MAX_DELAY_PITCH;delay1++) {
				for (delay2=MIN_DELAY;delay2<MAX_DELAY_PITCH;delay2++) {
					if (check[delay1][delay2] == 1) {
						check[delay1][delay2] = 0;
						p_max = -MAXFLOAT; old_seqs_index = 0; flag = 0;
						for (i=0;i<num_old_seqs;i++) {
							switch (old_seqs->array[i][timeFrame-1][0]) {
							case PITCH_1:
								delay_1 = (int)old_seqs->array[i][timeFrame-1][1];
								if (abs(delay_1-delay1) < LIMIT) {
									trans_prob = old_seqs_prob->array[i]
										+ logPDynm1To2New->array[delay_1][delay1][delay2];
									if (p_max<trans_prob) {
										p_max = trans_prob;
										old_seqs_index = i;
										flag = 1;
									}
								}
								break;
							case PITCH_2:
								delay_1 = (int)old_seqs->array[i][timeFrame-1][1];
								delay_2 = (int)old_seqs->array[i][timeFrame-1][2];
								if (abs(delay_1-delay1) < LIMIT && abs(delay_2-delay2) < LIMIT) {
									trans_prob = old_seqs_prob->array[i]
										+ getLogPtchDynm2d(delay1-delay_1,delay2-delay_2) 
										+ logPDynm2To2Para;
									if (p_max<trans_prob) {
										p_max = trans_prob;
										old_seqs_index = i;
										flag = 1;
									}
								}
								break;
							}
						}
						if (flag == 1) {
							new_seqs->array[num_new_seqs][timeFrame][0] = PITCH_2;
							new_seqs->array[num_new_seqs][timeFrame][1] = (unsigned char)delay1;
							new_seqs->array[num_new_seqs][timeFrame][2] = (unsigned char)delay2;
							for (t=0;t<timeFrame;t++)
								for (i=0;i<3;i++)
									new_seqs->array[num_new_seqs][t][i] = old_seqs->array[old_seqs_index][t][i];
								new_seqs_prob->array[num_new_seqs] = p_max+p2_obs_raw->array[delay1][delay2];
								num_new_seqs++;
						}
					}
				}
			}

			// prune	
			if (num_new_seqs > max_num_new_seqs) {
				max_value = -MAXFLOAT; min_value = MAXFLOAT;
				for (i=0;i<num_new_seqs;i++) {
					if (max_value<new_seqs_prob->array[i]) {
						max_value = new_seqs_prob->array[i];
					}
					if (min_value>new_seqs_prob->array[i] && 
						new_seqs_prob->array[i] > -MAXFLOAT + 100.0) {
						min_value = new_seqs_prob->array[i];
					}
				}
				selection_value = min_value;
				do {
					selection_value = (max_value+selection_value)/2.0;
					selection_cnt = 0;
					for (i=0;i<num_new_seqs;i++) {
						if (new_seqs_prob->array[i]>selection_value) {
							selection_cnt++;
						}
					}

				} while (selection_cnt>max_num_new_seqs);	
				selection_cnt = 0;
				for (i=0;i<num_new_seqs;i++) {
					if (new_seqs_prob->array[i]>selection_value) {
						swap_seqs_prob->array[selection_cnt] = new_seqs_prob->array[i];
						swap_float_pp = swap_seqs->array[selection_cnt];
						swap_seqs->array[selection_cnt] = new_seqs->array[i];
						new_seqs->array[i] = swap_float_pp;
						selection_cnt++;
					}
				}
				// update
				num_new_seqs = selection_cnt;
				tmp_seqs = new_seqs; new_seqs = swap_seqs; swap_seqs = tmp_seqs;
				tmp_seqs_prob = new_seqs_prob; new_seqs_prob = swap_seqs_prob;
				swap_seqs_prob = tmp_seqs_prob;
			}
			
			
			// update
			tmp_seqs = old_seqs; old_seqs = new_seqs; new_seqs = tmp_seqs;
			tmp_seqs_prob = old_seqs_prob; old_seqs_prob = new_seqs_prob;
			new_seqs_prob = tmp_seqs_prob;
			num_old_seqs = num_new_seqs; num_new_seqs = 0;
			
			Free_FLOAT2D(p2_obs_raw);
    }
  }
  
  // get tracks
  p_max = -MAXFLOAT; old_seqs_index = 0;

	for (i=0;i<num_old_seqs;i++) {
		if (p_max<old_seqs_prob->array[i]) {
			p_max = old_seqs_prob->array[i];
			old_seqs_index = i;
		}
	}
	
		for (time=0;time<p1_obs->row;time++) {
			switch (old_seqs->array[old_seqs_index][time][0]) {
			case PITCH_0: 
				track1->array[time] = 0;
				track2->array[time] = 0;
				break;
			case PITCH_1:
				track1->array[time] = (int)old_seqs->array[old_seqs_index][time][1];
				track2->array[time] = 0;
				break;
			case PITCH_2:
				track1->array[time] = (int)old_seqs->array[old_seqs_index][time][1];
				track2->array[time] = (int)old_seqs->array[old_seqs_index][time][2];
				break;
			}   
		}  
		
		Free_FLOAT1D(p0_obs); Free_FLOAT2D(p1_obs);
		Free_FLOAT3D(corr); 
		Free_INT3D(valid_peaks); Free_INT2D(chan_selected);
}
