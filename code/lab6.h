#include <XC.h>
#include <stdio.h>
#include <stdlib.h>
 
#define LCD_RS  LATBbits.LATB2
#define LCD_RW  LATBbits.LATB1
#define LCD_E   LATBbits.LATB0
#define LCD_D4  LATBbits.LATB12
#define LCD_D5  LATBbits.LATB13
#define LCD_D6  LATBbits.LATB14
#define LCD_D7  LATBbits.LATB15

#define DHT11_OUT LATAbits.LATA3
#define DHT11     PORTAbits.RA3
#define CHARS_PER_LINE 16

#define	SW1     PORTBbits.RB6  


#pragma config FNOSC = FRCPLL       // Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2     // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20     // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_2     // Divide After PLL (now 40 MHz)
 
#pragma config FWDTEN = OFF         // Watchdog Timer Disabled
#pragma config FPBDIV = DIV_1       // PBCLK = SYCLK


#define SYSCLK 40000000L
#define Baud2BRG(desired_baud)( (SYSCLK / (16*desired_baud))-1)
void Lab6PinConfig()
{
    CFGCON = 0;
    ANSELBbits.ANSB3 = 1; 
    TRISBbits.TRISB3 = 1; 
    
    //LCD OUTPUTS
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB1=0;
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB12=0;
    TRISBbits.TRISB13=0;
    TRISBbits.TRISB14=0;
    TRISBbits.TRISB15=0;

    //SW
	TRISBbits.TRISB6 = 1;

	INTCONbits.MVEC  = 1;
}
void UART2Configure(int baud_rate)
{
    // Peripheral Pin Select
    U2RXRbits.U2RXR = 4;    //SET RX to RB8
    RPB9Rbits.RPB9R = 2;    //SET RB9 to TX

    U2MODE = 0;         // disable autobaud, TX and RX enabled only, 8N1, idle=HIGH
    U2STA = 0x1400;     // enable TX and RX
    U2BRG = Baud2BRG(baud_rate); // U2BRG = (FPb / (16*baud)) - 1
    
    U2MODESET = 0x8000;     // enable UART2
}
void ADCConf(void)
{
    AD1CON1CLR = 0x8000;    // disable ADC before configuration
    AD1CON1 = 0x00E0;       // internal counter ends sampling and starts conversion (auto-convert), manual sample
    AD1CON2 = 0;            // AD1CON2<15:13> set voltage reference to pins AVSS/AVDD
    AD1CON3 = 0x0f01;       // TAD = 4*TPB, acquisition time = 15*TAD 
    AD1CON1SET=0x8000;      // Enable ADC
}
int ADCRead(char analogPIN)
{
    AD1CHS = analogPIN << 16;    // AD1CHS<16:19> controls which analog pin goes to the ADC
 
    AD1CON1bits.SAMP = 1;        // Begin sampling
    while(AD1CON1bits.SAMP);     // wait until acquisition is done
    while(!AD1CON1bits.DONE);    // wait until conversion done
 
    return ADC1BUF0;             // result stored in ADC1BUF0
}
void Timer1us(unsigned char t) 
{
     T1CON = 0x8000; // enable Timer1, source PBCLK, 1:1 prescaler
 
    // delay 100us per loop until less than 100us remain
    while( t >= 100)
    {
        t-=100;
        TMR1 = 0;
        while( TMR1 < SYSCLK/10000);
    }
 
    // delay 10us per loop until less than 10us remain
    while( t >= 10)
    {
        t-=10;
        TMR1 = 0;
        while( TMR1 < SYSCLK/100000);
    }
 
    // delay 1us per loop until finished
    while( t > 0)
    {
        t--;
        TMR1 = 0;
        while( TMR1 < SYSCLK/1000000);
    }
    // turn off Timer1 so function is self-contained
    T1CONCLR = 0x8000;
}
void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Timer1us(250);
}

float LM355_Read()
{	
	float voltage =ADCRead(5)*3.3/1023.0;
	return (100.0*(voltage-2.73));
}

void LCD_pulse (void)
{
	LCD_E=1;
    Timer1us(40);
	LCD_E=0;
}



void LCD_byte (unsigned char x)
{

   LCD_D7=x & 0x80 ? 1 : 0;

   LCD_D6=x & 0x40 ? 1 : 0;

   LCD_D5=x & 0x20 ? 1 : 0;

   LCD_D4=x & 0x10 ? 1 : 0;

   LCD_pulse();

   Timer1us(40);

   LCD_D7=x & 0x08 ? 1 : 0;

   LCD_D6=x & 0x04 ? 1 : 0;

   LCD_D5=x & 0x02 ? 1 : 0;

   LCD_D4=x & 0x01 ? 1 : 0;

   LCD_pulse();

}
void WriteData (unsigned char x)
{
	LCD_RS=1;
	LCD_byte(x);
	waitms(2);
}

void WriteCommand (unsigned char x)
{
	LCD_RS=0;
	LCD_byte(x);
	waitms(5);
}

void LCD_4BIT (void)
{
	LCD_E=0; // Resting state of LCD's enable is zero
	LCD_RW=0; // We are only writing to the LCD in this program
	waitms(20);
	// First make sure the LCD is in 8-bit mode and then change to 4-bit mode
	WriteCommand(0x33);
	WriteCommand(0x33);
	WriteCommand(0x32); // Change to 4-bit mode

	// Configure the LCD
	WriteCommand(0x28);
	WriteCommand(0x0c);
	WriteCommand(0x01); // Clear screen command (takes some time)
	waitms(20); // Wait for clear screen command to finsih.
}

void LCDprint(char * string, unsigned char line, int clear)
{
	int j;
	WriteCommand(line==2?0xc0:0x80);
	waitms(5);
	for(j=0; string[j]!=0; j++)	WriteData(string[j]);// Write the message
	if(clear) for(; j<CHARS_PER_LINE; j++) WriteData(' '); // Clear the rest of the line
}

void Initiate_DHT11()			
{

    
    TRISAbits.TRISA3=0;
	DHT11_OUT = 1;		
	waitms(20);	
	DHT11_OUT = 0;		
	waitms(20);	
	DHT11_OUT = 1;		
	
	return;
}

void Verify()
{
    TRISAbits.TRISA3=1;
	while(DHT11==1);
	while(DHT11==0);
	while(DHT11==1);

	return;
}
int Receive_data()		
{
	int dat=0,i;	
	for (i=0; i<8; i++)
	{
		while(DHT11==0);
		Timer1us(30);

		if(DHT11 == 1)	
			dat = (dat<<1)|(0x01);
		else	
			dat = (dat<<1);
		while(DHT11==1);
	}
	return dat;
}


void DHT11_Read(float* Temp, float* Hum)
{
	int Hum_High,Hum_Low,Temp_High,Temp_Low; 
   
	Initiate_DHT11();  
	Verify();
	Hum_High=Receive_data();	
	Hum_Low=Receive_data();	    
	Temp_High=Receive_data();	
	Temp_Low=Receive_data();
	
	*Temp=Temp_High+Temp_Low/10.0;
	*Hum=Hum_High+Hum_Low/10.0;
	
	return;
}

void SW1_Check(int* state)
{
    if(SW1==1) return ;
	else  waitms(20);
	if(SW1==1) return ;

	while(SW1==0);

    if(*state<5) (*state)++;
    else *state = 0;

    return;
 
}