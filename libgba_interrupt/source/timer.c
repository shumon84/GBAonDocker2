#include "gba.h"
#include "timer.h"

u32 DefaultTimerCount[4];
u32 CurrentTimerCount[4];

/**
 * @brief タイマを有効にする
 *
 * @param[in] num 有効にするタイマ番号
 * @return numが不正なときtrue,それ以外のときfalse
 *
 * タイマの設定は事前にしておく
 */
bool TimerStart(u32 num){
  if(num>3){
    return true;
  }
  REG_TMXCNT_H(num)|=TIMER_START;
  return false;
}

/**
 * @brief タイマを無効にする
 *
 * @param[in] num 有効にするタイマ番号
 * @return numが不正なときtrue,それ以外のときfalse
 *
 * 設定値はそのまま残る
 */
bool TimerStop(u32 num){
  if(num>3){
    return true;
  }
  REG_TMXCNT_H(num)&=~TIMER_START;
  return false;
}

/**
 * @brief タイマのカスケード機能を有効にする
 *
 * @param[in] num 有効にするタイマ番号
 * @return numが不正なときtrue,それ以外のときfalse
 *
 * タイマの起動はされない
 */
bool TimerCascade(u32 num){
  if(num>3){
    return true;
  }
  REG_TMXCNT_H(num)|=TIMER_CASCADE;
  return false;
}

/**
 * @brief タイマの設定をする
 *
 * @param[in] num 有効にするタイマ番号
 * @param[in] cnt タイマの設定値
 * @return numが不正なときtrue,それ以外のときfalse
 *
 * cntを設定する。元の設定値は上書きされる。
 */
bool TimerSetup(u32 num,u16 cnt){
  if(num>3){
    return true;
  }
  REG_TMXCNT_H(num)=cnt;
  return false;
}

/**
 * @brief タイマの値を取得する
 *
 * @param[in] num 値を取得するタイマ番号
 * @return numが不正なとき0,それ以外のときタイマnの現在の値
 */
u32 TimerGet(u32 num){
  if(num>3){
    return 0;
  }
  return CurrentTimerCount[num];
}

/**
 * @brief タイマのデフォルト値を設定する
 *
 * @param[in] num デフォルト値を設定するタイマ番号
 * @param[in] count 設定したいデフォルト値
 * @return numが不正なときtrue,それ以外のときfalse
 */
bool TimerSetCount(u32 num, u32 count){
  if(num>3){
    return true;
  }
  DefaultTimerCount[num]=count;
  REG_TMXCNT_L(num)=count;
  return false;
}

/**
 * @brief タイマをリセットする
 *
 * @param[in] num リセットするタイマ番号
 * @return numが不正なときtrue,それ以外のときfalse
 */
bool TimerReset(u32 num){
  if(num>3){
    return true;
  }
  REG_TMXCNT_L(num)=DefaultTimerCount[num];
  return false;
}

/**
 * @brief タイマの値を更新する
 *
 * main()から毎ループ呼び出してタイマを更新する
 */
void TimerUpdate(void){
  int i;
  for(i=0;i<4;i++){
    CurrentTimerCount[i]=REG_TMXCNT_L(i);
  }
}
