#include "receive.h"

extern EventQueue ReceiverEvents;

//This should be called Frequently from the main loop.
TXRX_DATA_TYPE Receiver(ReceiverData *RData)
{
    int index ;
    PulseWidthStatus PWidth ;
    uint16_t CurrentTime ;
    EventQueueEntry Current_Event ;
    TXRX_DATA_TYPE ReturnValue = 0;

    CurrentTime = MPE_RECEIVE_TIMER->R; //Get the approximate current time stamp.
    if ((CurrentTime - RData->LastEdgeTimeStamp) > MISSING_EDGE_TIMEOUT ){

        //Here we have had no transmissions in a while
        RData->CurrentRcvState = Initial_Expect_Rising ;
    }
    index = GetEvent() ;
    if (index != -1 ) {

        //Here we have an edge to deal with, -1 indicates no event in queue
        Current_Event.Edge = ReceiverEvents.Events[(uint16_t)index].Edge ;
        Current_Event.TimeStamp = ReceiverEvents.Events[(uint16_t)index].TimeStamp ;

        //Now insert receiver state machine here
        RData->LastEdgeTimeStamp = Current_Event.TimeStamp ; //This marks the last time that we got any time stamp at all
        switch (RData->CurrentRcvState){

        // Ignore transitions until we get a rising one.
        case Initial_Expect_Rising :
            if (Current_Event.Edge == Rising) {

                //Leading edge of initial lead-in bit
                RData->CurrentRcvState = Initial_Expect_Falling ;
                RData->RisingEdgeTimeStamp = Current_Event.TimeStamp ;
            }
            break ;
        case Initial_Expect_Falling :
            if (Current_Event.Edge == Rising){
                RData->CurrentRcvState = Initial_Expect_Rising ; //Out of sequence start over
            }
            else {
                RData->FallingEdgeTimeStamp = Current_Event.TimeStamp ;  //Figure out when it happens
                RData->PulseWidth = RData->FallingEdgeTimeStamp - RData->RisingEdgeTimeStamp ; // And Test validity
                PWidth = TestWidth(RData->PulseWidth) ;
                if (PWidth == Valid_FullBit) {

                    //Here we have a valid full initial bit
                    RData->CurrentRecvdData = 0 ; // Start all over for receiver
                    RData->MidBitTimeStamp = RData->FallingEdgeTimeStamp ; // By definition at mid bit....
                    RData->BitsLeftToGet = TXRX_DATA_TYPE_LENGTH ;
                    RData->CurrentRcvState = MidBit_Expect_Rising ; //Next bit is start of "real" data
                }
                else RData->CurrentRcvState = Initial_Expect_Rising ; //Likely a noise pulse, start over
            }
            break ;
        case MidBit_Expect_Falling :
            if (Current_Event.Edge == Rising) {

                //Out of sequence - start over
                RData->CurrentRcvState = Initial_Expect_Rising ;
            }
            else {
                RData->FallingEdgeTimeStamp = Current_Event.TimeStamp ;
                RData->PulseWidth = RData->FallingEdgeTimeStamp - RData->MidBitTimeStamp ; // Get width relative to last mid-bit
                PWidth = TestWidth(RData->PulseWidth) ;
                if (PWidth == Valid_HalfBit) {

                    //Here we have a half-bit, phasing transition
                    RData->CurrentRcvState = MidBit_Expect_Rising ; // Got to expect a rising edge at mid-bit
                }
                else {
                    if (PWidth == Valid_FullBit) {

                        // Rising Edge at mid-bit , clock in a 1
                        RData->CurrentRecvdData <<= 1 ; // Room for new bit
                        --RData->BitsLeftToGet ;
                        if (RData->BitsLeftToGet == 0){

                            //All done Start over
                            RData->LastValidReceived = RData->CurrentRecvdData ;
                            ReturnValue = RData->CurrentRecvdData;
                            RData->CurrentRcvState = Initial_Expect_Rising ;
                        }
                        else {
                            RData->MidBitTimeStamp = RData->FallingEdgeTimeStamp ; //New mark for mid-bit
                            RData->CurrentRcvState = MidBit_Expect_Rising    ; //And Expect a rising edge
                        }
                    }
                    else{
                        RData->CurrentRcvState = Initial_Expect_Rising ; // Bad pulse width
                    }
                }
            }
            break ;

            //We arrived here from a valid mid-bit transition previously
        case MidBit_Expect_Rising :
            if (Current_Event.Edge == Falling) {

                //Out of sequence - start over
                RData->CurrentRcvState = Initial_Expect_Rising ;
            }
            else {
                RData->RisingEdgeTimeStamp = Current_Event.TimeStamp ;
                RData->PulseWidth = RData->RisingEdgeTimeStamp - RData->MidBitTimeStamp ; // Get width relative to last mid-bit
                PWidth = TestWidth(RData->PulseWidth) ;
                if (PWidth == Valid_HalfBit) {

                    //Here we have a half-bit, phasing transition
                    RData->CurrentRcvState = MidBit_Expect_Falling ; // Got to expect a falling edge at mid-bit
                }
                else {
                    if (PWidth == Valid_FullBit) {

                        // Rising Edge at mid-bit , clock in a 1
                        RData->CurrentRecvdData <<= 1 ; // Room for new bit
                        RData->CurrentRecvdData |= 0x01 ;
                        --RData->BitsLeftToGet ;
                        if (RData->BitsLeftToGet == 0){

                            //All done Start over
                            RData->LastValidReceived = RData->CurrentRecvdData ;
                            ReturnValue = RData->CurrentRecvdData;
                            RData->CurrentRcvState = Initial_Expect_Rising ;
                        }
                        else {
                            RData->MidBitTimeStamp = RData->RisingEdgeTimeStamp ; //New mark for mid-bit
                            RData->CurrentRcvState = MidBit_Expect_Falling    ; //And Expect a falling edge
                        }
                    }
                    else{
                        RData->CurrentRcvState = Initial_Expect_Rising ; // Bad pulse width
                    }
                }
            }
            break ;
        default:
            RData->CurrentRcvState = Initial_Expect_Rising ;
            break ;
        }
    }
    return ReturnValue;
}

