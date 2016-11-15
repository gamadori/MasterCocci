/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : SM1.c
**     Project     : CocciMaster
**     Processor   : MC56F84451VLF
**     Component   : SynchroMaster
**     Version     : Component 02.347, Driver 02.05, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-14, 19:54, # CodeGen: 8
**     Abstract    :
**         This component "SynchroMaster" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings    :
**         Synchro type                : MASTER
**
**         Serial channel              : QSPI0
**
**         Protocol
**             Init baud rate          : 1_875MHz
**             Clock edge              : falling
**             Width                   : 8 bits
**             Empty character         : 0
**             Empty char. on input    : RECEIVED
**
**         Registers
**             Input buffer            : QSPI0_SPDRR [E0B2]
**             Output buffer           : QSPI0_SPDTR [E0B3]
**             Control register        : QSPI0_SPSCR [E0B0]
**             Mode register           : QSPI0_SPDSR [E0B1]
**             Baud setting reg.       : QSPI0_SPSCR [E0B0]
**
**         Input interrupt
**             Vector name             : INT_QSPI0_RCV
**             Priority                : 1
**
**         Output interrupt
**             Vector name             : INT_QSPI0_XMIT
**             Priority                : 1
**
**         Used pins                   :
**         ----------------------------------------------------------
**              Function    | On package |    Name
**         ----------------------------------------------------------
**               Input      |     25     |  GPIOC8/MISO0/RXD0/XB_IN9
**               Output     |     27     |  GPIOC10/MOSI0/XB_IN5/MISO0
**               Clock      |     26     |  GPIOC9/SCK0/XB_IN4
**         ----------------------------------------------------------
**
**     Contents    :
**         RecvChar        - byte SM1_RecvChar(SM1_TComData *Chr);
**         SendChar        - byte SM1_SendChar(SM1_TComData Chr);
**         RecvBlock       - byte SM1_RecvBlock(SM1_TComData *Ptr, word Size, word *Rcv);
**         SendBlock       - byte SM1_SendBlock(SM1_TComData *Ptr, word Size, word *Snd);
**         ClearRxBuf      - byte SM1_ClearRxBuf(void);
**         ClearTxBuf      - byte SM1_ClearTxBuf(void);
**         GetCharsInRxBuf - word SM1_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word SM1_GetCharsInTxBuf(void);
**         GetError        - byte SM1_GetError(SM1_TError *Err);
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
** @file SM1.c
** @version 02.05
** @brief
**         This component "SynchroMaster" implements MASTER part of synchronous
**         serial master-slave communication.
*/         
/*!
**  @addtogroup SM1_module SM1 module documentation
**  @{
*/         

/* MODULE SM1. */

#include "Events.h"
#include "SM1.h"

#define OVERRUN_ERR      0x01U         /* Overrun error flag bit   */
#define CHAR_IN_RX       0x08U         /* Char is in RX buffer     */
#define FULL_TX          0x10U         /* Full transmit buffer     */
#define RUNINT_FROM_TX   0x20U         /* Interrupt is in progress */
#define FULL_RX          0x40U         /* Full receive buffer      */

static byte SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Unused */
                                       /*       2 - Unused */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Running int from TX */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Unused */
static byte ErrFlag;                   /* Error flags for GetError method */
volatile word SM1_InpLen;              /* Length of input buffer's content */
static SM1_TComData *InpPtrR;          /* Pointer for reading from input buffer */
static SM1_TComData *InpPtrW;          /* Pointer for writing to input buffer */
static SM1_TComData InpBuffer[SM1_INP_BUF_SIZE]; /* Input buffer SPI commmunication */
volatile word SM1_OutLen;              /* Length of output bufer's content */
static SM1_TComData *OutPtrR;          /* Pointer for reading from output buffer */
static SM1_TComData *OutPtrW;          /* Pointer for writing to output buffer */
static SM1_TComData OutBuffer[SM1_OUT_BUF_SIZE]; /* Output buffer for SPI commmunication */

