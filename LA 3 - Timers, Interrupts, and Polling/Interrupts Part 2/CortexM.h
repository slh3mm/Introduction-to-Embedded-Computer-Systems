#ifndef CORTEXM_H
#define CORTEXM_H

#include <stdint.h>
/**
 * Disables Interrupts
 *
 * @param  none
 * @return none
 *
 * @brief  Sets the I bit in the PRIMASK to disable interrupts.
 */
void DisableInterrupts(void); // Disable interrupts

/**
 * Enables Interrupts
 *
 * @param  none
 * @return none
 *
 * @brief  clears the I bit in the PRIMASK to enable interrupts
 */
void EnableInterrupts(void);  // Enable interrupts

/**
 * Enters low power sleep mode waiting for interrupt (WFI instruction)
 * processor sleeps until next hardware interrupt
 * returns after ISR has been run
 *
 * @param  none
 * @return none
 *
 * @brief  Enters low power sleep mode waiting for interrupt
 */
void WaitForInterrupt(void);

#endif
