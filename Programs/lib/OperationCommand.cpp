#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "extern.h"
#include "SPIM_Ras.h"
#include "OperationCommand.h"



#define AAA 100
extern float pitch;

OperationCommand::OperationCommand(void)
{
}

void OperationCommand::bothStance(int num, unsigned short vol[][40]){
      for (int j = 0; j < 20; j++) {
        L[j] =  820 + vol[num][j + 20] * 3.28 ;
        R[j] =  820 + vol[num][j + 20] * 3.28 ;
      }
}
void OperationCommand::bothSwing(int num, unsigned short vol[][40]){
      for (int j = 0; j < 20; j++) {
        L[j] =  820 + vol[num][j] * 3.28 ;
        R[j] =  820 + vol[num][j] * 3.28 ;
      }
}

void OperationCommand::rightStancePhase(int num,unsigned short vol[][40]){
      for (int j = 0; j < 20; j++) {
        L[j] =  820 + vol[num][j] * 3.28 ;
        R[j] =  820 + vol[num][j + 20] * 3.28 ;
      }
     walking_phase_frag = 1;
}
void OperationCommand::leftStancePhase(int num, unsigned short vol[][40]){
      for (int j = 0; j < 20; j++) {
        L[j] =  820 + vol[num][j + 20] * 3.28 ;
        R[j] =  820 + vol[num][j] * 3.28 ;
      }
      walking_phase_frag = 0;
}


void OperationCommand::rightStancePhaseContinuous(int num,unsigned short vol[][40], unsigned short *time_tmp, int finit_time){
      double xtt,xt, ytt,yt,yout,xin;
      xtt =time_tmp[num];
      xin =countinious_time;
      if(num == 0){
        xt =0;       
      }else{
        xt =time_tmp[num-1];       
      }

      for (int j = 0; j < 20; j++) {
        if(num == 0){
            ytt = vol[0][j];
            yt = vol[9][j+20];    
        }else {
            ytt = vol[num][j];
            yt = vol[num-1][j];      
        }
        yout = (ytt-yt)/(xtt-xt)*(xin - xt)+yt;
        L[j] =  820 + yout * 3.28 ;
        if(num == 0){
            ytt = vol[0][j+20];
            yt = vol[9][j];    
        }else {
            ytt = vol[num][j+20];
            yt = vol[num-1][j+20];       
        }
        yout = (ytt-yt)/(xtt-xt)*(xin - xt)+yt;
        R[j] =  820 + yout * 3.28 ;
      }
     walking_phase_frag = 1;
}

void OperationCommand::leftStancePhaseContinuous(int num, unsigned short vol[][40], unsigned short *time_tmp, int finit_time){
      double xtt,xt, ytt,yt,yout,xin;
      xtt =time_tmp[num];
      xin =countinious_time;
      // printf("num-----%d\n",num);
      if(num == 0){
        xt =0;       
      }else{
        xt =time_tmp[num-1];       
      }
      // printf("%f\n", xin - xt);
      
      for (int j = 0; j < 20; j++) {
        if(num == 0){
            ytt = vol[0][j+20];
            yt = vol[9][j];
 
        }else{
            ytt = vol[num][j+20];
            yt = vol[num-1][j+20];       
        }
        yout = (ytt-yt)/(xtt-xt)*(xin - xt)+yt;
        L[j] =  820 + yout * 3.28 ;

        if(num == 0){
            ytt = vol[0][j];
            yt = vol[9][j+20];    
        }else{
            ytt = vol[num][j];
            yt = vol[num-1][j];      
        }
        yout = (ytt-yt)/(xtt-xt)*(xin - xt)+yt;
        R[j] =  820 + yout * 3.28 ;
      }
      walking_phase_frag = 0;
  
}

