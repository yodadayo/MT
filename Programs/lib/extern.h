#ifndef Extern_h
#define Extern_h

#include <stdint.h>
#define ID_NUM 4

extern  volatile uint32_t volume[12];
extern  uint16_t MAX[12];
extern  uint16_t MIN[12];
extern uint8_t SS[7];
extern  volatile uint16_t analogL[24];
extern  volatile uint16_t analogR[24];
extern  volatile uint32_t analogInSlave[12];
extern  volatile uint16_t L[24];
extern  volatile uint16_t R[24];
extern  volatile uint16_t slave_TDR ;
extern  uint32_t rangeNormalMode;
extern  uint32_t rangePluseMode;
extern  int32_t rangeH;
extern  int32_t rangeL;
extern  volatile short accData[3];
extern  volatile short gyrData[3];
extern  volatile short MpuData[6];
extern  uint16_t plusecnt[12];
extern  uint16_t RATE_HIGH;
extern  uint16_t RATE_LOW;
extern  uint16_t pluse_devide;
extern  unsigned short pressure_ref_time_right_tmp[9];
extern  unsigned short pressure_ref_time_left_tmp[9];
extern  unsigned short pressure_ref_array_right_tmp[9][40];
extern  unsigned short pressure_ref_array_left_tmp[9][40];
extern int lockLeft;
extern int lockRight;
extern int timeP;
extern int keep_RL_swing;
extern uint16_t RCThresholdLeft;
extern uint16_t RCThresholdRight;
extern unsigned char *buff;	
// extern uint16_t gyroData[3];
extern uint8_t walking_phase_frag;
extern int adjusting_flag;
extern uint16_t countinious_time;

extern volatile uint16_t vWriteValue[12];
extern volatile uint16_t vReadValue[12];
extern volatile int16_t mWriteValue[ID_NUM];
/////

// extern double errSum, lastErr;
// extern double kp, ki, kd;



#endif
