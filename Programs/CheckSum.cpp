/*

file: MoterControl.cpp
purpose: a test for Raspberry Pi to communicate with the Open CM board sending and receiving strings.

compile with the command :
g++ -o ./M_TEST ./MoterControl.cpp ./lib/SlaveControlCommand.cpp ./lib/OperationCommand.cpp ./lib/SPIM_Ras.cpp ./lib/rs232.cpp -l wiringPi -lrt -lbcm2835

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
#include "lib/rs232.h"

#define SERIAL_PORT 24
#define ANGLE 11.377778
#define BAUD_RATE 57600
#define BUF_SIZE 64

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

void handler_SPI(union sigval);
void handler100(union sigval);

int count = 0;

uint16_t valvecnt[8];

int i=0;
char str_send[2][BUF_SIZE]; // send data buffer
unsigned char str_recv[BUF_SIZE]; // recv data buffer

// for check sum test
#define BUFFER 10
char cm_send[1][BUFFER];
unsigned char cm_recv;

void handler_SPI(union sigval sv){

  RS232_cputs(SERIAL_PORT, str_send[i]); // sends string on serial
  int n = RS232_PollComport(SERIAL_PORT, str_recv, (int)BUF_SIZE);
  if(n > 0){
    str_recv[n] = 0;   /* always put a "null" at the end of a string! */
  }
  i++;
  i %= 2;
  
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

  // switch( (count/5)%2 ){
  // case 0:
  //   printf("Moter >> right\n");
  //   break;
  // case 1:
  //   printf("Moter >> left\n");
  //   break;
  // }
  // count++;
  
  RS232_cputs(SERIAL_PORT, str_send[i]); // sends string on serial
  printf("Sent to Arduino: '%s'\n", str_send[i]);
  usleep(50*1000);
  int n = RS232_PollComport(SERIAL_PORT, str_recv, (int)BUF_SIZE);
  if(n > 0){
    str_recv[n] = '\0';   /* always put a "null" at the end of a string! */
    printf("Received %i bytes: '%s'\n", n, (char *)str_recv);
  }
  i++;
  i %= 2;
  
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
	memset(&se, 0, sizeof(se));
	se.sigev_value.sival_int = 4;
	se.sigev_notify = SIGEV_THREAD;
	se.sigev_notify_function = handler100;
	se.sigev_notify_attributes = NULL;
	timer_create(CLOCK_REALTIME, &se, &timerid_thread);
	timer_settime(timerid_thread, 0, &itval100, NULL);
	
}

int main(void){

  // for check sum test
  int sum = 0; int ref; char *cm_str; int value[BUFFER];
  cm_str = (char *)malloc(10);
  if(cm_str == NULL){
    printf("NO MEMORY"); exit(0);
  }
  strcpy(cm_str,"N");
  sprintf(cm_send[0],"%d",3);
  sprintf(cm_send[1],"%d",1);
  sprintf(cm_send[2],"%d",4);
  sprintf(cm_send[3],"%d",1);
  sprintf(cm_send[4],"%d",5);
  sprintf(cm_send[5],"%d",9);
  sprintf(cm_send[6],"%d",2);
  sprintf(cm_send[7],"%d",6);
  sprintf(cm_send[8],"%d",5);
  sprintf(cm_send[9],"%d",3);
  // till here
  
  char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
  // strcpy(str_send[0], "This is a test string.");
  // strcpy(str_send[1], "This is another test string.");
  strcpy(str_send[0], "test string");
  strcpy(str_send[1], "another test string");
  
  if(RS232_OpenComport(SERIAL_PORT, BAUD_RATE, mode))
  {
    printf("Can not open comport\n");
    return(0);
  }
 
  usleep(2000000);  /* waits 2000ms for stable condition */
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  // // start timer
  // timerSetting();

  while(1){
    RS232_cputs(SERIAL_PORT, cm_send[i]);
    usleep(50*1000);
    i++;
    if(i >= BUFFER){
      RS232_cputs(SERIAL_PORT, cm_str);
      usleep(50*1000);
      for(int j=0;j<BUFFER;j++){
	value[j] = atoi(cm_send[j]);
	sum += value[j];
      }
      ref = sum % 10;
      RS232_PollComport(SERIAL_PORT, &cm_recv, 16);
      printf("the sum is %d\n",sum);
      if(ref == (int)cm_recv){
	printf("true\n"); break;
      }
      else{
	printf("false\n");break;
      }
    }
  }
  return 0;
  
}
