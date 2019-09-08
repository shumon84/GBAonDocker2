#ifndef TIMER_H_
#define TIMER_H_

#include "gba.h"

#define REG_TMXCNT(x)   *(&REG_TM0CNT+x)
#define REG_TMXCNT_L(x) *(vu16*)(&REG_TMXCNT(x))
#define REG_TMXCNT_H(x) *(((vu16*)(&REG_TMXCNT(x)))+1)

#define TIMER_FREQ_PER_1    0
#define TIMER_FREQ_PER_64   1
#define TIMER_FREQ_PER_256  2
#define TIMER_FREQ_PER_1024 3

#define TIMER_CASCADE   BIT(2)
#define TIMER_INTERRUPT BIT(6)
#define TIMER_START     BIT(7)

bool TimerStart(u32 num);
bool TimerStop(u32 num);
bool TimerCascade(u32 num);
bool TimerSetup(u32 num,u16 cnt);
u32 TimerGet(u32 num);
bool TimerSetCount(u32 num,u32 count);
bool TimerReset(u32 num);
void TimerUpdate(void);
#endif
