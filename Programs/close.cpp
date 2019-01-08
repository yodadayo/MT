/*

file: close.cpp
purpose: close valves

compile with the command :
g++ -o ./CLOSE ./close.cpp ./lib/SlaveControlCommand.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835

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


inline uint16_t vtops(uint16_t vol)
{
  return 0.286*(vol-800)+100;
}

int main(void){
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  ssr::dynamixel::DynamixelV2 m(SERIAL_PORT, BAUD_RATE);
  usleep(1000);
  
  // // start timer
  // timerSetting();
  
  usleep(1*1000*1000);
  
  for(int i=0; i<10; i++)
    vWriteValue[i] = CLOSE;

  SCC.transfer_volume_all_analog(CS_VALVE);
  
  
  for(int i=0; i<V_NUM; i++){
    if(i != 7)
      printf("ValvePressure %d >> %d\n", i, vtops(vReadValue[i]));
    else 
      printf("ValvePressure 9 >> %d\n", vtops(vReadValue[9]));
  }
  printf("---------------------\n\n");
  

  usleep(1*1000);
  
  return 0;
  
}
