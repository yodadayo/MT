//DA-コンバータ
#include "daconv_v2.h"
#define V_REF 5		//V_REF=5V
#define M 0xFFFF		//デフォルト
#define C 0x8000		//デフォルト
#define OFFSET_CODE 0x2000
#define V_SIGGND 0

int DA_Write(int pin, double V_OUT, int DA_SS){
	int DAC_CODE, INPUT_CODE, group, channnel;
	unsigned char buff[3];
	group = pin / 8;  //groupの指定 0,1
	channnel = pin % 8; //channnelの指定 0~7 *すべてのチャンネルを同時に選択は未実装*
	if(group == 0){
		buff[0] = 0b11000000 + 0b00001000 + channnel;
	}
	else if(group == 1){
		buff[0] = 0b11000000 + 0b00010000 + channnel;
	}
	else if(group == 2){
		buff[0] = 0b11000000 + 0b00000000;//all group and all channel
	}

	DAC_CODE = 4 * OFFSET_CODE +  ( V_OUT - V_SIGGND ) * pow(2,16) / ( 4 * V_REF );
	INPUT_CODE = ( DAC_CODE - C + pow( 2,15 )) * pow( 2,16 ) / ( M + 1 );
	buff[1] = (INPUT_CODE >> 8) & 0b11111111;
	buff[2] =INPUT_CODE & 0b11111111;
	//printf("Buff = %3x%3x%3x\n", buff[0], buff[1],buff[2]);
	digitalWrite(DA_SS, 0);	                	//SS信号をLOW出力にして通信開始
	wiringPiSPIDataRW(0, buff,3);
	digitalWrite(DA_SS, 1);	                	//SS信号をHIGH出力にして通信
	
	//digitalWrite(DA_LDAC, 0);
	//digitalWrite(DA_LDAC, 1);
}

/*
	switch(mode){
	case 3: //change X register
		if(group == 0){
			buff[0] = 0b11000000 + 0b00001000 + channnel;
		}
		else if(group == 1){
			buff[0] = 0b11000000 + 0b00010000 + channnel;
		}
		else if(group == 2){
			buff[0] = 0b11000000 + 0b00000000;//all group and all channel
		}
		break;
	case 2: //change C register
		if(group == 0){
			buff[0] = 0b10000000 + 0b00001000 + channnel;
		}
		else if(group == 1){
			buff[0] = 0b10000000 + 0b00010000 + channnel;
		}
		else if(group == 2){
			buff[0] = 0b10000000 + 0b00000000;//all group and all channel
		}
		break;
	case 1: //change M register
		if(group == 0){
			buff[0] = 0b01000000 + 0b00001000 + channnel;
		}
		else if(group == 1){
			buff[0] = 0b01000000 + 0b00010000 + channnel;
		}
		else if(group == 2){
			buff[0] = 0b01000000 + 0b00000000;//all group and all channel
		}
		break;
	}
	*/