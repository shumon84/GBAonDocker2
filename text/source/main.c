/* main.c */

#include "gba.h"
#include "text.h"

int main(void){
  TextInit();
  TextSetCursor(9,10);
  TextPut("Hello,World!");
  while(1){}
}
