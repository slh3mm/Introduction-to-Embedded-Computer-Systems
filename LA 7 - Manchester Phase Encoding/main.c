#include <Configure.h>
#include "msp.h"
#include <stdio.h>
#include <stdbool.h>
#include "Clock.h"
#include "CortexM.h"
#include "LED.h"
#include "UART0.h"
#include "TransmitTimer.h"
#include "ReceiveTimerInputCapture.h"
#include "transmit.h"
#include "receive.h"


// Global variable
TransmitterData Xmit1;
ReceiverData Rcv1;
EventQueue ReceiverEvents;

// Function prototypes
void InitializeTransmitterReceiverVariables(TransmitterData *TData, ReceiverData *RData, EventQueue *EQ1);
void InitializeTransmitReceivePortPins(void);
void StartTimers(void);
void StopTimers(void);

#define BUFFER_SIZE 64
#define TRUE 1
#define FALSE 0

void main(void)
{
    bool ClockSystemError = false;
	uint32_t i;
	uint8_t CharacterEntered = (char) 0;
	char prompt_string[BUFFER_SIZE];
    TXRX_DATA_TYPE CurrentReceivedCharacter;
    TXRX_DATA_TYPE PreviousReceivedCharacter;

    DisableInterrupts();

    // Configure the clock system. If there is an error,
    // continue trying to initialize.
    ClockSystemError = InitializeClockSystem();
    while (ClockSystemError) {
        ClockSystemError = InitializeClockSystem();
    }

    // Initialize hardware and global variables.
	InitializeTransmitReceivePortPins();
	InitializeTransmitterReceiverVariables(&Xmit1,&Rcv1,&ReceiverEvents);
	UART0_Init();

	EnableInterrupts();

    // "Clear" the screen with a carriage return ('\r') and line feed ('\n'),
    // followed by the prompt string.
    for (i = 0; i < BUFFER_SIZE; i++) prompt_string[i] = (char) 0;
    sprintf(prompt_string,"\r\nEnter the character to transmit or @ to Exit: ");
    UART0_OutString(prompt_string);

    // Wait for a character to be entered.
    CharacterEntered = UART0_InChar();

    while (CharacterEntered != '@') {

		// Now, transmit the character and echo upon reception.
        for (i = 0; i < BUFFER_SIZE; i++) prompt_string[i] = (char) 0;
        sprintf(prompt_string,"\r\nTransmitting %c using MPE. ",(char) CharacterEntered);
        UART0_OutString(prompt_string);

#if (TRANSMIT_RECEIVE_STATUS == TRANSMIT_RECEIVE_ENABLED)

        // Transmit character followed by End Of Transmission (EOT) character.
        Xmit1.Transmit_Data_Buffer[0] = CharacterEntered;
        Xmit1.Transmit_Data_Buffer[1] = (char) EOT;

        StartTimers(); // Initiates transmission via MPE.

        // Call the receive function until the EOT character is received.
        CurrentReceivedCharacter = Receiver(&Rcv1);
        while (CurrentReceivedCharacter != EOT) {

            // If an actual character has been received (versus the Receiver() function returning zero)
            // then store the previous character that was entered.
            if (CurrentReceivedCharacter) PreviousReceivedCharacter = CurrentReceivedCharacter;
            CurrentReceivedCharacter = Receiver(&Rcv1);
        }

        StopTimers(); // Halts transmission

#else
        PreviousReceivedCharacter = CharacterEntered;
#endif
        // Finally, echo received character to the screen.
        for (i = 0; i < BUFFER_SIZE; i++) prompt_string[i] = (uint8_t) 0;
        sprintf(prompt_string,"\r\nReceived %c using MPE.\r\n",(char) PreviousReceivedCharacter);
        UART0_OutString(prompt_string);

        CharacterEntered = (char) 0;
        for (i = 0; i < BUFFER_SIZE; i++) prompt_string[i] = (char) 0;
        sprintf(prompt_string,"\r\nEnter the character to transmit or @ to Exit: ");
        UART0_OutString(prompt_string);

        // Wait for a character to be entered.
        CharacterEntered = UART0_InChar();
	}
    for (i = 0; i < BUFFER_SIZE; i++) prompt_string[i] = (char) 0;
    sprintf(prompt_string,"\r\nExiting...");
    UART0_OutString(prompt_string);

    while (TRUE);
}

void InitializeTransmitterReceiverVariables(TransmitterData *TData, ReceiverData *RData, EventQueue *EQ1)
{
    uint32_t i;

    // Initialize the transmitter/receiver global variables.

    // Transmitter begins in the start-bit state.
    TData->Transmit_Data = SOT   ;
    TData->Bits_Remaining = 2 ;
    TData->Transmitter_State = StartBit ;
    TData->Transmit_Clock_Phase = Low ;
    TData->InterwordTimeout = INTERWORD_DELAY;
    TData->Transmit_Data_Buffer_Index = 0;
    for (i = 0; i < TXRX_BUFFER_LENGTH; i++) TData->Transmit_Data_Buffer[i] = 0;

    EQ1->GetIndex = 0 ;
    EQ1->PutIndex = 0 ;
    EQ1->QueueSize   = 0 ;
    RData->CurrentRcvState = Initial_Expect_Rising ;
    RData->CurrentRecvdData =      0 ;
    RData->FallingEdgeTimeStamp =  0 ;
    RData->RisingEdgeTimeStamp =   0 ;
    RData->MidBitTimeStamp =       0 ;
    RData->PulseWidth =            0 ;
    RData->CurrentRecvdData =      0 ;
    RData->LastValidReceived = 0;
}

void InitializeTransmitReceivePortPins(void)
{
    // Note that the TX-DATA pin needs to be driven low, even in receiver mode.
    SET_TX_DATA_LOW;
    SET_TX_DATA_AS_AN_OUTPUT;

    // This pin is used to create a second TXD for loop self-test mode.
    SET_TX2_DATA_LOW;
    SET_TX2_DATA_AS_AN_OUTPUT;

    // Debug pin used to monitor Transmit_Clock_Phase field in TransmitterData structure
    // (see configure.h)
    SET_TRANSMIT_CLK_PHASE_PIN_AS_AN_OUTPUT;
    SET_TRANSMIT_CLK_PHASE_PIN_LOW;

    // For normal operation, only one of these would be selected. For self-test
    // (with no radio), we'll simply select as a transmitter.
#if (TRANSMIT_RECEIVE_MODE == TRANSMIT_OR_SELFTEST)
    ENABLE_TRANSMIT;
#else
    ENABLE_RECEIVE;
#endif
    SET_TR_AS_AN_OUTPUT;
}

void StartTimers(void)
{
	ReceiveTimerCaptureInitialization(&InsertEvent,&InsertEvent);
	TransmitTimerInitialization(&Transmitter,TACCR0_VALUE);
}

void StopTimers(void)
{
	TransmitTimerStop();
	ReceiveTimerStop();
}
