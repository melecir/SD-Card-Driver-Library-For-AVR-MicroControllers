#ifndef sd_header
#define sd_header
/*-----------------------------------------------------------------------*/
/* library for SD cards(SDSC & SDHC &SDXC) for CodeVisionAVR            */   
/* Version: 1.1   
/* Date: 2012
/* Author: Milad Jahandideh
/* WebSite:  Melec.ir                                                     */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* SPI configuration  //befor seting spi registers call spi_init();      */
/*  SPI MODE 0  / MSB FIRST  /SPI TYPE MASTER                            */
/*-----------------------------------------------------------------------*/
#define  SPI_PORT        PORTB
#define  SPI_DDR         DDRB
#define  SPI_PORT_MOSI   5
#define  SPI_PORT_MISO   6
#define  SPI_PORT_Sck    7
#define  SPI_PORT_SS     4
#define  SPI_PORT_CS     0  

#define  cs   SPI_PORT.SPI_PORT_CS

/*-----------------------------------------------------------------------*/
/* COMMANDS  FOR SD CARDS                                                */
/*-----------------------------------------------------------------------*/
#define cmd0    (0x40+0)
#define cmd1    (0x40+1)
#define cmd8    (0x40+8)
#define cmd55   (0x40+55)
#define ACMD41  (0x40+41)
#define cmd17   (0x40+17)
#define cmd24   (0x40+24)
#define cmd13   (0x40+13)
#define cmd58   (0x40+58)
/*-----------------------------------------------------------------------*/
/* functions                                                             */
/*                                                                       */
/*-----------------------------------------------------------------------*/
#pragma used+
void spi_init(void);//call this function befor setting spi registers.
unsigned char sd_send_command(unsigned char cmd,unsigned long int arg);
unsigned char sd_init(void);
unsigned char  read_sd(unsigned long int  sector,unsigned char* buffer);
unsigned char  write_sd(unsigned long int sector,unsigned char* buffer);
#pragma used-
/*-----------------------------------------------------------------------*/
/* var                                                                   */
/*-----------------------------------------------------------------------*/
 bit ocr_bit30;
/*-----------------------------------------------------------------------*/
/* spi init                                                              */
/*-----------------------------------------------------------------------*/
void spi_init(void){
SPI_DDR=(1<<SPI_PORT_MOSI)|(1<<SPI_PORT_Sck)|(1<<SPI_PORT_SS)|(1<<SPI_PORT_CS);
}
/*-----------------------------------------------------------------------*/
/* send command to sd                                                    */
/*-----------------------------------------------------------------------*/
unsigned char sd_send_command(unsigned char cmd,unsigned long int arg){
unsigned char resp=255,crc=0x01;  
if(cmd==0x40) crc=0x95;    //crc for cmd0 
if(cmd==0x48) crc=0x87;    //crc for cmd8
cs=1;
spi(0xff);
cs=0;
spi(0xff);
spi(cmd);
spi(arg>>24);
spi(arg>>16);
spi(arg>>8);
spi(arg);
spi(crc);
while((resp = spi(0xFF)) == 0xff);
return resp;
}
/*-----------------------------------------------------------------------*/
/* sd init                                                               */
/*-----------------------------------------------------------------------*/
unsigned char sd_init(){
  unsigned char i, resp,resp_cmd8_buff[3];
  unsigned int j=0;       
  unsigned long int ACMD41_ARG;  
     for(i=0;i<100;i++){       // 74+ clock pulse for  init
       cs=1;
      spi(0xff); 
       }  
    resp=sd_send_command(cmd0,0);      //cmd0 with arg 0 
    if(resp!=0x01) return 255;        //if there is no card resp will be 255 . 
    resp=sd_send_command(cmd8,0x000001AA);  //cmd8 with arg 0x000001AA
    
              
            ACMD41_ARG=0X00000000;    //for sd v1  
            spi(0xff);  //for get response  of cmd8 32bit 
            spi(0xff);  //for get response  of cmd8 32bit 
            resp_cmd8_buff[0]=spi(0xff); //for get response  of cmd8 32bit    
            resp_cmd8_buff[1]=spi(0xff);  //for get response  of cmd8 32bit   
            if(resp_cmd8_buff[0]==0x01 && resp_cmd8_buff[1]==0xAA){ 
            ACMD41_ARG=0X40000000;           // sd v2                       
            }
           
            do{                         
              resp=sd_send_command(cmd55,0);   //cmd55 
              resp=sd_send_command(ACMD41,ACMD41_ARG);  //ACMD41
              j++;  
              if(j==30000) return resp;                  
             }while(resp !=0); 
                if(ACMD41_ARG==0X40000000 ){  //for sd v2 
                resp=sd_send_command(cmd58,0); // get ocr register 
                if(resp !=0) return resp;      
                cs=0;
                resp=spi(0xff); 
                spi(0xff);
                spi(0xff);
                spi(0xff);  
                   if((resp=resp&0x40)==0){    
                   ocr_bit30=0;      //SDSC
                   }
                   else{
                   ocr_bit30=1;    //SDHC & SDXC 
                    }
                }
                cs=1;
             return 0;             
}

/*-----------------------------------------------------------------------*/
/* read sd                                                               */
/*-----------------------------------------------------------------------*/
unsigned char  read_sd(unsigned long int  sector,unsigned char* buffer){
unsigned char resp;
unsigned int i; 
if(ocr_bit30==0){  //sector*512 in SDSC 
sector=sector<<9;
}
     
          resp=sd_send_command(cmd17,sector);  //cmd17  with arg sector*512            
          if(resp !=0x00) return resp;          
          while((resp = spi(0xFF)) != 0xFE); //wait to get read data token 0XFE     
           for(i=0;i<512;i++){              //get 512 byte data 
                *buffer++ = spi(0xFF);  
                 }
                 spi(0xFF);               //crc byte1
                 spi(0xFF);               //crc byte2   
                 resp=0; 
                 cs=1;
                 return resp;             
}



/*-----------------------------------------------------------------------*/
/* write to sd                                                            */
/*-----------------------------------------------------------------------*/
unsigned char write_sd(unsigned long int sector,unsigned char* buffer){
unsigned char resp,resp2;
unsigned int i;
if(ocr_bit30==0){
sector=sector<<9;   //sector*512 in SDSC 
}
  resp=sd_send_command(cmd24,sector);    //cmd24 for write one block data
  if(resp !=0x00) return resp; 
  cs=0;
  spi(0xff);
  spi(0xFE);
    for(i=0;i<512;i++){                //write 512 byte data 
     spi(*buffer++);
    }               
    spi(0xff);
    spi(0xff);
    resp=spi(0xff);  
    if((resp=resp&0x1A)!=0 ) return resp; // check data response
    while((resp=spi(0xff))==0);    //wait until card is busy 
    resp=sd_send_command(cmd13,0);  //cmd13 for sd status 
    if(resp !=0) return resp;     //byte1 R2 
    resp2=spi(0xff);              //byte2 R2 
    cs=1;
    return resp2;
}

/*-----------------------------------------------------------------------*/
/* end                                                                   */
/*-----------------------------------------------------------------------*/
#endif 
