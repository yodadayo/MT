#include "extern.h"
#include "rs232.h"
#include "MoterControl.h"
#include <stdio.h>

MoterControl::MoterControl(void)
{
}

void MoterControl::SerialInit(int serialport, int baudrate, const char *mode){
  if(RS232_OpenComport(serialport, baudrate, mode))
  {
    printf("Can not open comport\n");
    return;
  }
  
  usleep(2*1000*1000); // waits 2000msec for stable condition
}

void MoterControl::SendData(int serialport, uint8_t mnum){

  unsigned char buff[2] = {0, 0};
  char *header, *footer;
  strcpy(header, "H"); strcpy(footer, "F");
  
  for(int i=0; i<mnum; i++){
    RS232_cputs(serialport, header); // header
    // if(RS232_SendByte(serialport, i+1 & 0xFF) < 0){ // sends the ID of the moter
    //   printf("failed\n");
    //   return ;
    // }
    buff[0] = (mWriteValue[i][0]>>0) & 0xFF;
    if(RS232_SendByte(serialport, buff[0]) < 0){ // sends the position(1 byte)
      printf("failed\n");
      return ;
    }
    buff[0] = (mWriteValue[i][1]>>8) & 0xFF;
    buff[1] = (mWriteValue[i][1]>>0) & 0xFF;
    if(RS232_SendBuf(serialport, buff, 2) < 0){ // sends the velocity(2 byte)
      printf("failed\n");
      return ;
    }
    RS232_cputs(serialport, footer); // footer
  }

}

void MoterControl::moter_init(int serialport, uint8_t mnum){

  int i,j;
  
  for(i=0; i<mnum; i++){
    mWriteValue[i][0] = 0;
  }
  for(i=0; i<mnum; i++){
    mWriteValue[i][1] = 200;
  }

  SendData(serialport, mnum);
  
}

MoterControl::~MoterControl(void)
{
}

MoterControl MC;
