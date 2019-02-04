#define GLOBAL_Q    24
long GlobalQ = GLOBAL_Q;      // Used for legacy GEL & Graph Debug.
#define MATH_TYPE FLOAT_MATH

#include <stdio.h>
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "IQmathLib.h"
#include "park.h"
#include "ipark.h"
#include "clarke.h"
#include "pi.h"
#include "svgen.h"
#include "f2833xpwm.h"
#include "f2833xileg_vdc.h"

// Define constants used:
#define PI2 1.570796327
#define PI23 2.09439510239
#define PI 3.141592654
#define STEP_SIZE 0.0314159265
#define ANGLE_CONV 0.00613592315

#define dim_Q 5
#define dim_R 2

unsigned int ot;
unsigned int val;

_iq angle = _IQ(0.0);

//Define Machine constants
_iq Lq = _IQ(0.002);
_iq Ld = _IQ(0.014);
_iq Rs = _IQ(1);
_iq p = _IQ(2);
_iq T_step = _IQ(0.0001);
_iq J = _IQ(0.005);
_iq B = _IQ(0.0005);

CLARKE c1;
PARK p1;
IPARK ip_v;

int ChSel[16] = {0};
int ACQPS[16] = {0};
int TrigSel[16] = {0};

SVGEN sv=SVGEN_DEFAULTS;
PWMGEN pwmgen = PWMGEN_DEFAULTS;

//PI controller variables

PI_CONTROLLER pi_id=PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pi_iq=PI_CONTROLLER_DEFAULTS;
PI_CONTROLLER pi_w=PI_CONTROLLER_DEFAULTS;

_iq kp_id=_IQ(0);
_iq kp_iq=_IQ(0);
_iq kp_w=_IQ(0);
_iq ki_id=_IQ(0);
_iq ki_iq=_IQ(0);
_iq ki_w=_IQ(0);

_iq id_max=_IQ(0);
_iq iq_max=_IQ(0);
_iq w_max=_IQ(0);
_iq id_min=_IQ(0);
_iq iq_min=_IQ(0);
_iq w_min=_IQ(0);

//PI controller variables end

//Voltage variables

_iq v_d=_IQ(1);
_iq v_q=_IQ(0);

_iq v_al=_IQ(1);
_iq v_be=_IQ(0);

//Voltage variables end

//Current variables

_iq i_a = _IQ(0);
_iq i_b = _IQ(1);
_iq i_al = _IQ(0);
_iq i_be = _IQ(0);

//Current variables end

//Estimated variables

_iq w_e=_IQ(0);
_iq id=_IQ(0);
_iq iq=_IQ(0);
_iq Tl_est=_IQ(0);

_iq angle=_IQ(0);
_iq angle_sin=_IQ(0);
_iq angle_cos=_IQ(1);

//Estimated variables end

//Reference variables

_iq id_ref = _IQ(0);    //Id reference
_iq w_e_ref = _IQ(0);     //Omega_electrical reference

//Reference variables end

_iq w_mult = _IQ(0);    //1.5*p*(Ld-Lq)/Id_ref to be updated as Id_ref changes

//Kalman filter constants


_iq Rs_Ld,Rs_Lq,B_J,pxLd_Lq,pxLq_Ld,pxLdLq_J,u_J,pT,T_Ld,T_Lq;


_iq Q[dim_Q][dim_Q] = {{_IQ(0.001),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0.001),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0.001),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0.001),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0.001)}};

_iq R[dim_R][dim_R] = {{_IQ(0.001),_IQ(0)},{_IQ(0),_IQ(0.001)}};

_iq tempR[dim_R][dim_R] = {{_IQ(0),_IQ(0)},{_IQ(0),_IQ(0)}};

_iq A[dim_Q][dim_Q] = {{_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)}};

_iq P[dim_Q][dim_Q] = {{_IQ(0.001),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0.001),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0.001),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0.001),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0.001)}};

_iq tempP[dim_Q][dim_Q] = {{_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)}};

_iq tempP2[dim_Q][dim_Q] = {{_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)}};