//This function places a new event in the event queue structure.
//The rising and falling edge detectors should call this handler with the appropriate info!
QueReturnErrors InsertEvent(EdgeType DetectedEdge, uint16_t CapturedTime)
{
    QueReturnErrors ReturnValue ;
    uint16_t putindex ;

    ReturnValue = NoError ;
    if (ReceiverEvents.QueueSize == SIZE_OF_RCV_QUE) {//Here Queue is already full
        ReturnValue = QueueFull ;
    }
    else {

        //Here we can insert a new event
        ++ReceiverEvents.QueueSize ;
        putindex = ReceiverEvents.PutIndex ;
        ReceiverEvents.Events[putindex].Edge = DetectedEdge ;
        ReceiverEvents.Events[putindex].TimeStamp = CapturedTime ;
        ++putindex ;

        // Modulo functionality: the AND operation will only work if SIZE_OF_RCV_QUE is a power of 2.
        // putindex &= SIZE_OF_RCV_QUE-1 ;
        if (putindex >= SIZE_OF_RCV_QUE) putindex = 0;

        ReceiverEvents.PutIndex = putindex ;
    }
    return ReturnValue ;
}

// This is called from within the main loop to see if there are any events on the queue, i.e. captured edges.
// Note that it disables interrupts to ensure that data is not overwritten by an interrupter
// if the return value is negative, then the queue is empty, else it returns an index to the oldest event in the
// queue
int GetEvent(void)
{

    int ReturnValue ;
    uint16_t getindex ;

    ReturnValue = -1 ;
    __asm ("  PUSH {CPSR}") ;
    __disable_interrupts();

    if (ReceiverEvents.QueueSize == 0) {

        //Nothing to be had!
        ReturnValue = -1 ;
    }
    else {
        getindex = ReceiverEvents.GetIndex ;
        ReturnValue = (int)(getindex) ;
        ++getindex ;

        // Modulo functionality: the AND operation will only work if SIZE_OF_RCV_QUE is a power of 2.
        // getindex &= SIZE_OF_RCV_QUE-1 ;
        if (getindex >= SIZE_OF_RCV_QUE) getindex = 0;

        ReceiverEvents.GetIndex = getindex ;
        --ReceiverEvents.QueueSize ;
    }

    __asm("  POP {CPSR}");
    __enable_interrupts();
    return ReturnValue ;
}

//This functions tests a current pulse width and determines if it is a valid width
PulseWidthStatus TestWidth(uint16_t CurrentPulse)
{
    PulseWidthStatus ReturnValue ;
    ReturnValue = Invalid_Width ;
    if ((CurrentPulse >= VALID_HALF_BIT_MIN) && (CurrentPulse <= VALID_HALF_BIT_MAX)) {
        ReturnValue = Valid_HalfBit ;
    }
    else {
        if ((CurrentPulse >= VALID_FULL_BIT_MIN) && (CurrentPulse <= VALID_FULL_BIT_MAX)){
            ReturnValue = Valid_FullBit ;
        }
    }
    return ReturnValue ;
}






