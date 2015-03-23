#if !defined(PITCH_TRACKING_H)
#define PITCH_TRACKING_H

#include "type-def.h"
#include "common.h"
#include "mem-util.h"
#include "math-util.h"
#include "multiband-filtering.h"
#include "stats-model-log.h"
#include "MeddisRate.h"

void Init_Pitch_Tracking(float p0To1, float p1To0, float p1To2,
	float p2To1, float mean, float aa);

void Track_Pitch(int *signal, int NumSamples, INT1D *track1, INT1D *track2,
	float n_root, float c0_new, float c2_new);

#endif // PITCH_TRACKING_H