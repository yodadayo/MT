/*

file: AngleTest.cpp
purpose: check an angle of joint at each pressure.

compile with the command :
g++ -o ./ANGLE ./AngleTest.cpp ./lib/SlaveControlCommand.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <signal.h>
#include <unistd.h>
// original library
#include "lib/initialization.h"
#include "lib/SPIM_Ras.h"
#include "lib/SlaveControlCommand.h"
#include "libdxl/DynamixelV2.h"

#define EXHAUST 3500
#define SUPPLY 3600
#define CLOSE 3700
#define V_NUM 8

#define CS_VALVE 6

// Moter 
#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 57600
#define ID_NUM 4

#define RIGHT_ROLL 3
#define RIGHT_PITCH 1
#define LEFT_ROLL 4
#define LEFT_PITCH 2

// Joint Select
#define R_KNEE 0
#define L_KNEE 1
#define R_ANKLE 2
#define L_ANKLE 3
#define CHECK 4


// 
//
// 

inline uint16_t vtops(uint16_t vol)
{
  return 0.286*(vol-800)+100;
}

inline uint16_t pstov(uint16_t ps)
{
  if(ps == EXHAUST || ps == SUPPLY || ps == CLOSE)
    return ps;
  else
    return 3.50*ps+450;
}

int main(void){
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  ssr::dynamixel::DynamixelV2 m(SERIAL_PORT, BAUD_RATE);
  
  usleep(1000);

  uint8_t joint; int i;
  
  //
  //
  //  Mode Setting
  //    (you can select the joint to check here)
  //
  //
  joint = L_ANKLE;


  // Valve Settings
  switch(joint){
  // left knee setting
  case L_KNEE:
    for(i = 0; i < V_NUM; i++){
      vWriteValue[i] = pstov(100);
      if(i == 7) vWriteValue[9] = pstov(100);
    }
    vWriteValue[9] = pstov(600);		// front
    vWriteValue[5] = pstov(600);		// back
    printf("Left Knee\n\n");
    break;

  // right knee setting
  case R_KNEE:
    for(i = 0; i < V_NUM; i++){      
      vWriteValue[i] = pstov(100);
      if(i == 7) vWriteValue[9] = pstov(100);      
    }
    vWriteValue[6] = pstov(500);		// front
    vWriteValue[2] = pstov(600);		// back
    printf("Right Knee\n\n");
    break;

  // left ankle setting
  case L_ANKLE:
    for(i = 0; i < V_NUM; i++){      
      vWriteValue[i] = pstov(100);
      if(i == 7) vWriteValue[9] = pstov(100);      
    }
    vWriteValue[3] = pstov(100);		// front
    vWriteValue[4] = pstov(600);		// back
    printf("Left Ankle\n\n");
    break;

  // right ankle setting
  case R_ANKLE:
    for(i = 0; i < V_NUM; i++){      
      vWriteValue[i] = pstov(100);
      if(i == 7) vWriteValue[9] = pstov(100);      
    }
    vWriteValue[0] = pstov(100);		// front
    vWriteValue[1] = pstov(600);		// back
    printf("Right Ankle\n\n");
    break;

  // free setting
  case CHECK:
    for(i = 0; i < V_NUM; i++){      
      vWriteValue[i] = pstov(100);
      if(i == 7) vWriteValue[9] = pstov(100);      
    }
    vWriteValue[6] = pstov(300);
    printf("check\n\n");
    break;

  default:
    printf("error\n");
    exit(1);
    break;

  }
  
  sleep(1);
    
  while(1){
    
    SCC.transfer_volume_all_analog(CS_VALVE);

    
    // For Debug
    for(i = 0; i < V_NUM; i++){
      if(i != 7)
	printf("TargetValvePressure %d >> %d\n", i, vWriteValue[i]);
      else 
	printf("TargetValvePressure 9 >> %d\n", vWriteValue[9]);
    }
    //
    
    
    for(i = 0; i < V_NUM; i++){
      if(i != 7)
	printf("ValvePressure %d >> %d\n", i, vtops(vReadValue[i]));
      else 
	printf("ValvePressure 9 >> %d\n", vtops(vReadValue[9]));
    }
    printf("---------------------\n\n");
    
    usleep(1*1000*1000);

  }
  
  return 0;
  
}
