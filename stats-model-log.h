#if !defined(STATS_MODEL_LOG_H)
#define STATS_MODEL_LOG_H

#include "type-def.h"
#include "common.h"

void Init_Log_Prob(void);

FLOAT1D *Log_Prob_0_Pitch(FLOAT3D *corr, INT2D *chan_selected,
			  INT3D *valid_peaks);

FLOAT2D *Log_Prob_1_Pitch(FLOAT3D *corr, INT2D *chan_selected,
			  INT3D *valid_peaks);

FLOAT2D *Log_Prob_2_Pitch_1_Frame(FLOAT3D *corr, INT2D *chan_selected,
				  INT3D *valid_peaks, int frame);

#endif // STATS_MODEL_Log_H
