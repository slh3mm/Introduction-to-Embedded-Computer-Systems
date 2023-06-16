 #include "msp.h"
#include <stdbool.h>
#include "Defines.h"
#include "CortexM.h"
#include "Clock.h"
#include "LED.h"
#include "TaskScheduler.h"
#include "SPI.h"
#include "FSM.h"
#include "ADC14.h"

// Function prototypes
void Task1 (FSMType *FSM);
void Task2 (FSMType *FSM);
void Task3 (FSMType *FSM);

// Global variables
ADCType g_ADC_0;
DACType g_DAC_D;
FilterType y;
uint32_t CurrentInputSample = 0;
uint32_t CumulativeSum = 0;
uint32_t MovingAverage = 0;
uint32_t Previous = 0;

TaskType Tasks[NUMBER_OF_TASKS] =
{
/*
 * Task Table
 * Task     Task Counter    Task Execution Period   Parameter
 */
 {&Task1,  0,              1,                      (FSMType *) &g_ADC_0},
 {&Task2,  0,              1,                      (FSMType *) &y},
 {&Task3,  0,              1,                      (FSMType *) &g_DAC_D}
};

void main(void)
{
    bool ClockSystemError = false;

    // Stop the watchdog timer.
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    DisableInterrupts();

    // Configure the clock system. If there is an error,
    // continue trying to initialize.
    ClockSystemError = InitializeClockSystem();
    while (ClockSystemError) {
        ClockSystemError = InitializeClockSystem();
    }

    InitializeLEDPortPins();
    InitializeTaskSchedulerTimer();
    InitializeUSCIB0ForSPI();
    InitializeADC();
    InitializeDACPins();

    // Initialize filter variables.
    InitializeAnalogInput(&g_ADC_0,ANALOG_INPUT_CHANNEL,ANALOG_INPUT_MEMORY,0);
    InitializeDACObject(&g_DAC_D, DAC_D);
    y.ADC = &g_ADC_0;
    y.DAC = &g_DAC_D;

    EnableInterrupts(); // Must occur before using USCI for I2C


    // Infinite loop
    while (TRUE)
    {
        WaitForInterrupt();
    }
}

// Function to sample from ADC
void Task1 (FSMType *FSM)
{
    ADCType *LocalADC = (ADCType *) FSM;

    while(ADC14->CTL0 & ADC14_CTL0_BUSY){};             // wait for BUSY to be zero
    while((ADC14->IFGR0 & ANALOG_INPUT_FLAG) == 0){};   // wait for ADC14IFG

    // Read current sensor input value.
    CurrentInputSample = ADC14->MEM[(LocalADC->ADCMemoryIndex)];

    // start single conversion
    ADC14->CTL0 |= ADC14_CTL0_SC;
}

// This task represents the filtering function.
void Task2 (FSMType *FSM)
{
    FilterType *Local_y = (FilterType *) FSM;

//    // Filter: y = average(x).
//    CumulativeSum += CurrentInputSample - Local_y->ADC->InputBuffer[(Local_y->ADC->BufferIndex)];
//    Local_y->DAC->DACOutputValue = CumulativeSum >> INPUT_AVERAGE_SHIFT;

    //Filter: Low Pass
    Previous = Local_y->DAC->DACOutputValue;
    Local_y->ADC->InputBuffer[(Local_y->ADC->BufferIndex)] = (uint16_t) CurrentInputSample;
    Local_y->DAC->DACOutputValue = ((Local_y->ADC->InputBuffer[(Local_y->ADC->BufferIndex)]) + (7*Previous)) >> 3;

    // Update input buffer with current sensor input.
//    Local_y->ADC->InputBuffer[(Local_y->ADC->BufferIndex)] = (uint16_t) CurrentInputSample;

    // Update buffer index.
    Local_y->ADC->BufferIndex++;
    if (Local_y->ADC->BufferIndex >= INPUT_BUFFER_LENGTH) Local_y->ADC->BufferIndex = 0;
}

// Function to send filter output to DAC
void Task3 (FSMType *FSM)
{
    DACType *LocalDAC = (DACType *) FSM;
    uint16_t NewDACValue = LocalDAC->DACOutputValue;

    // 14-bit data needs to be left-justified for 14-bit DAC.
    NewDACValue = NewDACValue << SCALING_FACTOR;

    while (SPIBusy());
    ENABLE_DAC_CS;

    SPISendByte((unsigned char) (WRITE_TO_INPUT_REGISTER_N_UPDATE_ALL_N | LocalDAC->DACAddress));
    SPISendByte((unsigned char) (NewDACValue >> 8)); // Most Significant Byte
    SPISendByte((unsigned char) (NewDACValue));      // Least Significant Byte

    while (SPIBusy());
    DISABLE_DAC_CS;
}
