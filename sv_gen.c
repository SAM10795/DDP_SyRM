/*
 * sv_gen.c
 *
 *  Created on: Mar 22, 2019
 *      Author: SAM10795
*/

#define MATH_TYPE FLOAT_MATH

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "IQmathLib.h"

#include "svgen.h"
#include "f2833xpwm.h"

SVGEN sv=SVGEN_DEFAULTS;
PWMGEN pwmgen = PWMGEN_DEFAULTS;

void Init_svpwm()
{
    pwmgen.PeriodMax=7500;
    pwmgen.HalfPerMax=pwmgen.PeriodMax/2;
    pwmgen.Deadband=200;
    PWM_INIT_MACRO(2,3,4,pwmgen);
}

void sv_pwm(float v_al, float v_be)
{
    sv.Ualpha = v_al;
    sv.Ubeta = v_be;
    SVGENDQ_MACRO(sv);

    pwmgen.MfuncC1=sv.Ta;
    pwmgen.MfuncC2=sv.Tb;
    pwmgen.MfuncC3=sv.Tc;

    PWM_MACRO(2,3,4,pwmgen);
}