void OperationCommand::bothStanceMotionSwitch(int analog_right,unsigned short vol[][40],unsigned short *time_tmp, int finit_time){
  if ((analog_right > RCThresholdRight)&&(lockRight==0)&&(lockLeft==0)&&(keep_RL_swing==0)){timeP=0; lockRight=1;}
  if (lockRight==1){
    if (timeP <= time_tmp[0]){ bothStance(0,vol);}
    else if (timeP <= time_tmp[1]){ bothStance(1,vol);}
    else if (timeP <= time_tmp[2]){ bothStance(2,vol);}
    else if (timeP <= time_tmp[3]){ bothStance(3,vol);}
    else if (timeP <= time_tmp[4]){ bothStance(4,vol);}
    else if (timeP <= time_tmp[5]){ bothStance(5,vol);}
    else if (timeP <= time_tmp[6]){ bothStance(6,vol);}
    else if (timeP <= time_tmp[7]){ bothStance(7,vol);}
    else if (timeP <= time_tmp[8]){ bothStance(8,vol);}
    else if (timeP <= time_tmp[9]){ bothStance(9,vol);}
    else if (timeP >= finit_time){lockRight=0; timeP=0; keep_RL_swing=1;}
  }
}

void OperationCommand::bothSwingMotionSwitch(int analog_left,unsigned short vol[][40],unsigned short *time_tmp, int finit_time){
  if ((analog_left > RCThresholdLeft)&&(lockRight==0)&&(lockLeft==0)&&(keep_RL_swing==1)){timeP=0; lockLeft=1;}
  if (lockLeft==1){
    if (timeP <= time_tmp[0]){ bothSwing(0,vol);}
    else if (timeP <= time_tmp[1]){ bothSwing(1,vol);}
    else if (timeP <= time_tmp[2]){ bothSwing(2,vol);}
    else if (timeP <= time_tmp[3]){ bothSwing(3,vol);}
    else if (timeP <= time_tmp[4]){ bothSwing(4,vol);}
    else if (timeP <= time_tmp[5]){ bothSwing(5,vol);}
    else if (timeP <= time_tmp[6]){ bothSwing(6,vol);}
    else if (timeP <= time_tmp[7]){ bothSwing(7,vol);}
    else if (timeP <= time_tmp[8]){ bothSwing(8,vol);}
    else if (timeP <= time_tmp[8]){ bothSwing(8,vol);}
    else if (timeP <= time_tmp[9]){ bothSwing(9,vol);}
    else if (timeP >= finit_time){lockLeft=0; timeP=0; keep_RL_swing=0;}
  }
}

void OperationCommand::rightStanceMotionSwitch(int analog_right,unsigned short vol[][40],unsigned short *time_tmp, int finit_time){
  if ((analog_right > RCThresholdRight)&&(lockRight==0)&&(lockLeft==0)&&(keep_RL_swing==0)){timeP=0; lockRight=1;adjusting_flag = 0;}
  if (lockRight==1){
    if (timeP <= time_tmp[0]){ rightStancePhase(0,vol);}
    else if (timeP <= time_tmp[1]){ rightStancePhase(1,vol);}
    else if (timeP <= time_tmp[2]){ rightStancePhase(2,vol);}
    else if (timeP <= time_tmp[3]){ rightStancePhase(3,vol);}
    else if (timeP <= time_tmp[4]){ rightStancePhase(4,vol);}
    else if (timeP <= time_tmp[5]){ rightStancePhase(5,vol);}
    else if (timeP <= time_tmp[6]){ rightStancePhase(6,vol);}
    else if (timeP <= time_tmp[7]){ rightStancePhase(7,vol);}
    else if (timeP <= time_tmp[8]){ rightStancePhase(8,vol);}
    else if (timeP <= time_tmp[9]){ rightStancePhase(9,vol);}
    else if (timeP >= finit_time){lockRight=0; timeP=0; keep_RL_swing=1;}
  }
}

