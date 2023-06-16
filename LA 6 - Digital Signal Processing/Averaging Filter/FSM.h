#ifndef FSM_H_
#define FSM_H_

#include "msp.h"
#include "SPI.h"

 // DAC Chip Select/Load Input
#define DAC_CS_BIT              1
#define DAC_CS_MASK             (0x01 << DAC_CS_BIT)
#define DAC_CS_PORT             P4
#define DAC_CS_OUT              DAC_CS_PORT->OUT
#define DAC_CS_DIR              DAC_CS_PORT->DIR
#define SET_DAC_CS_AS_AN_OUTPUT DAC_CS_DIR |= DAC_CS_MASK
#define DISABLE_DAC_CS          DAC_CS_OUT |= DAC_CS_MASK
#define ENABLE_DAC_CS           DAC_CS_OUT &= ~DAC_CS_MASK

// DAC Clear (Active Low)
#define DAC_CLEAR_BIT               3
#define DAC_CLEAR_MASK              (0x01 << DAC_CLEAR_BIT)
#define DAC_CLEAR_PORT              P4
#define DAC_CLEAR_OUT               DAC_CLEAR_PORT->OUT
#define DAC_CLEAR_DIR               DAC_CLEAR_PORT->DIR
#define SET_DAC_CLEAR_AS_AN_OUTPUT  DAC_CLEAR_DIR |= DAC_CLEAR_MASK
#define DISABLE_DAC_CLEAR           DAC_CLEAR_OUT |= DAC_CLEAR_MASK
#define ENABLE_DAC_CLEAR            DAC_CLEAR_OUT &= ~DAC_CLEAR_MASK

// Commands
#define WRITE_TO_INPUT_REGISTER_N               (0x00 << 4)
#define UPDATE_DAC_REGISTER_N                   (0x01 << 4)
#define WRITE_TO_INPUT_REGISTER_N_UPDATE_ALL_N  (0x02 << 4)
#define WRITE_TO_AND_UPDATE_N                   (0x03 << 4)
#define POWER_DOWN_N                            (0x04 << 4)
#define NO_OPERATION                            (0x0F << 4)

// Address (N)
#define DAC_A       0x00
#define DAC_B       0x01
#define DAC_C       0x02
#define DAC_D       0x03
#define ALL_DACS    0x0F

/*
 * The FSMType structure is the base for all derived FSM types, and would be used
 * (and possibly augmented) for information that is common to all FSM types.
 *
 * Regardless, for all derived FSM types, the first field must be of type FSMType
 * so that derived types can be cast as a pointer to FSMType so that the objects can
 * be passed to the tasks scheduled by the task scheduler (see Tasks.h).
 *
 */
typedef struct {
    uint32_t Value;
} FSMType;

// Associated with a DAC output
typedef struct {
    FSMType FSM;
    uint16_t DACAddress;
    uint16_t DACOutputValue;
} DACType;

// Associated with an analog input
typedef struct {
    FSMType FSM;
    uint16_t AnalogChannel;     // Analog channel associated with input
    uint16_t ADCMemoryIndex;    // ADC conversion memory index for (analog) input
    uint16_t BufferIndex;       // Input buffer index that points to most recent input value
    uint16_t InputBuffer[INPUT_BUFFER_LENGTH];
} ADCType;

// This type represents the filter processing from ACD to DAC.
typedef struct {
    FSMType FSM;
    ADCType *ADC;
    DACType *DAC;
} FilterType;

// Function Prototypes
void InitializeFSM(FSMType *FSM, uint16_t Value);
void InitializeDACPins(void);
void InitializeDACObject(DACType *Dac, uint16_t DACAddress);
void InitializeAnalogInput(ADCType *AnalogInput,uint16_t AnalogChannel,uint16_t ADCMemoryIndex,
                              uint16_t SensorID);

#endif /* FSM_H_ */

