#ifndef MoterControl_h
#define MoterControl_h

#include <stdint.h>
#include <unistd.h>

class MoterControl
{
public:
  MoterControl(void);
  void SerialInit(int serialport, int baudrate, const char *mode);
  void SendData(int serialport, uint8_t mnum);
  void moter_init(int serialport, uint8_t mnum);
  ~MoterControl(void);
  
private:
};

extern MoterControl MC;
#endif
