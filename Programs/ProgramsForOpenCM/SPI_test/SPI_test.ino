
void setup() {
  // put your setup code here, to run once:
    SerialUSB.begin(); // opens serial port
    SerialUSB.attachInterrupt(usbInterrupt);
}

void usbInterrupt(byte nCount, byte* incomingByte){
  // for(unsigned int i=0; i < nCount; i++)
    SerialUSB.write(incomingByte, nCount);
}

void loop() {
  // put your main code here, to run repeatedly:
//  if( SerialUSB.available() ){
//    char incomingByte = SerialUSB.read(); // read byte
//    SerialUSB.print(incomingByte); // send it back
//  }
}

