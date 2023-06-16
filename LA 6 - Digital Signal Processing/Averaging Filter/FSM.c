# include "FSM.h"

void InitializeFSM(FSMType *FSM, uint16_t Value)
{
    FSM->Value = Value;
}

/*
 * InitializeDACPins
 *
 * Set up the direction registers and initial states for the output pins used to interface with the DAC
 *
 * No arguments
 *
 * Return type: Void
 */
void InitializeDACPins(void)
{
    DISABLE_DAC_CS;
    SET_DAC_CS_AS_AN_OUTPUT;

    DISABLE_DAC_CLEAR;
    SET_DAC_CLEAR_AS_AN_OUTPUT;

}

/*
 * InitializeDACObject
 *
 * Define the initial values for the DAC struct, as demonstrated in main.c.
 *
 * Arguments:
 *  - DAC *Dac:                 Pointer to DAC struct in use
 *  - int DACNum:               Identifier for DAC output A through D
 *  - unsigned int * ArrayPtr   Pointer to the first element of sine table (array) associated with DAC
 *  - int ArrayLength           Variable for length of sine table associated with DAC
 *
 *  Return type: void
 */
void InitializeDACObject(DACType *Dac, uint16_t DACAddress)
{
    Dac->DACAddress = DACAddress;
    Dac->DACOutputValue = 0;
}

void InitializeAnalogInput(ADCType *AnalogInput,uint16_t AnalogChannel,uint16_t ADCMemoryIndex,
                              uint16_t SensorID)
{
    InitializeFSM((FSMType *) AnalogInput,SensorID);

    AnalogInput->AnalogChannel = AnalogChannel;
    AnalogInput->ADCMemoryIndex = ADCMemoryIndex;
    AnalogInput->BufferIndex = 0;
    for (uint32_t i = 0; i < INPUT_BUFFER_LENGTH; i++) AnalogInput->InputBuffer[i] = 0;
}
