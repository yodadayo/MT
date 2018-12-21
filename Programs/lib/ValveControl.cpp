#include "arduino.h"
#include "variant.h"
#include <stdio.h>
#include <extern.h>
#include "ValveControl.h"
#include <SPIS.h>

#define SS0 10 //Left  side
#define SS1 4  //Right side
#define SS2 3  //Right back
#define SS3 2  //Left  back
#define D 1

ValveControl::ValveControl(void)
{
}
/////////////////////////////////////////
void ValveControl::valve_init(void){
  pinMode(22, OUTPUT);pinMode(23, OUTPUT);pinMode(24, OUTPUT);pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);pinMode(27, OUTPUT);pinMode(28, OUTPUT);pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);pinMode(31, OUTPUT);pinMode(32, OUTPUT);pinMode(33, OUTPUT);
  pinMode(42, OUTPUT);pinMode(43, OUTPUT);pinMode(44, OUTPUT);pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);pinMode(47, OUTPUT);pinMode(48, OUTPUT);pinMode(49, OUTPUT);
  pinMode(50, OUTPUT);pinMode(51, OUTPUT);pinMode(52, OUTPUT);pinMode(53, OUTPUT);
  digitalWrite(22, LOW);digitalWrite(23, LOW);digitalWrite(24, LOW);digitalWrite(25, LOW);//A1
  digitalWrite(26, LOW);digitalWrite(27, LOW);digitalWrite(28, LOW);digitalWrite(29, LOW);//A3
  digitalWrite(30, LOW);digitalWrite(31, LOW);digitalWrite(32, LOW);digitalWrite(33, LOW);//A5
  digitalWrite(42, LOW);digitalWrite(43, LOW);digitalWrite(44, LOW);digitalWrite(45, LOW);
  digitalWrite(46, LOW);digitalWrite(47, LOW);digitalWrite(48, LOW);digitalWrite(49, LOW);
  digitalWrite(50, LOW);digitalWrite(51, LOW);digitalWrite(52, LOW);digitalWrite(53, LOW);
}
////////////////////////////////////////////////////////
void ValveControl::initialization_slave_board(void){
  Serial.begin(115200);
  analogReadResolution(12);
  valve_init();
  SPI.beginSlave(10);
  pinMode(9, INPUT);
  //pressure_sensor_calibration();
  NVIC_DisableIRQ(SPI0_IRQn);
  NVIC_ClearPendingIRQ(SPI0_IRQn);
  NVIC_SetPriority(SPI0_IRQn, 0);
  NVIC_EnableIRQ(SPI0_IRQn);
}
/////////////////////////////////////////////
void ValveControl::pluse_control(uint16_t pin,uint16_t rate,uint16_t vnum){
    RATE_HIGH = rate;
    if(plusecnt[vnum]<RATE_HIGH){
      digitalWrite(pin,HIGH);
    }
    else {
      digitalWrite(pin,LOW);
      if(plusecnt[vnum]>RATE_LOW)plusecnt[vnum]=0;
    }
    plusecnt[vnum]++;
}
///////////////////////////////////////////
uint32_t ValveControl::pressure_adjust(uint32_t vnum,  int32_t targetVol ,int32_t range){
  int32_t analog;
  analog =  analogRead(vnum);
  //22～33////A0～A5
  if ((vnum >= 0) &&(vnum <= 5)) {
    if (analog <= targetVol-(range/2) ) {
			digitalWrite(vnum * 2 + 22, HIGH);//supply
			digitalWrite(vnum * 2 + 23, LOW);
		}
		else if( (analog >= targetVol-(range/2)) &&(analog <= targetVol+(range/2)) ){
			digitalWrite(vnum * 2 + 22, LOW);//close
			digitalWrite(vnum * 2 + 23, LOW);
		}
		else if (  analog  >= targetVol+(range/2)) {
			digitalWrite(vnum * 2 + 22, LOW);//exhaust
			digitalWrite(vnum * 2 + 23, HIGH);
		}
  }
  //42～53////A6～A11
  else if ((vnum >= 6) &&(vnum <=11)) {
    if (analog <= targetVol-(range/2) ) {
      digitalWrite(vnum * 2 + 30, HIGH);
      digitalWrite(vnum * 2 + 31, LOW);
    }
    else if( (analog >= targetVol-(range/2)) &&(analog <= targetVol+(range/2)) ){
			digitalWrite(vnum * 2 + 30, LOW);
			digitalWrite(vnum * 2 + 31, LOW);
		}
		else if (  analog  >= targetVol+(range/2)) {
			digitalWrite(vnum * 2 + 30, LOW);
			digitalWrite(vnum * 2 + 31, HIGH);
		}
  }
  return(analog);
}
///////////////////////////////////////////
uint32_t ValveControl::pressure_adjust_using_pluse(uint32_t vnum,  int32_t targetVol ,int32_t range){
  int32_t analog;
  analog =  analogRead(vnum);
  int d1 = 1;int d2 = 1;
  //22～33////A0～A5
  if ((vnum >= 0) &&(vnum <= 5)) {
    if(targetVol>3000){
        digitalWrite(vnum * 2 + 22, LOW);//close
        digitalWrite(vnum * 2 + 23, LOW);
    }
    else if (analog <= targetVol-(rangeH) ) {
      digitalWrite(vnum * 2 + 22, HIGH);///吸気
      digitalWrite(vnum * 2 + 23, LOW);
    }
    else if ( (analog >= targetVol-(rangeH)) &&(analog <= targetVol-(rangeL))  ) {
      pluse_control(vnum * 2 + 22,128/RATE_LOW, vnum);//pluse_control_liner_change( vnum * 2 + 22 , vol , analog);///パルス吸気
      digitalWrite(vnum * 2 + 23, LOW);
    }
    else if ( (analog >= targetVol-(rangeL)) &&(analog <= targetVol-(range/2))  ) {
      pluse_control(vnum * 2 + 22,128/RATE_HIGH, vnum);//pluse_control_liner_change( vnum * 2 + 22 , vol , analog);///パルス吸気
      digitalWrite(vnum * 2 + 23, LOW);
    }
    else if( (analog >= targetVol-(range/2)) &&(analog <= targetVol+(range/2)) ){
      digitalWrite(vnum * 2 + 22, LOW); ///close
      digitalWrite(vnum * 2 + 23, LOW);
    }
    else if (  (analog  >= targetVol+(range/2)) && (analog  <= targetVol+(rangeL)) ) {
      digitalWrite(vnum * 2 + 22, LOW);
      pluse_control(vnum * 2 + 23,128/RATE_HIGH, vnum);//pluse_control_liner_change( vnum * 2 + 23 , vol , analog);///パルス排気
    }
    else if (  (analog  >= targetVol+(rangeL)) && (analog  <= targetVol+(rangeH)) ) {
      digitalWrite(vnum * 2 + 22, LOW);
      pluse_control(vnum * 2 + 23,128/RATE_LOW, vnum);//pluse_control_liner_change( vnum * 2 + 23 , vol , analog);///パルス排気
    }
    else if (  analog  >= targetVol+(rangeH)) {
      digitalWrite(vnum * 2 + 22, LOW);
      digitalWrite(vnum * 2 + 23, HIGH);
    }
  }
  //42～53////A6～A11
  else if ((vnum >= 6) &&(vnum <=11)) {
    if(targetVol>3000){
        digitalWrite(vnum * 2 + 30, LOW);//close
        digitalWrite(vnum * 2 + 31, LOW);
    }
    else if (analog <= targetVol-(rangeH) ) {
      digitalWrite(vnum * 2 + 30, HIGH);///吸気
      digitalWrite(vnum * 2 + 31, LOW);
    }
    else if ( (analog >= targetVol-(rangeH)) &&(analog <= targetVol-(rangeL))  ) {
      pluse_control(vnum * 2 + 30,128/RATE_LOW, vnum);//pluse_control_liner_change( vnum * 2 + 22 , vol , analog);///パルス吸気
      digitalWrite(vnum * 2 + 31, LOW);
    }
    else if ( (analog >= targetVol-(rangeL)) &&(analog <= targetVol-(range/2))  ) {
      pluse_control(vnum * 2 + 30,128/RATE_HIGH, vnum);//pluse_control_liner_change( vnum * 2 + 22 , vol , analog);///パルス吸気
      digitalWrite(vnum * 2 + 31, LOW);
    }
    else if( (analog >= targetVol-(range/2)) &&(analog <= targetVol+(range/2)) ){
      digitalWrite(vnum * 2 + 30, LOW); ///close
      digitalWrite(vnum * 2 + 31, LOW);
    }
    else if (  (analog  >= targetVol+(range/2)) && (analog  <= targetVol+(rangeL)) ) {
      digitalWrite(vnum * 2 + 30, LOW);
      pluse_control(vnum * 2 + 31,128/RATE_HIGH, vnum);//pluse_control_liner_change( vnum * 2 + 23 , vol , analog);///パルス排気
    }
    else if (  (analog  >= targetVol+(rangeL)) && (analog  <= targetVol+(rangeH)) ) {
      digitalWrite(vnum * 2 + 30, LOW);
      pluse_control(vnum * 2 + 31,128/RATE_LOW, vnum);//pluse_control_liner_change( vnum * 2 + 23 , vol , analog);///パルス排気
    }
    else if (  analog  >= targetVol+(rangeH)) {
      digitalWrite(vnum * 2 + 30, LOW);
      digitalWrite(vnum * 2 + 31, HIGH);
    }
  }
  return(analog);
}
//////////////////////////////////////////////////////
void ValveControl::pressure_adjust_all(void){
  for(int i=0; i<12; i++)pressure_adjust(i,volume[i],rangeNormalMode);
}
////////////////////////////////////////////////////////
void ValveControl::pressure_adjust_using_pluse_all(void){
  for(int i=0; i<12; i++)pressure_adjust_using_pluse(i,volume[i],rangePluseMode);
}
////////////////////////////////////////////////////////
void ValveControl::all_exhaust(void){
	for(uint32_t vnum=0; vnum < 12 ; vnum++){
		if ((vnum >= 0) &&(vnum <= 5)) {
			digitalWrite(vnum * 2 + 22, LOW);
			digitalWrite(vnum * 2 + 23, HIGH);
		}
		else if ((vnum >= 6) &&(vnum <=11)) {
	  	digitalWrite(vnum * 2 + 30, LOW);
			digitalWrite(vnum * 2 + 31, HIGH);
		}
	}
}
////////////////////////////////////////////////////////
void ValveControl::all_supply(void){
		for(uint32_t vnum=0; vnum < 12 ; vnum++){
			if ((vnum >= 0) &&(vnum <= 5)) {
				digitalWrite(vnum * 2 + 22, HIGH);
				digitalWrite(vnum * 2 + 23, LOW);
			}
		  	else if ((vnum >= 6) &&(vnum <=11)) {
		  		digitalWrite(vnum * 2 + 30, HIGH);
				digitalWrite(vnum * 2 + 31, LOW);
			}
		}
}
////////////////////////////////////////////////////////
void ValveControl::all_close(void){
    for(uint32_t vnum=0; vnum < 12 ; vnum++){
      if ((vnum >= 0) &&(vnum <= 5)) {
        digitalWrite(vnum * 2 + 22, LOW);
        digitalWrite(vnum * 2 + 23, LOW);
      }
        else if ((vnum >= 6) &&(vnum <=11)) {
          digitalWrite(vnum * 2 + 30, LOW);
        digitalWrite(vnum * 2 + 31, LOW);
      }
    }
}
////////////////////////////////////////////////////////
void ValveControl::printall_get_data(void){
  Serial.print(volume[0]); Serial.print("  ");
  Serial.print(volume[1]); Serial.print("  ");
  Serial.print(volume[2]); Serial.print("  ");
  Serial.print(volume[3]); Serial.print("  ");
  Serial.print(volume[4]); Serial.print("  ");
  Serial.print(volume[5]); Serial.print("  ");
  Serial.print(volume[6]); Serial.print("  ");
  Serial.print(volume[7]); Serial.print("  ");
  Serial.print(volume[8]); Serial.print("  ");
  Serial.print(volume[9]); Serial.print("  ");
  Serial.print(volume[10]); Serial.print("  ");
  Serial.print(volume[11]); Serial.println("  ");
}

void ValveControl::printall_analog(void){
  int analog;
  analog=analogRead(0);Serial.print(analog); Serial.print("  ");
  analog=analogRead(1);Serial.print(analog); Serial.print("  ");
  analog=analogRead(2);Serial.print(analog); Serial.print("  ");
  analog=analogRead(3);Serial.print(analog); Serial.print("  ");
  analog=analogRead(4);Serial.print(analog); Serial.print("  ");
  analog=analogRead(5);Serial.print(analog); Serial.print("  ");
  analog=analogRead(6);Serial.print(analog); Serial.print("  ");
  analog=analogRead(7);Serial.print(analog); Serial.print("  ");
  analog=analogRead(8);Serial.print(analog); Serial.print("  ");
  analog=analogRead(9);Serial.print(analog); Serial.print("  ");
  analog=analogRead(10);Serial.print(analog); Serial.print("  ");
  analog=analogRead(11);Serial.print(analog); Serial.println("  ");
}

ValveControl VC;