_iq C[dim_R][dim_Q] = {{_IQ(0.001),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0.001)}};

_iq tempC[dim_R][dim_Q] = {{_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)}};

_iq K[dim_Q][dim_R] = {{_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0)},
                       {_IQ(0),_IQ(0)}};

_iq X[dim_Q] = {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)};
_iq X_o[dim_Q] = {_IQ(0),_IQ(0),_IQ(0),_IQ(0),_IQ(0)};

_iq Y[dim_R] = {_IQ(0),_IQ(0)};

void update_id(int id_ref_new)
{
    w_mult = _IQdiv(_IQmpy(w_mult,id_ref),id_ref_new);
    id_ref = id_ref_new;
}

void getP_predict()
{
    int i=0,j=0,k=0;
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            tempP[i][j] = _IQ(0);
            for(k=0;k<dim_Q;k++)
            {
                tempP[i][j]+=_IQmpy(A[i][k],P[k][j]);
            }
        }
    }
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            P[i][j] = Q[i][j];
            for(k=0;k<dim_Q;k++)
            {
                P[i][j]+=_IQmpy(tempP[i][k],A[j][k]);
            }
        }
    }
}

void getK()
{
    int i=0,j=0,k=0;

    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_R;j++)
        {
            tempC[i][j]=_IQ(0);
            for(k=0;k<dim_Q;k++)
            {
                tempC[i][j] += _IQmpy(P[i][k],C[j][k]);
            }
        }
    }
    //tempC = P_ixC' = 5x2

    for(i=0;i<dim_R;i++)
    {
        for(j=0;j<dim_R;j++)
        {
            tempR[i][j] = R[i][j];
            for(k=0;k<dim_Q;k++)
            {
                tempR[i][j] += _IQmpy(C[i][k],tempC[k][j]);
            }
        }
    }

    _iq detR = _IQmpy(tempR[0][0],tempR[1][1]) - _IQmpy(tempR[0][1],tempR[1][0]);
    tempR[0][0] = _IQdiv(tempR[0][0],detR);
    tempR[0][1] = _IQdiv(tempR[0][1],detR);
    tempR[1][0] = _IQdiv(tempR[1][0],detR);
    tempR[1][1] = _IQdiv(tempR[1][1],detR);

    _iq temp = tempR[0][0];
    tempR[0][0] = tempR[1][1];
    tempR[1][1] = temp;
    tempR[0][1] = -tempR[0][1];
    tempR[1][0] = -tempR[1][0];

    for(i=0;i<dim_Q;i++)
    {
        K[i][0] = _IQmpy(tempC[i][0],tempR[0][0]) + _IQmpy(tempC[i][1],tempR[1][0]);
        K[i][1] = _IQmpy(tempC[i][0],tempR[0][1]) + _IQmpy(tempC[i][1],tempR[1][1]);
    }

}

void getP_correct()
{
    int i=0,j=0,k=0;
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            tempP[i][j] = -(_IQmpy(K[i][0],C[0][j]) + _IQmpy(K[i][1],C[1][j]));
            tempP2[i][j] = P[i][j];
        }
    }
    tempP[0][0] = _IQ(1) + tempP[0][0];
    tempP[1][1] = _IQ(1) + tempP[1][1];
    tempP[2][2] = _IQ(1) + tempP[2][2];
    tempP[3][3] = _IQ(1) + tempP[3][3];
    tempP[4][4] = _IQ(1) + tempP[4][4];

    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            P[i][j] = _IQ(0);
            for(k=0;k<dim_Q;k++)
            {
                P[i][j] += _IQmpy(tempP[i][k],P[k][j]);
            }
        }
    }
}

_iq sgn(_iq x)
{
    return (x>0?_IQ(1):(x==0?_IQ(0):_IQ(-1)));
}

unsigned int GTB(unsigned int num)
{
    unsigned int mask = num >> 1;
    while (mask != 0)
    {
        num = num ^ mask;
        mask = mask >> 1;
    }
    return (num+1);
}

