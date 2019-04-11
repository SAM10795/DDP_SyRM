#define GLOBAL_Q    24
long GlobalQ = GLOBAL_Q;      // Used for legacy GEL & Graph Debug.
#define MATH_TYPE FLOAT_MATH

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "IQmathLib.h"
#include "clarke.h"
#include "park.h"
#include "ipark.h"
#include "pi.h"
#include "Gpio_encoder.h"
#include "sv_gen.h"
#include "f2833xileg_vdc.h"

#define PI2 1.570796327
#define PI23 2.09439510239
#define PI 3.141592654
#define SQRT32 0.8660254
#define STEP_SIZE 0.0314159265
#define ANGLE_CONV 0.00613592315
#define  ONEbySQRT3   0.57735026918963    /* 1/sqrt(3) */

#define PWM1_INT_ENABLE 1
#define PWM1_TIMER_TBPRD 0x1D4B //1D4B => 7499 => 10kHz

#define dim_Q 5
#define dim_R 2

#define THRESH 10000

CLARKE c1;
PARK p1;

int ChSel[16] = {0};
int ACQPS[16] = {0};
int TrigSel[16] = {0};

int iter_count=0;

float offA = 0.0;
float offB = 0.0;

float i_a;
float i_b;

unsigned int i=0;

int meas = 0;
int start = 0;

float m_ia[200];
float m_ib[200];
float m_id[200];
float m_iq[200];
float m_w[200];

float Lq = 0.35;
float Ld = 0.086;
float Rs = 1.3;
float p = 2;
float T_step = 0.0001;
float J = 0.068;
float B = 0.00675;

float Vdc = 575;
float Vs = 0.0;

IPARK ip_v;

float a[3] = {0.0,0.0,0.0};
float w[3] = {0.0,0.0,0.0};

PI_CONTROLLER pi_id=PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pi_iq=PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pi_w=PI_CONTROLLER_DEFAULTS;

float kp_id=110.0;//50Hz //2200.0
float kp_iq=54.0;//100Hz //540.0;
float kp_w=100.0;
float ki_id=3.714e-4;//0.0004;
float ki_iq=0.0015;
float ki_w=1e-5;

float vd=0.0;
float vq=0.0;

float v_al=0.0;
float v_be=0.0;

float w_e=0;
float id=0;
float iq=0;
float Tl_est=0;

volatile int sn = 0;
volatile int s = 0;

float id_ref = 1.68;
float w_ref = 0;

float w_mult = 0.22;    //1.5*p*(Ld-Lq)/Id_ref to be updated as Id_ref changes

__interrupt void epwm1_timer_isr(void);

volatile int EPwm1TimerIntCount;
unsigned long TimerCount;

void
InitEPwmTimer(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
    EDIS;

    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through

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
    TimerCount = 0;

    InitEPwmTimer();

    IER |= M_INT3;

    PieCtrlRegs.PIEIER3.bit.INTx1 = PWM1_INT_ENABLE;

    EINT;       // Enable Global interrupt INTM
    ERTM;       // Enable Global realtime interrupt DBGM

}

void Init_adc()
{
    ChSel[0] = 4;
    ChSel[1] = 5;
    ACQPS[0] = 4;
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

void InitPI()
{
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
    pi_w.Umax = 12.7;
    pi_w.Umin = -12.7;
}

void init_reg()
{

    InitSysCtrl();

    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    SetupEPwmTimer();

    InitEncoder();

    Init_adc();

    Init_svpwm();

    Vs = Vdc*ONEbySQRT3;

    InitPI();

}

float min(float a, float b)
{
    return a>b?b:a;
}

float max(float a, float b)
{
    return a>b?a:b;
}

void pi_controller()
{
    pi_id.Ref = id_ref;
    pi_id.Fbk = id;
    PI_MACRO(pi_id);
    vd = pi_id.Out - pi_iq.Ref*p*w[0]*Lq;//(pi_id.Out - iq*w_e*Lq)/Vs;

    pi_w.Ref = p*w_ref;
    pi_w.Fbk = p*w[0];
    PI_MACRO(pi_w);

    pi_iq.Ref = (pi_w.Out)*w_mult;
    pi_iq.Fbk = iq;
    PI_MACRO(pi_iq);
    vq = pi_iq.Out + id_ref*p*w[0]*Ld;//(pi_iq.Out + id*w_e*Ld)/Vs;

}

void satv()
{
    float Vmag = vd*vd + vq*vq;
    if(Vmag>(Vs*Vs))
    {
        float vm = sqrt(Vmag);
        vd = vd/vm;
        vq = vq/vm;
    }
    else
    {
        vd = vd/Vs;
        vq = vq/Vs;
    }
}

void inv_park(float a)
{
    satv();
    ip_v.Ds=vd;
    ip_v.Qs=vq;
    ip_v.Sine=sin(a);
    ip_v.Cosine=cos(a);
    IPARK_MACRO(ip_v);
    v_al = ip_v.Alpha;
    v_be = ip_v.Beta;
}

void get_speed()
{
    w[2] = w[1];
    w[1] = w[0];
    if(EPwm1TimerIntCount>10)
    {
        w[0] = (625.0*PI)/(float)EPwm1TimerIntCount;
    }
    a[2] = a[1];
    a[1] = a[0];
    if(a[1]>a[2] && (a[1]-a[2])>PI)
    {
        w[0] = -w[0];
        if(w[2]>5&&w[1]>5)
        {
            w[0] = -w[0];
        }
    }
    if(a[1]<a[2] && (a[2] - a[1])<PI)
    {
        w[0] = -w[0];
        if(w[2]>5&&w[1]>5)
        {
            w[0] = -w[0];
        }
    }
    if(w[2]<-5 && w[1]<-5)
    {
        w[0] =(w[0]>0)?-w[0]:w[0];
    }
    w[0] = min(w[0],300);
    w[0] = max(w[0],-300);
    w_e = p*w[0];
    EPwm1TimerIntCount=0;
}

void record(int i)
{
    m_id[i] = id;
    m_iq[i] = iq;
    m_w[i] = w[0];
    m_ia[i] = i_a;
    m_ib[i] = i_b;
}

void signal_acq(float a)
{
    i_a = ((AdcMirror.ADCRESULT0-offA)*0.00073242)*(22.22);
    i_b = ((AdcMirror.ADCRESULT1-offB)*0.00073242)*(22.22);

    if(start && meas<200 && (TimerCount%200)==0)
    {
        record(meas++);
    }

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

void ref()
{
    w_ref = 50*PI;
    if(TimerCount>10)
    {
        start = 1;
    }
    if(TimerCount>100000)
    {
        w_ref = -50*PI;
    }
    if(TimerCount>200000)
    {
        w_ref = 0.0;
    }
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
    TimerCount++;
    EPwm1TimerIntCount++;
    if(i>THRESH)
    {
        ref();                  //set references

        a[0] = get_angle();

        sn = (GpioDataRegs.GPADAT.bit.GPIO24);
        if(s!=sn || EPwm1TimerIntCount>15000)
        {
            s = sn;
            get_speed();
        }

        signal_acq(a[0]);

        pi_controller();

        inv_park(a[0]);

        sv_pwm(v_al,v_be);

    }
    else
    {
        setOffset(i++);
    }

    EPwm1Regs.ETCLR.bit.INT = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
