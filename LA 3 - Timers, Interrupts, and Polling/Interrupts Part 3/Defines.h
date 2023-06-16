#ifndef DEFINES_H_
#define DEFINES_H_

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE

#define READ_PORT_PIN(PORT,MASK,BIT) ((PORT & MASK) >> BIT)
#define PUSHBUTTON_ACTIVE_STATE 0

#define NVIC_INTERRUPT_ENABLE(REG,BIT) NVIC->ISER[REG] |= 0x00000001 << BIT)
#define NVIC_INTERRUPT_DISABLE(REG,BIT) NVIC->ICER[REG] |= 0x00000001 << BIT)

#define CLOCK_FREQUENCY 48000000

#if (CLOCK_FREQUENCY == 48000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__1       // For 48 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__4       // For 12 MHz SMCLK
#define ONE_MICROSECOND_DELAY 48
#elif (CLOCK_FREQUENCY == 24000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__2       // For 24 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__4       // For 12 MHz SMCLK
#elif (CLOCK_FREQUENCY == 12000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__4       // For 12 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__4       // For 12 MHz SMCLK
#elif (CLOCK_FREQUENCY == 3000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__16      // For 3 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__16      // For 3 MHz SMCLK
#endif

#define ONE_MILLISECOND_DELAY (1000*ONE_MICROSECOND_DELAY)

#endif
