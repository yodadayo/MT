/*

file: MotionProgram.cpp
purpose: making motions of the robot to walk and run.

compile with the command :
g++ -o ./MOTION ./MotionProgram.cpp ./lib/SlaveControlCommand.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835

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

// Walking Setting
#define W_PHASE 10

// Running Setting
#define R_PHASE 6

// Moter 
#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 57600
#define ID_NUM 4

#define RIGHT_ROLL 3
#define RIGHT_PITCH 1
#define LEFT_ROLL 4
#define LEFT_PITCH 2

// Valve
#define EXHAUST 3500
#define SUPPLY 3600
#define CLOSE 3700
#define V_NUM 8

#define CS_VALVE 6

// mode
#define WALK 1
#define RUN 2
#define TEST 3

// timer
struct itimerspec  itval1, itval5, itval10, itval100;
struct sigevent se;
struct sigaction sa;
timer_t timerid_thread_1,timerid_thread_spi,timerid_thread;
timer_t timerid_signal;

void timerSetting(void);

void handler1(union sigval);
void handler100(union sigval);

uint16_t cycle = 0;
uint16_t w_cycle = 0;

// test parameter
uint16_t ValveTest[3] = {600, 300,0};
uint16_t MoterTest[3] = {-20, 0, 20};

// Parameter
uint16_t ValvePressure_w[8][W_PHASE] = { {600, 0, 300, 600, 0, 300, 600, 0}, // left front femur (0)
					 {0}, // left back femur (1)
					 {0}, // left front shin (2)
					 {0}, // right front femur (3)
					 {0}, // right back femur (4)
					 {0}, // right front shin (5)
					 {0}, // left back shin (6)
					 {0} // right back shin (9)
};

uint16_t ValvePressure_r[8][R_PHASE] = { {600, 0}, // left front femur ()
					 {0}, // left back femur ()
					 {0}, // left front shin ()
					 {0}, // left back shin ()
					 {0}, // right front femur ()
					 {0}, // right back femur ()
					 {0}, // right front shin ()
					 {0} // right back shin ()
}; 

int16_t MoterValue_w[ID_NUM][W_PHASE][2] = // {deg, rpm}
  { { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} },
    { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} },
    { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} },
    { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} },
    { {20,100}, {-20,300}, {0,300}, {0,100} },
    { {-20,100}, {20,300}, {0,300}, {0,100} }
    /* moterID: 1 2 3 4 */
  };


using namespace ssr::dynamixel;


void handler1(union sigval sv){

  int i;

  // // walking mode
  // if(w_cycle / 100 > 0){

  //   for(i=0; i<V_NUM; i++){

  //     vWriteValue[i] = ValvePressure_w[i][cycle];
  //     if(i == 7) vWriteValue[9] = ValvePressure_w[i][cycle];

  //   }
    
  //   for(i=0; i<ID_NUM; i++){

  //     if(i == 0){ // right leg
  // 	mWriteValue[i][0] = MoterValue_w[cycle][i][0];
  // 	mWriteValue[i][1] = MoterValue_w[cycle][i][1];
  //     }
  //     else // left leg
  // 	mWriteValue[i][0] = -MoterValue_w[cycle][i][0];
      
  //     // Safety constraints (Leave this uncommented!!)
  //     if(mWriteValue[i][0] < -35) mWriteValue[i][0] = -35;
  //     else if(35 < mWriteValue[i][0]) mWriteValue[i][0] = 35;
      
  //   }
    
  //   w_cycle = 0; cycle++;
  //   if(cycle > PHASE) cycle = 0;
    
  // }
  // w_cycle++;

  
  // // running mode
  // if(r_cycle / 100 > 0){

  //   for(i=0; i<V_NUM; i++){

  //     vWriteValue[i] = ValvePressure_r[i][cycle];
  //     if(i == 7) vWriteValue[9] = ValvePressure_r[i][cycle];

  //   }
    
  //   for(i=0; i<ID_NUM; i++){

  //     if(i == 0){ // right leg
  // 	mWriteValue[i][0] = MoterValue_r[cycle][i][0];
  // 	mWriteValue[i][1] = MoterValue_r[cycle][i][1];
  //     }
  //     else // left leg
  // 	mWriteValue[i][0] = -MoterValue_r[cycle][i][0];
      
  //     // Safety constraints (Leave this uncommented!!)
  //     if(mWriteValue[i][0] < -35) mWriteValue[i][0] = -35;
  //     else if(35 < mWriteValue[i][0]) mWriteValue[i][0] = 35;
      
  //   }
    
  //   r_cycle = 0; cycle++;
  //   if(cycle > PHASE) cycle = 0;
    
  // }
  // r_cycle++;


  // test mode
  if(w_cycle / 1000 > 0){ // activate every 1sec

    for(i=0; i<3; i++){

      vWriteValue[i] = ValveTest[i];
      if(i == 7) vWriteValue[9] = ValveTest[i];

    }
    
    for(i=0; i<ID_NUM; i++){

      if(i == 0) // right leg
	mWriteValue[i][0] = MoterTest[i];
      else // left leg
	mWriteValue[i][0] = -MoterTest[i];
      // mWriteValue[i][1] = MoterValue_w[cycle][i][1];
      
      // Safety constraints (Leave this uncommented!!)
      if(mWriteValue[i][0] < -30) mWriteValue[i][0] = -30;
      else if(30 < mWriteValue[i][0]) mWriteValue[i][0] = 30;
      
    }
    
    w_cycle = 0; cycle++;
    if(cycle > PHASE) cycle = 0;
    
  }
  w_cycle++;
  
  
  SCC.transfer_volume_all_analog(CS_VALVE);
  
}

void handler100(union sigval sv){

  for(int i=0; i<V_NUM; i++){
    if(i != 7)
      printf("ValvePressure %d >> %d\n", i, vReadValue[i]);
    else 
      printf("ValvePressure 9 >> %d\n", vReadValue[i]);
  }

}


int main(void){
 
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);

  SPIR.begin(CS_VALVE);

  // Moter Setting
  DynamixelV2 m(SERIAL_PORT, BAUD_RATE);
  m.TorqueEnable(RIGHT_ROLL);
  m.TorqueEnable(RIGHT_PITCH);
  m.TorqueEnable(LEFT_ROLL);
  m.TorqueEnable(LEFT_PITCH);

  // start timer
  timerSetting();

  while(1){
    
    usleep(1*1000);
    
    for(i=0; i<ID_NUM; i++){
      // m.SetTargetVelocity(i+1, (uint16_t)mWriteValue[i][1]);
      m.MovePosition(i+1, -mWriteValue[i][0]*4096/360+2048);
    }
    
  }
  
  return 0;

  m.TorqueDisable(RIGHT_ROLL);
  m.TorqueDisable(RIGHT_PITCH);
  m.TorqueDisable(LEFT_ROLL);
  m.TorqueDisable(LEFT_PITCH);
  
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
  se.sigev_notify_function = handler1;
  se.sigev_notify_attributes = NULL;
  timer_create(CLOCK_REALTIME, &se, &timerid_thread_1);
  timer_settime(timerid_thread_1, 0, &itval1, NULL);
  
  
  // // 100mm sec timers -------------------------------
  memset(&se, 0, sizeof(se));
  se.sigev_value.sival_int = 4;
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_notify_function = handler100;
  se.sigev_notify_attributes = NULL;
  timer_create(CLOCK_REALTIME, &se, &timerid_thread);
  timer_settime(timerid_thread, 0, &itval100, NULL);
  
}
