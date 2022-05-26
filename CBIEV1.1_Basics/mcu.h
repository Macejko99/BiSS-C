/**
 * \file mcu.h
 * \brief Header for MCU module
 *
 * File contains description of initialization functions for the peripheries of the MCU,
 * e.g. system clocks, PWM, ADC, QEP, ..., according to user needs.
 *
 * \author Viktor
 *
 * \defgroup group_mcu MCU module
 * Module contains initialization functions for the peripheries of the MCU
 * \{
 */

#ifndef MCU_H_
#define MCU_H_

#define MCU_MULINT  20 //!< Sets multiplier for PLL to 20x.
#define MCU_MULFAC  0 //!< Sets fractional multiplier for PLL to 0x.
#define MCU_PLLDIV  1 //!< Sets PLL divider to /2.
// working frequency is set to 20 MHz * 20 /2 = 200 MHz

#define MCU_XTALOSC 1 //!< Selects system oscilator to crystal

#define MCU_CPUFREQ_MHZ 200L //!< Selected CPU frequency


/**
 * \brief Function for disabling watchdog.
 */
void mcu_disableDog(void);

/**
 * \brief Initializes microcontroller clocks.
 *
 *
 * The final MCU clock frequency is computed as f_CPU=f_XTAL*(MulInt+MulFrac)/PllDiv .
 * The parameters MulInt, MulFrac and PllDiv are defined by macros.
 *
 * \note Function is based on: \n
 * \a F2837xD_SysCtrl.c - F2837xD Device System Control Initialization & Support Functions. \n
 * Texas Instruments F2837xD Support Library v210 from Tue Nov  1 14:46:15 CDT 2016 \n
 * Copyright (C) 2013-2016 Texas Instruments Incorporated
 */
void mcu_initClocks(void);

/**
 * \brief Initializes SPI for communication with EA DOGS164 display
 *
 * \param u16Baudrate_kHz Baudrate in kHz, maximum is 1000 kHz
 *
 */
void mcu_initSPI(unsigned u16Baudrate_kHz);

/**
 *  \brief Initializes and enables interrupts
 *
 *  \param *adcInterrupt Pointer to interrupt handler for ADCA interrupt
 *  \param *pwmInterrupt Pointer to interrupt handler for EPWM3 interrupt
 */
void mcu_initInterrupts(void (*adcInterrupt)(void), void (*pwmInterrupt)(void));

/** \} */ // end of group_mcu



#endif /* MCU_H_ */
