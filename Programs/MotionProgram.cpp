/*

file: MotionProgram.cpp
purpose: making motions of the robot.

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
#include <fstream>
// original library
#include "lib/initialization.h"
#include "lib/SPIM_Ras.h"
#include "lib/SlaveControlCommand.h"
#include "libdxl/DynamixelV2.h"

// Walking Setting
#define W_PHASE 8

#define SPEED 100 // default: 100

// First Step From Standing
#define FIRST_PHASE 4

// Moter 
#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 57600
#define ID_NUM 4

#define RIGHT_ROLL 3
#define RIGHT_PITCH 1
#define LEFT_ROLL 4
#define LEFT_PITCH 2

// Valve
#define E 3500 // Exhaust
#define S 3600 // Supply
#define C 3700 // Close
#define V_NUM 8

#define CS_VALVE 6

// mode
#define WALK 1
#define TEST 3
uint8_t mode = 0;

// timer
struct itimerspec  itval1, itval5, itval10, itval100;
struct sigevent se;
struct sigaction sa;
timer_t timerid_thread_1,timerid_thread_spi,timerid_thread;
timer_t timerid_signal;

void timerSetting(void);

void handler1(union sigval);
void handler100(union sigval);

// test parameter
uint16_t ValveTest[4] = {600, 300, 0, 300};
uint16_t MoterTest[4] = {-20, 0, 20, 0};

// // Parameter For Photograph
// uint16_t ValvePressure[8] = {E, // right front shin (0)
// 				S, // right back shin (1)
// 				E, // right back femur (2)
// 				E, // left front shin (3)
// 				E, // left back shin (4)
// 				E, // left back femur (5)
// 				S, //  right front femur(6)
// 				S // left front femur (9)
// };


// // Parameter (left leg is frontward in the initial state)

uint16_t vstep[8][W_PHASE] = { {S, S, S, S, S, E, S, S},
			      // right front shin (0)
			       {E, E, S, E, E, S, E, E},
			      // right back shin (1)
			       {E, E, E, E, S, S, S, E},
			      // right back femur (2)
			       {S, E, S, S, S, S, S, S},
			      // left front shin (3)
			       {E, S, E, E, E, E, S, E},
			      // left back shin (4)
			       {S, S, S, E, E, E, E, E},
			      // left back femur (5)
			       {S, S, S, S, E, E, E, S},
			      // right front femur(6)
			       {E, E, E, S, S, S, S, S}
			      // left front femur (9)
}; // if W_PHASE is 8

int16_t mstep[ID_NUM][W_PHASE] = // {deg, rpm}
  { { 5, -3, -3, -3, 5, 10, 30, 30}, // RIGHT_PITCH
    { 5, 10, 30, 30, 5, -3, -3, -3}, // LEFT_PITCH
    { -5, -15, -25, -20, -5, 0, 5, 5}, // RIGHT_ROLL
    { -5, 0, 5, 5, -5, -15, -25, -20} // LEFT_ROLL
  }; // if W_PHASE is 8

// First Step Parameter(Starting From Standing)
const int16_t firststep_m[ID_NUM][FIRST_PHASE] = // {deg, rpm}
  { { 13, 30, 30, 30}, // RIGHT_PITCH
    { 13, 10, 10, 0}, // LEFT_PITCH
    { 0, 5, 5, 5}, // RIGHT_ROLL
    { 0, -15, -15, -25} // LEFT_ROLL
  };
const uint16_t firststep_v[8][FIRST_PHASE] = { {S, E, S, S},
			      // right front shin (0)
			       {E, S, E, E},
			      // right back shin (1)
			       {E, S, S, E},
			      // right back femur (2)
			       {S, S, S, S},
			      // left front shin (3)
			       {E, E, S, E},
			      // left back shin (4)
			       {E, E, E, E,,
			      // left back femur (5)
			       {S, E, E, E},
			      // right front femur(6)
			       {S, S, S, S}
			      // left front femur (9)
}; // if W_PHASE is 8
uint8_t fs_flag = 1;


// Standing Parameter
uint16_t stance_v[V_NUM] = {S, S, E, S, S, E, S, S};
int16_t stance_m[ID_NUM] = {0, 0, 0, 0};

uint16_t logtimer = 0;





using namespace ssr::dynamixel;

uint8_t cycle = 1;
uint16_t w_cycle = 0;

// 
// Transform values (v = Voltage, ps = Pressure) 
// 

inline uint16_t vtops(uint16_t vol)
{
  if(vol == E || vol == S || vol == C)
    return vol;
  else
    return 0.286*(vol-800)+100;
}

inline uint16_t pstov(uint16_t ps)
{
  if(ps == E || ps == S || ps == C)
    return ps;
  else
    return 3.50*ps+450;
}

//
//
//
//
// Handler Settings
//
//
//
//

void handler1(union sigval sv){

  int i, j; j=0;

  switch(mode){
  case WALK:
    if(fs_flag){

      for(i = 0; i < ID_NUM; i++){
	
	if((i+1)%2 == 1){ // right leg
	  if(cycle != 0)
	    mWriteValue[i] = ((SPEED - w_cycle)*firststep_m[i][cycle - 1] + w_cycle*firststep_m[i][cycle])/SPEED;
	  else
	    mWriteValue[i] = ((SPEED - w_cycle)*firststep_m[i][9] + w_cycle*firststep_m[i][cycle])/SPEED;
	}
	else{ // left leg
	  if(cycle != 0)
	    mWriteValue[i] = -((SPEED - w_cycle)*firststep_m[i][cycle - 1] + w_cycle*firststep_m[i][cycle])/SPEED;
	  else
	    mWriteValue[i] = -((SPEED - w_cycle)*firststep_m[i][9] + w_cycle*firststep_m[i][cycle])/SPEED;
	}
	
      }
      
      if(w_cycle / SPEED > 0){
	
	for(i = 0; i < V_NUM; i++){
	  
	  vWriteValue[i] = pstov(firststep_v[i][cycle]);
	  if(i == 7) vWriteValue[9] = pstov(firststep_v[i][cycle]);
	  
	}
	
	w_cycle = 0; cycle++;
	if(cycle > FIRST_PHASE - 1){
	  cycle = 0; fs_flag = 0; logtimer++;
	}
    
      }
      w_cycle++;
      
    }
    
    else{
      
      for(i = 0; i < ID_NUM; i++){
	
	if((i+1)%2 == 1){ // right leg
	  if(cycle != 0)
	    mWriteValue[i] = ((SPEED - w_cycle)*mstep[i][cycle - 1] + w_cycle*mstep[i][cycle])/SPEED;
	  else
	    mWriteValue[i] = ((SPEED - w_cycle)*mstep[i][9] + w_cycle*mstep[i][cycle])/SPEED;
	}
	else{ // left leg
	  if(cycle != 0)
	    mWriteValue[i] = -((SPEED - w_cycle)*mstep[i][cycle - 1] + w_cycle*mstep[i][cycle])/SPEED;
	  else
	    mWriteValue[i] = -((SPEED - w_cycle)*mstep[i][9] + w_cycle*mstep[i][cycle])/SPEED;
	}
	
      }
      
      if(w_cycle / SPEED > 0){
	
	for(i = 0; i < V_NUM; i++){
	  
	  vWriteValue[i] = pstov(vstep[i][cycle]);
	  if(i == 7) vWriteValue[9] = pstov(vstep[i][cycle]);
	  
	}
	
	w_cycle = 0; cycle++;
	if(cycle > W_PHASE - 1){
	  cycle = 0;
	  logtimer++;
	}
    
      }
      w_cycle++;
    }
    
    break;
    
  case TEST:
    // test mode
    if(w_cycle / 1000 > 0){ // activate every 1sec
      
      for(i = 0; i < V_NUM; i++){
	
	vWriteValue[i] = pstov(ValveTest[cycle]);
	// if(cycle / 2 == 0)
	//   vWriteValue[i] = S;
	// else vWriteValue[i] = E;
	if(i == 7) vWriteValue[9] = pstov(ValveTest[cycle]);
	
      }
      
      for(i = 0; i < 2; i++){
	
	mWriteValue[i] = MoterTest[cycle];
        	
      }
      
      w_cycle = 0; cycle++;
      if(cycle / 4 > 0) cycle = 0;
      
    }
    w_cycle++;

    break;

  default:
    printf("NO SETTINGS");
    exit(1);
    break;
  }
    
  SCC.transfer_volume_all_analog(CS_VALVE);
  
}

void handler100(union sigval sv){

  for(int i = 0; i < V_NUM; i++){
    switch(vWriteValue[i]){
    case S:
      if(i != 7)
	printf("TargetValvePressure %d >> SUPPLY\n", i);
      else 
	printf("TargetValvePressure 9 >> SUPPLY\n");
      break;
      
    case E:
      if(i != 7)
	printf("TargetValvePressure %d >> EXHAUST\n", i);
      else 
	printf("TargetValvePressure 9 >> EXHAUST\n");
      break;
      
    case C:
      if(i != 7)
	printf("TargetValvePressure %d >> CLOSE\n", i);
      else 
	printf("TargetValvePressure 9 >> CLOSE\n");
      break;
      
    default:
      if(i != 7)
	printf("TargetValvePressure %d >> %d\n", i, pstov(vWriteValue[i]));
      else 
	printf("TargetValvePressure 9 >> %d\n", pstov(vWriteValue[9]));
      break;
    }
  }
  for(int i = 0; i < V_NUM; i++)
    if(i != 7)
      printf("ValvePressure %d >> %d\n", i, vtops(vReadValue[i]));
    else 
      printf("ValvePressure 9 >> %d\n", vtops(vReadValue[9]));
  printf("cycle >> %d\n", cycle);
  printf("---------------------\n\n");

}

//
//
//
//
// Timer Setting
//
//
//
//

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
  
  
  // 100mm sec timers -------------------------------
  memset(&se, 0, sizeof(se));
  se.sigev_value.sival_int = 4;
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_notify_function = handler100;
  se.sigev_notify_attributes = NULL;
  timer_create(CLOCK_REALTIME, &se, &timerid_thread);
  timer_settime(timerid_thread, 0, &itval100, NULL);
  
}

//
//
//
//
// Main Function
//
//
//
//

int main(void){

  
  //
  //
  //  Mode Setting
  //    (you can select the motion here)
  //
  //
  mode = WALK;


  int i; uint8_t first = cycle - 1;
  
  // valve board are controlled in 16MHz
  SPIR.initialization(16000000,1000000);
  SPIR.begin(CS_VALVE);

  // Moter Setting
  DynamixelV2 m(SERIAL_PORT, BAUD_RATE);
  m.TorqueEnable(RIGHT_ROLL);
  m.TorqueEnable(RIGHT_PITCH);
  m.TorqueEnable(LEFT_ROLL);
  m.TorqueEnable(LEFT_PITCH);

  // Making Data File
  std::ofstream log("Parameter.csv");
  log << "\t";
  for(i = 0; i < V_NUM; i++){
      log << "Muscle" << i << "\t";
  }
  for(i = 0; i < ID_NUM; i++){
    log << "Moter" << i+1;
    if(i != ID_NUM - 1) log << "\t";
    else log << "\n";
  }
  
  
  // Initial setting (if the mode is WALK)

  for(i = 0; i < V_NUM; i++){
	
    vWriteValue[i] = pstov(stance_v[i]);
    if(i == 7) vWriteValue[9] = pstov(stance_v[i]);
    
  }
  
  for(i = 0; i < ID_NUM; i++){
    
    if(i%2 == 1){ // right leg
      mWriteValue[i] = -stance_m[i];
    }
    else // left leg
      mWriteValue[i] = stance_m[i];
    
    // Safety constraints (Leave this uncommented!!)
    if(mWriteValue[i] < -40) mWriteValue[i] = -40;
    else if(40 < mWriteValue[i]) mWriteValue[i] = 40;
    
  }

  SCC.transfer_volume_all_analog(CS_VALVE);
  
  for(i = 0; i < ID_NUM; i++){
    m.MovePosition(i+1, -mWriteValue[i]*4096/360+2048);
  }
    
  printf("READY...\n\n\n");
  sleep(3);
  
  //////
  for(i = 0; i < V_NUM; i++){
	
    vWriteValue[i] = pstov(firststep_v[i][0]);
    if(i == 7) vWriteValue[9] = pstov(firststep_v[i][0]);
    
  }
  
  for(i = 0; i < ID_NUM; i++){
    
    if(i%2 == 1){ // right leg
      mWriteValue[i] = -firststep_m[i][0];
    }
    else // left leg
      mWriteValue[i] = firststep_m[i][0];
    
    // Safety constraints (Leave this uncommented!!)
    if(mWriteValue[i] < -40) mWriteValue[i] = -40;
    else if(40 < mWriteValue[i]) mWriteValue[i] = 40;
    
  }

  SCC.transfer_volume_all_analog(CS_VALVE);
  
  for(i = 0; i < ID_NUM; i++){
    m.MovePosition(i+1, -mWriteValue[i]*4096/360+2048);
  }
  
  for(i = 0; i < V_NUM; i++){
    log << logtimer << "\t";
    log << vReadValue[i];
    if((i+1) % (V_NUM - 1) != 0) log << "\t";
    else log << "\n";
  }
    
  printf("READY...\n\n\n");
  usleep(50*(SPEED/100)*1000);
  //////

  
  
  // start timer
  timerSetting();

  while(1){
    
    usleep(20*1000);
	
    // Safety constraints for moters (Leave this uncommented!!)
    if(mWriteValue[i] < -40) mWriteValue[i] = -40;
    else if(40 < mWriteValue[i]) mWriteValue[i] = 40;
    
    for(i = 0; i < ID_NUM; i++){
      m.MovePosition(i+1, -mWriteValue[i]*4096/360+2048);
    }
    
    for(i = 0; i < V_NUM; i++){
      log << vReadValue[i];
      if((i+1) % (V_NUM - 1) != 0) log << "\t";
      else log << "\n";
    }

    // if(logtimer > 1) break;
    
  }

  m.TorqueDisable(RIGHT_ROLL);
  m.TorqueDisable(RIGHT_PITCH);
  m.TorqueDisable(LEFT_ROLL);
  m.TorqueDisable(LEFT_PITCH);

  log.close();
  
  return 0;
  
}
