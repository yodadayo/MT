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

using namespace ssr::dynamixel;

struct itimerspec  itval1, itval100;
struct sigevent se;
struct sigaction sa;
timer_t timerid_thread_1,timerid_thread_spi,timerid_thread;
timer_t timerid_signal;

void timerSetting(void);

void handler_SPI(union sigval);

uint16_t count = 0; uint8_t cycle = 0;

int16_t MoterValue[ID_MAX][PHASE][2] = // {deg, rpm}
  { { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} },
    { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} }
    /* moterID: 1 2 3 4 */
  };

void handler_SPI(union sigval sv){
  
if((count/5)%2 == 0){
    vWriteValue[0] = SUPPLY;
    vWriteValue[1] = EXHAUST;
  }
  else{
    vWriteValue[0] = EXHAUST;
    vWriteValue[1] = SUPPLY;
  }
  SCC.transfer_volume_all_analog(CS_VALVE);
  
}

void timerSetting(void){
  
	itval1.it_value.tv_sec = 0;     
	itval1.it_value.tv_nsec = 1*1000*1000;
	itval1.it_interval.tv_sec = 0;  
	itval1.it_interval.tv_nsec = 1*1000*1000;

	itval100.it_value.tv_sec = 0;     
	itval100.it_value.tv_nsec = 100*1000*1000;
	itval100.it_interval.tv_sec = 0;  
	itval100.it_interval.tv_nsec = 100*1000*1000;
	//-------------------------------------------------
	// 1mm sec timers ---------------------------------

	// memset(&se, 0, sizeof(se));
	// se.sigev_value.sival_int = 2;
	// se.sigev_notify = SIGEV_THREAD;
	// se.sigev_notify_function = handler_SPI;
	// se.sigev_notify_attributes = NULL;
	// timer_create(CLOCK_REALTIME, &se, &timerid_thread_spi);
	// timer_settime(timerid_thread_spi, 0, &itval1, NULL);


	// 100mm sec timers -------------------------------
	// memset(&se, 0, sizeof(se));
	// se.sigev_value.sival_int = 4;
	// se.sigev_notify = SIGEV_THREAD;
	// se.sigev_notify_function = handler100;
	// se.sigev_notify_attributes = NULL;
	// timer_create(CLOCK_REALTIME, &se, &timerid_thread);
	// timer_settime(timerid_thread, 0, &itval100, NULL);

	
	// memset(&se, 0, sizeof(se));
	// se.sigev_value.sival_int = 2;
	// se.sigev_notify = SIGEV_THREAD;
	// se.sigev_notify_function = handler2000;
	// se.sigev_notify_attributes = NULL;
	// timer_create(CLOCK_REALTIME, &se, &timerid_thread);
	// timer_settime(timerid_thread, 0, &itval2000, NULL);
	
}

int main(void){

  int i;
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  DynamixelV2 m(SERIAL_PORT, 57600);
  m.TorqueEnable(1);
  m.TorqueEnable(2);
  m.TorqueEnable(3);

  // // start timer
  // timerSetting();

  while(1){
  // test
  
    for(i=0; i<ID_MAX; i++){
      if(i%2 == 0){
	mWriteValue[i][0] = MoterValue[cycle][i][0];
	mWriteValue[i][1] = MoterValue[cycle][i][1];
      }
      else 
	mWriteValue[i][0] = -MoterValue[cycle][i][0];
      if(mWriteValue[i][0] < -35) mWriteValue[i][0] = -35;
      else if(35 < mWriteValue[i][0]) mWriteValue[i][0] = 35;
    }
    cycle++;
    
    for(i=0; i<ID_MAX-1; i++){
      // m.SetTargetVelocity(i+1, (uint16_t)mWriteValue[i][1]);
      m.MovePosition(i+1, -mWriteValue[i][0]*4096/360+2048);
    }
    
    for(i=0; i<ID_MAX; i++){
      switch( mWriteValue[i][0] ){
      case 0:
	printf("Moter%d >> front\n", i); break;
      case 20:
	printf("Moter%d >> right\n", i); break;
      case -20:
	printf("Moter%d >> left\n", i); break;
      default:
	printf("Moter%d >> error\n", i); break;
      }
    }
    
    if(cycle/PHASE > 0) cycle = 0;

    usleep(1*1000*1000);
    
  // till here
  }

  m.TorqueDisable(1);
  m.TorqueDisable(2);
  m.TorqueDisable(3);
  
  return 0;
  
}
