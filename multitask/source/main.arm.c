/* main.c */

#include "gba.h"
#include "timer.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

/* void IrqHandler(){ */
/*   REG_IME=0; */
/*   u16 flag=REG_IF; */

/*   TextToggle(); */

/*   /\* REG_IF と REG_IME は *\/ */
/*   /\* 再設定する必要がある *\/ */
/*   REG_IF=flag; */
/*   REG_IME=1; */
/*   return; */
/* } */

void IrqTimer0(){
  TextToggle();
}

void IrqVBLANK(){
	static u32 frame=0;
	frame++;
	TextSetCursor(0,1);
	TextPrintf("VBLANK=%08d",frame);
}

void Init(){
  TextInit();
  TextHideBackGround();
  TimerSetup(0,TIMER_FREQ_PER_256|TIMER_INTERRUPT);
  TimerSetCount(0,0xe000);

  /* 割り込みの設定 */
	irqInit();
	irqSet(IRQ_TIMER0,IrqTimer0);
	irqSet(IRQ_VBLANK,IrqVBLANK);
	irqEnable(IRQ_TIMER0|IRQ_VBLANK);
}

int main(void){
  Init();

  TimerStart(0);

  TextPut("HELLO,WORLD!");

  while(1){
    WaitForVsync();
  }
}
