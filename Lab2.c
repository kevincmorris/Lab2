
// ******************************************************************************************* //
//  Kevin Morris
//  Due: 10/17/14







// File:         lab2.c
// Date:         08-20-2010
// Authors:      Roman Lysecky
// Revised:		 08-27-2013 (R. Thamvichai)
//
// Description:  Software code for Lab 0 assignment for ECE 372 Fall 2010. Sample code toggles
//               a user specified LED on the Microchip 16-bit 28-pin starter board. The user
//               specified LED is received using the PIC's UART.
//
// Requirements: This softwrae code requires the MPLAB C30 Compiler or MPLAB XC16 (MPLAB C Compiler
//               for PIC24 MCUs)
//
// Credits:      Software code based upon sample code provided with Microchip 16-bit 28-pin
//               Development board.
//
// ******************************************************************************************* //

// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>


_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
         BKBUG_ON & COE_ON & ICS_PGx1 &
         FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )


// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
// This option can be set by selecting "Configuration Bits..." under the Configure
// menu in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
         IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //
// Defines to simply UART's baud rate generator (BRG) register
// given the oscillator frequency and PLLMODE.

#define XTFREQ          7372800         	  // On-board Crystal frequency
#define PLLMODE         4               	  // On-chip PLL setting (Fosc)
#define FCY             (XTFREQ*PLLMODE)/2    // Instruction Cycle Frequency (Fosc/2)

#define BAUDRATE         115200
#define BRGVAL          ((FCY/BAUDRATE)/16)-1


volatile char KeypadScanReturn = 0;
volatile int scanKeypad = 0; // a press has been made


// ******************************************************************************************* //

// main function

// CN detects press
// CN ISR notes change
// if change, KeypadScan() investigates the press
// if(scanKeypad = 1){ KeypadScanReturn = KeypadScan();}
//
//



// ******************************************************************************************* //

    // Outputs:
    //      RA0 [LATAbits.LATA0 = O or 1] --> row 1
    //      RA1 [LATAbits.LATA1 = 0 or 1] --> row 2
    //      RB2 [LATBbits.LATB2 = 0 or 1] --> row 3
    //      RB3 [LATBbits.LATB3 = 0 or 1] --> row 4

    // Inputs:
    //      RB5  [PORTBbits.RB5  == ... ] --> col 1
    //      RB10 [PORTBbits.RB10 == ... ] --> col 2
    //      RB10 [PORTBbits.RB11 == ... ] --> col 3


char KeypadScan(){
    key = -1;
    
    

    // set Outputs to 0111
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 1;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
        // if inputs 011, then key = 1
        // else if inputs 101, then key = 2
        // else if inputs 110, then key = 3
    if(PORTBbits & 0x0C20 == 0x0420){
        key = '1';
    }
    else if(PORTBbits & 0x0C20 == 0x0820){
        key = '2';
    }
    else if(PORTBbits & 0x0C20 == 0x0C00){
        key = '3';
    }
        
    // set outputs to 1011
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 0;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
        // if inputs 011, then key = 4
        // else if inputs 101, then key = 5
        // else if inputs 110, then key = 6
    if(PORTBbits & 0x0C20 == 0x0420){
        key = '4';
    }
    else if(PORTBbits & 0x0C20 == 0x0820){
        key = '5';
    }
    else if(PORTBbits & 0x0C20 == 0x0C00){
        key = '6';
    }
    
    // set outputs to 1101
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 1;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
        // if inputs 011, then key = 7
        // else if inputs 101, then key = 8
        // else if inputs 110, then key = 9
    if(PORTBbits & 0x0C20 == 0x0420){
        key = '7';
    }
    else if(PORTBbits & 0x0C20 == 0x0820){
        key = '8';
    }
    else if(PORTBbits & 0x0C20 == 0x0C00){
        key = '9';
    }
    // set outputs to 1110
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 1;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
        // if inputs 011, then key = *
        // else if inputs 101, then key = 0
        // else if inputs 110, then key = #
    if(PORTBbits & 0x0C20 == 0x0420){
        key = '*';
    }
    else if(PORTBbits & 0x0C20 == 0x0820){
        key = '0';
    }
    else if(PORTBbits & 0x0C20 == 0x0C00){
        key = '#';
    }
    
    
    return key;
}

void __attribute__((interrupt)) _CNInterrupt(void){
    
    scanKeypad = 1;
    IFS1bits.CNIF = 0;
    
    
    }


