/* Minimum_Source*/
#define BUFFER 10
int sum = 0; int int_send; int value;

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin();
  SerialUSB.attachInterrupt(usbInterrupt);
}

void usbInterrupt(byte *incomingByte, byte nCount){
  if(*incomingByte != 'N'){
    value = *incomingByte - '0';
    sum += value;
  }
  else{
    int_send = sum % 10;
    SerialUSB.write(&int_send, 1);
    sum = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
}

