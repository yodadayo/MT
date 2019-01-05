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
#include "lib/MoterControl.h"
#include "libdxl/DynamixelV2.h"

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 57600

#define EXHAUST 3500
#define SUPPLY 3600
#define CLOSE 3700

#define CS_VALVE 6

int main(void){
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  ssr::dynamixel::DynamixelV2 m(SERIAL_PORT, BAUD_RATE);

  // // start timer
  // timerSetting();

  for(int i=0; i<10; i++)
    vWriteValue[i] = EXHAUST;

  SCC.transfer_volume_all_analog(CS_VALVE);
  
  m.TorqueDisable(1);
  m.TorqueDisable(2);
  m.TorqueDisable(3);
  m.TorqueDisable(4);

  usleep(1*1000*1000);
  
  return 0;
  
}