/*
** ===================================================================
**     Method      :  SM1_RecvChar (component SynchroMaster)
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). 
**         For information about SW overrun behavior please see
**         <General info page>.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on selected CPU and receiver
**         is configured to use DMA controller then this method only
**         sets the selected DMA channel. Status of the DMA transfer
**         can then be checked using method GetCharsInRxBuf. See
**         typical usage for details about communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - A pointer to the received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK - The valid data is received.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode.
**                           ERR_RXEMPTY - No data in receiver.
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last char or block received. In
**                           polling mode, this error code is returned
**                           only when the hardware supports detection
**                           of the overrun error. If interrupt service
**                           is enabled, and input buffer allocated by
**                           the component is full, the component
**                           behaviour depends on <Input buffer size>
**                           property : if property is 0, last received
**                           data-word is preserved (and previous is
**                           overwritten), if property is greater than 0,
**                           new received data-word are ignored.
**                           ERR_FAULT - Fault error was detected from
**                           the last char or block received. In the
**                           polling mode the ERR_FAULT is return until
**                           the user clear the fault flag bit, but in
**                           the interrupt mode ERR_FAULT is returned
**                           only once after the fault error occured.
**                           This error is supported only on the CPUs
**                           supports the faul mode function - where
**                           <Fault mode> property is available.
** ===================================================================
*/
byte SM1_RecvChar(SM1_TComData *Chr)
{
  register byte FlagTmp;

  if (SM1_InpLen > 0U) {               /* Is number of received chars greater than 0? */
    EnterCritical();                   /* Disable global interrupts */
    SM1_InpLen--;                      /* Decrease number of received chars */
    *Chr = *(InpPtrR++);               /* Read the char */
    /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
    if (InpPtrR >= (InpBuffer + SM1_INP_BUF_SIZE)) { /* Is the pointer out of the receive buffer? */
      InpPtrR = InpBuffer;             /* Set pointer to the first item into the receive buffer */
    }
    /*lint -restore Enable MISRA rule (17.2) checking. */
    FlagTmp = SerFlag;                 /* Safe the flags */
    SerFlag &= (byte)~(OVERRUN_ERR | FULL_RX); /* Clear flag "char in RX buffer" */
    ExitCritical();                    /* Enable global interrupts */
  }
  else {
    return ERR_RXEMPTY;                /* Receiver is empty */
  }
  if ((FlagTmp & (OVERRUN_ERR | FULL_RX)) != 0U) { /* Is the overrun occured? */
    return ERR_OVERRUN;                /* If yes then return error */
  }
  else {
    return ERR_OK;
  }
}

/*
** ===================================================================
**     Method      :  SM1_SendChar (component SynchroMaster)
**     Description :
**         Sends one character to the channel.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method only sets the selected DMA channel. The status of the
**         DMA transfer can then be checked using GetCharsInTxBuf
**         method. See the typical usage for details about
**         communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled (only if
**                           output DMA is supported and enabled)
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte SM1_SendChar(SM1_TComData Chr)
{
  if (SM1_OutLen == SM1_OUT_BUF_SIZE) { /* Is number of chars in buffer the same as the size of transmit buffer? */
    return ERR_TXFULL;                 /* If yes then error */
  }
  EnterCritical();                     /* Disable global interrupts */
  SM1_OutLen++;                        /* Increase number of bytes in the transmit buffer */
  *(OutPtrW++) = Chr;                  /* Store char to buffer */
  /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
  if (OutPtrW >= (OutBuffer + SM1_OUT_BUF_SIZE)) { /* Is the pointer out of the transmit buffer? */
    OutPtrW = OutBuffer;               /* Set pointer to the first item in the transmit buffer */
  }
  /*lint -restore Enable MISRA rule (17.2) checking. */
  setRegBit(QSPI0_SPSCR,SPTIE);        /* Enable transmit interrupt */
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SM1_RecvBlock (component SynchroMaster)
**     Description :
**         If any data received, this method returns the block of the
**         data and its length (and incidental error), otherwise it
**         returns error code (it does not wait for data).
**         If less than requested number of characters is received only
**         the available data is copied from the receive buffer to the
**         user specified destination and the ERR_EXEMPTY value is
**         returned.
**         This method is available only if non-zero length of input
**         buffer is defined.
**         For information about SW overrun behavior please see
**         <General info page>.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method only sets the selected DMA channel. The status of the
**         DMA transfer can then be checked using GetCharsInRxBuf
**         method. See the typical usage for details about
**         communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - A pointer to the block of received data
**         Size            - The size of the block
**       * Rcv             - Pointer to a variable where an actual
**                           number of copied characters is stored
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK - The valid data is received.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode.
**                           ERR_RXEMPTY - It was not possible to read
**                           requested number of bytes from the buffer.
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last char or block received. If
**                           interrupt service is enabled, and input
**                           buffer allocated by the component is full,
**                           the component behaviour depends on <Input
**                           buffer size> property : if property is 0,
**                           last received data-word is preserved (and
**                           previous is overwritten), if property is
**                           greater than 0, new received data-word are
**                           ignored.
**                           ERR_FAULT - Fault error was detected from
**                           the last char or block received. In the
**                           polling mode the ERR_FAULT is return until
**                           the user clear the fault flag bit, but in
**                           the interrupt mode ERR_FAULT is returned
**                           only once after the fault error occured.
**                           This error is supported only on the CPUs
**                           supports the faul mode function - where
**                           <Fault mode> property is available.
** ===================================================================
*/
byte SM1_RecvBlock(SM1_TComData *Ptr,word Size,word *Rcv)
{
  register word count;                 /* Number of received chars */
  register byte Result = ERR_OK;       /* Most serious error */
  register byte ResultTmp;             /* Last error */

  count = 0U;
  while (count < Size) {
    ResultTmp = SM1_RecvChar(Ptr++);   /* Receive one character */
    if (ResultTmp > Result) {          /* Is last error most serious than through error state? */
      Result = ResultTmp;              /* If yes then prepare error value to return */
    }
    if ((ResultTmp != ERR_OK)&&(ResultTmp != ERR_OVERRUN)) { /* Receiving given number of chars */
      break;                           /* Break data block receiving */
    }
    count++;
  }
  *Rcv = count;                        /* Return number of received chars */
  return Result;                       /* Return most serious error */
}

