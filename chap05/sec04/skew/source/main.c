/* main.c */

#include "gba.h"
#include "text.h"
#include "sprite.h"
#include "deform.h"
#include "input.h"

void WaitForVsync(){
  while(REG_VCOUNT>=160);
  while(REG_VCOUNT<160);
}

void Init(){
  /* テキストシステムの設定 */
  TextInit();

  /* OAMとパレットの設定 */
  u16* oam=OBJ_BASE_ADR;
  u16* pal=OBJ_COLORS;
  for(u32 i=0;i<deformTilesLen/2;i++){
    oam[i]=deformTiles[i];
  }
  for(u32 i=0;i<deformPalLen/2;i++){
    pal[i]=deformPal[i];
  }

  /* スプライトの設定 */
  SetMode(MODE_0|BG0_ON|OBJ_ENABLE|OBJ_1D_MAP);
  SpriteInit();
  SpriteSetUp(0,0,Sprite_64x64,(240-128)/2,(160-128)/2);
  SpriteEnableDoubleSize(0);
  SpriteEnableRotationScaling(0,0);
}

u32 GetTan(u16 angle){
  ObjAffineSource src={
		       256,
		       256,
		       angle*180};
  ObjAffineDest dst={};
  ObjAffineSet(&src,&dst,1,2);
  u32 tan=Div(dst.pb<<8,dst.pa);
  return tan;
}

void XSkew(u32 num,u16 angle){
  u32 tan=GetTan(angle);
  AP(num)->pa=256;
  AP(num)->pb=tan;
  AP(num)->pc=0;
  AP(num)->pd=256;
}

void YSkew(u32 num,u16 angle){
  u32 tan=GetTan(angle);
  AP(num)->pa=256;
  AP(num)->pb=0;
  AP(num)->pc=tan;
  AP(num)->pd=256;
}

u16 x=0;
u16 y=0;

void Update(){
  InputUpdate();
  TextSetCursor(0,0);
  TextPrintf("X=%5d\n"
	     "Y=%5d\n",x,y);
  
  /* Aボタンで変形をリセット */
  if(InputIsKeyPressed(KEY_A)){
    SpriteRotationScalingInit(0);
  }
  /* 十字キーで剪断 */
  if(InputIsKeyPressed(KEY_LEFT)){
    x+=5;
    XSkew(0,x);
  }
  if(InputIsKeyPressed(KEY_RIGHT)){
    x-=5;
    XSkew(0,x);
  }
  if(InputIsKeyPressed(KEY_UP)){
    y+=5;
    YSkew(0,y);
  }
  if(InputIsKeyPressed(KEY_DOWN)){
    y-=5;
    YSkew(0,y);
  }
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
