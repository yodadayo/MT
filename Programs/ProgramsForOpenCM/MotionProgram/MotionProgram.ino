/*

file: MotionProgram.ino
purpose: receiving the data from Raspberry pi and actuating the moters to the
 designated position with designated velocity (in real-time)

*/

#define ID_MAX 4
Dynamixel Dxl(3); // Open CM 485 Exp

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin();
  Dxl.begin(1); // baud rate of the dynamixel: 57600Hz
  for(int i=1; i<=ID_MAX; i++){
    Dxl.jointMode(i);
    Dxl.setPosition(i,0,300); // all moters will be set to initial position
  }
  delay(1000);
  
}

void usbInterrupt(byte *incomingByte, byte nCount){
  
  uint8 id = 0; int8 position = 0; uint16 velocity = 0;
  
  if(nCount == 1){ // if the data is 1 byte
    if(*incomingByte == 'H')
      id++;
    else if(*incomingByte == 'F'){
      Dxl.setPosition(id, -position*4096/360+2048, velocity);
      if(id/ID_MAX > 0) id = 0;
    }
    else
      position = (int8)*incomingByte;
  }
  else if(nCount == 2){ // if the data is 2 byte
    velocity = ( (uint16)( *(incomingByte+1) & 0x00FF) | ( *(incomingByte)<<8 & 0xFF00) )/0.229;
  }
  else return;
  
}

void loop() {
  // put your main code here, to run repeatedly: 
}