/*
** ===================================================================
**     Method      :  SM1_SendBlock (component SynchroMaster)
**     Description :
**         Send a block of characters to the channel. This method is
**         only available if a non-zero length of output buffer is
**         defined.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and
**         transmitter is configured to use DMA controller then this
**         method only sets the selected DMA channel. The status of the
**         DMA transfer can then be checked using GetCharsInTxBuf
**         method. See the typical usage for details about
**         communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send
**         Size            - Size of the block
**       * Snd             - Pointer to number of data that are sent
**                           (moved to buffer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled (only if
**                           output DMA is supported and enabled)
**                           ERR_TXFULL - It was not possible to send
**                           requested number of bytes
** ===================================================================
*/
byte SM1_SendBlock(SM1_TComData *Ptr,word Size,word *Snd)
{
  register word count;                 /* Number of sent chars */
  register byte Result = ERR_OK;       /* Last error */
  SM1_TComData *TmpPtr = Ptr;          /* Temporary output buffer pointer */

  for (count = 0U; count < Size; count++) {
    Result = SM1_SendChar(*TmpPtr++);  /* Send one character */
    if (Result != ERR_OK) {            /* Sending given number of chars */
      break;                           /* Break data block sending */
    }
  }
  *Snd = count;                        /* Return number of sended chars */
  return Result;                       /* Return error code */
}

