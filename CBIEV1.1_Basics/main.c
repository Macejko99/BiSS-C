/**
 * \file main.c
 * \brief Main file for the project
 * \author Viktor
 * \date 4th March 2021
 *
 * Last update: 4th March 2021
 *
 */

#include <F2837xD_device.h>
#include "mcu.h"
//#include "bissc.h"





/**
 * \brief Delay function defined in \a F2837xD_usDelay.asm
 */
extern void _F28x_usDelay(long LoopCount);
#define CPU_RATE 5.0L //!< Sets CPU rate for DELAY_US macro.
#define DELAY_US(A)  _F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L) //!< Sets CPU rate for DELAY_US macro.

#include"mcu.h"
#include"spi_communication.h"




unsigned int skusobnePole[10];
unsigned RxData[10];
unsigned PositionRaw = 0;
unsigned CRC_value = 0;
unsigned Error_bits = 0;

// main function
int main(void)
{
    unsigned i=0;

    // initialize MCU clocks
    mcu_initClocks();

    for (i=0; i<10; i++){
        RxData[i]=0;
    }

    // initialize SPI
    mcu_initSPI(500);


EALLOW;
//GpioCtrlRegs.GPBDIR.bit.GPIO58=1;
//GpioCtrlRegs.GPBDIR.bit.GPIO62=1;
EDIS;
    while(1){
//spi_transmitChar(0x55);
        DELAY_US(200);
//GpioDataRegs.GPBTOGGLE.bit.GPIO58=1;
//GpioDataRegs.GPBTOGGLE.bit.GPIO62=1;
        RxData[0]= spi_transmitChar(0);
        RxData[1]= spi_transmitChar(0);
        RxData[2]= spi_transmitChar(0);
        RxData[3]= spi_transmitChar(0);
        PositionRaw = ((RxData[0] & 0x07) << 10) | (RxData[1] << 2) | (RxData[2] & 0xC0);
        Error_bits = (RxData[2] & 0x30) >> 4;
        CRC_value = (RxData[2] & 0x0F << 2) | ((RxData[3] & 0xC0) >> 6);
        










    }

	return 0;
}
