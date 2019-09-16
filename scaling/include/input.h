/* input.h */

#ifndef INPUT_H
#define INPUT_H

#include"gba.h"

#define IS_PRESSED(input,key) ((~(input))&(key))

/* キーステートを表す列挙体 */
typedef enum KEYSTATE{
  NOT_PUSHED,      /* 放されているとき */
  PRESSED,         /* 押したとき */
  RELEASED,        /* 放したとき */
  REPEAT,          /* 押されているとき */
} KEYSTATE;

/* 十字キーの方向を表す列挙体 */
typedef enum DIRECTION{
  NEUTRAL,         /* 何も押していない */
  DIR_N,           /* 上 */
  DIR_NW,          /* 右上 */
  DIR_W,           /* 右 */
  DIR_SW,          /* 右下 */
  DIR_S,           /* 下 */
  DIR_SE,          /* 左下 */
  DIR_E,           /* 左 */
  DIR_NE,          /* 左上 */
} DIRECTION;

void InputUpdate();
KEYSTATE InputGetKeyState(KEYPAD_BITS key);
u32 InputIsKeyNotPushed(KEYPAD_BITS key);
u32 InputIsKeyPressed(KEYPAD_BITS key);
u32 InputIsKeyReleased(KEYPAD_BITS key);
u32 InputIsKeyRepeat(KEYPAD_BITS key);
DIRECTION InputGetDirection();
#endif
