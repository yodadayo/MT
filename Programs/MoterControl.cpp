/*

file: MoterControl.cpp
purpose: a test for Raspberry Pi to move Dynamixel to the right angle.

compile with the command :
g++ -o ./M_TEST ./MoterControl.cpp ./lib/SlaveControlCommand.cpp ./lib/OperationCommand.cpp ./lib/SPIM_Ras.cpp ./lib/rs232.cpp ./lib/MoterControl.cpp -l wiringPi -lrt -lbcm2835

*/

#include <stdio.h>
#include <stdlib.h>
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
#include "lib/motion.h"
#include "lib/SPIM_Ras.h"
#include "lib/SlaveControlCommand.h"
#include "lib/OperationCommand.h"
#include "lib/MoterControl.h"
#include "lib/rs232.h"

#define SERIAL_PORT 24
#define BAUD_RATE 57600
#define ID_MAX 4
#define PHASE 4

#define EXHAUST 3500
#define SUPPLY 3600
#define CLOSE 3700

#define CS_VALVE 6

struct itimerspec  itval1, itval5, itval10 ,itval100, itval2000;
struct sigevent se;
struct sigaction sa;
timer_t timerid_thread_1,timerid_thread_spi,timerid_thread;
timer_t timerid_signal;

void timerSetting(void);

void handler_SPI(union sigval);
void handler100(union sigval);

uint16_t count = 0; uint8_t cycle = 0;

int16_t MoterValue[ID_MAX][PHASE][2] = // {deg, rpm}
  { { {0,100}, {90,100}, {0,100}, {-90,100} },
    { {90,200}, {0,200}, {-90,200}, {0,200} },
    { {0,300}, {90,300}, {0,300}, {-90,300} },
    { {-90,100}, {0,100}, {90,100}, {0,100} }
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

void handler2000(union sigval sv){

  // if(count/9 > 0){
    for(int i=0; i<ID_MAX; i++){
      mWriteValue[i][0] = MoterValue[i][cycle][0];
      mWriteValue[i][1] = MoterValue[i][cycle][1];

      // safety constraints (please adjust to the excursion of the robot)
      if( mWriteValue[i][0] < -100 ){
      	mWriteValue[i][0] = -100;
      }
      else if( 100 < mWriteValue[i][0] ){
      	mWriteValue[i][0] = 100
      }
    }
    // count = 0;
    MC.SendData(SERIAL_PORT, ID_MAX);
    for(int i=0; i<ID_MAX; i++){
      switch( mWriteValue[i][0] ){
      case 0:
	printf("Moter%d >> front\n", i); break;
      case 90:
	printf("Moter%d >> right\n", i); break;
      case -90:
	printf("Moter%d >> left\n", i); break;
      }
    }
    // }
    // count++;
  cycle++;

  if(cycle/PHASE > 0) cycle = 0;
  
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

	itval2000.it_value.tv_sec = 2;     
	itval2000.it_value.tv_nsec = 0;
	itval2000.it_interval.tv_sec = 2;  
	itval2000.it_interval.tv_nsec = 0;
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

	
	memset(&se, 0, sizeof(se));
	se.sigev_value.sival_int = 2;
	se.sigev_notify = SIGEV_THREAD;
	se.sigev_notify_function = handler2000;
	se.sigev_notify_attributes = NULL;
	timer_create(CLOCK_REALTIME, &se, &timerid_thread);
	timer_settime(timerid_thread, 0, &itval2000, NULL);
	
}

int main(void){
  
  char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
  
  MC.SerialInit(SERIAL_PORT, BAUD_RATE, mode);
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  // start timer
  timerSetting();

  while(1){
  }
  return 0;
  
}
