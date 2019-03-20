#define GLOBAL_Q    24
long GlobalQ = GLOBAL_Q;      // Used for legacy GEL & Graph Debug.
#define MATH_TYPE FLOAT_MATH

#include <stdio.h>
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "IQmathLib.h"
#include "clarke.h"
#include "park.h"
#include "ipark.h"
#include "pi.h"
#include "Gpio_encoder.h"
#include "svgen.h"
#include "f2833xpwm.h"
#include "f2833xileg_vdc.h"

// Define constants used:
#define PI2 1.570796327
#define PI23 2.09439510239
#define PI 3.141592654
#define SQRT32 0.8660254
#define STEP_SIZE 0.0314159265
#define ANGLE_CONV 0.00613592315

#define PWM1_INT_ENABLE 1
#define PWM1_TIMER_TBPRD 0x1D4B //1D4B => 7499 => 10kHz

#define dim_Q 5
#define dim_R 2

#define THRESH 10000

SVGEN sv=SVGEN_DEFAULTS;
PWMGEN pwmgen = PWMGEN_DEFAULTS;

CLARKE c1;
PARK p1;

int ChSel[16] = {0};
int ACQPS[16] = {0};
int TrigSel[16] = {0};

float offA = 0.0;
float offB = 0.0;

float i_a = 0;
float i_b = 0;

unsigned int i =0;

//Define Machine constants
float Lq = 0.35;
float Ld = 0.2;
float Rs = 1.3;
float p = 2;    //pole pairs
float T_step = 0.0001;
float J = 0.005;
float B = 0.0005;

float Vdc = 48.6;
float Vs = 0.0;

IPARK ip_v;

float a = 0.0;
float w = 0.0;
float w_rpm = 0.0;
int s = 0;

float angle = 0.0;

float freq_e = 1;

//PI controller variables

PI_CONTROLLER pi_id=PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pi_iq=PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pi_w=PI_CONTROLLER_DEFAULTS;

float kp_id=2.20;
float kp_iq=1.2570;
float kp_w=0.30;
float ki_id=8.168;
float ki_iq=8.168;
float ki_w=0.10;

float id_max=5;
float iq_max=5;
float w_max=500;
float id_min=-5;
float iq_min=-5;
float w_min=-500;

//PI controller variables end

//Voltage variables

float vd=0.0;
float vq=0.0;

float v_al=1.0;
float v_be=0.0;

//Voltage variables end

//Estimated variables

float w_e=0;
float id=0;
float iq=0;
float Tl_est=0;

//Estimated variables end

//Reference variables

float id_ref = 1.0;    //Id reference
float w_e_ref = 0;     //Omega_electrical reference

//Reference variables end

float w_mult = 0.45;    //1.5*p*(Ld-Lq)/Id_ref to be updated as Id_ref changes

//Kalman filter constants
//

__interrupt void epwm1_timer_isr(void);

unsigned int EPwm1TimerIntCount;

void
InitEPwmTimer(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
    EDIS;

    //
    // Setup Sync
    //
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through

    //
    // Allow each timer to be sync'ed
    //
    EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;

    EPwm1Regs.TBPHS.half.TBPHS = 100;

    EPwm1Regs.TBPRD = PWM1_TIMER_TBPRD;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = PWM1_INT_ENABLE;  // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;       // Start all the timers synced
    EDIS;
}

void
SetupEPwmTimer(void)
{
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.EPWM1_INT = &epwm1_timer_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    EPwm1TimerIntCount = 0;

    InitEPwmTimer();

    IER |= M_INT3;

    PieCtrlRegs.PIEIER3.bit.INTx1 = PWM1_INT_ENABLE;

    //
    // Enable global Interrupts and higher priority real-time debug events
    //
    EINT;       // Enable Global interrupt INTM
    ERTM;       // Enable Global realtime interrupt DBGM

}


void Init_svpwm()
{
    pwmgen.PeriodMax=7500;
    pwmgen.HalfPerMax=pwmgen.PeriodMax/2;
    pwmgen.Deadband=200;
    PWM_INIT_MACRO(2,3,4,pwmgen);
}

