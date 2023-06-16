#ifndef ADC14_H_
#define ADC14_H_

#include "msp.h"
#include "Defines.h"

#define ANALOG_INPUT_BIT        0
#define ANALOG_INPUT_MASK       (0x01 << ANALOG_INPUT_BIT)
#define ANALOG_INPUT_PORT       P6
#define ANALOG_INPUT_CHANNEL    ADC14_MCTLN_INCH_15
#define ANALOG_INPUT_MEMORY     0
#define ANALOG_INPUT_FLAG       ADC14_IFGR0_IFG0

void InitializeADC(void);

#endif /* ADC14_H_ */
