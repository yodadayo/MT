#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "extern.h"
#include "SPIM_Ras.h"
#include <math.h>
#define SPI_CHANNEL 0
#define SPI_CHANNEL2 1	


SPIM_Ras::SPIM_Ras(void)
{
}
short SPIM_Ras::initialization(int speed, int speed2){
	pinMode(13, INPUT);
	pullUpDnControl(13,PUD_UP);
	pinMode(12, OUTPUT);
	digitalWrite(12, 0);
	pinMode(14, OUTPUT);
	digitalWrite(14, 0);
	// pullUpDnControl(14,PUD_UP);		
	// buff = (unsigned char *)malloc(6);
	if((wiringPiSPISetupMode (SPI_CHANNEL, speed,0)) < 0){
		printf("wiringPiSPISetup error \n");
		return -1 ;
	}
	if((wiringPiSPISetupMode (SPI_CHANNEL2, speed2,0)) < 0){
		printf("wiringPiSPISetup error \n");
		return -1 ;
	}
	if(wiringPiSetup() == -1){
		printf("wiringPiSetup error\n");
		return -1;
	}
	// *buff = 0x0;	
}
short SPIM_Ras::begin(char pin){				
	pinMode(pin, OUTPUT);
	digitalWrite(pin, 1);
}

uint16_t SPIM_Ras::transfer(char pin, uint16_t data){
	unsigned char  buff[2]={0,0};
	digitalWrite(pin, 0);
	buff[0] = (data>>8)&0xFF; buff[1] = data&0xFF;
	wiringPiSPIDataRW(SPI_CHANNEL, buff, 2);   
	data = ( (buff[1]) & 0x00FF )|( (buff[0]<<8) & 0xFF00 );
	digitalWrite(pin, 1);
	return data;
}

uint16_t SPIM_Ras::transferArm(char pin, uint16_t data){
	unsigned char  buff[2]={0,0};
	digitalWrite(pin, 0);
	buff[0] = data & 0xFF; 
	wiringPiSPIDataRW(SPI_CHANNEL2, buff,1);
	data = buff[0] & 0xFF ;
	digitalWrite(pin, 1);
	return data;
}

uint16_t SPIM_Ras::transferEncoder1(char pin, uint16_t data){

	unsigned char  buff[2]={0,0};
	digitalWrite(pin, 0);
	// delayMicroseconds(1);
	buff[0] = (data>>8)&0xFF; buff[1] = data&0xFF;
	wiringPiSPIDataRW(SPI_CHANNEL2, buff, 2);   
	data = ( (buff[1]) & 0x00FF )|( (buff[0]<<8) & 0xFF00 );
	digitalWrite(pin, 1);


	return (data >>3) & 0xFFFF;
}

uint64_t SPIM_Ras::transferEncoder3(char pin, uint64_t data){
	unsigned char buff[7]={0,0,0,0,0,0};

	digitalWrite(pin, 0);
	// delayMicroseconds(1);
	wiringPiSPIDataRW(SPI_CHANNEL2, buff, 7);   
	data  =  (( ((buff[0]<<8) +buff[1])>>3 )&0xFFF)*pow(2,0);
	data += ( ((buff[2]<<8) +buff[3])&0xFFF)*pow(2,12);
	data += (((buff[5]<<8) +buff[6])>>5)*pow(2,24);
	// data += (buff[3]&0xFF)*pow(2,16);
	// // data += (buff[4]&0xFF)*pow(2,40);
	// data += (buff[5]&0xFF)*pow(2,40);
	// data += (buff[6]&0xFF)*pow(2,32);
	// data += (buff[6]&0xFF)*pow(2,56);
	// data += (buff[7]&0xFF)*pow(2,48);

	// printf("%d\n", ((buff[2]<<8) +buff[3])&0xfff);
	// printf("%d\n", ((buff[0]<<8) +buff[1])>>3);
	// printf("%d\n", ((buff[5]<<8) +buff[6])>>5);

	digitalWrite(pin, 1);
	return data;
}

SPIM_Ras SPIR;