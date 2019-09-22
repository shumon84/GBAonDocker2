/* main.arm.c */

#include "gba.h"
#include "timer.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

u32 frame=0;
bool isToggle=false;

void VBlankIrqHandler(){
  frame++;
}

void Timer0IrqHandler(){
  isToggle=true;
}

void Init(){
  TextInit();
	TimerSetup(0,TIMER_FREQ_PER_256|TIMER_INTERRUPT);
	TimerSetCount(0,0xe000);

  /* 割り込みの設定 */
	irqInit();
	irqSet(IRQ_TIMER0,Timer0IrqHandler);
	irqSet(IRQ_VBLANK,VBlankIrqHandler);
	irqEnable(IRQ_TIMER0|IRQ_VBLANK);
}

void Update(){
    TextSetCursor(0,0);
    TextPrintf("FRAME=%u",frame);
		if(isToggle){
			TextToggleBackGround();
			isToggle=false;
		}
}

int main(void){
  Init();

	TimerStart(0);

  while(1){
    Update();
    WaitForVsync();
  }
}
