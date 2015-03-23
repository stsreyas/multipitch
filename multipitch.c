/********************************************************************/
/* This C program is used for performing multipitch tracking for	*/
/* noisy speech. The algorithm is presented in the following		*/
/* article:															*/
/* Mingyang Wu, DeLiang Wang, and Guy J. Brown (2002): "A			*/
/* multi-pitch tracking algorithmfor noisy speech," Will be			*/
/* presented in ICASSP2002, Orlando, Florida, USA, May 13-17, 2002. */
/* An extend version of the work also can be seen in: Mingyang Wu,  */
/* DeLiang Wang and Guy J. Brown (2001): "A Multipitch Tracking		*/
/* Algorithm for Noisy Speech," Technical Report OSU-CISRC-12/01-25,*/
/* Department of Computer and Information Science, Ohio State		*/
/* University, Columbus, Ohio, USA.									*/
/*																	*/
/* This C program runs under Microsoft visual C++					*/
/********************************************************************/

#include "multipitch.h"

int main (int argc, char **argv)
{
	INT1D *track1, *track2;
	int *signal, NumSamples;

	// initialize pitch tracking
	Init_Pitch_Tracking(P0_TO_1, P1_TO_0, P1_TO_2, P2_TO_1, P_M, LAMDA);

	// load signal
	// input signal is a sequence of integers, if the inputs are float, 
	// this can be changed easily.
	signal=readSignalAscii(INPUT_FILE,&NumSamples);

	// track pitch
	NEW(track1); NEW(track2); // init mem of two pitch tracks
	Track_Pitch(signal, NumSamples, // signal input
		track1, track2, // two pitch track output
		N_ROOT, P_C0, P_C2); // model parameters

	// The 2 pitch tracks are written into OUTPUT_FILE_1 and 2
	// ASCII format, readable by matlab
	// Outputs are integers, which correspond to sample delay of pitch periods.
	// If wants frequency output, please use Fs/d, where Fs is the sample frequency 
	// and d is the output.
	Save_Int1D_Ascii(track1->array, track1->row, OUTPUT_FILE_1);
	Save_Int1D_Ascii(track2->array, track2->row, OUTPUT_FILE_2);	
}