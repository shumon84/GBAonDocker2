/* main.c */

#include "gba.h"
#include "text.h"
#include "sprite.h"
#include "sushi.h"
#include "input.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

void Init(){
  /* テキストシステムの設定 */
  TextInit();

  TextSetCursor(7,6);
  TextPut("DON'T STOP SUSHI!");
  TextSetCursor(3,15);
  TextPut("A BUTTON->SPIN SPIN SPIN");
  TextSetCursor(3,16);
  TextPut("B BUTTON->SPEED UP");

  /* OAMとパレットの設定 */
  u16* oam=OBJ_BASE_ADR;
  u16* pal=OBJ_COLORS;
  for(u32 i=0;i<sushiTilesLen/2;i++){
    oam[i]=sushiTiles[i];
  }
  for(u32 i=0;i<sushiPalLen/2;i++){
    pal[i]=sushiPal[i];
  }

  /* スプライトの設定 */
  SetMode(MODE_0|BG0_ON|OBJ_ENABLE|OBJ_1D_MAP);
  SpriteInit();
  SpriteSetUp(0,0,Sprite_16x16,112,72);
  SpriteEnableDoubleSize(0);
  SpriteEnableRotationScaling(0,0);
}

void rotate(u32 num,u16 angle){
  // angle<<8*(180<<8/0xFFFF)
  ObjAffineSource src={256,256,angle*180};
  ObjAffineDest dst={};
  ObjAffineSet(&src,&dst,1,2);
  AP(num)->pa=dst.pa;
  AP(num)->pb=dst.pb;
  AP(num)->pc=dst.pc;
  AP(num)->pd=dst.pd;
}

u32 angle=0;
u32 speed=0;

void Update(){
  InputUpdate();
  if(InputIsKeyPressed(KEY_B)){
    speed++;
  }
  if(InputIsKeyRepeat(KEY_A)){
    angle+=speed;
    if(angle>=360){
      angle-=360;
    }
  };
  rotate(0,angle);

  TextSetCursor(0,0);
  TextPrintf("angle=%d\n"
             "spped=%d\n",
             angle,speed);
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
