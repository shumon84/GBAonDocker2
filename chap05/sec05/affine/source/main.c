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

/* 回転拡縮モード */
/* 方向キーで拡縮 */
/* LRで回転       */
void rotationScalingMode(){
  if(InputIsKeyPressed(KEY_LEFT)){
    SpriteScaling(0,110,100);
  }
  if(InputIsKeyPressed(KEY_RIGHT)){
    SpriteScaling(0,90,100);
  }
  if(InputIsKeyPressed(KEY_UP)){
    SpriteScaling(0,100,110);
  }
  if(InputIsKeyPressed(KEY_DOWN)){
    SpriteScaling(0,100,90);
  }
  if(InputIsKeyPressed(KEY_L)){
    SpriteRotation(0,10);
  }
  if(InputIsKeyPressed(KEY_R)){
    SpriteRotation(0,-10);
  }
}

/* 剪断変形モード */
/* 方向キーで剪断 */
void skewMode(){
  if(InputIsKeyPressed(KEY_LEFT)){
    SpriteXSkew(0,10);
  }
  if(InputIsKeyPressed(KEY_RIGHT)){
    SpriteXSkew(0,-10);
  }
  if(InputIsKeyPressed(KEY_UP)){
    SpriteYSkew(0,10);
  }
  if(InputIsKeyPressed(KEY_DOWN)){
    SpriteYSkew(0,-10);
  }
}

u16 mode=0;

void Update(){
  InputUpdate();
  TextSetCursor(0,0);
  /* Aボタンで変形をリセット */
  if(InputIsKeyPressed(KEY_A)){
    SpriteRotationScalingInit(0);
  }
  /* Bボタンでモード切り替え */
  if(InputIsKeyPressed(KEY_B)){
    mode^=1;
  }
  /* 各モードの処理を実行 */
  if(mode){
    TextPut("ROTATION SCALING MODE");
    rotationScalingMode();
  }else{
    TextPut("SKEW MODE            ");
    skewMode();
  }
}

int main(void){
  Init();

  while(1){
    Update();
    WaitForVsync();
  }
}
