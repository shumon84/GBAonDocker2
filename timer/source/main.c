/* main.c */

#include <stdio.h>
#include "gba.h"
#include "timer.h"
#include "input.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

void Init(){
  TextInit();

  /* タイマの設定 */
  TimerSetup(0,TIMER_FREQ_PER_1|TIMER_INTERRUPT);
  TimerSetup(1,TIMER_FREQ_PER_1|TIMER_INTERRUPT);

  TimerSetCount(0,0);
  TimerSetCount(1,0);

  TimerStart(0);
  TimerStart(1);
}

void Update(){
  TimerUpdate();
  InputUpdate();

  int tm0=TimerGet(0);
  int tm1=TimerGet(1);
  TextSetCursor(0,0);
  TextPrintf("TM0CNT_H=%5d\n",tm0);
  TextPrintf("TM1CNT_H=%5d\n",tm1);

  if(InputIsKeyPressed(KEY_A)){
    TextToggle();
  }
  if(InputIsKeyPressed(KEY_B)){
    TextToggleBackGround();
  }
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
