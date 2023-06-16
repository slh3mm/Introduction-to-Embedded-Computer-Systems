#ifndef MANCHESTERPHASEENCODINGPACKET_RECEIVE_H_
#define MANCHESTERPHASEENCODINGPACKET_RECEIVE_H_

#include "Configure.h"
#include "msp.h"
#include "CortexM.h"

//Receiver Definitions and declarations
enum CapturedEdge {Rising,Falling} ;  //these are the 2 types of edges in the received signal
typedef enum CapturedEdge EdgeType ;
typedef struct {
   EdgeType     Edge      ;    // Which type of edge was received
   uint16_t TimeStamp ;    // When we got it.
} EventQueueEntry ;

typedef struct {
   EventQueueEntry Events[SIZE_OF_RCV_QUE] ; //What each entry looks like
   uint16_t  QueueSize ;                   //Current size of queue
   uint16_t  GetIndex ;                 //Where we get data from
   uint16_t  PutIndex ;                 //Where we put new data
} EventQueue ;

enum QueueErrors {NoError,QueueFull,QueueEmpty} ;
typedef enum QueueErrors QueReturnErrors       ;

//The following typedefs are for the receiver section
typedef enum Rcv_States {Initial_Expect_Rising,Initial_Expect_Falling,MidBit_Expect_Rising,MidBit_Expect_Falling} ReceiverStates ;
typedef struct {
   ReceiverStates  	CurrentRcvState      ;   // State for state machine implementation
   uint16_t		RisingEdgeTimeStamp  ;   // Time stamp at leading edge of signal
   uint16_t		FallingEdgeTimeStamp ;   // Time stamp for falling edge
   uint16_t		PulseWidth           ;   // Difference in time between edges
   uint16_t		MidBitTimeStamp      ;   // Time Stamp of last valid mid-bit transition
   uint16_t		LastEdgeTimeStamp    ;   // When the last edge occurred, regardless of polarity
   TXRX_DATA_TYPE	CurrentRecvdData    ;    // Data that is being shifted in
   TXRX_DATA_TYPE   LastValidReceived;
   uint16_t		BitsLeftToGet        ;   // Number of bits to go in reception.
}ReceiverData;

typedef enum PulseWidths {Invalid_Width,Valid_HalfBit,Valid_FullBit} PulseWidthStatus ;

TXRX_DATA_TYPE Receiver(ReceiverData *RData);

//Receiver Queue functions:
//This function places a new event in the event queue structure.
//The rising and falling edge detectors should call this handler with the appropriate info!
QueReturnErrors InsertEvent(EdgeType DetectedEdge, uint16_t CapturedTime);

//This is called from within the main loop to see if there are any events on the que, i.e. captured edges.
//Note that it disables interrupts to ensure that data is not overwritten by an interrupter
//If no event is pending, it returns a -1, otherwise it returns an index into the queue that corresponds to the oldest
//event.
int GetEvent(void);

//This functions tests a current pulse width and determines if it is a valid width
PulseWidthStatus TestWidth(uint16_t CurrentPulse);

#endif /* MANCHESTERPHASEENCODINGPACKET_RECEIVE_H_ */
