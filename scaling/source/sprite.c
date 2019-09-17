/* sprite.c */

#include"gba.h"
#include"sprite.h"

void AffineMerge(u32 num,ObjAffineDest *aff);
u32 Tan(u16 angle);

/**
 * @brief スプライトの初期化
 * 全てのスプライトを画面外に移動させるだけで
 * その他のフラグは初期化されない
 */
void SpriteInit(){
  for(u32 i=0;i<128;i++){
    SpriteSetCoordinate(i,SCREEN_WIDTH,SCREEN_HEIGHT);
  }
}

/**
 * @brief スプライトの座標を設定する
 * @param[in] num オブジェクト番号
 * @param[in] x   X軸の絶対座標
 * @param[in] y   Y軸の絶対座標
 */
void SpriteSetCoordinate(u32 num,u32 x,u32 y){
  SP(num)->X=x;
  SP(num)->Y=y;
}

/**
 * @brief スプライトの形状を設定
 * @param[in] num   オブジェクト番号
 * @param[in] shape 設定する形状
 */
void SpriteSetShape(u32 num,enum SPRITE_SHAPES shape){
  SP(num)->Shape=shape;
}

/**
 * @brief スプライトのサイズを設定
 * @param[in] num  オブジェクト番号
 * @param[in] size 設定するサイズ
 */
void SpriteSetSize(u32 num,enum SPRITE_SIZE size){
  SP(num)->Size=size;
}

/**
 * @brief スプライトのサイズと形状を設定
 * @param[in] num  オブジェクト番号
 * @param[in] form 設定するサイズと形状
 */
void SpriteSetForm(u32 num,enum SPRITE_SIZECODE form){
  SpriteSetShape(num,form>>2);
  SpriteSetSize(num,form&0x03);
}

/**
 * @brief スプライトにキャラクタを設定
 * @param[in] num       オブジェクト番号
 * @param[in] character 設定するキャラクタ番号
 */
void SpriteSetCharacter(u32 num,u32 character){
  SP(num)->Character=character;
}

/**
 * @brief スプライトのカラーパレットを設定
 * @param[in] num     オブジェクト番号
 * @param[in] palette 設定するパレット番号
 * paletteが負のとき、256色/1パレットモードを設定する
 */
void SpriteSetPalette(u32 num,s32 palette){
  if(palette<0){
    /* paletteが負のとき */
    SP(num)->ColorMode=Color_256x1;
  }
  else{
    /* paletteが正のとき */
    SP(num)->ColorMode=Color_16x16;
    SP(num)->Palette=palette;
  }
}

/**
 * @brief オブジェクトをセットアップ
 * @param[in] character 設定するキャラクタ番号
 * @param[in] palette   設定するパレット番号
 * @param[in] form      設定するサイズと形状
 * @param[in] x         設定するX軸の絶対座標
 * @param[in] y         設定するY軸の絶対座標
 * @return セットアップしたオブジェクトのオブジェクト番号
 */
u32 SpriteSetUp(u32 character,s32 palette,enum SPRITE_SIZECODE form,u32 x,u32 y){
  static u32 numOfSprites=0;
  SpriteSetCharacter(numOfSprites,character);
  SpriteSetPalette(numOfSprites,palette);
  SpriteSetForm(numOfSprites,form);
  SpriteSetCoordinate(numOfSprites,x,y);
  return numOfSprites++;
}

/**
 * @brief スプライトを移動させる
 * @param[in] num オブジェクト番号
 * @param[in] dx  X軸の変化量
 * @param[in] dy  Y軸の変化量
 */
void SpriteMove(u32 num,s32 dx,s32 dy){
  SP(num)->X+=dx;
  SP(num)->Y+=dy;
}

/**
 * @brief スプライトを水平方向に反転
 * @param[in] num オブジェクト番号
 */
void SpriteHFlip(u32 num){
  SP(num)->HFlip^=1;            /* HFlipのビットを反転 */
}

/**
 * @brief スプライトを垂直方向に反転
 * @param[in] num オブジェクト番号
 */
void SpriteVFlip(u32 num){
  SP(num)->VFlip^=1;            /* VFlipのビットを反転 */
}

