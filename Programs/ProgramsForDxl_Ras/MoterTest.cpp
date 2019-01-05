/*

	compile with the command :
g++ MoterTest.cpp ./libdynamixel/DynamixelV2.cpp

*/

#include "./libdynamixel/DynamixelV2.h"
#include <stdio.h>
// #include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace ssr::dynamixel;

int main(void){

  int position, c;
	
	DynamixelV2 m("/dev/ttyUSB0", 57600);
	m.TorqueEnable(1);
	m.TorqueEnable(2);
	m.TorqueEnable(3);
	usleep(1*1000*1000);
	
	position = 20;
	m.MovePosition(1,-position*4096/360+2048);
	m.MovePosition(2,-position*4096/360+2048);
	usleep(1*1000*1000);
	
	position = -20;
	m.MovePosition(1,-position*4096/360+2048);
	m.MovePosition(2,-position*4096/360+2048);
	usleep(1*1000*1000);
	
	position = 0;
	m.MovePosition(1,-position*4096/360+2048);
	m.MovePosition(2,-position*4096/360+2048);
	usleep(1*1000*1000);

	m.TorqueDisable(1);
	m.TorqueDisable(2);
	m.TorqueDisable(3);

	// while(1){
	//   if((c = getchar()) != EOF) break;
	// }
	return 0;
}

