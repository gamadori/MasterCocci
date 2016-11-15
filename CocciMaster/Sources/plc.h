/*
 * plc.h
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */

#ifndef PLC_H_
#define PLC_H_
enum StatusIO
{
	stIONull,
	stCmdOutput,
	stWaitTrasmit,
	stCmdInput,
	stWaitReceve
};

#define MAX_INPUTS		24

#define NUM_ANALOGIC	8

#define NUM_OUTPUT		16

#define NUM_INPUTS		8




#define NUM_INPUTS_BLOCK	3

#define NUM_OUTPUTS_BLOCK	2


#define inpStartFiltered			((filteredInpBuffer[0] & Bits[0]) == Bits[0])

#define inpPwrOnFiltered			((filteredInpBuffer[0] & Bits[7]) == Bits[7])

		
#define inpDipSwitchMode			((inputBuffer[0] & Bits[0]) == Bits[0])

#define inpDipSwitchStation			((inputBuffer[0] & 0x1E) >> 1) 

#define inpAxeSelected				(((inputBuffer[1] >> 6) | (inputBuffer[2] << 2)) & 0x3FF)	//(~((((inputBuffer[1] >> 6) | (inputBuffer[2] << 2)) & 0x3FF) | 0xFC00))



#define outSpeed1(value)	(value) ? (outValue[0] |= Bits[0]) : (outValue[0] &= ~Bits[0])

#define outSpeed2(value)	(value) ? (outValue[0] |= Bits[1]) : (outValue[0] &= ~Bits[1])

#define outSpeed3(value)	(value) ? (outValue[0] |= Bits[2]) : (outValue[0] &= ~Bits[2])

#define outSpeed4(value)	(value) ? (outValue[0] |= Bits[3]) : (outValue[0] &= ~Bits[3])



extern word ADCValues[];


extern word inputBuffer[];

extern word inpValue[];

extern word inpFrz[];

extern word inpFrzValue[];


extern word outputBuffer[];

extern word outValue[];

extern word outFrz[];

extern word outFrzValue[];

extern short stIO;

extern short ioTrasmitCompleted;

extern short ioReceveCompleted;

extern word filterVal[NUM_INPUTS_BLOCK][NUM_INPUTS];

extern word filteredInpBuffer[NUM_INPUTS_BLOCK];

extern bool ioWorked;

extern word busInputs[];

extern word busOutputs[];

void InitPlc();

bool IOServer();

void AnalogicInit();

void AnalogicServer();

void FilterInputs();

void SetFilter(byte id, word filter);

void SetDevice(short device, byte state);

#endif /* PLC_H_ */