/*
** ===================================================================
**     Method      :  SM1_ClearRxBuf (component SynchroMaster)
**     Description :
**         Clears the receive buffer. This method is available only if
**         a non-zero length of input buffer is defined.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method only stops the selected DMA channel.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte SM1_ClearRxBuf(void)
{
  EnterCritical();                     /* Disable global interrupts */
  SM1_InpLen = 0U;                     /* Set number of chars in the transmit buffer to 0 */
  InpPtrW = InpBuffer;                 /* Set pointers on the first item in the transmit buffer */
  InpPtrR = InpBuffer;
  SerFlag &= (byte)~(OVERRUN_ERR | FULL_RX); /* Clear flags */
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SM1_ClearTxBuf (component SynchroMaster)
**     Description :
**         Clears the transmit buffer. This method is only available if
**         a non-zero length of output buffer is defined.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method only stops the selected DMA channel.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte SM1_ClearTxBuf(void)
{
  EnterCritical();                     /* Disable global interrupts */
  SM1_OutLen = 0U;                     /* Set number of chars in the receive buffer to 0 */
  OutPtrW = OutBuffer;                 /* Set pointers on the first item in the receive buffer */
  OutPtrR = OutBuffer;
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SM1_GetCharsInRxBuf (component SynchroMaster)
**     Description :
**         Returns the number of characters in the input buffer.
**         Note: If the Interrupt service is disabled, and the Ignore
**         empty character is set to yes, and a character has been
**         received, then this method returns 1 although it was an
**         empty character.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method returns the number of characters in the receive
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the input buffer.
** ===================================================================
*/
/*
word SM1_GetCharsInRxBuf(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  SM1_GetCharsInTxBuf (component SynchroMaster)
**     Description :
**         Returns the number of characters in the output buffer.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method returns the number of characters in the transmit
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the output buffer.
** ===================================================================
*/
/*
word SM1_GetCharsInTxBuf(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  SM1_GetError (component SynchroMaster)
**     Description :
**         Returns a set of errors on the channel (errors that cannot
**         be returned in given methods). The component accumulates
**         errors in a set; after calling [GetError] this set is
**         returned and cleared. This method is available only if the
**         "Interrupt service/event" property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Err             - A pointer to the returned set of errors
**     Returns     :
**         ---             - Error code (if GetError did not succeed),
**                           possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte SM1_GetError(SM1_TError *Err)
{
  EnterCritical();                     /* Disable global interrupts */
  Err->err = 0U;
  Err->errName.OverRun = (((ErrFlag & OVERRUN_ERR) != 0U)? 1U : 0U); /* Overrun error */
  Err->errName.RxBufOvf = (((ErrFlag & FULL_RX) != 0U)? 1U : 0U); /* Buffer overflow */
  ErrFlag = 0x00U;                     /* Reset error flags */
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SM1_InterruptRx (component SynchroMaster)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_ERROR        0x01U
#define ON_FULL_RX      0x02U
#define ON_RX_CHAR      0x04U
#define ON_RX_CHAR_EXT  0x08U
#pragma interrupt alignsp saveall
void SM1_InterruptRx(void)
{
  register word Data;                  /* Temporary variable for data */
  register byte Flags = 0U;            /* Temporary variable for flags */
  register word Status;                /* Temporary variable for flags */

  Status = getReg(QSPI0_SPSCR);        /* Read the device error register */
  Data = getReg(QSPI0_SPDRR);          /* Read data from receiver */
  if (getRegBit(QSPI0_SPSCR,OVRF)) {   /* Occured any overflow condition between QSPI0_SPSCR and QSPI0_SPDRR reading? */
    getReg(QSPI0_SPDRR);               /* Clear the overflow flag */
    Status |= QSPI0_SPSCR_OVRF_MASK;   /* Set overrun flag */
  }
  if (Status & QSPI0_SPSCR_OVRF_MASK) { /* Occured any overflow condition? */
    SerFlag |= OVERRUN_ERR;            /* If yes then set the Error flag for RecvChar/Block method */
    ErrFlag |= OVERRUN_ERR;            /* If yes then set the Error flag for GetError method */
    Flags |= ON_ERROR;                 /* If yes then set the OnError flag */
  }
  if (SM1_InpLen < SM1_INP_BUF_SIZE) { /* Is number of bytes in the receive buffer lower than size of buffer? */
    SM1_InpLen++;                      /* Increse number of chars in the receive buffer */
    *(InpPtrW)++ = (SM1_TComData)Data; /* Save received char to the receive buffer */
    /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
    if (InpPtrW >= (InpBuffer + SM1_INP_BUF_SIZE)) { /* Is the pointer out of the receive buffer? */
      InpPtrW = InpBuffer;             /* Set pointer on the first item into the receive buffer */
    }
    /*lint -restore Enable MISRA rule (17.2) checking. */
    Flags |= ON_RX_CHAR;               /* If yes then set the OnRxChar flag */
    if (SM1_InpLen == SM1_INP_BUF_SIZE) { /* Is number of bytes in the receive buffer equal to the size of buffer? */
      Flags |= ON_FULL_RX;             /* Set flag "OnFullRxBuf" */
    }
  }
  else {
    SerFlag |= FULL_RX;                /* Set flag "full RX buffer" */
    ErrFlag |= FULL_RX;                /* Set flag "full RX buffer" for GetError method */
    Flags |= ON_ERROR;                 /* Set the OnError flag */
  }
  if (Flags & ON_ERROR) {              /* Is any error flag set? */
    SM1_OnError();                     /* Invoke user event */
  }
  else {
    if (Flags & ON_RX_CHAR) {          /* Is OnRxChar flag set? */
      SM1_OnRxChar();                  /* Invoke user event */
    }
    if (Flags & ON_FULL_RX) {          /* Is OnTxChar flag set? */
      SM1_OnFullRxBuf();               /* Invoke user event */
    }
  }
}

/*
** ===================================================================
**     Method      :  SM1_InterruptTx (component SynchroMaster)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_FREE_TX  0x01U
#define ON_TX_CHAR  0x02U
#pragma interrupt alignsp saveall
void SM1_InterruptTx(void)
{
  register byte Flags = 0U;            /* Temporary variable for flags */

  if (SerFlag & RUNINT_FROM_TX) {      /* Is flag "running int from TX" set? */
    Flags |= ON_TX_CHAR ;              /* Set the OnTXChar flag */
  }
  if (SM1_OutLen) {                    /* Is number of bytes in the transmit buffer greather then 0? */
    SM1_OutLen--;                      /* Decrease number of chars in the transmit buffer */
    SerFlag |= RUNINT_FROM_TX;         /* Set flag "running int from TX"? */
    setReg(QSPI0_SPDTR,*(OutPtrR++));  /* Store char to transmitter register */
    /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
    if (OutPtrR >= (OutBuffer + SM1_OUT_BUF_SIZE)) { /* Is the pointer out of the transmit buffer? */
      OutPtrR = OutBuffer;             /* Set pointer on the first item into the transmit buffer */
    }
    /*lint -restore Enable MISRA rule (17.2) checking. */
  }
  else {
    SerFlag &= (byte)~(RUNINT_FROM_TX|FULL_TX); /* Clear "running int from TX" and "full TX buff" flags */
    clrRegBit(QSPI0_SPSCR,SPTIE);      /* Disable transmit interrupt */
    Flags |= ON_FREE_TX;               /* Set the OnFreeTXBuf flag */
  }
  if ((Flags & ON_TX_CHAR) != 0x00U) { /* Is flag "OnTxChar" set? */
    SM1_OnTxChar();                    /* Invoke user event */
  }
  if ((Flags & ON_FREE_TX) != 0x00U) { /* Is flag "OnFreeTxBuf" set? */
    SM1_OnFreeTxBuf();                 /* Invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  SM1_Init (component SynchroMaster)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void SM1_Init(void)
{
  /* QSPI0_SPSCR: SPR=4,DSO=0,ERRIE=0,MODFEN=0,SPRIE=0,SPMSTR=1,CPOL=0,CPHA=0,SPE=0,SPTIE=0,SPRF=0,OVRF=0,MODF=1,SPTE=0 */
  setReg(QSPI0_SPSCR,0x8102);          /* Set control register */
  /* QSPI0_SPDSR: WOM=0,TDMAEN=0,RDMAEN=0,BD2X=0,SSB_IN=0,SSB_DATA=0,SSB_ODM=0,SSB_AUTO=0,SSB_DDR=1,SSB_STRB=0,SSB_OVER=1,SPR3=0,DS=7 */
  setReg(QSPI0_SPDSR,0xA7);            /* Set data size and control register */
  /* QSPI0_SPWAIT: ??=0,??=0,??=0,WAIT=0x1FFF */
  setReg(QSPI0_SPWAIT,0x1FFF);         /* Set data size and control register */
  SerFlag = 0U;                        /* Reset all flags */
  ErrFlag = 0U;                        /* Reset all flags in mirror */
  SM1_InpLen = 0U;                     /* No char in the receive buffer */
  InpPtrR = InpBuffer;                 /* Set pointer on the first item in the receive buffer */
  InpPtrW = InpBuffer;
  SM1_OutLen = 0U;                     /* No char in the transmit buffer */
  OutPtrR = OutBuffer;                 /* Set pointer on the first item in the transmit buffer */
  OutPtrW = OutBuffer;
  setRegBits(QSPI0_SPSCR,(QSPI0_SPSCR_ERRIE_MASK|QSPI0_SPSCR_SPRIE_MASK|QSPI0_SPSCR_SPE_MASK)); /* Enable device and receive/error interrupts */
}

/* END SM1. */

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