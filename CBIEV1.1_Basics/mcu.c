/**
 * \file mcu.c
 * \brief Source code for MCU module
 * \date 05.10.2019
 * File contains initialization functions for the peripheries of the MCU,
 * e.g. system clocks, PWM, ADC, QEP, ..., according to user needs.
 *
 * \note Functions are based on: \n
 * Texas Instruments F2837xD Support Library v210 from Tue Nov  1 14:46:15 CDT 2016 \n
 * Copyright (C) 2013-2016 Texas Instruments Incorporated
 *
 */

#include <F2837xD_device.h>
#include "mcu.h"

/**
 * \brief Delay function defined in \a F2837xD_usDelay.asm
 */
extern void _F28x_usDelay(long LoopCount);
#define CPU_RATE 5.0L //!< Sets CPU rate for DELAY_US macro.
#define DELAY_US(A)  _F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L) //!< Sets CPU rate for DELAY_US macro.


// internal function for PLL initialization
void mcu_initSysPll(void);
//internal function for ADC trim
void mcu_trimADC(void);
// internal function for pullups configuration on unbonded pins
void mcu_enableUnbondedIOPullups(void);

// function for clock initialization
void mcu_initClocks(void){

    // Disable the watchdog
    mcu_disableDog();

    // set pullups on unbonded pins
    mcu_enableUnbondedIOPullups();

    // trim ADC - according to errata, ADC may not be trimmed
    mcu_trimADC();

    // initialize PLL
    mcu_initSysPll();

}

// function for disabling watchdog
void mcu_disableDog(void)
{
    volatile Uint16 temp;

    // Grab the clock config first so we don't clobber it
    EALLOW;
    temp = WdRegs.WDCR.all & 0x0007;
    WdRegs.WDCR.all = 0x0068 | temp;
    EDIS;

}

// function for Adc trimming in case it is not factory set
void mcu_trimADC(void){

    EALLOW;
    // first, enable clocks
    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
//    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
//    CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

    // second, check if device is trimmed
    if(AnalogSubsysRegs.ANAREFTRIMA.all == 0x0000){ //0x5D1B6
       // Device is not trimmed--apply static calibration values of 0x7BDD
       AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
       AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
//        AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
//        AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
    }

    // finally, disable clocks
    CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
//    CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
//    CpuSysRegs.PCLKCR13.bit.ADC_D = 0;

    EDIS;
}

// function for enabling pullups on unbonded pins
void mcu_enableUnbondedIOPullups(void){
    //
    //bits 8-10 have pin count
    //
    unsigned char pin_count = ((DevCfgRegs.PARTIDL.all & 0x00000700) >> 8) ;

    //
    //5 = 100 pin
    //6 = 176 pin
    //7 = 337 pin
    //
    if(pin_count == 5)
    {
        EALLOW;
        GpioCtrlRegs.GPAPUD.all = ~0xFFC003E3;  //GPIOs 0-1, 5-9, 22-31
        GpioCtrlRegs.GPBPUD.all = ~0x03FFF1FF;  //GPIOs 32-40, 44-57
        GpioCtrlRegs.GPCPUD.all = ~0xE10FBC18;  //GPIOs 67-68, 74-77, 79-83, 93-95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFFF;  //GPIOs 128-159
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
        EDIS;
    }
    else if (pin_count == 6)
    {
        EALLOW;
        GpioCtrlRegs.GPCPUD.all = ~0x80000000;  //GPIO 95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  //GPIOs 128-159 except for 133
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
        EDIS;
    }
    else
    {
        //do nothing - this is 337 pin package
    }
}

