// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"

// ******************************************************************************************* //

void KeypadInitialize() {
	
	// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This 
	// configuration should ensure that if any key is pressed, a change notification interrupt 
	// will be generated.

	// configure outputs
	TRISAbits.TRISA0=0;
	TRISAbits.TRISA1=0;
	TRISBbits.TRISB2=0;
	TRISBbits.TRISB3=0;

	// configure inputs 
	TRISBbits.TRISB5=1;
	TRISBbits.TRISB10=1;
	TRISBbits.TRISB11=1;

	// configure outputs to open drain
	ODCAbits.ODA0=1;
	ODCAbits.ODA1=1;
	ODCBbits.ODB2=1;
	ODCBbits.ODB3=1;

	// set ouputs to zero to allow change notification
	LATAbits.LATA0=0;
	LATAbits.LATA1=0;
	LATBbits.LATB2=0;
	LATBbits.LATB3=0;

	//set internal pull up resistors for inputs 
	CNPU1bits.CN27PUE = 1;
	CNPU1bits.CN15PUE = 1;
	CNPU1bits.CN16PUE = 1;

	// set change interupt flag to zero and enable change ISR
	IFS1bits.CNIF = 0;
	IEC1bits.CNIE = 1;

}

// ******************************************************************************************* //

char KeypadScan() {
	char key = -1;
	
	
	// TODO: Implement the keypad scanning procedure to detect if exactly one button of the 
	// keypad is pressed. The function should return:
	//
	//      -1         : Return -1 if no keys are pressed.
	//      '0' - '9'  : Return the ASCII character '0' to '9' if one of the 
	//                   numeric (0 - 9) keys are pressed.
	//      '#'        : Return the ASCII character '#' if the # key is pressed. 
	//      '*'        : Return the ASCII character '*' if the * key is pressed. 
	//       -1        : Return -1 if more than one key is pressed simultaneously.
	// Notes: 
	//        1. Only valid inputs should be allowed by the user such that all invalid inputs 
	//           are ignored until a valid input is detected.
	//        2. The user must release all keys of the keypad before the following key press
	//           is processed. This is to prevent invalid keypress from being processed if the 
	//           users presses multiple keys simultaneously.
	//
	return key;
}

// ******************************************************************************************* //
