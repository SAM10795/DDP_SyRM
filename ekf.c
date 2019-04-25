/*
 * ekf.c
 *
 *  Created on: Apr 25, 2019
 *      Author: SAM10795
 */
#define MATH_TYPE FLOAT_MATH
#include "ekf.h"
#include "IQmathLib.h"

data_t Q[dim_Q][dim_Q] = {{1e-1,0,0,0,0},
                        {0,1e-1,0,0,0},
                        {0,0,1e-2,0,0},
                        {0,0,0,1e-5,0},
                        {0,0,0,0,1e-1}};    //prediction confidence

data_t R[dim_R][dim_R] = {{1e-1,0},{0,1e-1}};   //measurement confidence

//data_t B = 9.444e-5;
//data_t J = 3.617e-4;
//data_t Ld = 0.014;
//data_t Lq = 0.002;
//data_t Rs = 0.62;
//data_t p = 4;
//data_t T_step = 1e-4;

data_t RsTLd = 0.99557143;      //1 - (Rs*T_step)/Ld
data_t RsTLq = 0.969;           //1 - (Rs*T_step)/Lq
data_t pLqLd = 5.7142857143e-05;    //(p*Lq*T_step)/Ld
data_t pLdLq = 0.0028;          //(p*Ld*T_step)/Lq
data_t pT = 2e-4;               //p*T_step
data_t pLdLqJ = 0.019906;       //1.5*p*T_step*(Ld-Lq)/J
data_t BTJ = 2.6110036e-05;     //B*T_step/J
data_t TJ = 0.276472215;            //T_step/J
data_t TLd = 0.007142857;       //T_step/Ld
data_t TLq = 0.05;              //T_step/Lq

data_t A[dim_Q][dim_Q]; //linearized state matrix

data_t P1[dim_Q][dim_Q];    //intermediate guess confidence

data_t C[dim_R][dim_Q]; //linearized measurement matrix

data_t K[dim_Q][dim_R]; //kalman gain matrix

data_t X_p[dim_Q];  //prediction output vector
data_t Y_p[dim_R];  //expected measurement vector

data_t cxp3;
data_t sxp3;

data_t sign(data_t x)
{
    return (data_t)((x > 0) - (x < 0));
}

void populate_A(data_t X[dim_Q])
{
    A[0][0] = RsTLd;
    A[0][1] = X[2]*pLqLd;
    A[0][2] = X[1]*pLqLd;
    A[0][3] = 0.0;
    A[0][4] = 0.0;

    A[1][0] = -X[2]*pLdLq;
    A[1][1] = RsTLq;
    A[1][2] = -X[0]*pLdLq;
    A[1][3] = 0;
    A[1][4] = 0;

    A[2][0] = X[1]*pLdLqJ;
    A[2][1] = X[0]*pLdLqJ;
    A[2][2] = (data_t)1.0 - BTJ*sign(X[2]);
    A[2][3] = 0;
    A[2][4] = -TJ;

    A[3][0] = 0;
    A[3][1] = 0;
    A[3][2] = pT;
    A[3][3] = (data_t)1.0;
    A[3][4] = 0;

    A[4][0] = 0;
    A[4][1] = 0;
    A[4][2] = 0;
    A[4][3] = 0;
    A[4][4] = (data_t)1.0;
}

void populate_X_p(data_t X[dim_Q], data_t U[dim_R])
{
    X_p[0] = X[0]*RsTLd + (X[1]*A[0][1]) + U[0]*TLd;
    X_p[1] = X[1]*RsTLq + (X[0]*A[1][0]) + U[1]*TLq;
    X_p[2] = X[2]*A[2][2] + X[0]*A[2][0] -X[4]*TJ;
    X_p[3] = X[3] + pT*X[2];
    X_p[4] = X[4];
}


void update_P1(data_t P[dim_Q][dim_Q])
{
    data_t temp[dim_Q][dim_Q];

    int i,j,k;

    //A*P
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            temp[i][j] = 0.0;
            for(k=0;k<dim_Q;k++)
            {
                temp[i][j] += A[i][k]*P[k][j];
            }
        }
    }

    //P1 = (A*P*A' + Q)
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            P1[i][j] = Q[i][j];
            for(k=0;k<dim_Q;k++)
            {
                P1[i][j] += temp[i][k]*A[j][k];
            }
        }
    }
}