void OperationCommand::leftStanceMotionSwitch(int analog_left,unsigned short vol[][40],unsigned short *time_tmp, int finit_time){
  if ((analog_left > RCThresholdLeft)&&(lockRight==0)&&(lockLeft==0)&&(keep_RL_swing==1)){timeP=0; lockLeft=1;adjusting_flag = 0;}
  if (lockLeft==1){
    if (timeP <= time_tmp[0]){ leftStancePhase(0,vol);}
    else if (timeP <= time_tmp[1]){ leftStancePhase(1,vol);}
    else if (timeP <= time_tmp[2]){ leftStancePhase(2,vol);}
    else if (timeP <= time_tmp[3]){ leftStancePhase(3,vol);}
    else if (timeP <= time_tmp[4]){ leftStancePhase(4,vol);}
    else if (timeP <= time_tmp[5]){ leftStancePhase(5,vol);}
    else if (timeP <= time_tmp[6]){ leftStancePhase(6,vol);}
    else if (timeP <= time_tmp[7]){ leftStancePhase(7,vol);}
    else if (timeP <= time_tmp[8]){ leftStancePhase(8,vol);}
    else if (timeP <= time_tmp[9]){ leftStancePhase(9,vol);}
    else if (timeP >= finit_time){lockLeft=0; timeP=0; keep_RL_swing=0;}
  }
}

uint16_t OperationCommand::rightStanceMotionSwitchContinuous(int analog_right,unsigned short vol[][40],unsigned short *time_tmp, int finit_time){
  if ((analog_right > RCThresholdRight)&&(lockRight==0)&&(lockLeft==0)&&(keep_RL_swing==0)){ countinious_time =0;lockRight=1;adjusting_flag = 0;}
  if (lockRight==1){
    if (countinious_time <= time_tmp[0]){ rightStancePhaseContinuous(0,vol,time_tmp,finit_time); return 0;}
    else if (countinious_time <= time_tmp[1]){ rightStancePhaseContinuous(1,vol,time_tmp,finit_time); return 1;}
    else if (countinious_time <= time_tmp[2]){ rightStancePhaseContinuous(2,vol,time_tmp,finit_time); return 2;}
    else if (countinious_time <= time_tmp[3]){ rightStancePhaseContinuous(3,vol,time_tmp,finit_time); return 3;}
    else if (countinious_time <= time_tmp[4]){ rightStancePhaseContinuous(4,vol,time_tmp,finit_time); return 4;}
    else if (countinious_time <= time_tmp[5]){ rightStancePhaseContinuous(5,vol,time_tmp,finit_time); return 5;}
    else if (countinious_time <= time_tmp[6]){ rightStancePhaseContinuous(6,vol,time_tmp,finit_time); return 6;}
    else if (countinious_time <= time_tmp[7]){ rightStancePhaseContinuous(7,vol,time_tmp,finit_time); return 7;}
    else if (countinious_time <= time_tmp[8]){ rightStancePhaseContinuous(8,vol,time_tmp,finit_time); return 8;}
    else if (countinious_time <= time_tmp[9]){ rightStancePhaseContinuous(9,vol,time_tmp,finit_time); return 9;}
    else if (countinious_time > finit_time){lockRight=0; keep_RL_swing=1; countinious_time =0; }
  }
  return 10;
}

uint16_t OperationCommand::leftStanceMotionSwitchContinuous(int analog_left,unsigned short vol[][40],unsigned short *time_tmp, int finit_time){
  if ((analog_left > RCThresholdLeft)&&(lockRight==0)&&(lockLeft==0)&&(keep_RL_swing==1)){countinious_time =0;lockLeft=1;adjusting_flag = 0;}
  if (lockLeft==1){
    if (countinious_time <= time_tmp[0]){ leftStancePhaseContinuous(0,vol,time_tmp,finit_time); return 0;}
    else if (countinious_time <= time_tmp[1]){ leftStancePhaseContinuous(1,vol,time_tmp,finit_time); return 1;}
    else if (countinious_time <= time_tmp[2]){ leftStancePhaseContinuous(2,vol,time_tmp,finit_time); return 2;}
    else if (countinious_time <= time_tmp[3]){ leftStancePhaseContinuous(3,vol,time_tmp,finit_time); return 3;}
    else if (countinious_time <= time_tmp[4]){ leftStancePhaseContinuous(4,vol,time_tmp,finit_time); return 4;}
    else if (countinious_time <= time_tmp[5]){ leftStancePhaseContinuous(5,vol,time_tmp,finit_time); return 5;}
    else if (countinious_time <= time_tmp[6]){ leftStancePhaseContinuous(6,vol,time_tmp,finit_time); return 6;}
    else if (countinious_time <= time_tmp[7]){ leftStancePhaseContinuous(7,vol,time_tmp,finit_time); return 7;}
    else if (countinious_time <= time_tmp[8]){ leftStancePhaseContinuous(8,vol,time_tmp,finit_time); return 8;}
    else if (countinious_time <= time_tmp[9]){ leftStancePhaseContinuous(9,vol,time_tmp,finit_time); return 9;}
    else if (countinious_time > finit_time){lockLeft=0; keep_RL_swing=0; countinious_time =0; }
  }
  return 10;
}

