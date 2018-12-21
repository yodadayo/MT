// #include <wiringPi.h>
// #include <wiringPiSPI.h>
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "extern.h"
#include "SPIM_Ras.h"
#include <math.h>
#include <bcm2835.h>
#define SPI_CHANNEL 0
#define SPI_CHANNEL2 1	


SPIM_Ras::SPIM_Ras(void)
{
}
short SPIM_Ras::initialization(int speed, int speed2){
	// pinMode(13, INPUT);
	// pullUpDnControl(13,PUD_UP);
	// pinMode(12, OUTPUT);
	// digitalWrite(12, 0);
	// pinMode(14, OUTPUT);
	// digitalWrite(14, 0);
	// pullUpDnControl(14,PUD_UP);		
	// buff = (unsigned char *)malloc(6);
	bcm2835_init();
	bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS_NONE, LOW);
	// *buff = 0x0;	
}
short SPIM_Ras::begin(char pin){				
	// pinMode(pin, OUTPUT);
	// digitalWrite(pin, 1);
}

uint16_t SPIM_Ras::transfer(char pin, uint16_t data){
	char  buff[2]={0,0};
	buff[0] = (data>>8)&0xFF; buff[1] = data&0xFF;
	bcm2835_gpio_write(pin, LOW);
	bcm2835_spi_transfern(buff, 2);
	bcm2835_gpio_write(pin, HIGH);
	data = ( (buff[1]) & 0x00FF )|( (buff[0]<<8) & 0xFF00 );
	return data;
}

uint16_t SPIM_Ras::transferArm(char pin, uint16_t data){
	// unsigned char  buff[2]={0,0};
	// digitalWrite(pin, 0);
	// buff[0] = data & 0xFF; 
	// wiringPiSPIDataRW(SPI_CHANNEL2, buff,1);
	// data = buff[0] & 0xFF ;
	// digitalWrite(pin, 1);
	return data;
}

uint16_t SPIM_Ras::transferEncoder1(char pin, uint16_t data){

	// unsigned char  buff[2]={0,0};
	// digitalWrite(pin, 0);
	// // delayMicroseconds(1);
	// buff[0] = (data>>8)&0xFF; buff[1] = data&0xFF;
	// wiringPiSPIDataRW(SPI_CHANNEL2, buff, 2);   
	// data = ( (buff[1]) & 0x00FF )|( (buff[0]<<8) & 0xFF00 );
	// digitalWrite(pin, 1);
	return (data >>4) & 0xFFF;
}

uint64_t SPIM_Ras::transferEncoder3(char pin, uint64_t data){
	// unsigned char buff[7]={0,0,0,0,0,0};

	// digitalWrite(pin, 0);
	// // delayMicroseconds(1);
	// wiringPiSPIDataRW(SPI_CHANNEL2, buff, 7);   
	// data  = (buff[0]&0xFF)*pow(2,8);
	// data += (buff[1]&0xFF)*pow(2,0);
	// data += (buff[2]&0xFF)*pow(2,24);
	// data += (buff[3]&0xFF)*pow(2,16);
	// // data += (buff[4]&0xFF)*pow(2,40);
	// data += (buff[5]&0xFF)*pow(2,40);
	// data += (buff[6]&0xFF)*pow(2,32);
	// // data += (buff[6]&0xFF)*pow(2,56);
	// data += (buff[7]&0xFF)*pow(2,48);

	// digitalWrite(pin, 1);
	return data;
}

SPIM_Ras SPIR;