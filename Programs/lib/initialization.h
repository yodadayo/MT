#ifndef Initialization_h
#define Initialization_h

#define SS_LEFT_SIDE 2
#define SS_RIGHT_SIDE 1
#define SS_LEFT_BACK 3
#define SS_RIGHT_BACK 0
			 
#define SPI_SPEED 1000000
#define FINT 500
#define ID_MAX 4

uint8_t SS[7]={SS_LEFT_BACK,SS_RIGHT_SIDE,SS_RIGHT_BACK,SS_LEFT_SIDE,4,6,7};
volatile uint16_t analogL[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile uint16_t analogR[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile uint32_t volume[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//･ﾐ･・ﾖ｣ｱ｣ｲ・
volatile uint32_t analogInSlave[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint32_t rangeNormalMode  = 400;
uint32_t rangePluseMode  = 150; //・ﾁｦｎｹ・
int32_t rangeH  = 400;
int32_t rangeL  = 200;
volatile uint16_t slave_TDR = 100;
volatile short accData[3];
volatile short gyrData[3];
volatile short MpuData[6];
volatile uint16_t L[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile uint16_t R[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

volatile uint16_t vWriteValue[12]={0,0,0,0,0,0,0,0,0,0};
volatile uint16_t vReadValue[12]={0,0,0,0,0,0,0,0,0,0};
volatile int16_t mWriteValue[ID_MAX][2]={0};
volatile int16_t mReadValue[ID_MAX][2]={0};

int Rflag = 1;
int readflag = -1;
int anaL=0;
int anaR=0;
int keep_RL_swing=0;
int timeP = 0;
int lockLeft =0;
int lockRight=0;
uint16_t plusecnt[12]={0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t RATE_HIGH = 1 ;
uint16_t RATE_LOW  = 1 ;
uint16_t pluse_devide = 4;
uint16_t RCThresholdLeft =560; //leftfoot
uint16_t RCThresholdRight =580;//rightfoot
unsigned char *buff;							//送受信用バッファ
uint8_t data_ssab_1[3];
uint8_t data_ssab_2[3];
int socketBuff[4];
uint16_t data_gyro[3][3];
// uint16_t gyroData[3];
uint8_t walking_phase_frag =0;
int adjusting_flag = 0;
uint16_t countinious_time;
////////
// double errSum, lastErr;
// double kp, ki, kd;

#endif