void populate_C()
{
    cxp3 = cos(X_p[3]);
    sxp3 = sin(X_p[3]);
    C[0][0] = cxp3;
    C[0][1] = -sxp3;
    C[0][2] = 0;
    C[0][3] = - X_p[0]*sxp3 - X_p[1]*cxp3;
    C[0][4] = 0;

    C[1][0] = sxp3;
    C[1][1] = cxp3;
    C[1][2] = 0;
    C[1][3] = cxp3*X_p[0] - sxp3*X_p[1];
    C[1][4] = 0;

}

void getK()
{
    data_t temp1[dim_R][dim_Q];
    data_t temp2[dim_R][dim_R];

    int i,j,k;

    //C*P1
    for(i=0;i<dim_R;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            temp1[i][j] = 0.0;
            for(k=0;k<dim_Q;k++)
            {
                temp1[i][j] += C[i][k]*P1[k][j];
            }
        }
    }

    //(C*P1*C'+ R)
    for(i=0;i<dim_R;i++)
    {
        for(j=0;j<dim_R;j++)
        {
            temp2[i][j] = R[i][j];
            for(k=0;k<dim_Q;k++)
            {
                temp2[i][j] += temp1[i][k]*C[j][k];
            }
        }
    }

    data_t det = temp2[0][0]*temp2[1][1] - temp2[0][1]*temp2[1][0];
    if(abs((float)det)<1e-3)
    {
        det = 1e-3;
    }
    data_t temp = temp2[0][0]/det;

    //inv(C*P1*C' + R)
    temp2[0][0] = temp2[1][1]/det;
    temp2[1][1] = temp;
    temp2[0][1] = -temp2[0][1]/det;
    temp2[1][0] = -temp2[1][0]/det;

    //C'*inv(C*P1*C' + R)
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_R;j++)
        {
            temp1[j][i] = 0.0;
            for(k=0;k<dim_R;k++)
            {
                temp1[j][i] += C[k][i]*temp2[k][j];
            }
        }
    }

    //K = P1*C'*inv(C*P*C' + R)
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_R;j++)
        {
            K[i][j] = 0.0;
            for(k=0;k<dim_Q;k++)
            {
                K[i][j] += P1[i][k]*temp1[j][k];
            }
        }
    }
}

void populate_Y_p()
{
    Y_p[0] = C[1][3];//X_p[0]*cxp3 - X_p[1]*sxp3;
    Y_p[1] = -C[0][3];//X_p[0]*sxp3 + X_p[1]*cxp3;

}

void get_output(data_t X_o[dim_Q], data_t Y[dim_R])
{
    Y_p[0] = Y[0] - Y_p[0];
    Y_p[1] = Y[1] - Y_p[1];

    int i,j;

    //X_o = X_p + K*(Y - Y_p)
    for(i=0;i<dim_Q;i++)
    {
        X_o[i] = X_p[i];
        for(j=0;j<dim_R;j++)
        {
            X_o[i] += K[i][j]*Y_p[j];
        }
    }
}

void update_P2(data_t P[dim_Q][dim_Q])
{
    data_t temp[dim_Q][dim_Q];

    int i,j,k;

    //(I - K*C)
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            temp[i][j] = 0.0;
            for(k=0;k<dim_R;k++)
            {
                temp[i][j] -= K[i][k]*C[k][j];
            }
        }
    }

    for(i=0;i<dim_Q;i++)
    {
        temp[i][i] += (data_t)1.0;
    }


    //P = (I-K*C)*P1
    for(i=0;i<dim_Q;i++)
    {
        for(j=0;j<dim_Q;j++)
        {
            P[i][j]= 0.0;
            for(k=0;k<dim_Q;k++)
            {
                P[i][j] += temp[i][k]*P1[k][j];
            }
        }
    }
}


void ekf(data_t x_in[dim_Q], data_t u_in[dim_R], data_t y_in[dim_R], data_t x_out[dim_Q], data_t p_in[dim_Q][dim_Q])
{
    populate_A(x_in);
    populate_X_p(x_in, u_in);
    update_P1(p_in);

    populate_C();
    getK();

    populate_Y_p();
    get_output(x_out,y_in);
    update_P2(p_in);
}
