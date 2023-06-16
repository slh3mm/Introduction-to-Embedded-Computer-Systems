/*
 * transmit.h
 *
 *  Created on: Nov 16, 2015
 *      Author: Todd DeLong
 */

#ifndef MANCHESTERPHASEENCODINGPACKET_TRANSMIT_H_
#define MANCHESTERPHASEENCODINGPACKET_TRANSMIT_H_

#include "Configure.h"
#include "msp.h"

/***************************************************************************************************
***** Typedef Section
***************************************************************************************************/
enum Transmit_States {StartBit,NormalXmit,InterWord} ;
enum XmitClockPhase  {High,Low} ;
typedef struct {
	TXRX_DATA_TYPE      	Transmit_Data;  						// Current data being transmitted.
	TXRX_DATA_TYPE      	Transmit_Data_Buffer[TXRX_BUFFER_LENGTH];	// Data packet to be transmitted.
	unsigned short			Transmit_Data_Buffer_Index;
	unsigned int        	Bits_Remaining        ;  //This is the number of bits remaining in the current transmission
	enum XmitClockPhase 	Transmit_Clock_Phase  ;  //This gets updated once every 1/2 bit period (500 uS in this case.)
	unsigned int        	InterwordTimeout      ;  //This represents a "dead" period between successive transmissions
	enum Transmit_States	Transmitter_State     ;  //This is the current state machine state for the transmitter
	unsigned long       	StartBitsRemaining ;
} TransmitterData ;

void Transmitter(TransmitterData * TData) ; //This routine is called every 500 uS by an interrupt handler.
void ReInitializeXmitter(TransmitterData * TData);

#endif /* MANCHESTERPHASEENCODINGPACKET_TRANSMIT_H_ */
