/*

file: moter_test.cpp
purpose: a test for Raspberry Pi to move Dynamixel to the right angle.

compile with the command :
g++ -o ./M_TEST ./moter_test.cpp ./lib/SlaveControlCommand.cpp ./lib/OperationCommand.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835

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

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 57600
#define ID_MAX 4
#define PHASE 4

#define EXHAUST 3500
#define SUPPLY 3600
#define CLOSE 3700

#define CS_VALVE 6

int16_t MoterValue[ID_MAX][PHASE][2] = // {deg, rpm}
  { { {-30,100}, {30,100}, {-30,100}, {30,100} },
    { {30,200}, {-30,200}, {30,200}, {-30,200} },
    { {0,300}, {40,300}, {0,300}, {-40,300} },
    { {-40,100}, {0,100}, {40,100}, {0,100} }
  };

int main(void){

  int i, j, cycle = 0;
  ssr::dynamixel::DynamixelV2 m(SERIAL_PORT, BAUD_RATE);

  m.TorqueEnable(1);
  m.TorqueEnable(2);
  m.TorqueEnable(3);


  for(j=0; j<PHASE; j++){
    for(i=0; i<ID_MAX-2; i++){
      m.MovePosition(i+1, -MoterValue[i][j][0]*4096/360+2048);
    // m.MovePosition(1, -MoterValue[1][0][0]*4096/360+2048);
    // m.MovePosition(2, -MoterValue[2][1][0]*4096/360+2048);
    }
    usleep(1*1000*1000);
  }

  
  m.TorqueDisable(1);
  m.TorqueDisable(2);
  m.TorqueDisable(3);

  return 0;
}

