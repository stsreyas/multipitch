#if !defined(COMMON_H)
#define COMMON_H

#define START_POINT 1200       	// starting point

#define MAX_SAMPLES 70000 	// max samples for the input signal 

#define SAMPLING_FREQUENCY  16000	// Hz 

#define TIME_FRAME 160		// time frame interval

#define MAX_DELAY 400		// max time lag 400
#define MAX_DELAY_PITCH 200

#define MIN_DELAY 32            // min time lag

#define NUM_CHANNEL 128		// number of channels

#define NEW_WINDOW_SIZE 256    // 16ms 

#define NEW_WINDOW_HALF (NEW_WINDOW_SIZE/2)

#define NEW_LARGE_WINDOW_HALF 240  // 30ms

#define LOWER_CF 80		// Hz

#define UPPER_CF 5000		// Hz

#define SAMPLE_SET_SIZE 200000

#define MAX_TIME_FRAME ((int)(MAX_SAMPLES/TIME_FRAME)+20)

#define HIGH_CHANNEL_FREQUENCY 800.0 // 800Hz

#define MID_CHANNEL_FREQUENCY 410.0

#define PITCH_0 ((unsigned char)0)
#define PITCH_1 ((unsigned char)1)
#define PITCH_2 ((unsigned char)2)

/* Model parameters */
#define N_ROOT 6.0 // r
#define P_C0 -451.0 // log(alpha0)
#define P_C2 -66.0 // log(alpha2)
#define P0_TO_1 0.0750 // Probability of omaga0 -> omaga1
#define P1_TO_0 0.0079 // Probability of omaga1 -> omaga0
#define P1_TO_2 0.0184 // Probability of omaga1 -> omaga2
#define P2_TO_1 0.0323 // Probability of omaga2 -> omaga1
#define LAMDA 2.4 // lamda in pitch dynamics
#define P_M 0.4 // m in pitch dynamics
#define Beta 5.0 // beta

/* others */
#define GAMMATONE_DELAY_FILE "data\\important_data\\gammatone_delay_file"

#endif // COMMON_H