/* main.c */

#include "gba.h"
#include "timer.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

void IrqHandler(){
  REG_IME=0;
  u16 flag=REG_IF;

  TextToggle();

  /* REG_IF と REG_IME は */
  /* 再設定する必要がある */
  REG_IF=flag;
  REG_IME=1;
  return;
}

void Init(){
  TextInit();
  TextHideBackGround();
  TimerSetup(0,TIMER_FREQ_PER_256|TIMER_INTERRUPT);
  TimerSetCount(0,0xe000);

  /* 割り込みの設定 */
  REG_IME=0;
  REG_IE=IRQ_TIMER0;
  INT_VECTOR=IrqHandler;
  REG_IME=1;
}

int main(void){
  Init();

  TimerStart(0);

  TextSetCursor(9,10);
  TextPut("HELLO,WORLD!");
  TextPrintf("\nsize = %d\n",sizeof(IrqHandler));
  TextPrintf("\nsize = %p\n",&INT_VECTOR);

  while(1){
    WaitForVsync();
  }
}
