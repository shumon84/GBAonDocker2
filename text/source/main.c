/* main.c */

#include "gba.h"
#include "text.h"

int main(void){
  TextInit();
  for(int i=0;i<5;i++){
    TextPrintf("%d\n",i);
  }
  TextSetCursor(9,10);
  TextPut("Hello,World!\n");
  while(1){}
}
