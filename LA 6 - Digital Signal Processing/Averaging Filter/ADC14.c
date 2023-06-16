#include "ADC14.h"

void InitializeADC(void)
{
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;             // 1) ADC14ENC = 0 to allow programming
    while(ADC14->CTL0 & ADC14_CTL0_BUSY){};     // 2) wait for BUSY to be zero

    // 3) ADC14CTL0
    ADC14->CTL0 = (ADC14_CTL0_SHP | ADC14_CTL0_SSEL__SMCLK | ADC14_CTL0_SHT1__32 | ADC14_CTL0_SHT0__32 | ADC14_CTL0_ON);
    // 31-30 ADC14PDIV  predivider,            00b = Predivide by 1
    // 29-27 ADC14SHSx  SHM source            000b = ADC14SC bit
    // 26    ADC14SHP   SHM pulse-mode          1b = SAMPCON the sampling timer
    // 25    ADC14ISSH  invert sample-and-hold  0b = not inverted
    // 24-22 ADC14DIVx  clock divider         000b = /1
    // 21-19 ADC14SSELx clock source select   100b = SMCLK
    // 18-17 ADC14CONSEQx mode select          00b = Single-channel, single-conversion
    // 16    ADC14BUSY  ADC14 busy              0b (read only)
    // 15-12 ADC14SHT1x sample-and-hold time 0011b = 32 clocks
    // 11-8  ADC14SHT0x sample-and-hold time 0011b = 32 clocks
    // 7     ADC14MSC   multiple sample         0b = no multiple
    // 6-5   reserved                          00b (reserved)
    // 4     ADC14ON    ADC14 on                1b = powered up
    // 3-2   reserved                          00b (reserved)
    // 1     ADC14ENC   enable conversion       0b = ADC14 disabled
    // 0     ADC14SC    ADC14 start             0b = No start (yet)

    // 4) ADC14CTL1
    ADC14->CTL1 = (uint32_t) (
            ((ANALOG_INPUT_MEMORY << ADC14_CTL1_CSTARTADD_OFS) & ADC14_CTL1_CSTARTADD_MASK)
            | ADC14_CTL1_RES__14BIT);
    // 20-16 STARTADDx  start addr          xxxxxb
    // 15-6  reserved                  0000000000b (reserved)
    // 5-4   ADC14RES   ADC14 resolution       11b = 14 bit, 16 clocks
    // 3     ADC14DF    data read-back format   0b = Binary unsigned
    // 2     REFBURST   reference buffer burst  0b = reference on continuously
    // 1-0   ADC14PWRMD ADC power modes        00b = Regular power mode

    // 5) Memory control register
    ADC14->MCTL[ANALOG_INPUT_MEMORY] = (ADC14_MCTLN_EOS | ANALOG_INPUT_CHANNEL);
    // 15   ADC14WINCTH Window comp threshold   0b = not used
    // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
    // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
    // 12   reserved                            0b (reserved)
    // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
    // 7    ADC14EOS    End of sequence         0b = Not end of sequence
    // 6-5  reserved                           00b (reserved)
    // 4-0  ADC14INCHx  Input channel       xxxxxb

    // 6) no interrupts
    ADC14->IER0 = 0;
    ADC14->IER1 = 0;

    // 7) analog mode on port pin for analog input
    ANALOG_INPUT_PORT->SEL1 |= ANALOG_INPUT_MASK;
    ANALOG_INPUT_PORT->SEL0 |= ANALOG_INPUT_MASK;

    // 8) enable and start conversion
    ADC14->CTL0 |= (ADC14_CTL0_ENC | ADC14_CTL0_SC);
}
