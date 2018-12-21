/* Minimum_Source*/
#define BUF_SIZE 64
char input[BUF_SIZE];
int i = 0;

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin();
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(SerialUSB.available()) {
    input[i] = SerialUSB.read();
    if(i >= BUF_SIZE || input[i] == '.') {
      input[i] = '\0';
      SerialUSB.write(input);
//      SerialUSB.write("\n");
      i = 0;
    }
    else { i++; }
  }
}
