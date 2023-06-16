#include "CortexM.h"

//*********** DisableInterrupts ***************
// disable interrupts
// inputs:  none
// outputs: none
void DisableInterrupts(void){
  __asm ("    CPSID  I\n"
         "    BX     LR\n");
}

//*********** EnableInterrupts ***************
// emable interrupts
// inputs:  none
// outputs: none
void EnableInterrupts(void){
  __asm  ("    CPSIE  I\n"
          "    BX     LR\n");
}

//*********** WaitForInterrupt ************************
// go to low power mode while waiting for the next interrupt
// inputs:  none
// outputs: none
void WaitForInterrupt(void){
  __asm  ("    WFI\n"
          "    BX     LR\n");
}
