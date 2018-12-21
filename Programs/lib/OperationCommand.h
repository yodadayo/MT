#ifndef OperationCommand_h
#define OperationCommand_h

class OperationCommand
{
  public:
  // SPI Configuration methodsasss
  OperationCommand(void);
  void bothStance(int num,unsigned short vol[][40]);
  void bothSwing(int num,unsigned short vol[][40]);
  void rightStancePhase(int num,unsigned short vol[][40]);
  void leftStancePhase(int num,unsigned short vol[][40]);
  void rightStancePhaseContinuous(int num,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  void leftStancePhaseContinuous(int num,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  /////
  void bothStanceMotionSwitch(int analog_right,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  void bothSwingMotionSwitch(int analog_left,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  void rightStanceMotionSwitch(int analog_right,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  void leftStanceMotionSwitch(int analog_left,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  uint16_t rightStanceMotionSwitchContinuous(int analog_right,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  uint16_t leftStanceMotionSwitchContinuous(int analog_left,unsigned short vol[][40],unsigned short *time_tmp, int finit_time);
  void swingMotion(unsigned short volL[][40],unsigned short volR[][40],unsigned short *time_tmp,int finit_time);
  void swingMotion(unsigned short vol[][40],unsigned short *time_tmp,int finit_time);
  uint16_t swingMotionContinuous(unsigned short vol[][40],unsigned short *time_tmp,int finit_time);
  void viewAllTransferVolume(void);
  void viewSomeTransferVolume(uint16_t first,uint16_t last);
  void viewAllRecieveVolume(void);
  void viewLeftSomeRecieveVolume(uint16_t first,uint16_t last);
  void viewRightSomeRecieveVolume(uint16_t first,uint16_t last);
  void allSupply(void);
  void allExhaust(void);


    private:

};
extern OperationCommand OC;
#endif
