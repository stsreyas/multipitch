#if !defined(MULTIBAND_FILTERING_H)
#define MULTIBAND_FILTERING_H

#include "type-def.h"

FLOAT2D *Gammatone_Filtering(int* signal, int numSamples);

void Cal_Envelope(FLOAT2D *sigFiltered);

FLOAT2D *Realign_Multiband_Sig(FLOAT2D *sigFiltered);

FLOAT3D *Cal_Correlogram(FLOAT2D *sigRealigned);

FLOAT3D *Cal_Correlogram_Large_Window(FLOAT2D *sigRealigned);

INT2D *Peak_Chan_Selection(FLOAT3D *corr1, FLOAT3D *corr2, INT3D *valid_peaks);

#endif // MULTIBAND_FILTERING_H