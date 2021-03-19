# ELEC291-LAB6

------------

### Introduction:

In this lab, we will be using the PIC32MX170F256B-50I/SP microcontroller, LM335 temperature sensor and DHT11 temperature and humidity sensor to meausre current temperature and humidity. The temperature data will be trasmitted from the MCU to a computer to plot a strip chart in real time via serial port using python.

### Team Member:
Jerry Shao, 84982321 \
Robin Yuan, 88011879

------------

### Extra Features:

1. DHT11 senor is used to provide humidity and temperature readings
2. LCD will refresh the readings every second
3. Mutiple readings are proivided, which are current temperature/humidity, thier historical maximum and minimum values respectively
4. Push-Button allows the users to cylce through the readings
5. Calibratable LM355 sesnor is used to ensure accurate temperature readings 
6. Auto-backlighting circuit is used so that users can read the temperature and humidity with ease in a dark environment
7. A dedicate program flasher is made to ensure we can utlize as many pins as possible
8. Improved algorithm that combines both sensors' readings provides better results
9. PIC32MX170F256B is used providing a higher comuting speed.
10. All axes are labed for the stip chart
11. Makefile is modified to include the new header file created for LAB6 making the source code look more concise
12. Portable Design; the system does not require any USB connection to operate. Instead, we can use a 9v battery to power the system.

------------

### PIN Assignment:


#define LCD_RS  LATBbits.LATB2\
#define LCD_RW  LATBbits.LATB1\
#define LCD_E   LATBbits.LATB0\
#define LCD_D4  LATBbits.LATB12\
#define LCD_D5  LATBbits.LATB13\
#define LCD_D6  LATBbits.LATB14\
#define LCD_D7  LATBbits.LATB15

#define DHT11_OUT LATAbits.LATA3\
#define DHT11     PORTAbits.RA3

#define	SW1     PORTBbits.RB6  

#### NOTE: 
DHT11 and DHT11_OUT shares the same physical pin. However, when sending out the data request from the MCU to the DHT11, the pin is used as a digital output and it writes to the latch. When reading the data from the sensor, the pin is configured as a digital input and the MCU directly reads the data from the port pin.

------------
### System Structure:
<img src="https://user-images.githubusercontent.com/68177491/111743592-2df4b700-8847-11eb-930a-67bf5d76816b.jpg" width="496" height="663"/>

------------
### Matou Sakura is the best girl

<img src="https://user-images.githubusercontent.com/68177491/111581768-1a7a1b00-8777-11eb-8804-cf1e24364a15.jpeg" width="627" height="872"/>
