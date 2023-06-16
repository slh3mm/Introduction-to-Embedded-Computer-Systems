#ifndef DEFINES_H_
#define DEFINES_H_

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE

#define READ_PORT_PIN(PORT,MASK,BIT) ((PORT & MASK) >> BIT) // Returns port pin value of 0 or 1.
#define PUSHBUTTON_ACTIVE_STATE 0 // Pushbutton is active-low

#define NVIC_INTERRUPT_ENABLE(REG,BIT)   NVIC->ISER[REG] |= (0x00000001 << BIT)
#define NVIC_INTERRUPT_DISABLE(REG,BIT)  NVIC->ICER[REG] |= (0x00000001 << BIT)
#define INTERRUPT_PRIORITY(PR)           (PR << 5)
#define PRIORITY_REGISTER(INT,PR)        NVIC->IP[(INT)] = ((NVIC->IP[(INT)]&0x00)|PR)

#define CLOCK_FREQUENCY 48000000

#if (CLOCK_FREQUENCY == 48000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__1       // For 48 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__4       // For 12 MHz SMCLK
#define ONE_MICROSECOND_DELAY 48
#define I2C_PRESCALE 240                   // 50 kHz = SMCLK/I2C_PRESCALE
#elif (CLOCK_FREQUENCY == 24000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__2       // For 24 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__4       // For 12 MHz SMCLK
#define ONE_MICROSECOND_DELAY 24
#define I2C_PRESCALE 240                   // 50 kHz = SMCLK/I2C_PRESCALE
#elif (CLOCK_FREQUENCY == 12000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__4       // For 12 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__4       // For 12 MHz SMCLK
#define ONE_MICROSECOND_DELAY 12
#define I2C_PRESCALE 240                   // 50 kHz = SMCLK/I2C_PRESCALE
#elif (CLOCK_FREQUENCY == 3000000)
#define MCLK_DIVIDER  CS_CTL1_DIVM__16      // For 3 MHz MCLK
#define SMCLK_DIVIDER CS_CTL1_DIVS__16      // For 3 MHz SMCLK
#define ONE_MICROSECOND_DELAY 3
#define I2C_PRESCALE 60                     // 50 kHz = SMCLK/I2C_PRESCALE
#endif

#define ONE_MILLISECOND_DELAY (1000*ONE_MICROSECOND_DELAY)

// Used to filter the input buffer. Note that INPUT_BUFFER_LENGTH = 2^INPUT_AVERAGE_SHIFT
#define INPUT_AVERAGE_SHIFT  6
#define INPUT_BUFFER_LENGTH  64
#define Fs 3840 // unit = Hertz
#define TASK_SCHEDULER_PERIOD 12500 // Equals (CLOCK_FREQUENCY/Fs), where Fs is sampling frequency

#define SCALING_FACTOR 2

#define NUMBER_OF_TASKS 3
#define TASK_SCHEDULER_PRIORITY 0

#endif
