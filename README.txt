This C program is used for producing the results presented in the following article:

Mingyang Wu, DeLiang Wang, and Guy J. Brown (2003): "A multipitch tracking algorithm 
for noisy speech," IEEE Transactions on Speech and Audio Processing, vol. 11, no. 3, pp. 229-241, May 2003.

We distribute these programs freely, but please cite the paper if you have made any use 
of these programs.

"multipitch.c": this program contains the main routine performing loading signal, 
doing pitch tracking and save the extracted pitch tracks.

"pitch-tracking.c": this program performs multipitch tracking.

"multiband-filtering.c" this program performs Gammatone filtering.

"stats-model-log.c": this program computes the observation probabilities.

Others are utility programs.

Under directory "signals\\", there are a number of signal samples.

Under directory "data\\important_data\\", there are some useful data files for the program.

If you want to process longer sample, you can change "#define MAX_SAMPLES 70000" in common.h
to a bigger number.

This program is used for tracking multiple pitch tracks in a noisy environment. If you only 
want to track multiple pitch periods without significant noise, you may tune P_C2 and P_C0 
in common.h. For differnt speech databases, P_c2 and P_c0 may be tuned for optimal performance.

By Mingyang Wu
Perception and Neurodynamics Lab.
Department of Computer and Information Science
The Ohio State University
2015 Neil Ave.
Columbus, OH 43210-1277, U.S.A.

Email: mwu@cis.ohio-state.edu
Phone: 614-292-7402 (OFFICE);
URL: http://www.cis.ohio-state.edu/~mwu
