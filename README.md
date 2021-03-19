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
2. LCD will refresh the readings every minute
3. Mutiple readings are proivided, which are current temperature/humidity, thier historical maximum and minimum values respectively
4. Push-Button allows the users to cylce through the readings
5. Calibratable LM355 sesnor is used to ensure accurate temperature readings 
6. Auto-backlighting circuit is used so that users can read the temperature and humidity with ease in a dark environment
7. A dedicate program flasher is made to ensure we can utlize as many pins as possible
8. Improved algorithm that combines both sensors' readings provides better results
9. PIC32MX170F256B is used providing a higher comuting speed.
10. Labeled axis for the stip chart
11. Makefile is modified to include the new header file created for LAB6 making the source code look more concise
12. Portable Design; the system does not require any USB connection to operate. Instead, we can use a 9v battery to power the system.

------------
### Matou Sakura is the best girl

<img src="https://user-images.githubusercontent.com/68177491/111581768-1a7a1b00-8777-11eb-8804-cf1e24364a15.jpeg" width="627" height="872"/>
