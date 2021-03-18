#include <XC.h>
#include "lab6.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1


void main(void)
{
	volatile unsigned long t=0;
    int adcval;
    float voltage;

	CFGCON = 0;
  
    UART2Configure(115200);  // Configure UART2 for a baud rate of 115200
 
    // Configure pins as analog inputs
    ANSELBbits.ANSB3 = 1;   // set RB3 (AN5, pin 7 of DIP28) as analog pin
    TRISBbits.TRISB3 = 1;   // set RB3 as an input
    
	TRISBbits.TRISB6 = 0;
	LATBbits.LATB6 = 0;	
	INTCONbits.MVEC = 1;
 
    ADCConf(); // Configure ADC
 

	while(TRUE)
	{ 
	

		t++;
		if(t==500000)
		{
			printf("*** WTF ***\r\n");
        	adcval = ADCRead(5); // note that we call pin AN5 (RB3) by it's analog number
        	voltage=adcval*3.3/1023.0;
        	printf("AN5=0x%04x, %.3fV\r", adcval, voltage);
        	fflush(stdout);
			t = 0;
		}
	}
}
