#include "Clock.h"

/*********************************************************************
 *
 * Description: Proper device configuration to enable operation at
 * MCLK, HMSMCLK = 48MHz and SMCLK = 12 MHz using the HFXT oscillator
 * as the clock source.
 * @param none
 * @return boolean
 *
 * 1. Configure VCORE level to 1
 * 2. Configure flash wait-state to 1
 * 3. Configure HFXT source is 48MHz XTAL
 * 4. Source MCLK & HSMCLK from HFXT
 *
 * NOTE: This assumes the default power state is AM0_LDO.
 */
bool InitializeClockSystem(void)
{
    uint32_t currentPowerState;
    bool Error = false;

    // Step 1: Transition to VCORE Level 1: AM0_LDO --> AM1_LDO

    // Get current power state, if it's not AM0_LDO, error out
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;
    if (currentPowerState != PCM_CTL0_CPM_0) {
        Error = true;
    }
    else {
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

        if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG) {

            // Error if transition was not successful
            Error = true;
        }
        else {
            if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1) {

                // Error if device is not in AM1_LDO mode
                Error = true;
            }
            else {

                // Step 2: Configure Flash wait-state to 1 for both banks 0 & 1
                FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
                        FLCTL_BANK0_RDCTL_WAIT_1;
                FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
                        FLCTL_BANK1_RDCTL_WAIT_1 ;

                // Step 3: Configure HFXT to use 48MHz crystal, source to MCLK & HSMCLK
                PJ->SEL0 |= (BIT2 | BIT3);                // Configure PJ.2/3 for HFXT function
                PJ->SEL1 &= ~(BIT2 | BIT3);

                CS->KEY = CS_KEY_VAL;                  // Unlock CS module for register access
                CS->CTL2 |= (CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE);
                while (CS->IFG & CS_IFG_HFXTIFG)
                    CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;

                // Select MCLK source = HFXT, MCLK divider = 1
                CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_DIVM_MASK | CS_CTL1_SELM_MASK)) |
                        (MCLK_DIVIDER | CS_CTL1_SELM__HFXTCLK);

                // Select HSMCLK/SMCLK source = HFXT, HSMCLK divider = 1, SMCLK divider = 4
                CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_DIVS_MASK | CS_CTL1_DIVHS_MASK | CS_CTL1_SELS_MASK)) |
                        (SMCLK_DIVIDER | CS_CTL1_DIVHS__1 | CS_CTL1_SELS__HFXTCLK);

                CS->KEY = 0;                            // Lock CS module from unintended accesses
            }
        }
    }

    return Error;
}
