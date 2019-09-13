/* text.c */

#include<stdio.h>
#include<stdarg.h>
#include"gba.h"
#include"text.h"
#include"font.h"

#define COLUMN_MAX (256/8)  /* columnの最大値 */
#define LINE_MAX   (256/8)  /* lineの最大値 */
#define ASCII(c)   ((c)-' ')

u32 charBase=1;
u32 screenBase=0;
vu16* map;

u16 backGround;
u32 column=0;
u32 line=0;

/**
 * @brief 画面をクリアする
 * 画面を全て空白で埋めることでクリアする
 */
void TextClear(){
  for(int line=0;line<LINE_MAX;line++){
    for(int column=0;column<COLUMN_MAX;column++){
      map[line*COLUMN_MAX+column]=ASCII(' ');
    }
  }
}

/**
 * @brief テキストシステムの初期化
 * BG0にテキストシステムを構築する
 */
void TextInit(){
  vu16* oam=CHAR_BASE_BLOCK(charBase); /* キャラクタデータ */
  vu16* pal=BG_COLORS;                 /* パレットデータ */
  map=SCREEN_BASE_BLOCK(screenBase);   /* マップデータ*/

  SetMode(MODE_0|BG0_ON);

  REG_BG0CNT=
    BG_PRIORITY(0)|
    CHAR_BASE(charBase)|
    BG_256_COLOR|
    SCREEN_BASE(screenBase)|
    TEXTBG_SIZE_256x256;

  /* キャラクタデータの読み込み */
  for(u32 i=0;i<fontTilesLen/2;i++){
    oam[i]=fontTiles[i];
  }

  /* パレットデータの読み込み */
  for(u32 i=0;i<16;i++){
    pal[i]=fontPal[i];
  }
  backGround=fontPal[0];

  /* 背景をクリア */
  TextClear();
}

/**
 * @brief カーソルの位置を変更する
 *
 * @param[in] c 列
 * @param[in] l 行
 */
void TextSetCursor(u32 c, u32 l){
  column=c;
  line=l;
}

/**
 * @brief 1文字出力する
 *
 * @param[in] column 出力する列
 * @param[in] line 出力する行
 * @param[in] c 出力する文字のASCIIコード
 */
void printAscii(u32 column, u32 line, char c){
  /* line,columnが不正な場合は中止 */
  if(column>COLUMN_MAX || line>LINE_MAX) return;
  /* タイルに表示する文字を割り当てる */
  map[line*COLUMN_MAX+column]=ASCII(c);
}

/**
 * @brief 文字列を出力する
 * \t,\b,\r,\n以外の制御文字は空白に置換される
 *
 * @param[in] str 出力する文字列
 */
void TextPut(char *str){
  /* line,columnが不正な場合は中止 */
  if(column>COLUMN_MAX || line>LINE_MAX) return;

  /* スプライトに表示する文字を割り当てる */
  for(u32 i=0;str[i]!=0;i++){
    switch(str[i]){
    case '\t':
      /* スペース2つをタブがわりにする */
      printAscii(column,line,' ');
      printAscii(column+1,line,' ');
      column+=2;
      break;
    case '\b':
      if(column>0){
        column--;
      }
      break;
    case '\r':
      column=0;
      break;
    case '\n':
      line++;
      column=0;
      break;
    case ' ' ... '~':
      printAscii(column,line,str[i]);
      column++;
      break;
    default:
      /* 上記以外の制御文字がきたら、 */
      /* とりあえず空白を出力しておく */
      printAscii(column,line,' ');
      column++;
      break;
    }
    if(column>COLUMN_MAX){
      column=0;
      line++;
    }
    if(line>LINE_MAX){
      /* 一番下まで出力したら全部消す */
      TextClear();
    }
  }
}

/**
 * @brief printfと同じ書式指定で出力する
 */
void TextPrintf(const char *format, ...){
  char str[COLUMN_MAX*LINE_MAX]={};
  va_list va;
  va_start(va, format);
  vsnprintf(str,COLUMN_MAX*LINE_MAX,format,va);
  va_end(va);
  TextPut(str);
}

/**
 * @brief テキストシステムを非表示
 */
void TextHide(){
  REG_DISPCNT&=~(0x1<<8);
}

/**
 * @brief テキストシステムを表示
 */
void TextShow(){
  REG_DISPCNT|=0x1<<8;
}

/**
 * @brief テキストシステムの表示と非表示を切り替え
 */
void TextToggle(){
  REG_DISPCNT^=0x1<<8;
}

/**
 * @brief テキストシステムの背景を非表示
 */
void TextHideBackGround(){
  vu16* pal=BG_COLORS;
  pal[0]=0x0;
}

/**
 * @brief テキストシステムの背景を表示
 */
void TextShowBackGround(){
  vu16* pal=BG_COLORS;
  pal[0]=backGround;
}

/**
 * @brief テキストシステムの背景の表示と非表示を切り替え
 */
void TextToggleBackGround(){
  vu16* pal=BG_COLORS;
  pal[0]^=backGround;
}
