#ifndef TIMERA0_H
#define TIMERA0_H

#define TIMER_A0_PERIOD 46875
#define PORT2_BIT                      4
#define PORT2_MASK                     (0x01 << PORT2_BIT)
#define PORT2_PORT                     P2
#define PORT2_IN                       PORT2_PORT->IN
#define PORT2_INPUT                    PORT2_PORT->DIR &= ~PORT2_MASK
#define PORT2_SEL0                     PORT2_PORT->SEL0 &= ~PORT2_MASK
#define PORT2_SEL1                     PORT2_PORT->SEL1 |= PORT2_MASK

void InitializeTimerA0(void);
void TA0_0_IRQHandler(void);

#endif