void Init_adc()
{
    ChSel[0] = 4;
    ChSel[1] = 5;
    ACQPS[0] = 1;
    ADC_MACRO_INIT(ChSel,TrigSel,ACQPS);
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

void init_reg()
{//
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the DSP2833x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the DSP2833x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    //InitGpio(); // Skipped for this example
    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the DSP2833x_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in DSP2833x_DefaultIsr.c.
    // This function is found in DSP2833x_PieVect.c.
    //
    InitPieVectTable();

    SetupEPwmTimer();

    InitEncoder();

    Init_adc();

    Init_svpwm();

    Vs = Vdc*0.7071;


    /************************************
     *      PI INITIALIZER BLOCK        *
     ************************************/
    pi_id.Kp = kp_id;
    pi_id.Ki = ki_id;
    pi_id.Umax = Vs;
    pi_id.Umin = -Vs;

    pi_iq.Kp = kp_iq;
    pi_iq.Ki = ki_iq;
    pi_iq.Umax = Vs;
    pi_iq.Umin = -Vs;

    pi_w.Kp = kp_w;
    pi_w.Ki = ki_w;
    pi_w.Umax = 10;
    pi_w.Umin = -10;

    /******************
     *  END OF BLOCK  *
     ******************/



}

float min(float a, float b)
{
    return a>b?b:a;
}

float max(float a, float b)
{
    return a>b?a:b;
}


/************************************
 *        PI CONTROLLER BLOCK       *
 ************************************/
void pi_controller()
{
    //pi_id
    pi_id.Ref = id_ref;
    pi_id.Fbk = id;
    PI_MACRO(pi_id);
    vd = (pi_id.Out - iq*w_e*Lq)/Vs;
    vd = min(vd,1.0);
    vd = max(vd,-1.0);

    //pi_we
    pi_w.Ref = w_e_ref;
    pi_w.Fbk = w_e;
    PI_MACRO(pi_w);

    //pi_iq
    pi_iq.Ref = (pi_w.Out+Tl_est)*w_mult;
    pi_iq.Fbk = iq;
    PI_MACRO(pi_iq);
    vq = (pi_iq.Out + id*w_e*Ld)/Vs;
    vq = min(vq,1.0);
    vq = max(vq,-1.0);

}


/**************************
 *   INVERSE PARK BLOCK   *
 **************************/
void inv_park(float a)
{
    //inv_park
    ip_v.Ds=vd;
    ip_v.Qs=vq;
    ip_v.Sine=sin(a);
    ip_v.Cosine=cos(a);
    IPARK_MACRO(ip_v);
    v_al = ip_v.Alpha;
    v_be = ip_v.Beta;
}


/*****************************
 *   SVPWM GENERATOR BLOCK   *
 *****************************/

void sv_pwm()
{
    sv.Ualpha = v_al;
    sv.Ubeta = v_be;
    SVGENDQ_MACRO(sv);

    pwmgen.MfuncC1=sv.Ta;
    pwmgen.MfuncC2=sv.Tb;
    pwmgen.MfuncC3=sv.Tc;

    PWM_MACRO(2,3,4,pwmgen);
}

void ref()
{
    w_e_ref += 0.00001;
    w_e_ref = min(w_e_ref,30.0);
}

void get_speed()
{
    w = PI*10000.0/(float)(EPwm1TimerIntCount);
    w_rpm = 300000.0/(float)(EPwm1TimerIntCount);
    w_e = p*w;
    EPwm1TimerIntCount = 0;
}

void signal_acq(float a)
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

    id = p1.Ds;
    iq = p1.Qs;
}


int main(void)
{
    init_reg();

    for(;;)
    {
    }
}

__interrupt void
epwm1_timer_isr(void)
{
    EPwm1TimerIntCount++;
    //s?(GpioDataRegs.GPASET.bit.GPIO8=1):(GpioDataRegs.GPACLEAR.bit.GPIO8=1);
    //s = 1-s;
    if(i>THRESH)
    {
        ref();
        int sn = GpioDataRegs.GPADAT.bit.GPIO9;
        if(s!=sn)
        {
            s = sn;
            get_speed();
        }

        signal_acq(a);

        a = (float)get_angle();
        a = (a-188.0)*(p);
        a = a*2.0*PI/1024.0;

//        angle = angle+(PI*freq_e/5000.0);
//        if(angle>2*PI)
//        {
//            angle = angle - (2*PI);
//        }
//
//        if(a>2*PI)
//        {
//            a = a - (2*PI);
//        }
//        if(a<0)
//        {
//            a = a + (2*PI);
//        }
//
//        v_al = cos(a+PI2);
//        v_be = sin(a+PI2);

        pi_controller();

        inv_park(a);

        sv_pwm();
    }
    else
    {
        setOffset(i++);
    }

    //
    // Clear INT flag for this timer
    //
    EPwm1Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
