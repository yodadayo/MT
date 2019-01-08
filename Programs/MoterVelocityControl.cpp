/*

file: MoterVelocityControl.cpp
purpose: check velocity control

compile with the command :
g++ -o ./VEL_CONTROL ./MoterVelocityControl.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835

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
#include "libdxl/DynamixelV2.h"

using namespace ssr::dynamixel;

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

#define DEFAULT_VELOCITY 130

int16_t MoterRotate[] = {20, 0, -20, 0};

// 
//
// 

int main(void){

  DynamixelV2 m(SERIAL_PORT, BAUD_RATE);
  // m.TorqueEnable(RIGHT_ROLL);
  // m.TorqueEnable(RIGHT_PITCH);
  // m.TorqueEnable(LEFT_ROLL);
  // m.TorqueEnable(LEFT_PITCH);
  
  usleep(1000);

  int i = 0;
  
  printf("Current Velocity of Moter 1 >> %d\n", m.GetTargetVelocity(1) );

  sleep(1);
    
  while(1){
    
    m.SetTargetVelocity(1, 10);
    printf("Target Velocity of Moter 1 >> 10\n");
    
    // For Debug
    printf("Current Velocity of Moter 1 >> %d\n", m.GetTargetVelocity(1) );
    //
    
    printf("---------------------\n\n");
    
    // m.MovePosition(1, MoterRotate[i]*4096/360+2048);
    // i++;
    // if(i / 4 > 1) i = 0;
    
    usleep(1*1000*1000);

  }
  
  // m.TorqueDisable(RIGHT_ROLL);
  // m.TorqueDisable(RIGHT_PITCH);
  // m.TorqueDisable(LEFT_ROLL);
  // m.TorqueDisable(LEFT_PITCH);
  
  return 0;
  
}
