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
	CNPU2bits.CN27PUE = 1;
	CNPU1bits.CN15PUE = 1;
	CNPU2bits.CN16PUE = 1;

	// set change interupt flag to zero and enable change ISR
	CNEN2bits.CN27IE = 1;
	CNEN1bits.CN15IE = 1;
	CNEN2bits.CN16IE = 1;
	IFS1bits.CNIF = 0;
	IEC1bits.CNIE = 1;
}

// ******************************************************************************************* //

char KeypadScan() {
	char key = -1;
	int buttonsPressed = 0;
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
	T1CON = 0x0030;
	TMR1 = 0;
	PR1 = 575;
	IFS0bits.T1IF = 0;
	_TON = 1;
	while(IFS0bits.T1IF == 0){};
	TMR1 = 0;
	_TON= 0;
	IFS0bits.T1IF = 0;

	
	
	   // set Outputs to 0111
        // if inputs 011, then key = 1
        // else if inputs 101, then key = 2
        // else if inputs 110, then key = 3
    // set outputs to 1011
        // if inputs 011, then key = 4
        // else if inputs 101, then key = 5
        // else if inputs 110, then key = 6
    // set outputs to 1101
        // if inputs 011, then key = 7
        // else if inputs 101, then key = 8
        // else if inputs 110, then key = 9
    // set outputs to 1110
        // if inputs 011, then key = *
        // else if inputs 101, then key = 0
        // else if inputs 110, then key = #
    
    
    key = '-1';
    buttonsPressed = 0;
    

    // set Outputs to 0111
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 1;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
        // if inputs 011, then key = 1
        // else if inputs 101, then key = 2
        // else if inputs 110, then key = 3
    if((PORTB & 0x0C20) == 0x0420){
        key = '3';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0820){
        key = '2';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0C00){
        key = '1';
        buttonsPressed++;
    }
        
    // set outputs to 1011
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 0;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
        // if inputs 011, then key = 4
        // else if inputs 101, then key = 5
        // else if inputs 110, then key = 6
    if((PORTB & 0x0C20) == 0x0420){
        key = '6';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0820){
        key = '5';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0C00){
        key = '4';
        buttonsPressed++;
    }
    
    // set outputs to 1101
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 1;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
        // if inputs 011, then key = 7
        // else if inputs 101, then key = 8
        // else if inputs 110, then key = 9
    if((PORTB & 0x0C20) == 0x0420){
        key = '9';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0820){
        key = '8';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0C00){
        key = '7';
        buttonsPressed++;
    }
    // set outputs to 1110
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 1;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
        // if inputs 011, then key = *
        // else if inputs 101, then key = 0
        // else if inputs 110, then key = #
    if((PORTB & 0x0C20) == 0x0420){
        key = '#';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0820){
        key = '0';
        buttonsPressed++;
    }
    else if((PORTB & 0x0C20) == 0x0C00){
        key = '*';
        buttonsPressed++;
    }
    
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    if (buttonsPressed == 1) {
        return key;
    }
    
	LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    return -1;
}

// ******************************************************************************************* //
