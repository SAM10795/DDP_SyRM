/*
 * ekf_cal.c
 *
 *  Created on: Mar 15, 2019
 *      Author: SAM10795
 */

/*
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


    Rs_Ld = Rs*T_step/Ld;
    Rs_Lq = Rs*T_step/Lq;
    B_J = B*T_step/J;
    pxLd_Lq = Ld*p*T_step/Lq;
    pxLq_Ld = Lq*p*T_step/Ld;
    pxLdLq_J = 1.5*p*T_step*(Ld-Lq)/J;
    u_J = 1/J;
    pT = p*T_step;
    T_Ld = T_step/Ld;
    T_Lq = T_step/Lq;


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

********************************
 *     PARAMETER ESTIMATION     *
 ********************************


void ekf()
{
    A[0][0] = 1) - Rs_Ld;
    A[0][1] = floatmpy(pxLq_Ld,X[2]);
    A[0][2] = floatmpy(pxLq_Ld,X[1]);
    A[0][3] = 0); A[0][4] = 0);
    A[1][0] = -floatmpy(pxLd_Lq,X[2]);
    A[1][1] = 1)-Rs_Lq;
    A[1][2] = -floatmpy(pxLd_Lq,X[0]);
    A[1][3] = 0); A[1][4] = 0);
    A[2][0] = floatmpy(pxLdLq_J,X[1]);
    A[2][1] = floatmpy(pxLdLq_J,X[0]);
    A[2][2] = 1) - floatmpy(B_J,sgn(X[2]));
    A[2][3] = 0); A[2][4] = u_J;
    A[3][0] = 0); A[3][1] = 0);
    A[3][2] = pT; A[3][3] = 1);
    A[3][4] = 0);
    A[4][0] = 0); A[4][1] = 0); A[4][2] = 0); A[4][3] = 0);
    A[4][4] = 1);

    X_o[0] = floatmpy(floatmpy(X[1],pxLq_Ld),X[2]) - floatmpy(Rs_Ld,X[0]) + floatmpy(v_d,T_Ld) + X[0];
    X_o[1] = floatmpy(v_q,T_Lq) - floatmpy(floatmpy(X[0],pxLq_Ld),X[2]) - floatmpy(Rs_Lq,X[1]) + X[1];
    X_o[2] = floatmpy(pxLdLq_J,floatmpy(X[0],X[1])) - floatmpy(B_J,abs(X[2])) - floatmpy(u_J,X[4]) + X[2];
    X_o[3] = floatmpy(pT,X[2]) + X[3];

    getP_predict();

    C[0][0] = floatcos(X_o[3]);
    C[0][1] = -floatsin(X_o[3]);
    C[0][2] = 0); C[0][3] = 0); C[0][4] = 0);
    C[1][0] = floatsin(X_o[3]);
    C[1][1] = floatcos(X_o[3]);
    C[1][2] = 0); C[1][3] = 0); C[1][4] = 0);

    Y[0] = floatmpy(X_o[0],C[0][0]) + floatmpy(X_o[1],C[0][1]);
    Y[1] = floatmpy(X_o[0],C[1][0]) + floatmpy(X_o[1],C[1][1]);

    getK();

    X[0] = floatmpy(K[0][0],(i_al-Y[0])) + floatmpy(K[0][1],(i_be-Y[1])) + X_o[0];
    X[1] = floatmpy(K[1][0],(i_al-Y[0])) + floatmpy(K[1][1],(i_be-Y[1])) + X_o[1];
    X[2] = floatmpy(K[2][0],(i_al-Y[0])) + floatmpy(K[2][1],(i_be-Y[1])) + X_o[2];
    X[3] = floatmpy(K[3][0],(i_al-Y[0])) + floatmpy(K[3][1],(i_be-Y[1])) + X_o[3];
    X[4] = floatmpy(K[4][0],(i_al-Y[0])) + floatmpy(K[4][1],(i_be-Y[1])) + X[4];

    id = X[0];
    iq = X[1];
    w_e = X[2];
    angle = X[3];
    Tl_est = X[4];

    angle_sin=floatsin(angle);
    angle_cos=floatcos(angle);

    getP_correct();
}

*/
