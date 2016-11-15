/*
 * Plc.c
 *
 *  Created on: Dec 7, 2014
 *      Author: gianluca
 */

#include "Cpu.h"

#include "Plc.h"

#include "SM1.h"

#include "CSInput.h"
#include "CSOutput.h"

#include "utility.h"
#include "Timer.h"
#include "Bits.h"
#include "CANSettings.h"

word inputBuffer[NUM_INPUTS_BLOCK];

word outputBuffer[NUM_OUTPUTS_BLOCK];


word outValue[NUM_OUTPUTS_BLOCK];

word outFrz[NUM_OUTPUTS_BLOCK];

word outFrzValue[NUM_OUTPUTS_BLOCK];


word inpValue[NUM_INPUTS_BLOCK];

word inpFrz[NUM_INPUTS_BLOCK];

word inpFrzValue[NUM_INPUTS_BLOCK];



word ADCValues[NUM_ANALOGIC];


// Filtraggio Ingressi
word prevInpBuffer[NUM_INPUTS_BLOCK];

word filteredInpBuffer[NUM_INPUTS_BLOCK];

byte iniInpBuffer[NUM_INPUTS_BLOCK];

dword filterTmrs[NUM_INPUTS_BLOCK][NUM_INPUTS];

word filterVal[NUM_INPUTS_BLOCK][NUM_INPUTS];



short ioTrasmitCompleted = 0;

short ioReceveCompleted = 0;

word ioNumSended = 0;

short stIO = stCmdOutput;

bool ioWorked = FALSE;

short ioTrasmitStatus = 0;

word busInputs[NUM_IO_BOARD];
word busOutputs[NUM_IO_BOARD];

/*=================
 * Inizializzazione
 * del Plc
 *=================*/
void InitPlc()
{
	short i;
	
	
	for (i = 0; i < NUM_OUTPUTS_BLOCK; ++i)
	{
		
		outValue[i] = 0;

		outFrz[i] = 0;

		outFrzValue[i] = 0;	
		
		outputBuffer[i] = 0;
		
		prevInpBuffer[i] = 0;
		
		filteredInpBuffer[i] = 0;
		
	
	}
	
	for (i = 0; i < NUM_INPUTS_BLOCK; ++i)
	{
		inpValue[i] = 0;

		inpFrz[i] = 0;

		inpFrzValue[i] = 0;
		
		inputBuffer[i] = 0;
	}
	
	
	for (i = 0; i < NUM_ANALOGIC; ++i)
	{
		ADCValues[i] = 0;
	}
	
	stIO = stCmdOutput;

}

/*
 * Setta le uscite
 *
 */
bool IOServer()
{			
	short i, id;
	byte chr = 0;
	
	byte buff[NUM_INPUTS_BLOCK];
	byte buffOut[NUM_OUTPUTS_BLOCK];
	
	for (i = 0; i < NUM_OUTPUTS_BLOCK; ++i)
	{
		outputBuffer[i] = buffOut[i] = (outValue[i] & ~outFrz[i]) | (outFrzValue[i] & outFrz[i]);
	}
	
	for (i = 0; i < NUM_INPUTS_BLOCK; ++i)
	{
		buff[i] = 0;
	}
	
	
	
	switch (stIO)
	{
		case stCmdOutput:					
			ioTrasmitCompleted = 0;
			stIO = stWaitTrasmit;
			
			
			SM1_SendBlock(buffOut, NUM_OUTPUTS_BLOCK, &ioNumSended);
			
			
			TMR_PLC = MSEC(1);	//MSEC(5);
			
			break;
			
		case stWaitTrasmit:
			if (ioTrasmitCompleted >= ioNumSended ||
				!TMR_PLC)
			{
				
				ioTrasmitStatus++;
				
				CSOutput_SetVal();	
				CSOutput_ClrVal();
						
				stIO = stCmdInput;
				
				TMR_PLC = MSEC(1);
			}
			
			break;
			
		case stCmdInput:					
			
			if (!TMR_PLC)
			{
							
				ioReceveCompleted = 0;
				
				
				CSInput_SetVal();
				
				SM1_ClearRxBuf();
				
				stIO = stWaitReceve;
				SM1_SendBlock(buff, NUM_INPUTS_BLOCK, &ioNumSended);
				
				
			}
			break;
			
		case stWaitReceve:
			
			if (ioReceveCompleted >= NUM_INPUTS_BLOCK && SM1_GetCharsInRxBuf() >= NUM_INPUTS_BLOCK)
			{
				
				
				SM1_RecvBlock(buff, NUM_INPUTS_BLOCK, &ioNumSended);
				
				if (ioNumSended == NUM_INPUTS_BLOCK)
				{
					

					for (i = 0; i < NUM_INPUTS_BLOCK; ++i)
					{
						id = NUM_INPUTS_BLOCK - i - 1;
						
						
						inputBuffer[id] = (buff[i] & ~inpFrz[id]) | (inpFrzValue[id] & inpFrz[id]);
					}
					
					if (!ioWorked)
					{											
						ioWorked = TRUE;
						TMR_START = SEC(1);
						
					}
				}
				
				stIO = stCmdOutput;
				CSInput_ClrVal();
				
			}			
			break;
	}
	
	return ioWorked;
}




/* 
 * Filtraggio dei segnali di ingresso
 */

void FilterInputs()
{ 
	byte tmp;
	short i;
	short j;
	
	tmp = (byte)~(prevInpBuffer[0] ^ inputBuffer[0]);
	
	for (j = 0; j < NUM_INPUTS_BLOCK; ++j)
	{		
		for (i = 0; i < NUM_INPUTS; ++i)
		{
			if (filterTmrs[j][i] >= TIMER_TICK)
				filterTmrs[j][i] -= TIMER_TICK;
			else
				filterTmrs[j][i] = 0;
			
			if (filterVal[j][i])
			{
				if (!(tmp & Bits[i]))
				{
					filterTmrs[j][i] = MSEC(filterVal[0][i]);
				}
				else if (!filterTmrs[j][i])
				{
					
					if (inputBuffer[j] & Bits[i])
						filteredInpBuffer[j] |= Bits[i];
					
					else
						filteredInpBuffer[j] &= ~Bits[i];
					
					//filterTmrs[0][i] = MSEC(filterVal[0][i]);
				}
			}
			else 
			{
				if ((inputBuffer[j] & Bits[i]) == Bits[i])
					filteredInpBuffer[j] |= Bits[i];
				
				else
					filteredInpBuffer[j] &= ~Bits[i];
				
			}
				
		}
		prevInpBuffer[j] = inputBuffer[j];
	}
	
	
}


/* 
 * Settaggio di un filtro di ingresso
 */
void SetFilter(byte id, word filter)
{
	filterVal[0][id] = filter;
	
}

/*
 * Attivazione o disattivazione di un azionamento
 */
void SetDevice(short device, byte state)
{
	short id;
	if (device < 8)
		(state) ? (outValue[0] |= Bits[device]) : (outValue[0] &= ~Bits[device]);
		
	else if (device < 16)
	{
		id = device - 8;
		(state) ? (outValue[1] |= Bits[id]) : (outValue[1] &= ~Bits[id]);
	}
}
