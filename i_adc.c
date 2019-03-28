/*
 * i_adc.c
 *
 *  Created on: Mar 22, 2019
 *      Author: SAM10795

#define GLOBAL_Q    24
long GlobalQ = GLOBAL_Q;      // Used for legacy GEL & Graph Debug.
#define MATH_TYPE FLOAT_MATH

#include "DSP28x_Project.h"
#include "IQmathLib.h"
#include "f2833xileg_vdc.h"
#include "clarke.h"
#include "park.h"

#define THRESH 10000

int ChSel[16] = {0};
int ACQPS[16] = {0};
int TrigSel[16] = {0};

float offA = 0.0;
float offB = 0.0;

float i_a = 0;
float i_b = 0;

CLARKE c1;
PARK p1;

void Init_adc()
{
    ChSel[0] = 4;
    ChSel[1] = 5;
    ACQPS[0] = 1;
    ADC_MACRO_INIT(ChSel,TrigSel,ACQPS);
}

void signal_acq(float a, float *id, float *iq)
{

    i_a = ((AdcMirror.ADCRESULT0-offA)*0.00073242)*(-20);
    i_b = ((AdcMirror.ADCRESULT1-offB)*0.00073242)*(-20);

    c1.As = i_a;
    c1.Bs = i_b;
    CLARKE_MACRO(c1);
    i_a = c1.Alpha;
    i_b = c1.Beta;

    p1.Alpha = i_a;
    p1.Beta = i_b;
    p1.Cosine = cos(a);
    p1.Sine = sin(a);
    PARK_MACRO(p1);

    *id = p1.Ds;
    *iq = p1.Qs;
}

void setOffset(unsigned int i)
{
    if(i<THRESH)
    {
        offA += (AdcMirror.ADCRESULT0);
        offB += (AdcMirror.ADCRESULT1);
    }
    else if(i == THRESH)
    {
        offA = offA/(float)i;
        offB = offB/(float)i;
    }
}
*/