void get_angle()
{
    ot = (1-GpioDataRegs.GPBDAT.bit.GPIO60)+
                    ((1-GpioDataRegs.GPBDAT.bit.GPIO61)<<1)+
                    ((1-GpioDataRegs.GPBDAT.bit.GPIO32)<<2)+
                    ((1-GpioDataRegs.GPBDAT.bit.GPIO33)<<3)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO24)<<4)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO25)<<5)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO26)<<6)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO11)<<7)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO10)<<8)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO9)<<9);

    val = GTB(ot);
    angle = _IQmpy(val,ANGLE_CONV);
}


void init_reg()
{

    /************************************
     *      ENCODER INITIALIZER BLOCK        *
     ************************************/

    EALLOW;

    GpioCtrlRegs.GPBPUD.bit.GPIO60=1;//bit 0
    GpioCtrlRegs.GPBPUD.bit.GPIO61=1;//bit 1
    GpioCtrlRegs.GPBPUD.bit.GPIO32=1;//bit 2
    GpioCtrlRegs.GPBPUD.bit.GPIO33=1;//bit 3
    GpioCtrlRegs.GPAPUD.bit.GPIO24=1;//bit 4
    GpioCtrlRegs.GPAPUD.bit.GPIO25=1;//bit 5
    GpioCtrlRegs.GPAPUD.bit.GPIO26=1;//bit 6
    GpioCtrlRegs.GPAPUD.bit.GPIO11=1;//bit 7
    GpioCtrlRegs.GPAPUD.bit.GPIO10=1;//bit 8
    GpioCtrlRegs.GPAPUD.bit.GPIO9=1;//bit 9

    GpioCtrlRegs.GPBMUX2.bit.GPIO60=0;//bit 0
    GpioCtrlRegs.GPBMUX2.bit.GPIO61=0;//bit 1
    GpioCtrlRegs.GPBMUX1.bit.GPIO32=0;//bit 2
    GpioCtrlRegs.GPBMUX1.bit.GPIO33=0;//bit 3
    GpioCtrlRegs.GPAMUX2.bit.GPIO24=0;//bit 4
    GpioCtrlRegs.GPAMUX2.bit.GPIO25=0;//bit 5
    GpioCtrlRegs.GPAMUX2.bit.GPIO26=0;//bit 6
    GpioCtrlRegs.GPAMUX1.bit.GPIO11=0;//bit 7
    GpioCtrlRegs.GPAMUX1.bit.GPIO10=0;//bit 8
    GpioCtrlRegs.GPAMUX1.bit.GPIO9=0;//bit 9

    GpioCtrlRegs.GPBDIR.bit.GPIO60=0;//bit 0
    GpioCtrlRegs.GPBDIR.bit.GPIO61=0;//bit 1
    GpioCtrlRegs.GPBDIR.bit.GPIO32=0;//bit 2
    GpioCtrlRegs.GPBDIR.bit.GPIO33=0;//bit 3
    GpioCtrlRegs.GPADIR.bit.GPIO24=0;//bit 4
    GpioCtrlRegs.GPADIR.bit.GPIO25=0;//bit 5
    GpioCtrlRegs.GPADIR.bit.GPIO26=0;//bit 6
    GpioCtrlRegs.GPADIR.bit.GPIO11=0;//bit 7
    GpioCtrlRegs.GPADIR.bit.GPIO10=0;//bit 8
    GpioCtrlRegs.GPADIR.bit.GPIO9=0;//bit 9

    GpioCtrlRegs.GPBQSEL2.bit.GPIO60=1;//bit0
    GpioCtrlRegs.GPBQSEL2.bit.GPIO61=1;//bit1
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32=1;//bit2
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33=1;//bit3
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24=1;//bit4
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25=1;//bit5
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26=1;//bit6
    GpioCtrlRegs.GPAQSEL1.bit.GPIO11=1;//bit7
    GpioCtrlRegs.GPAQSEL1.bit.GPIO10=1;//bit8
    GpioCtrlRegs.GPAQSEL1.bit.GPIO9=1;//bit9


    EDIS;

    /******************
     *  END OF BLOCK  *
     ******************/


    /************************************
     *      PI INITIALIZER BLOCK        *
     ************************************/
    pi_id.Kp = kp_id;
    pi_id.Ki = ki_id;

    pi_iq.Kp = kp_iq;
    pi_iq.Ki = ki_iq;

    pi_w.Kp = kp_w;
    pi_w.Ki = ki_w;

    /******************
     *  END OF BLOCK  *
     ******************/


    /******************************
     *    PWM INITIALIZER BLOCK   *
     ******************************/

    //set the PWM params here
    //set channel numbers
    pwmgen.PeriodMax=7500;
    pwmgen.HalfPerMax=pwmgen.PeriodMax/2;
    pwmgen.Deadband=200;
    PWM_INIT_MACRO(2,3,4,pwmgen);

    /******************
     *  END OF BLOCK  *
     ******************/


    /**********************************
     *    DATA AQ INITIALIZER BLOCK   *
     **********************************/

    //set channel and pin numbers, trig and acqpssel
    //ChSel[1] = 1;
    //ChSel[2] = 9;
    //ChSel[7] = 10;
    //ACQPS[0] = 1;
    //ADC_MACRO_INIT(ChSel,TrigSel,ACQPS);

    /******************
     *  END OF BLOCK  *
     ******************/


    /****************************************
     *    EKF CONSTANTS INITIALIZER BLOCK   *
     ****************************************/

    Rs_Ld = _IQdiv(_IQmpy(Rs,T_step),Ld);
    Rs_Lq = _IQdiv(_IQmpy(Rs,T_step),Lq);
    B_J = _IQdiv(_IQmpy(J,T_step),J);
    pxLd_Lq = _IQmpy(_IQmpy(_IQdiv(Ld,Lq),p),T_step);
    pxLq_Ld = _IQmpy(_IQmpy(_IQdiv(Lq,Ld),p),T_step);
    pxLdLq_J = _IQdiv(_IQmpy(_IQmpy(_IQ(1.5),_IQmpy(p,T_step)),(Ld-Lq)),J);
    u_J = _IQdiv(_IQ(1),J);
    pT = _IQmpy(p,T_step);
    T_Ld = _IQdiv(T_step,Ld);
    T_Lq = _IQdiv(T_step,Lq);


}

