/* main.arm.c */

#include "gba.h"
#include "timer.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

bool isToggleBackGround=false;
u32 frame=0;

void IrqHandler(){
  REG_IME=0;
  u16 flag=REG_IF;

  switch(flag){
	case IRQ_TIMER0:
		isToggleBackGround=true;
		break;
	case IRQ_VBLANK:
		frame++;
		break;
	}

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
  REG_IE=IRQ_TIMER0|IRQ_VBLANK;
	REG_DISPSTAT=LCDC_VBL;
  INT_VECTOR=IrqHandler;
  REG_IME=1;
}

int main(void){
  Init();

  TimerStart(0);

  TextSetCursor(9,10);
  TextPut("HELLO,WORLD!");

  while(1){
		if(isToggleBackGround){
			TextToggleBackGround();
			isToggleBackGround=false;
		}
		TextSetCursor(9,11);
		TextPrintf("FRAME=%u",frame);
    WaitForVsync();
  }
}