/**
 * @brief スプライトの幅を取得
 * @param[in] num オブジェクト番号
 * @return オブジェクトnumの横幅
 */
u32 SpriteGetWidth(u32 num){
  enum SPRITE_SIZECODE form=SP(num)->Shape<<2|SP(num)->Size;
  u32 width=0;

  switch(form){
  case Sprite_8x8   :
  case Sprite_8x16  :
  case Sprite_8x32  : width=8; break;
  case Sprite_16x8  :
  case Sprite_16x16 :
  case Sprite_16x32 : width=16; break;
  case Sprite_32x8  :
  case Sprite_32x16 :
  case Sprite_32x32 :
  case Sprite_32x64 : width=32; break;
  case Sprite_64x32 :
  case Sprite_64x64 : width=64; break;
  }

  return width;
}

/**
 * @brief スプライトの高さを取得
 * @param[in] num オブジェクト番号
 * @return オブジェクトnumの横幅
 */
u32 SpriteGetHeight(u32 num){
  enum SPRITE_SIZECODE form=SP(num)->Shape<<2|SP(num)->Size;
  u32 height=0;

  switch(form){
  case Sprite_8x8   :
  case Sprite_16x8  :
  case Sprite_32x8  : height=8; break;
  case Sprite_8x16  :
  case Sprite_16x16 :
  case Sprite_32x16 : height=16; break;
  case Sprite_8x32  :
  case Sprite_16x32 :
  case Sprite_32x32 :
  case Sprite_64x32 : height=32; break;
  case Sprite_32x64 :
  case Sprite_64x64 : height=64; break;
  }

  return height;
}

/**
 * @brief X軸方向に画面に写っているか
 * @param[in] num オブジェクト番号
 * @return X軸方向に画面に写っているか
 */
u32 SpriteIsVisibleX(u32 num){
  return SP(num)->X<SCREEN_WIDTH-SpriteGetWidth(num);
}

/**
 * @brief Y軸方向に画面に写っているか
 * @param[in] num オブジェクト番号
 * @return Y軸方向に画面に写っているか
 */
u32 SpriteIsVisibleY(u32 num){
  return SP(num)->Y<SCREEN_HEIGHT-SpriteGetHeight(num);
}

/**
 * @brief 画面に写っているか
 * @param[in] num オブジェクト番号
 * @return 画面に写っているか
 */
u32 SpriteIsVisible(u32 num){
  return SpriteIsVisibleX(num)&&SpriteIsVisibleY(num);
}

/**
 * @brief スプライトを表示する
 * @param[in] num オブジェクト番号
 */
void SpriteShow(u32 num){
  SP(num)->Disable=0;
}

/**
 * @brief スプライトを非表示にする
 * @param[in] num オブジェクト番号
 */
void SpriteHide(u32 num){
  SP(num)->Disable=1;
}

/**
 * @brief スプライトの表示と非表示を切り替える
 * @param[in] num オブジェクト番号
 */
void SpriteToggle(u32 num){
  SP(num)->Disable^=1;		/* Disableのビットを反転 */
}

/**
 * @brief 回転格縮を許可する
 * @param[in] num オブジェクト番号
 * @param[in] affnum アフィンパラメータ番号
 */
void SpriteEnableRotationScaling(u32 num,u32 affnum){
  ASP(num)->RotationScaling=1;
  SpriteSetAffine(num,affnum);
  SpriteRotationScalingInit(num);
}

/**
 * @brief 回転格縮を許可しない
 * @param[in] num オブジェクト番号
 */
void SpriteDisableRotationScaling(u32 num){
  ASP(num)->RotationScaling=0;
}

/**
 * @brief 倍加表示を許可する
 * @param[in] num オブジェクト番号
 */
void SpriteEnableDoubleSize(u32 num){
  ASP(num)->DoubleSize=1;
}

/**
 * @brief 倍加表示を許可しない
 * @param[in] num オブジェクト番号
 */
void SpriteDisableDoubleSize(u32 num){
  ASP(num)->DoubleSize=0;
}

/**
 * @brief アフィンパラメータ番号をセットする
 * @param[in] num オブジェクト番号
 * @param[in] affnum アフィンパラメータ番号
 */
void SpriteSetAffine(u32 num,u32 affnum){
  ASP(num)->Affine=affnum;
}

