/* main.c */

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
  TimerSetup(0,TIMER_FREQ_PER_1);

  TimerStart(0);
}

void Update(){
  TimerUpdate();
  InputUpdate();

  int tm0=TimerGet(0);

  TextSetCursor(0,0);
  TextPrintf("TM0CNT_H=%5d\n",tm0);
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
