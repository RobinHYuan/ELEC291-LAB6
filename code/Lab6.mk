SHELL=cmd
CC = xc32-gcc
OBJCPY = xc32-bin2hex
ARCH = -mprocessor=32MX170F256B
OBJ = Lab6.o
PORTN=$(shell type COMPORT.inc)
HEADER = include/lab6.h 

Lab6.elf: $(OBJ)
	$(CC) $(ARCH) -o Lab6.elf Lab6.o -mips16 -DXPRJ_default=default -legacy-libc -Wl,-Map=Lab6.map
	$(OBJCPY) Lab6.elf
	@echo Success!
   
Lab6.o: Lab6.c lab6.h
	$(CC) -mips16 -g -x c -c $(ARCH) -MMD -o Lab6.o Lab6.c  -DXPRJ_default=default -legacy-libc

clean:
	@del *.o *.elf *.hex *.d *.map 2>NUL
	
LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	pro32.exe -p -v Lab6.hex

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	c:\putty\putty.exe -serial $(PORTN) -sercfg 115200,8,n,1,N -v

dummy: Lab6.hex Lab6.map
	$(CC) --version

explorer:
	@explorer .