/**
 * @brief 回転格縮を元に戻す
 * @param[in] num オブジェクト番号
 */
void SpriteRotationScalingInit(u32 num){
  u32 aff=ASP(num)->Affine;
  AP(aff)->pa=256;
  AP(aff)->pb=0;
  AP(aff)->pc=0;
  AP(aff)->pd=256;
}

/**
 * @brief オブジェクトを回転拡縮
 * @param[in] num オブジェクト番号
 * @param[in] xScale X軸方向の拡大比(百分率)
 * @param[in] yScale Y軸方向の拡大比(百分率)
 * @param[in] angle 反時計回りに回転させる角度(度数法)
 */
void SpriteRotationScaling(u32 num,s16 xScale,s16 yScale,u16 angle){
  ObjAffineSource src={
		       Div(25600,xScale), // ((xScale/100)^-1)<<8
		       Div(25600,yScale), // ((yScale/100)^-1)<<8
		       angle*180};        // angle<<8*(180<<8/0xFFFF)
  ObjAffineDest dst={};
  ObjAffineSet(&src,&dst,1,2);
  AffineMerge(num,&dst);
}

/**
 * @brief オブジェクトを回転
 * @param[in] num オブジェクト番号
 * @param[in] angle 反時計回りに回転させる角度(度数法)
 */
void SpriteRotation(u32 num,u16 angle){
  SpriteRotationScaling(num,100,100,angle);
}

/**
 * @brief オブジェクトを拡縮
 * @param[in] num オブジェクト番号
 * @param[in] xScale X軸方向の拡大比(百分率)
 * @param[in] yScale Y軸方向の拡大比(百分率)
 */
void SpriteScaling(u32 num,s16 xScale,s16 yScale){
  SpriteRotationScaling(num,xScale,yScale,0);
}

/**
 * @brief オブジェクトを水平剪断
 * @param[in] num オブジェクト番号
 * @param[in] angle 剪断させる角度(度数法)
 */
void SpriteXSkew(u32 num,u16 angle){
  u32 tan=Tan(angle);
  ObjAffineDest dst={};
  dst.pa=256;
  dst.pb=tan;
  dst.pc=0;
  dst.pd=256;
  AffineMerge(num,&dst);
}

/**
 * @brief オブジェクトを垂直剪断
 * @param[in] num オブジェクト番号
 * @param[in] angle 剪断させる角度(度数法)
 */
void SpriteYSkew(u32 num,u16 angle){
  u32 tan=Tan(angle);
  ObjAffineDest dst={};
  dst.pa=256;
  dst.pb=0;
  dst.pc=tan;
  dst.pd=256;
  AffineMerge(num,&dst);
}

/**
 * @brief 正接(tan)を求める
 * @param[in] angle 求めたい角度
 */
u32 Tan(u16 angle){
  ObjAffineSource src={
		       256,
		       256,
		       angle*180}; // angle<<8*(180<<8/0xFFFF)
  ObjAffineDest dst={};
  ObjAffineSet(&src,&dst,1,2);
  u32 tan=Div(dst.pb<<8,dst.pa);
  return tan;
}

/**
 * @brief オブジェクトの回転格縮を合成
 * @param[in] num オブジェクト番号
 * @param[in] ope 合成するアフィン
 * 以下の行列積を計算します
 * | aff.pa aff.pc 0 |   | ope.pa ope.pc 0 |
 + | aff.pb aff.pd 0 | x | ope.pb ope.pd 0 |
 * |      0      0 1 |   |      0      0 1 |
 */
void AffineMerge(u32 num,ObjAffineDest *ope){
  u32 aff=ASP(num)->Affine;
  u32 pa=AP(aff)->pa;
  u32 pb=AP(aff)->pb;
  u32 pc=AP(aff)->pc;
  u32 pd=AP(aff)->pd;

  AP(aff)->pa=(pa*ope->pa+pc*ope->pb)>>8;
  AP(aff)->pb=(pb*ope->pa+pd*ope->pb)>>8;
  AP(aff)->pc=(pa*ope->pc+pc*ope->pd)>>8;
  AP(aff)->pd=(pb*ope->pc+pd*ope->pd)>>8;
}
