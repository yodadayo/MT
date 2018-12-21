#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "extern.h"
#include "SPIM_Ras.h"
#include <unistd.h>


#include "SlaveControlCommand.h"


SlaveControlCommand::SlaveControlCommand(void)
{
}
/////////////////////////////////////////
//iak tuushinnoizunotame medianwo toru kannsuuwo youi
int med3_8bit(uint8_t a, uint8_t b, uint8_t c)
{
      if (a >= b)
         if (b >= c)
             return b;
         else if (a <= c)
             return a;
         else
             return c;
      else if (a > c)
             return a;
      else if (b > c)
              return c;
      else
              return b;
}
int med3_16bit(uint16_t a, uint16_t b, uint16_t c)
{
      if (a >= b)
         if (b >= c)
             return b;
         else if (a <= c)
             return a;
         else
             return c;
      else if (a > c)
             return a;
      else if (b > c)
              return c;
      else
              return b;
}

uint16_t SlaveControlCommand::transfer_volume(uint8_t SSpin,uint8_t pin ,uint16_t vol ) {
    uint16_t Rdata;
    Rdata = SPIR.transfer(SSpin, ((pin) << 12) | (vol) << 0);
    //printf("%d  ",Rdata & 0xFFF);
    return Rdata;
  //return Rdata;
}
uint16_t buff_gyro;
/////////////////////////////////////////
void SlaveControlCommand::transfer_volume_all(uint8_t SSpin){
  //Left  side/////////////////////////
  transfer_volume(SSpin,  0, vWriteValue[0] ); 
  transfer_volume(SSpin,  1, vWriteValue[1] ); 
  transfer_volume(SSpin,  2, vWriteValue[2] );
  transfer_volume(SSpin,  3, vWriteValue[3] ); 
  transfer_volume(SSpin,  4, vWriteValue[4] ); 
  transfer_volume(SSpin,  5, vWriteValue[5] );
  transfer_volume(SSpin,  6, vWriteValue[6] );
  transfer_volume(SSpin,  7, vWriteValue[7] );
  transfer_volume(SSpin,  8, vWriteValue[8] );
  transfer_volume(SSpin,  9, vWriteValue[9] );
}

void SlaveControlCommand::transfer_volume_all_analog(uint8_t SSpin){
  uint16_t Rdata;
  Rdata=transfer_volume(SSpin,  0, vWriteValue[0] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  1, vWriteValue[1] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  2, vWriteValue[2] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  3, vWriteValue[3] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  4, vWriteValue[4] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  5, vWriteValue[5] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  6, vWriteValue[6] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  7, vWriteValue[7] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  8, vWriteValue[8] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  9, vWriteValue[9] ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  10, 0 ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  Rdata=transfer_volume(SSpin,  11, 0 ); vReadValue[(Rdata >> 12)&0xF]=(Rdata >> 0)&0x0FFF;
  }
//honnraino kansuu korewonaitokashinaito
// void SlaveControlCommand::getDateFromSSAB(uint8_t *data, uint8_t SSpin){
//   data[2] = SPIR.transferArm(SSpin,0);
//   data[0] = SPIR.transferArm(SSpin,1);
//   data[1] = SPIR.transferArm(SSpin,2);
// }



int SlaveControlCommand::getDataFromGyro(uint8_t SSpin, uint16_t *data){
  // int buff_gyro0[3], buff_gyro1[3], buff_gyro2[3];

  // buff_gyro2[0] = (SPIR.transfer(SSpin,0))& 0xFFF;
  // buff_gyro0[0] = (SPIR.transfer(SSpin,1))& 0xFFF;
  // buff_gyro1[0] = (SPIR.transfer(SSpin,2))& 0xFFF;
  // // usleep(100);
  // buff_gyro2[1] = (SPIR.transfer(SSpin,0))& 0xFFF;
  // buff_gyro0[1] = (SPIR.transfer(SSpin,1))& 0xFFF;
  // buff_gyro1[1] = (SPIR.transfer(SSpin,2))& 0xFFF;
  // // usleep(100);
  // buff_gyro2[2] = (SPIR.transfer(SSpin,0))& 0xFFF;
  // buff_gyro0[2] = (SPIR.transfer(SSpin,1))& 0xFFF;
  // buff_gyro1[2] = (SPIR.transfer(SSpin,2))& 0xFFF;

  // data[2] = med3_16bit(buff_gyro2[0],buff_gyro2[1],buff_gyro2[2]);
  // data[0] = med3_16bit(buff_gyro0[0],buff_gyro0[1],buff_gyro0[2]);
  // data[1] = med3_16bit(buff_gyro1[0],buff_gyro1[1],buff_gyro1[2]);
  


  buff_gyro = SPIR.transfer(SSpin,0);
  if( ((buff_gyro >> 12) & 0xF) >3){return 0;}
  data[(buff_gyro >> 12) & 0xF] = buff_gyro & 0xFFF;
  // usleep(100);
  buff_gyro = SPIR.transfer(SSpin,1);
  if( ((buff_gyro >> 12) & 0xF) >3){return 0;}
  data[(buff_gyro >> 12) & 0xF] = buff_gyro & 0xFFF;
  // usleep(100);
  buff_gyro = SPIR.transfer(SSpin,2);
  if( ((buff_gyro >> 12) & 0xF) >3){return 0;}
  data[(buff_gyro >> 12) & 0xF] = buff_gyro & 0xFFF;
  // usleep(100);
  // buff_gyro = SPIR.transfer(SSpin,0);
  // data[2] = buff_gyro & 0xFFF;
  // buff_gyro = SPIR.transfer(SSpin,1);
  // data[1] = buff_gyro & 0xFFF;
  // buff_gyro = SPIR.transfer(SSpin,2);
  // data[0] = buff_gyro & 0xFFF;
}


void SlaveControlCommand::getDataFromSSAB(uint8_t SSpin, uint16_t *data){
  int buff0[3], buff1[3], buff2[3];

  buff2[0] = SPIR.transferArm(SSpin,0);
  buff0[0] = SPIR.transferArm(SSpin,1);
  buff1[0] = SPIR.transferArm(SSpin,2);

  buff2[1] = SPIR.transferArm(SSpin,0);
  buff0[1] = SPIR.transferArm(SSpin,1);
  buff1[1] = SPIR.transferArm(SSpin,2);

  buff2[2] = SPIR.transferArm(SSpin,0);
  buff0[2] = SPIR.transferArm(SSpin,1);
  buff1[2] = SPIR.transferArm(SSpin,2);
  
  data[2] = med3_8bit(buff2[0],buff2[1],buff2[2]);
  data[0] = med3_8bit(buff0[0],buff0[1],buff0[2]);
  data[1] = med3_8bit(buff1[0],buff1[1],buff1[2]);
}
void SlaveControlCommand::getDataFromEncoder1(uint8_t SSpin, uint16_t *data){
  uint16_t buff;
  buff = SPIR.transferEncoder1(SSpin,0);
  *data = ( (buff>>0 )>>0 ) & 0xFFF;
  
}
void SlaveControlCommand::getDataFromEncoder3(uint8_t SSpin, uint16_t *data){
  uint64_t buff;
  buff = SPIR.transferEncoder3(SSpin,0);
  data[0] = ( (buff>>0 )>>0 ) & 0xFFF;
  data[1] = ( (buff>>12)>>0 ) & 0xFFF;
  data[2] = ( (buff>>24)>>0 ) & 0xFFF;

}


SlaveControlCommand SCC;