// function for initializing PLL
void mcu_initSysPll(void)
{
    Uint16 SCSR, WDCR, WDWCR, intStatus;
    if((MCU_XTALOSC == ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)    &&
       (MCU_MULINT        == ClkCfgRegs.SYSPLLMULT.bit.IMULT)           &&
       (MCU_MULFAC        == ClkCfgRegs.SYSPLLMULT.bit.FMULT)           &&
       (MCU_PLLDIV       == ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV))
    {
        //
        // Everything is set as required, so just return
        //
        return;
    }

    // 1. set-up reference clock source to XTAL
    if(MCU_XTALOSC != ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)
    {
        EALLOW;
        ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF=0;            // Turn on XTALOSC
        ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;     // Clk Src = XTAL
        EDIS;
    }


    // if pll multiplier is not set as required, continue with set up
    if(MCU_MULINT != ClkCfgRegs.SYSPLLMULT.bit.IMULT ||
       MCU_MULFAC != ClkCfgRegs.SYSPLLMULT.bit.FMULT)
    {
        Uint16 i;

        //
        // This bit is reset only by POR
        //
        if(DevCfgRegs.SYSDBGCTL.bit.BIT_0 == 1)
        {
            //
            // The user can optionally insert handler code here. This will only
            // be executed if a watchdog reset occurred after a failed system
            // PLL initialization. See your device user's guide for more
            // information.
            //
            // If the application has a watchdog reset handler, this bit should
            // be checked to determine if the watchdog reset occurred because
            // of the PLL.
            //
            // No action here will continue with retrying the PLL as normal.
            //
        }

        EALLOW;
        // 2.a) Bypass PLL and set dividers to /1
        ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0;
        asm(" RPT #20 || NOP");
        // 2.b) Set clock divider to /1
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 0;

        //
        // 2.d) Lock the PLL five times. This helps ensure a successful start.
        // Five is the minimum recommended number. The user can increase this
        // number according to allotted system initialization time.
        //
        for(i = 0; i < 5; i++)
        {
            //
            // Turn off PLL
            //
            ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 0;
            asm(" RPT #20 || NOP");

            //
            // Write multiplier, which automatically turns on the PLL
            //
            ClkCfgRegs.SYSPLLMULT.all = ((MCU_MULFAC << 8U) | MCU_MULINT);

            //
            // Wait for the SYSPLL lock counter
            //
            while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1)
            {
                //
                // Uncomment to service the watchdog
                //
                // ServiceDog();
            }
        }
    }


    // 2.e) Set divider one setting higher than the final desired value
    // to limit current increase when switching to PLL
    if(MCU_PLLDIV != 63) // if highest divider is not selected
    {
         ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = MCU_PLLDIV + 1; // increase divider by one
    }else // otherwise
    {
         ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = MCU_PLLDIV; // use the desired value
    }
    EDIS;

    // 2.f) Set up the watchdog, be aware of TI warning:
    //
    //      *CAUTION*
    // It is recommended to use the following watchdog code to monitor the PLL
    // startup sequence. If your application has already cleared the watchdog
    // SCRS[WDOVERRIDE] bit this cannot be done. It is recommended not to clear
    // this bit until after the PLL has been initiated.
    //

    // Backup User Watchdog
    SCSR = WdRegs.SCSR.all;
    WDCR = WdRegs.WDCR.all;
    WDWCR = WdRegs.WDWCR.all;

    // Disable windowed functionality, reset counter
    EALLOW;
    WdRegs.WDWCR.all = 0x0;
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA;

    // Disable global interrupts
    intStatus = __disable_interrupts();

    // Configure for watchdog reset and to run at max frequency
    WdRegs.SCSR.all = 0x0;
    WdRegs.WDCR.all = 0x28;

    // 2.g) This bit is reset only by power-on-reset (POR) and will not be cleared
    // by a WD reset
    DevCfgRegs.SYSDBGCTL.bit.BIT_0 = 1;

    // 2.h) Enable PLLSYSCLK is fed from system PLL clock
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;

    // Delay to ensure system is clocking from PLL prior to clearing status bit
    asm(" RPT #20 || NOP");

    // 2.i) Clear bit
    DevCfgRegs.SYSDBGCTL.bit.BIT_0 = 0;

    // 2.j) Set the divider to user value
    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = MCU_PLLDIV;

    // 2.k) Restore user watchdog, first resetting counter
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA;

    WDCR |= 0x28;                     // Setup WD key--KEY bits always read 0
    WdRegs.WDCR.all = WDCR;
    WdRegs.WDWCR.all = WDWCR;
    WdRegs.SCSR.all = SCSR & 0xFFFE;  // Mask write to bit 0 (W1toClr)

    // Restore state of ST1[INTM].
    if(!(intStatus & 0x1))
    {
        EINT;
    }


    // Restore state of ST1[DBGM]. This was set by the __disable_interrupts()
    // intrinsic previously.
    if(!(intStatus & 0x2))
    {
        asm(" CLRC DBGM");
    }


    //
    // 200 PLLSYSCLK delay to allow voltage regulator to stabilize prior
    // to increasing entire system clock frequency.
    //
    asm(" RPT #200 || NOP");


    EDIS;
}

