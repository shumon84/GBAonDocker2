/* main.arm.c */

#include "gba.h"
#include "text.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

u32 frame=0;

void IrqHandler(){
  REG_IME=0;
  u16 flag=REG_IF;

  switch(flag){
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

  /* 割り込みの設定 */
  REG_IME=0;
  REG_IE=IRQ_VBLANK;
  REG_DISPSTAT=LCDC_VBL;
  INT_VECTOR=IrqHandler;
  REG_IME=1;
}

void Update(){
    TextSetCursor(0,0);
    TextPrintf("FRAME=%u",frame);
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
