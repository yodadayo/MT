
#include <cstring>
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

#define EXHAUST 3500
#define SUPPLY 3600
#define CLOSE 3700

#define CS_VALVE 6

struct itimerspec  itval1, itval5, itval10 ,itval100;
struct sigevent se;
struct sigaction sa;
timer_t timerid_thread_1,timerid_thread_spi,timerid_thread;
timer_t timerid_signal;

void timerSetting(void);

int count = 0;
uint16_t valvepressure[2] = {600, 0};

uint16_t valvecnt[8];

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

void handler100(union sigval sv){

  switch( (count/5)%2 ){
  case 0:
    printf("Valve mode >> ON\n");
    break;
  case 1:
    printf("Valve mode >> OFF\n");
    break;
  }
  count++;
  
}


int main(void){
 
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  // start timer
  timerSetting();

  while(1){
  }
  return 0;
  
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

	memset(&se, 0, sizeof(se));
	se.sigev_value.sival_int = 2;
	se.sigev_notify = SIGEV_THREAD;
	se.sigev_notify_function = handler_SPI;
	se.sigev_notify_attributes = NULL;
	timer_create(CLOCK_REALTIME, &se, &timerid_thread_spi);
	timer_settime(timerid_thread_spi, 0, &itval1, NULL);


	// // 100mm sec timers -------------------------------
	memset(&se, 0, sizeof(se));
	se.sigev_value.sival_int = 4;
	se.sigev_notify = SIGEV_THREAD;
	se.sigev_notify_function = handler100;
	se.sigev_notify_attributes = NULL;
	timer_create(CLOCK_REALTIME, &se, &timerid_thread);
	timer_settime(timerid_thread, 0, &itval100, NULL);
	
}
