/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CSInput.c
**     Project     : CocciMaster
**     Processor   : MC56F84451VLF
**     Component   : BitIO
**     Version     : Component 02.086, Driver 01.20, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-14, 19:55, # CodeGen: 9
**     Abstract    :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings    :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       13            |  GPIOC5/XB_IN7
**             ----------------------------------------------------
**
**         Port name                   : GPIOC_Low
**
**         Bit number (in port)        : 5
**         Bit mask of the port        : 0020
**
**         Initial direction           : Output (direction can be changed)
**         Safe mode                   : yes
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : GPIOC_DR  [E221]
**         Port control register       : GPIOC_DDR [E222]
**         Port function register      : GPIOC_PER [E223]
**
**         Optimization for            : speed
**     Contents    :
**         SetDir - void CSInput_SetDir(bool Dir);
**         GetVal - bool CSInput_GetVal(void);
**         PutVal - void CSInput_PutVal(bool Val);
**         ClrVal - void CSInput_ClrVal(void);
**         SetVal - void CSInput_SetVal(void);
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
** @file CSInput.c
** @version 01.20
** @brief
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
*/         
/*!
**  @addtogroup CSInput_module CSInput module documentation
**  @{
*/         

/* MODULE CSInput. */

#include "CSInput.h"
/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "Cpu.h"


/*
** ===================================================================
**     Method      :  CSInput_GetVal (component BitIO)
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool CSInput_GetVal(void)

**  This method is implemented as a macro. See CSInput.h file.  **

*/

/*
** ===================================================================
**     Method      :  CSInput_PutVal (component BitIO)
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void CSInput_PutVal(bool Val)
{
  if (Val) {                           /* Is it one to be written? */
    Shadow_GPIOC_DR |= CSInput_PIN_MASK; /* Set bit in shadow variable */
    setRegBits(GPIOC_DR,CSInput_PIN_MASK); /* Set bit on port */
  }
  else {                               /* Is it zero to be written? */
    Shadow_GPIOC_DR &= (word)~(word)CSInput_PIN_MASK; /* Clear bit in shadow variable */
    clrRegBits(GPIOC_DR,CSInput_PIN_MASK); /* Clear bit on port */
  }
}

/*
** ===================================================================
**     Method      :  CSInput_SetVal (component BitIO)
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CSInput_SetVal(void)

**  This method is implemented as a macro. See CSInput.h file.  **
*/

/*
** ===================================================================
**     Method      :  CSInput_ClrVal (component BitIO)
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CSInput_ClrVal(void)

**  This method is implemented as a macro. See CSInput.h file.  **
*/

/*
** ===================================================================
**     Method      :  CSInput_SetDir (component BitIO)
**     Description :
**         This method sets direction of the component.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void CSInput_SetDir(bool Dir)
{
  if (Dir) {                           /* Is given direction output? */
    setReg(GPIOC_DR,((getReg(GPIOC_DR)) & (word)~(word)CSInput_PIN_MASK) | (Shadow_GPIOC_DR & CSInput_PIN_MASK)); /* Restore correct value of output from shadow variable */
    setRegBits(GPIOC_DDR,CSInput_PIN_MASK); /* Set direction to output */
  }
  else {                               /* Is given direction input? */
    clrRegBits(GPIOC_DDR,CSInput_PIN_MASK); /* Set direction to input */
  }
}

/* END CSInput. */

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
