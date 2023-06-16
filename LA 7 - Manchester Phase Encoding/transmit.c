#include "transmit.h"

//This routine manages the actual transmitter and is called every 1/2 period by a periodic interrupt.
void Transmitter(TransmitterData* TData)
{
    enum XmitClockPhase Phase;
    unsigned long  BitValueToTransmit ;

    /* Each half bit period constitutes a separate clock "phase" for transmitter
     * purposes. For option B, bit transitions occur on the "falling edge" of the transmitter
     * clock, where a 1 is transmitted with a LOW->HIGH transition on TX_DATA, and a 0 is
     * transmitted with a HIGH->LOW transition on TX_DATA. Thus, when transmitting a logic 1,
     * TX_DATA = 0 when Phase = High followed by TX_DATA = 1 when Phase = Low.  Conversely,
     * when transmitting a logic 0, TX_DATA = 1 when Phase = High followed by TX_DATA = 0
     * when Phase = Low.
     *
     *    Phase = High
     *       ---------
     *               |
     *               |
     *               | Phase = Low
     *               -------------
     *
     */
    if (TData->Transmit_Clock_Phase == Low) {
        TData->Transmit_Clock_Phase = High;
        SET_TRANSMIT_CLK_PHASE_PIN_HIGH;
    }
    else {
        TData->Transmit_Clock_Phase = Low;
        SET_TRANSMIT_CLK_PHASE_PIN_LOW;
    }
    Phase = TData->Transmit_Clock_Phase;

    // Mask off the bit to be transmitted, either MSB or LSB.
    BitValueToTransmit = TData->Transmit_Data & BIT_TO_BE_TRANSMITTED;

    switch(TData->Transmitter_State) {

        // This state is used to send the equivalent of a 1-0 preamble to start the receiver in the right state.
        case StartBit :
            switch(Phase) {
                case Low :

                    // Transmit the current MSB, which represents the next bit to be transmitted.
                    if (BitValueToTransmit != 0) {SET_TX_DATA_HIGH; SET_TX2_DATA_HIGH;}
                    else {SET_TX_DATA_LOW; SET_TX2_DATA_LOW;}

                    // Update the MSB.
                    TData->Transmit_Data <<= 1 ;

                    // If the 1-0 start bit sequence is completed (because the number of bits
                    // remaining equals 0), prepare to begin normal transmission.
                    if (--TData->Bits_Remaining == 0) {

                        // Transfer the buffered transmission data to Transmit_Data, update
                        // the value of Bits_Remaining, and the next-state value.
                        TData->Transmit_Data  = TData->Transmit_Data_Buffer[TData->Transmit_Data_Buffer_Index];
                        TData->Bits_Remaining = TXRX_DATA_TYPE_LENGTH ;
                        TData->Transmitter_State = NormalXmit ;
                    }
                break ;
                case High :

                    // Transmit the complemented value of the bit to be transmitted, which
                    // produces the correct edge transition for the transmitted bit.
                    if (BitValueToTransmit != 0) {SET_TX_DATA_LOW; SET_TX2_DATA_LOW;}
                    else {SET_TX_DATA_HIGH; SET_TX2_DATA_HIGH;}
                break ;
            }
        break ;

        case NormalXmit :
            switch(Phase) {
                case Low :

                    // Transmit the current MSB, which represents the next bit to be transmitted.
                    if (BitValueToTransmit != 0) {SET_TX_DATA_HIGH; SET_TX2_DATA_HIGH;}
                    else {SET_TX_DATA_LOW; SET_TX2_DATA_LOW;}

                    // Update the MSB.
                    TData->Transmit_Data <<= 1 ;

                    // If all of the bits have been transmitted, prepare to transition to the
                    // InterWord state.
                    if (--TData->Bits_Remaining == 0) {
                        TData->InterwordTimeout = INTERWORD_DELAY ;
                        TData->Transmitter_State = InterWord ;
                    }
                break ;

                // Transmit the complemented value of the bit to be transmitted, which
                // produces the correct edge transition for the transmitted bit.
                case High :
                    if (BitValueToTransmit != 0) {SET_TX_DATA_LOW; SET_TX2_DATA_LOW;}
                    else {SET_TX_DATA_HIGH; SET_TX2_DATA_HIGH;}
                break ;
            }

        break ;

        case InterWord :
            switch(Phase) {
                case Low :
                    SET_TX_DATA_LOW;
                    if (--TData->InterwordTimeout == 0){
                        ReInitializeXmitter(TData) ;
                    }

                break ;
                case High :
                    SET_TX_DATA_LOW;
                break ;
            }

        break ;
        default :
            ReInitializeXmitter(TData) ;
            //Other initialization here.....
        break ;

    }
}

//This function re-initializes the transmitter section
void ReInitializeXmitter(TransmitterData * TData)
{
    // Enter the start bit state to send a 1-0 sequence which guarantees a valid full bit to start.
    // This will be a binary 1-0 for the startup.
    TData->Transmit_Data = SOT   ;
    TData->Bits_Remaining = 2 ;
    TData->Transmitter_State = StartBit ;
    TData->Transmit_Clock_Phase = Low ;
    TData->Transmit_Data_Buffer_Index++;
    if (TData->Transmit_Data_Buffer_Index == TXRX_BUFFER_LENGTH) {
        TData->Transmit_Data_Buffer_Index = 0;
    }
}

