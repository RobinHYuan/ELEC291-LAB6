#include <XC.h>
#include "lab6.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1

#define _XTAL_FREQ 4000000

void main()
{
	volatile unsigned long t=0;

    float Temp=0,Temp_LM355=0,Temp_DHT11=0,Hum_DHT11=0;
    float Max_Temp=0,Min_Temp=99,Max_Hum=0,Min_Hum=99;
	int state = 0;
	char string [16] = " " ;
	char percent = '%';
	
  	Lab6PinConfig();
    UART2Configure(115200);  // Configure UART2 for a baud rate of 115200
    ADCConf(); // Configure ADC
 	LCD_4BIT();
 	
 	waitms(1000);
 	
 
 	
	while(TRUE)
	{ 		
    		Temp_LM355=LM355_Read();
        	DHT11_Read(&Temp_DHT11, &Hum_DHT11);
        	Temp=0.3*Temp_LM355+0.7*Temp_DHT11;
        	
        	printf("%.3f\n",Temp);
        	fflush(stdout); 
        	
        	if(Temp>Max_Temp) Max_Temp=Temp;
        	if(Temp<Min_Temp) Min_Temp=Temp;
        	if(Hum_DHT11>Max_Hum) Max_Hum=Hum_DHT11;
        	if(Hum_DHT11<Min_Hum) Min_Hum=Hum_DHT11;
        	
			switch( state )
			
			{	
				case 0:
					LCDprint("Temperature:",1,1);
					LCDprint("              C",2,1);
					sprintf(string,"Current: %.1f\xdf",Temp);  
					LCDprint(string,2,0);
				break;
			
				case 1:
					LCDprint("          C",2,1);
					sprintf(string,"Max: %.1f\xdf",Max_Temp);  
					LCDprint(string,2,0);
				break;
				
				case 2:
					LCDprint("          C",2,1);
					sprintf(string,"Min: %.1f\xdf",Min_Temp);  
					LCDprint(string,2,0);
				break;
				
				case 3:
					LCDprint("Humidity:",1,1);
					sprintf(string,"Current: %.1f%c",Hum_DHT11,percent);
					LCDprint(string,2,1);
				break;
				
				case 4:
					LCDprint("Humidity:",1,1);
					sprintf(string,"Max: %.1f%c",Max_Hum,percent);
					LCDprint(string,2,1);
				break;
				
				case 5: 
					LCDprint("Humidity:",1,1);
					sprintf(string,"Min: %.1f%c",Min_Hum,percent);
					LCDprint(string,2,1);
				break;
			}
			

			waitms(1000);
			SW1_Check(&state);
	}
}
