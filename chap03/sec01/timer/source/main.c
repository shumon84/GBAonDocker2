/* main.c */

#include "gba.h"
#include "text.h"

/* タイマの設定に使うマクロ */
#define TIMER_FREQ_PER_1    0
#define TIMER_FREQ_PER_64   1
#define TIMER_FREQ_PER_256  2
#define TIMER_FREQ_PER_1024 3
#define TIMER_CASCADE       BIT(2)
#define TIMER_INTERRUPT     BIT(6)
#define TIMER_START         BIT(7)

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

  /* タイマの起動 */
  REG_TM0CNT_H|=TIMER_START;
  REG_TM1CNT_H|=TIMER_START;
  REG_TM2CNT_H|=TIMER_START;
  REG_TM3CNT_H|=TIMER_START;
}

void Update(){
  TextSetCursor(0,0);
  TextPrintf("TM0CNT_L=%5u\n",REG_TM0CNT_L);
  TextPrintf("TM1CNT_L=%5u\n",REG_TM1CNT_L);
  TextPrintf("TM2CNT_L=%5u\n",REG_TM2CNT_L);
  TextPrintf("TM3CNT_L=%5u\n",REG_TM3CNT_L);
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