/********************************
 *      SIGNAL ACQUISITION      *
 ********************************/
void signal_acq()
{
    i_a = AdcMirror.ADCRESULT0;
    i_b = AdcMirror.ADCRESULT1;
    w_e = AdcMirror.ADCRESULT2;

    c1.As = i_a;
    c1.Bs = i_b;
    CLARKE_MACRO(c1);
    i_al = c1.Alpha;
    i_be = c1.Beta;
}


/********************************
 *     PARAMETER ESTIMATION     *
 ********************************/
void ekf()
{
    A[0][0] = _IQ(1) - Rs_Ld;
    A[0][1] = _IQmpy(pxLq_Ld,X[2]);
    A[0][2] = _IQmpy(pxLq_Ld,X[1]);
    A[0][3] = _IQ(0); A[0][4] = _IQ(0);
    A[1][0] = -_IQmpy(pxLd_Lq,X[2]);
    A[1][1] = _IQ(1)-Rs_Lq;
    A[1][2] = -_IQmpy(pxLd_Lq,X[0]);
    A[1][3] = _IQ(0); A[1][4] = _IQ(0);
    A[2][0] = _IQmpy(pxLdLq_J,X[1]);
    A[2][1] = _IQmpy(pxLdLq_J,X[0]);
    A[2][2] = _IQ(1) - _IQmpy(B_J,sgn(X[2]));
    A[2][3] = _IQ(0); A[2][4] = u_J;
    A[3][0] = _IQ(0); A[3][1] = _IQ(0);
    A[3][2] = pT; A[3][3] = _IQ(1);
    A[3][4] = _IQ(0);
    A[4][0] = _IQ(0); A[4][1] = _IQ(0); A[4][2] = _IQ(0); A[4][3] = _IQ(0);
    A[4][4] = _IQ(1);

    X_o[0] = _IQmpy(_IQmpy(X[1],pxLq_Ld),X[2]) - _IQmpy(Rs_Ld,X[0]) + _IQmpy(v_d,T_Ld) + X[0];
    X_o[1] = _IQmpy(v_q,T_Lq) - _IQmpy(_IQmpy(X[0],pxLq_Ld),X[2]) - _IQmpy(Rs_Lq,X[1]) + X[1];
    X_o[2] = _IQmpy(pxLdLq_J,_IQmpy(X[0],X[1])) - _IQmpy(B_J,abs(X[2])) - _IQmpy(u_J,X[4]) + X[2];
    X_o[3] = _IQmpy(pT,X[2]) + X[3];

    getP_predict();

    C[0][0] = _IQcos(X_o[3]);
    C[0][1] = -_IQsin(X_o[3]);
    C[0][2] = _IQ(0); C[0][3] = _IQ(0); C[0][4] = _IQ(0);
    C[1][0] = _IQsin(X_o[3]);
    C[1][1] = _IQcos(X_o[3]);
    C[1][2] = _IQ(0); C[1][3] = _IQ(0); C[1][4] = _IQ(0);

    Y[0] = _IQmpy(X_o[0],C[0][0]) + _IQmpy(X_o[1],C[0][1]);
    Y[1] = _IQmpy(X_o[0],C[1][0]) + _IQmpy(X_o[1],C[1][1]);

    getK();

    X[0] = _IQmpy(K[0][0],(i_al-Y[0])) + _IQmpy(K[0][1],(i_be-Y[1])) + X_o[0];
    X[1] = _IQmpy(K[1][0],(i_al-Y[0])) + _IQmpy(K[1][1],(i_be-Y[1])) + X_o[1];
    X[2] = _IQmpy(K[2][0],(i_al-Y[0])) + _IQmpy(K[2][1],(i_be-Y[1])) + X_o[2];
    X[3] = _IQmpy(K[3][0],(i_al-Y[0])) + _IQmpy(K[3][1],(i_be-Y[1])) + X_o[3];
    X[4] = _IQmpy(K[4][0],(i_al-Y[0])) + _IQmpy(K[4][1],(i_be-Y[1])) + X[4];

    id = X[0];
    iq = X[1];
    w_e = X[2];
    angle = X[3];
    Tl_est = X[4];

    angle_sin=_IQsin(angle);
    angle_cos=_IQcos(angle);

    getP_correct();
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
    v_d = pi_id.Out - _IQmpy(_IQmpy(iq,w_e),_IQ(Lq));

    //pi_we
    pi_w.Ref = w_e_ref;
    pi_w.Fbk = w_e;
    PI_MACRO(pi_w);

    //pi_iq
    pi_iq.Ref = _IQmpy((pi_w.Out+Tl_est),w_mult);
    pi_iq.Fbk = iq;
    PI_MACRO(pi_iq);
    v_q = pi_iq.Out + _IQmpy(_IQmpy(id,w_e),_IQ(Ld));

}


/**************************
 *   INVERSE PARK BLOCK   *
 **************************/
void inv_park()
{
    //inv_park
    ip_v.Ds=v_d;
    ip_v.Qs=v_q;
    ip_v.Sine=angle_sin;
    ip_v.Cosine=angle_cos;
    IPARK_MACRO(ip_v);
    v_al = ip_v.Alpha;
    v_be = ip_v.Beta;
}


/*****************************
 *   SVPWM GENERATOR BLOCK   *
 *****************************/

void svpwm_gen()
{
    sv.Ualpha=_IQ(0);
    sv.Ubeta=_IQ(1);
    SVGENDQ_MACRO(sv);

    pwmgen.MfuncC1=_IQ(0);//sv.Ta;
    pwmgen.MfuncC2=_IQ(1);//sv.Tb;
    pwmgen.MfuncC3=_IQ(-1);//sv.Tc;

    PWM_MACRO(2,3,4,pwmgen);
}



int main(void)
{
    init_reg();

    //signal_acq();

    //ekf();

    //pi_controller();

    //inv_park();

    svpwm_gen();

    return 0;
}