//20160204
void OperationCommand::swingMotion(unsigned short volL[][40],unsigned short volR[][40],unsigned short *time_tmp,int finit_time){
    for(int i=0; i<=9; i++)if(timeP <= time_tmp[i])rightStancePhase(i,volL);
    for(int i=0; i<=9; i++)if(timeP <= (time_tmp[i]+time_tmp[8]+finit_time))leftStancePhase(i,volR);
    if (timeP >= (finit_time*2)){ timeP=0;}
}
//20160204
void OperationCommand::swingMotion(unsigned short vol[][40],unsigned short *time_tmp,int finit_time){
    // printf("time%d : \n",timeP);
    for(int i=0; i<=9; i++)if(timeP <= time_tmp[i]){rightStancePhase(i,vol);break;}
    if(timeP > finit_time){
      for(int i=0; i<=9; i++)if(timeP <= (time_tmp[i]+finit_time)){leftStancePhase(i,vol);break;}
    }
    if (timeP >= (finit_time*2)){ timeP=0;}

}
//20161006
uint16_t buff1=0;
uint16_t buff2=0;
uint16_t OperationCommand::swingMotionContinuous(unsigned short vol[][40],unsigned short *time_tmp,int finit_time){
    RCThresholdLeft=0;
    RCThresholdRight=0;
    
    buff1 = rightStanceMotionSwitchContinuous( 150, vol, time_tmp, finit_time);
    printf("%d",buff1);
    buff2 = leftStanceMotionSwitchContinuous(  150, vol, time_tmp, finit_time);
    printf("--%d\n",buff2);
}
void OperationCommand::viewAllTransferVolume(void){
    for(int i=0 ;i<20;i++){printf("%d",L[i]);printf(" , ");}
    for(int t=0 ;t<20;t++){printf("%d",R[t]);printf(" , ");}
    printf(" \n");
}
void OperationCommand::viewSomeTransferVolume(uint16_t first,uint16_t last){
    for(int i=first ;i<=last;i++){printf("%d",L[i]);printf(" , ");}
    for(int t=first ;t<=last;t++){printf("%d",R[t]);printf(" , ");}
     printf(" \n");
}
void OperationCommand::viewAllRecieveVolume(void){
    for(int i=0 ;i<24;i++){printf("%d",analogL[i]);printf(" , ");}
      printf(" \n");
    for(int t=0 ;t<24;t++){printf("%d",analogR[t]);printf(" , ");}
     printf(" \n\n");
}
void OperationCommand::viewLeftSomeRecieveVolume(uint16_t first,uint16_t last){
    for(int i=first ;i<=last;i++){printf("%d",analogL[i]);printf(" , ");}
    //for(int t=first ;t<=last;t++){printf(analogR[t]);printf(" , ");}
     printf(" \n");
}
void OperationCommand::viewRightSomeRecieveVolume(uint16_t first,uint16_t last){
    //for(int i=first ;i<=last;i++){printf(analogL[i]);printf(" , ");}
    for(int t=first ;t<=last;t++){printf("%d",analogR[t]);printf(" , ");}
     printf(" \n");
}
void OperationCommand::allSupply(void){
    for(int i=0 ;i<=24-1;i++){L[i]=3000; R[i]=3000;}
}
void OperationCommand::allExhaust(void){
    for(int i=0 ;i<=24-1;i++){L[i]=0; R[i]=0;}
}


OperationCommand OC;