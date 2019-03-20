/*
 * DSP2833x_Gpio_encoder.c
 *
 *  Created on: Feb 15, 2019
 *      Author: SAM10795
 */

#include "DSP28x_Project.h"
#include "DSP2833x_Examples.h"
#include "IQmathLib.h"


void InitEncoder(void)
{
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

unsigned int get_angle(void)
{
    unsigned int ot = (1-GpioDataRegs.GPBDAT.bit.GPIO60)+
                    ((1-GpioDataRegs.GPBDAT.bit.GPIO61)<<1)+
                    ((1-GpioDataRegs.GPBDAT.bit.GPIO32)<<2)+
                    ((1-GpioDataRegs.GPBDAT.bit.GPIO33)<<3)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO24)<<4)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO25)<<5)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO26)<<6)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO11)<<7)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO10)<<8)+
                    ((1-GpioDataRegs.GPADAT.bit.GPIO9)<<9);

    return GTB(ot);
}
