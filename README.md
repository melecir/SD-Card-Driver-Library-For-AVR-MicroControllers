# SD Memory Card Library for AVR Microcontrollers

Note: I originally wrote this small library back in 2012 while working on my university project, Wave Audio Player with AVR Microcontrollers and an SD Card. I'm sharing it here as a way to preserve the memory and reflect on that time.

It is tested in `CodeVisionAVR IDE` with `ATMEGA32A`

# How to Use
 
Include `mega32.h` and `spi.h` headers and then this header `sd_lib.h`

For example our microcontroller is `ATmega32`

```c
#include <mega32.h>

#include <spi.h>

#include "sd_lib.h"
```
Init SPI Registers 

```c
SPCR=0x50;

SPSR=0x01;
```

## Init SD Card
It returns `0` If it is initiated correctly
```
unsigned char  sd_init(void);
```

Finally use `read_sd` and `read_sd` functions to read data.

```
unsigned char  read_sd(unsigned long int  sector,unsigned char* buffer);

unsigned char  write_sd(unsigned long int sector,unsigned char* buffer);

```
Input is SD Card sector number and output is sector size buffer array.
