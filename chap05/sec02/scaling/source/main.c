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

  TextSetCursor(10,6);
  TextPut("BIG SUSHI!");
  TextSetCursor(3,15);
  TextPut("UP,DOWN    -> Vertical");
  TextSetCursor(3,16);
  TextPut("LEFT,RIGHT -> Horizontal");

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

void Scaling(u32 num,u16 Sx,u16 Sy){
  OBJAFFINE* aff = (OBJAFFINE*)OAM + num;
  aff->pa=Sx;
  aff->pb=0;
  aff->pc=0;
  aff->pd=Sy;
}

s16 x=256;
s16 y=256;
void Update(){
  InputUpdate();

  if(InputIsKeyRepeat(KEY_LEFT)){
    x+=10;
  }
  if(InputIsKeyRepeat(KEY_RIGHT)){
    x-=10;
  }
  if(InputIsKeyRepeat(KEY_UP)){
    y+=10;
  }
  if(InputIsKeyRepeat(KEY_DOWN)){
    y-=10;
  }

  Scaling(0,x,y);

  TextSetCursor(0,0);
  TextPrintf("x=%.3f           \n"
	     "y=%.3f           \n",
	     x/256.0,y/256.0);
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
