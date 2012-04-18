# SD Memory Card Library for AVR Microcontrollers

Note: I originally wrote this small library back in 2012 while working on my university project, Wave Audio Player with AVR Microcontrollers and an SD Card. I'm sharing it here as a way to preserve the memory and reflect on that time.

# How to Use:
 
Include "mega32.h" and "spi.h" headers and then our header: "sd_lib.h" . For Example our Microcontroller is ATmega32

#include <mega32.h>

#include <spi.h>

#include "sd_lib.h"

Init SPI Registers 

SPCR=0x50;

SPSR=0x01;

Init SD-Card. It Returns "0" If It is initiated properly

unsigned char  sd_init(void);

Finally use read and write functions as mush as you need. Input is SD-Card Sector number and output is sector size buffer array.

unsigned char  read_sd(unsigned long int  sector,unsigned char* buffer);

unsigned char  write_sd(unsigned long int sector,unsigned char* buffer);

