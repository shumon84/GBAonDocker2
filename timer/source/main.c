/* main.c */

#include "gba.h"
#include "timer.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

void Init(){
  TextInit();

  /* タイマの設定 */
  REG_TM0CNT_H=TIMER_FREQ_PER_256;
  REG_TM1CNT_H=TIMER_FREQ_PER_1|TIMER_CASCADE;
  REG_TM2CNT_H=TIMER_FREQ_PER_256;
  REG_TM3CNT_H=TIMER_FREQ_PER_1024|TIMER_CASCADE;

  REG_TM0CNT_H|=TIMER_START;
  REG_TM1CNT_H|=TIMER_START;
  REG_TM2CNT_H|=TIMER_START;
  REG_TM3CNT_H|=TIMER_START;
}

void Update(){
  u32 tm0=REG_TM0CNT_L;
  u32 tm1=REG_TM1CNT_L;
  u32 tm2=REG_TM2CNT_L;
  u32 tm3=REG_TM3CNT_L;

  TextSetCursor(0,0);
  TextPrintf("TM0CNT_L=%5d\n",tm0);
  TextPrintf("TM1CNT_L=%5d\n",tm1);
  TextPrintf("TM2CNT_L=%5d\n",tm2);
  TextPrintf("TM3CNT_L=%5d\n",tm3);
}

void IrqTimer(){
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
