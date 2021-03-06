/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : TI1.c
**     Project     : CocciMaster
**     Processor   : MC56F84451VLF
**     Component   : TimerInt
**     Version     : Component 02.161, Driver 02.06, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-11, 22:17, # CodeGen: 5
**     Abstract    :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings    :
**         Timer name                  : PIT0 (16-bit)
**         Compare name                : PIT0_Modulo
**         Counter shared              : No
**
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 60000000 Hz
**           Initial period/frequency
**             Xtal ticks              : 8000
**             microseconds            : 1000
**             milliseconds            : 1
**             seconds (real)          : 0.001
**             Hz                      : 1000
**             kHz                     : 1
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : PIT0_CNTR [E102]
**              Mode                   : PIT0_CTRL [E100]
**              Run                    : PIT0_CTRL [E100]
**              Prescaler              : PIT0_CTRL [E100]
**
**         Compare registers
**              Compare                : PIT0_MOD  [E101]
**
**         Flip-flop registers
**     Contents    :
**         No public methods
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file TI1.c
** @version 02.06
** @brief
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
*/         
/*!
**  @addtogroup TI1_module TI1 module documentation
**  @{
*/         

/* MODULE TI1. */

#include "Events.h"
#include "TI1.h"



/* Internal method prototypes */
static void SetCV(word Val);
static void SetPV(byte Val);

/*
** ===================================================================
**     Method      :  SetCV (component TimerInt)
**
**     Description :
**         Sets compare or preload register value. The method is called 
**         automatically as a part of several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void SetCV(word Val)
{
  setReg(PIT0_MOD,Val);                /* Store given value to the compare register */
}

/*
** ===================================================================
**     Method      :  SetPV (component TimerInt)
**
**     Description :
**         Sets prescaler value. The method is called automatically as a 
**         part of several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void SetPV(byte Val)
{
  setRegBitGroup(PIT0_CTRL,PRESCALER,Val); /* Store given value to the prescaler */
}

/*
** ===================================================================
**     Method      :  TI1_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void TI1_Init(void)
{
  /* PIT0_CTRL: SLAVE=0,??=0,??=0,??=0,??=0,??=0,CLKSEL=0,??=0,PRESCALER=0,PRF=0,PRIE=1,CNT_EN=0 */
  setReg(PIT0_CTRL,0x02);              /* Set up control register */
  SetCV((word)0xEA60);                 /* Store appropriate value to the compare register according to the selected high speed CPU mode */
  SetPV((byte)0x00);                   /* Set prescaler register according to the selected high speed CPU mode */
  setRegBit(PIT0_CTRL,CNT_EN);         /* Run counter */
}

/*
** ===================================================================
**     Method      :  TI1_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma interrupt alignsp saveall
void TI1_Interrupt(void)
{
  clrRegBit(PIT0_CTRL,PRF);            /* Reset interrupt request flag */
  TI1_OnInterrupt();                   /* Invoke user event */
}

/* END TI1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
