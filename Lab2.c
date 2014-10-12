// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings. 
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to 
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & 
		 BKBUG_ON & COE_ON & ICS_PGx1 & 
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //

// Varible used to indicate that the current configuration of the keypad has been changed,
// and the KeypadScan() function needs to be called.

volatile char scanKeypad;
volatile int numPasswords = 1;

int checkDatabase(unsigned short searchPass, unsigned short *searchLocation);

typedef enum stateTypeEnum {initState} stateType;
	
// ******************************************************************************************* //

int main(void)
{
	char key;
	char prevKey = 'N';
	stateType state;
	int check = 0;
	int linePos = 0;
	unsigned short *database = NULL;
	int counter = 3;
	unsigned short currentPass = 0;
	int checkPasswordFound = 0;
	
	database = (unsigned short*)malloc(numPasswords * sizeof(unsigned short));
	database[0] = 1234;
	
	// TODO: Initialize and configure IOs, LCD (using your code from Lab 1), 
	// UART (if desired for debugging), and any other configurations that are needed.
	
	LCDInitialize();
	KeypadInitialize();	
	
	// TODO: Initialize scanKeypad variable.
	scanKeypad = KeypadScan();
	
	// These statements initialize the 32-bit timer corresponding to TMR4,5.  The timer
	// is set to have a prescaler of 256 and to have a 2 second cycle.  We take the first
	// sixteen bits of the binary representation of the 2 second cycle, store them in PR4,
	// and then store the remaining in PR5.  The timer will start as OFF.
	TMR4 = 0;
	TMR5 = 0;
	PR4 = 0b1110000011111111;
	PR5 = 0b1;
	IFS1bits.T5IF = 0;
	T4CONbits.TON = 0;
    T4CONbits.T32 = 1;
    T4CONbits.TCKPS0 = 1;
    T4CONbits.TCKPS1 = 1;
		

	
// 		Check value legend:
//		-1 means bad password
// 		1 means good password
// 		2 means go to Set Mode
	
	
	
	LCDClear();
	
	while(1)
	{
	
		// TODO: Once you create the correct keypad driver (Part 1 of the lab assignment), write
		// the C program that use both the keypad and LCD drivers to implement the 4-digit password system.

		if (check != 2) {					// ENTER/STAY IN USER MODE
			LCDMoveCursor(0,0);				
			LCDPrintString("Enter:");		
			LCDMoveCursor(1,linePos);		
		}
		else {								// ENTER SET MODE
			LCDCLear();
			LCDMoveCursor(0,0);
			LCDPrintString("Set Mode");
			linePos = 0;
			
			while (linePos != 5) {			// Scan while not 5 characters entered
				if (scanKeypad == 1) {
					key = KeypadScan();
				
				// These two statements check if the key is numeric or if the key is one of the symbol
				// characters programmed on the keypad.  If the key is numeric, it will increment the
				// password for storage/checking purposes later.  If they key is a symbol and it is not
				// the # at the end of a password, it will proceed to invalid input statements.
				if (key >= '0' && key <= '9') {	
					currentPass = currentPass + (key - '0')*(10^counter);
					--counter;
					LCDMoveCursor(1,linePos);
					LCDPrintChar(key);
					++linePos;
				}
				else if (key != -1) {
					LCDMoveCursor(1,linePos);
					LCDPrintChar(key);
					
					if (scanKeypad = '#' && linePos == 4)
						check = 1;
					else
						check = -2;	
					++linePos;
				}
				}			
			}
			
			// If check == 1, then the user has entered a valid combination for a password, and we will
			// check if the number has already been entered, or if it needs to be stored in the array
			// of stored password.  If check != 1, We will be outputting the situation of "invalid."
			if (check == 1) {
				checkPasswordFound = checkDatabase(currentPass, database);
				if (checkPasswordFound == 1){
					LCDCLear();
					LCDMoveCursor(0,0);
					LCDPrintString("Already");
					LCDMoveCursor(1,0);
					LCDPrintString("Stored");
					
					T4CONbits.TON = 1;
					while(IFS1bits.T5IF == 0){};
					T4CONbits.TON = 0;
					IFS1bits.T5IF = 0;
					LCDClear();
				}	
				else {
					++numPasswords;
					database = realloc(database, numPasswords*sizeof(unsigned short));
					database[numPasswords - 1] = currentPass;
					
					LCDCLear();
					LCDMoveCursor(0,0);
					LCDPrintString("Valid");

					T4CONbits.TON = 1;
					while(IFS1bits.T5IF == 0){};
					T4CONbits.TON = 0;
					IFS1bits.T5IF = 0;
					LCDClear();
				}	
			}
			else {
				LCDCLear();
				LCDMoveCursor(0,0);
				LCDPrintString("Invalid");

				T4CONbits.TON = 1;
				while(IFS1bits.T5IF == 0){};
				T4CONbits.TON = 0;
				IFS1bits.T5IF = 0;
				LCDClear();
				
			}		
			
			prevKey = 'N';
			linePos = 0;
			counter = 3;
			currentPass = 0;
			check = 0;
			
		}	
		
	// This switch statement checks the scanKeypad variable
		if (scanKeypad == 1) {
			key = KeypadScan();
			switch (key) {
				case '-1':
			 		break;
			 	
			 	
				case '*':
					++linePos;
					LCDPrintChar(key);
					if (prevKey >= '0' && prevKey <= '9') {
						check = -1;
					else if (prevKey == '*')
						check = 2;
				 	break;
			 	
			 	
				case '#':
					LCDPrintChar(key);
					check = -1;
					++linePos;
			 		break;
			 	
			 	
				default:
					currentPass = currentPass + (key - '0')*(10^counter);
					++linePos;
					--counter;
					LCDPrintChar(key);
					if (linePos == 5) {
						check = 1;	
					}	

					break;
			}
			prevKey = key;
		
		
	// 		Check value legend:
	//		-1 means bad password
	// 		1 means cleared password
	// 		2 means go to Set Mode
		
	//		OUTPUTTING FOR USER MODE
			if (check == -1) {
				LCDCLear();
				LCDMoveCursor(0,0);
				LCDPrintString("Bad");
			
				T4CONbits.TON = 1;
				while(IFS1bits.T5IF == 0){};
				T4CONbits.TON = 0;
				IFS1bits.T5IF = 0;
				LCDClear();
			
				prevKey = 'N';
				linePos = 0;
				counter = 3;
				currentPass = 0;
				check = 0;
			}
			else if (check == 1) {
				LCDCLear();
				LCDMoveCursor(0,0);
			
				checkPasswordFound = checkDatabase(currentPass, database);
				if (checkPasswordFound == 1){
					LCDPrintString("Good");
				}	
				else {
					LCDPrintString("Bad");	
				}	
	
				T4CONbits.TON = 1;
				while(IFS1bits.T5IF == 0){};
				T4CONbits.TON = 0;
				IFS1bits.T5IF = 0;
				LCDClear();
			
				prevKey = 'N';
				linePos = 0;
				counter = 3;
				currentPass = 0;
				check = 0;
				}
			}			
		}
	return 0;
}

// ******************************************************************************************* //
// Defines an interrupt service routine that will execute whenever any enable
// input change notifcation is detected.
// 
//     In place of _ISR and _ISRFAST, we can directy use __attribute__((interrupt))
//     to inform the compiler that this function is an interrupt. 
//
//     _CNInterrupt is a macro for specifying the interrupt for input change 
//     notification.
//
// The functionality defined in an interrupt should be a minimal as possible
// to ensure additional interrupts can be processed. 
void __attribute__((interrupt)) _CNInterrupt(void)
{	
	// TODO: Clear interrupt flag
	IFS1bits.CNIF = 0;
	
	// TODO: Detect if *any* key of the keypad is *pressed*, and update scanKeypad
	// variable to indicate keypad scanning process must be executed.
	
	scanKeypad = 1;
}

// ******************************************************************************************* //


int checkDatabase(unsigned short searchPass, unsigned short *searchLocation) {
	
	int i = 0;
	
	for (i = 0; i < numPasswords; ++i) {
		if (searchLocation[i] == searchPass) {
			return 1;
		}		
	}		
	return -1;
}	


/*

			++numPasswords;
			searchLocation = realloc(searchLocation, numPasswords*sizeof(unsigned short));
			searchLocation[i+1] = searchPass;
			*/