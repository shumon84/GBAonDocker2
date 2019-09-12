/* text.h */

#ifndef TEXT_H
#define TEXT_H

#include "gba.h"

void TextInit();
void TextClear();
void TextSetCursor(u32 c, u32 l);
void TextPut(char *str);
void TextPrintf(const char *format, ...);
void TextHide();
void TextShow();
void TextToggle();
void TextHideBackGround();
void TextShowBackGround();
void TextToggleBackGround();
#endif