// function for SPI initialization
void mcu_initSPI(unsigned u16Baudrate_kHz){

    // set up GPIO pins
    // GPIO58 - 61 are used
    EALLOW;
    // set GPIO as SPI pins
    GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 2; // clock - SPICLKB
    GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 2; // chip select - SPISTEB
    //GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1; // general output
    //GpioDataRegs.GPBSET.bit.GPIO59 = 1; // set to high
    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 2; // MOSI - SPISIMOB
    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 2; // MISO - SPISOMIB

    // lock GPIO settings
    GpioCtrlRegs.GPBLOCK.bit.GPIO58 = 1;
    GpioCtrlRegs.GPBLOCK.bit.GPIO59 = 1;
    GpioCtrlRegs.GPBLOCK.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBLOCK.bit.GPIO61 = 1;

    // enable clocks
    CpuSysRegs.PCLKCR8.bit.SPI_B = 1;

    EDIS;

    // step 1 - reset SPI
    SpibRegs.SPICCR.bit.SPISWRESET = 0;
    // step 2a - select master slave mode
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1; // set as master
    // step 2b - set clk polarity and phase
    SpibRegs.SPICCR.bit.CLKPOLARITY = 1; //
    SpibRegs.SPICTL.bit.CLK_PHASE = 0; // falling edge without delay
    // step 2c - set the baudrate
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 7;//2; // default setting on reset, means SysCLK/4
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 0x7F; //(MCU_CPUFREQ_MHZ*1000/4)/u16Baudrate_kHz - 1;
    // step 2d - set high speed mode
    // not neccessary
    // step 2e - set character length
    SpibRegs.SPICCR.bit.SPICHAR = 7; // 8-bit
    // step 2f - clear flags
    SpibRegs.SPISTS.bit.BUFFULL_FLAG = 1;
    SpibRegs.SPISTS.bit.INT_FLAG = 0;
    SpibRegs.SPISTS.bit.OVERRUN_FLAG = 1;
    // step 2g - enable SPISTE inversion
    // not needed
    // step 2h - enable 3-wire mode
    // not needed
    // step 2i - enable FIFO enhancement
    // not needed
    // Step 2j: Enable transmit (TALK) if needed
    SpibRegs.SPICTL.bit.TALK = 1;
    // Step 2k: Set emulation suspend mode (SOFT, FREE)
    SpibRegs.SPIPRI.bit.SOFT = 1;
    SpibRegs.SPIPRI.bit.FREE = 1;   // free run
    // step 3 - enable interrupts
    // not needed
    // step 4 - release SPI from reset
    SpibRegs.SPICCR.bit.SPISWRESET = 1;
}

// function for initialization of interrupts
void mcu_initInterrupts(void (*adcInterrupt)(void), void (*pwmInterrupt)(void)){

    IFR = 0x0000;                       // clear CPU interrupt flags
    PieCtrlRegs.PIEACK.all = 0xFFFF;    // clear PIE flags

    EALLOW;
    // INT1.1 - ADCA1
    PieVectTable.ADCA1_INT = adcInterrupt;  // routing interrupt
    // INT3.3 - EPWM3
    //PieVectTable.EPWM3_INT = pwmInterrupt;  // routing interrupt
    EDIS;

    // PIE - enable individual interrupts
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // ADCA1
   // PieCtrlRegs.PIEIER3.bit.INTx3 = 1;  // EPWM3


    // enable individual interrupt in CPU
    IER |= 0x0001;                    // enable INT1
    //IER |= 0x0004;                      // enable INT3


    // enable PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    EINT;
}
