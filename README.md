# SD-Card-Driver-Lib-For-AVR-MicroControllers
SD Card Driver Library For AVR MicroControllers. It is tested in CodeVisionAVR IDE and ATMEGA32A

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

This Code was written by me several years ago (2012) and I just wanted to have it here for memory :)
