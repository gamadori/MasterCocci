/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : AS1.c
**     Project     : CocciMaster
**     Processor   : MC56F84451VLF
**     Component   : AsynchroSerial
**     Version     : Component 02.611, Driver 02.07, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-14, 19:24, # CodeGen: 7
**     Abstract    :
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
**     Settings    :
**         Serial channel              : QSCI0
**
**         Protocol
**             Init baud rate          : 19200baud
**             Width                   : 8 bits
**             Stop bits               : 1
**             Parity                  : none
**             Breaks                  : Disabled
**             Input buffer size       : 64
**             Output buffer size      : 64
**
**         Registers
**             Input buffer            : QSCI0_DATA [E084]
**             Output buffer           : QSCI0_DATA [E084]
**             Control register        : QSCI0_CTRL1 [E081]
**             Mode register           : QSCI0_CTRL1 [E081]
**             Baud setting reg.       : QSCI0_RATE [E080]
**
**         Input interrupt
**             Vector name             : INT_QSCI0_RCV
**             Priority                : 1
**
**         Output interrupt
**             Vector name             : INT_QSCI0_TDRE
**             Priority                : 1
**
**         Used pins:
**         ----------------------------------------------------------
**           Function | On package           |    Name
**         ----------------------------------------------------------
**            Input   |     6                |  GPIOC3/TA0/CMPA_O/RXD0/CLKIN1
**            Output  |     5                |  GPIOC2/TXD0/TB0/XB_IN2/CLKO0
**         ----------------------------------------------------------
**
**
**
**     Contents    :
**         RecvChar        - byte AS1_RecvChar(AS1_TComData *Chr);
**         SendChar        - byte AS1_SendChar(AS1_TComData Chr);
**         RecvBlock       - byte AS1_RecvBlock(AS1_TComData *Ptr, word Size, word *Rcv);
**         SendBlock       - byte AS1_SendBlock(AS1_TComData *Ptr, word Size, word *Snd);
**         ClearRxBuf      - byte AS1_ClearRxBuf(void);
**         ClearTxBuf      - byte AS1_ClearTxBuf(void);
**         GetCharsInRxBuf - word AS1_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word AS1_GetCharsInTxBuf(void);
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
** @file AS1.c
** @version 02.07
** @brief
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
*/         
/*!
**  @addtogroup AS1_module AS1 module documentation
**  @{
*/         

/* MODULE AS1. */

#include "AS1.h"
#include "Events.h"


#define OVERRUN_ERR      0x01U         /* Overrun error flag bit    */
#define FRAMING_ERR      0x02U         /* Framing error flag bit    */
#define PARITY_ERR       0x04U         /* Parity error flag bit     */
#define CHAR_IN_RX       0x08U         /* Char is in RX buffer      */
#define FULL_TX          0x10U         /* Full transmit buffer      */
#define RUNINT_FROM_TX   0x20U         /* Interrupt is in progress  */
#define FULL_RX          0x40U         /* Full receive buffer       */
#define NOISE_ERR        0x80U         /* Noise error flag bit      */
#define IDLE_ERR         0x0100U       /* Idle character flag bit   */
#define BREAK_ERR        0x0200U       /* Break detect              */
#define COMMON_ERR       0x0800U       /* Common error of RX       */

static word SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Framing error */
                                       /*       2 - Parity error */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Running int from TX */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Noise error */
                                       /*       8 - Idle character  */
                                       /*       9 - Break detected  */
                                       /*      10 - Unused */
                                       /*      11 - Unused */
static word InpLen;                    /* Length of input buffer's content */
static AS1_TComData *InpPtrR;          /* Pointer for reading from input buffer */
static AS1_TComData *InpPtrW;          /* Pointer for writing to input buffer */
static AS1_TComData InpBuffer[AS1_INP_BUF_SIZE]; /* Input buffer for SCI communication */
static word OutLen;                    /* Length of output bufer's content */
static AS1_TComData *OutPtrR;          /* Pointer for reading from output buffer */
static AS1_TComData *OutPtrW;          /* Pointer for writing to output buffer */
static AS1_TComData OutBuffer[AS1_OUT_BUF_SIZE]; /* Output buffer for SCI communication */

/*
** ===================================================================
**     Method      :  HWEnDi (component AsynchroSerial)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the bean.
**         The method is called automatically as a part of the Enable and 
**         Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  getReg(QSCI0_STAT);                  /* Reset interrupt request flags */
  setRegBits(QSCI0_CTRL1, (QSCI0_CTRL1_TE_MASK | QSCI0_CTRL1_RE_MASK | QSCI0_CTRL1_RFIE_MASK | QSCI0_CTRL1_REIE_MASK)); /* Enable device */
}

/*
** ===================================================================
**     Method      :  AS1_RecvChar (component AsynchroSerial)
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). This method is enabled only if the receiver property
**         is enabled.
**         [Note:] Because the preferred method to handle error and
**         break exception in the interrupt mode is to use events
**         <OnError> and <OnBreak> the return value ERR_RXEMPTY has
**         higher priority than other error codes. As a consequence the
**         information about an exception in interrupt mode is returned
**         only if there is a valid character ready to be read.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method only sets the selected DMA channel. Then the status
**         of the DMA transfer can be checked using GetCharsInRxBuf
**         method. See an example of a typical usage for details about
**         the communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to a received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_BREAK - Break character is detected
**                           (only when the <Interrupt service> property
**                           is disabled and the <Break signal> property
**                           is enabled)
**                           ERR_COMMON - common error occurred (the
**                           <GetError> method can be used for error
**                           specification)
**                           Version specific information for Freescale
**                           56800 derivatives 
**                           DMA mode:
**                           If DMA controller is available on the
**                           selected CPU and the receiver is configured
**                           to use DMA controller then only ERR_OK,
**                           ERR_RXEMPTY, and ERR_SPEED error code can
**                           be returned from this method.
** ===================================================================
*/
byte AS1_RecvChar(AS1_TComData *Chr)
{
  register byte Result = ERR_OK;       /* Return error code */

  if (InpLen > 0x00U) {                /* Is number of received chars greater than 0? */
    EnterCritical();                   /* Disable global interrupts */
    InpLen--;                          /* Decrease number of received chars */
    *Chr = *(InpPtrR++);               /* Received char */
    /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
    if (InpPtrR >= (InpBuffer + AS1_INP_BUF_SIZE)) { /* Is the pointer out of the receive buffer? */
      InpPtrR = InpBuffer;             /* Set pointer to the first item into the receive buffer */
    }
    /*lint -restore Enable MISRA rule (17.2) checking. */
    Result = (byte)((SerFlag & (OVERRUN_ERR|COMMON_ERR|FULL_RX))? ERR_COMMON : ERR_OK);
    SerFlag &= (word)~(word)(OVERRUN_ERR|COMMON_ERR|FULL_RX|CHAR_IN_RX); /* Clear all errors in the status variable */
    ExitCritical();                    /* Enable global interrupts */
  } else {
    return ERR_RXEMPTY;                /* Receiver is empty */
  }
  return Result;                       /* Return error code */
}

/*
** ===================================================================
**     Method      :  AS1_SendChar (component AsynchroSerial)
**     Description :
**         Sends one character to the channel. If the component is
**         temporarily disabled (Disable method) SendChar method only
**         stores data into an output buffer. In case of a zero output
**         buffer size, only one character can be stored. Enabling the
**         component (Enable method) starts the transmission of the
**         stored data. This method is available only if the
**         transmitter property is enabled.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method only sets selected DMA channel. Then the status of
**         the DMA transfer can be checked using GetCharsInTxBuf method.
**         See an example of a typical usage for details about
**         communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte AS1_SendChar(AS1_TComData Chr)
{
  if (OutLen == AS1_OUT_BUF_SIZE) {    /* Is number of chars in buffer is the same as a size of the transmit buffer */
    return ERR_TXFULL;                 /* If yes then error */
  }
  EnterCritical();                     /* Disable global interrupts */
  OutLen++;                            /* Increase number of bytes in the transmit buffer */
  *(OutPtrW++) = Chr;                  /* Store char to buffer */
    /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
  if (OutPtrW >= (OutBuffer + AS1_OUT_BUF_SIZE)) { /* Is the pointer out of the transmit buffer */
    OutPtrW = OutBuffer;               /* Set pointer to first item in the transmit buffer */
  }
    /*lint -restore Enable MISRA rule (17.2) checking. */
  setRegBit(QSCI0_CTRL1, TEIE);        /* Enable transmit interrupt */
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS1_RecvBlock (component AsynchroSerial)
**     Description :
**         If any data is received, this method returns the block of
**         the data and its length (and incidental error), otherwise it
**         returns an error code (it does not wait for data).
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is enabled.
**         If less than requested number of characters is received only
**         the available data is copied from the receive buffer to the
**         user specified destination. The value ERR_EXEMPTY is
**         returned and the value of variable pointed by the Rcv
**         parameter is set to the number of received characters.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method only sets the selected DMA channel. Then the status
**         of the DMA transfer can be checked using GetCharsInRxBuf
**         method. See an example of a typical usage for details about
**         communication using DMA.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of received data
**         Size            - Size of the block
**       * Rcv             - Pointer to real number of the received data
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data. Only
**                           available data has been returned.
**                           ERR_COMMON - common error occurred (the
**                           GetError method can be used for error
**                           specification)
**                           Version specific information for Freescale
**                           56800 derivatives
**                           DMA mode: If DMA controller is available on
**                           the selected CPU and the receiver is
**                           configured to use DMA controller then only
**                           ERR_OK, ERR_RXEMPTY, and ERR_SPEED error
**                           codes can be returned from this method.
** ===================================================================
*/
byte AS1_RecvBlock(AS1_TComData *Ptr,word Size,word *Rcv)
{
  register word count;                 /* Number of received chars */
  register byte result = ERR_OK;       /* Last error */

  for (count = 0x00U; count < Size; count++) {
    switch (AS1_RecvChar(Ptr++)) {     /* Receive data and test the return value*/
    case ERR_RXEMPTY:                  /* No data in the buffer */
      if (result == ERR_OK) {          /* If no receiver error reported */
        result = ERR_RXEMPTY;          /* Return info that requested number of data is not available */
      }
     *Rcv = count;                     /* Return number of received chars */
      return result;
    case ERR_COMMON:                   /* Receiver error reported */
      result = ERR_COMMON;             /* Return info that an error was detected */
      break;
    default:
      break;
    }
  }
  *Rcv = count;                        /* Return number of received chars */
  return result;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS1_SendBlock (component AsynchroSerial)
**     Description :
**         Sends a block of characters to the channel.
**         This method is available only if non-zero length of the
**         output buffer is defined and the transmitter property is
**         enabled.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method only sets the selected DMA channel. Then the status
**         of the DMA transfer can be checked using GetCharsInTxBuf
**         method. See typical usage for details about communication
**         using DMA.
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
**                           ERR_TXFULL - It was not possible to send
**                           requested number of bytes
** ===================================================================
*/
byte AS1_SendBlock(AS1_TComData *Ptr,word Size,word *Snd)
{
  register word count;                 /* Number of sent chars */
  register byte result = ERR_OK;       /* Last error */
  register AS1_TComData *tmpPtr = Ptr;

  for (count = 0x00U; count < Size; count++) {
    result = AS1_SendChar(*tmpPtr++);
    if (result != ERR_OK) {            /* Sending given number of chars */
      *Snd = count;                    /* Return number of sent chars */
      return result;                   /* Return last error */
    }
  }
  *Snd = count;                        /* Return number of sended chars */
  return result;                       /* Return error code */
}

/*
** ===================================================================
**     Method      :  AS1_ClearRxBuf (component AsynchroSerial)
**     Description :
**         Clears the receive buffer.
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is enabled.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method only stops selected DMA channel.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte AS1_ClearRxBuf(void)
{
  EnterCritical();                     /* Disable global interrupts */
  InpLen = 0x00U;                      /* Set number of chars in the transmit buffer to 0 */
  InpPtrW = InpBuffer;                 /* Set pointers on the first item in the transmit buffer */
  InpPtrR = InpBuffer;
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS1_ClearTxBuf (component AsynchroSerial)
**     Description :
**         Clears the transmit buffer.
**         This method is available only if non-zero length of the
**         output buffer is defined and the receiver property is
**         enabled.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method only stops selected DMA channel.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte AS1_ClearTxBuf(void)
{
  EnterCritical();                     /* Disable global interrupts */
  OutLen = 0x00U;                      /* Set number of chars in the receive buffer to 0 */
  OutPtrW = OutBuffer;                 /* Set pointers on the first item in the receive buffer */
  OutPtrR = OutBuffer;
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS1_GetCharsInRxBuf (component AsynchroSerial)
**     Description :
**         Returns the number of characters in the input buffer. This
**         method is available only if the receiver property is enabled.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this
**         method returns the number of characters in the receive
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the input
**                           buffer.
** ===================================================================
*/
word AS1_GetCharsInRxBuf(void)
{
  return InpLen;                       /* Return number of chars in receive buffer */
}

/*
** ===================================================================
**     Method      :  AS1_GetCharsInTxBuf (component AsynchroSerial)
**     Description :
**         Returns the number of characters in the output buffer. This
**         method is available only if the transmitter property is
**         enabled.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         method returns the number of characters in the transmit
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the output
**                           buffer.
** ===================================================================
*/
word AS1_GetCharsInTxBuf(void)
{
  return OutLen;                       /* Return number of chars in the transmitter buffer */
}

/*
** ===================================================================
**     Method      :  AS1_InterruptRx (component AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the bean's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_ERROR    0x01U
#define ON_FULL_RX  0x02U
#define ON_RX_CHAR  0x04U
#pragma interrupt alignsp saveall
void AS1_InterruptRx(void)
{
  register AS1_TComData Data;          /* Temporary variable for data */
  register word OnFlags = 0x00U;       /* Temporary variable for flags */
  register word StatReg = getReg(QSCI0_STAT); /* Read status register */

  if((StatReg & (QSCI0_STAT_FE_MASK)) != 0x00U) { /* Is HW framing error detected? */
    return;                            /* Then decode error type in Error ISR */
  }
  Data = (AS1_TComData)getReg(QSCI0_DATA); /* Read data from the receiver */
  if(StatReg & (QSCI0_STAT_OR_MASK)) { /* Is HW overrun error detected? */
    setReg(QSCI0_STAT, 0U);            /* Reset error request flags */
    SerFlag |= (OVERRUN_ERR);          /* Set flag OVERRUN_ERR */
    OnFlags |= (ON_ERROR);             /* Set flag "OnError" */
  }
  if (InpLen < AS1_INP_BUF_SIZE) {     /* Is number of bytes in the receive buffer lower than size of buffer? */
    InpLen++;                          /* Increase number of chars in the receive buffer */
    *(InpPtrW++) = Data;               /* Save received char to the receive buffer */
  /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
    if (InpPtrW >= (InpBuffer + AS1_INP_BUF_SIZE)) { /* Is the pointer out of the receive buffer? */
      InpPtrW = InpBuffer;             /* Set pointer on the first item into the receive buffer */
    }
  /*lint -restore Enable MISRA rule (17.2) checking. */
    OnFlags |= ON_RX_CHAR;             /* Set flag "OnRXChar" */
    if (InpLen == AS1_INP_BUF_SIZE) {  /* Is number of bytes in the receive buffer equal as a size of buffer? */
      OnFlags |= ON_FULL_RX;           /* If yes then set flag "OnFullRxBuff" */
    }
  } else {
    SerFlag |= FULL_RX;                /* If yes then set flag buffer overflow */
    OnFlags |= ON_ERROR;               /* Set flag "OnError" */
  }
  if ((OnFlags & ON_ERROR) != 0x00U) { /* Was error flag detect? */
    AS1_OnError();                     /* If yes then invoke user event */
  }
  if ((OnFlags & ON_RX_CHAR) != 0x00U) { /* Is OnRxChar flag set? */
    AS1_OnRxChar();                    /* If yes then invoke user event */
  }
  if ((OnFlags & ON_FULL_RX) != 0x00U) { /* Is OnFullRxBuf flag set? */
    AS1_OnFullRxBuf();                 /* If yes then invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  AS1_InterruptTx (component AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the bean's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_FREE_TX  0x01U
#define ON_TX_CHAR  0x02U
#pragma interrupt alignsp saveall
void AS1_InterruptTx(void)
{
  register word OnFlags = 0x00U;       /* Temporary variable for flags */

  if ((SerFlag & RUNINT_FROM_TX) != 0x00U) { /* Is flag "running int from TX" set? */
    OnFlags |= (ON_TX_CHAR);           /* Set flag "OnTxChar" */
  }
  SerFlag &= (word)~(word)(RUNINT_FROM_TX); /* Reset flag "running int from TX" */
  if (OutLen > 0x00U) {                /* Is number of bytes in the transmit buffer greater then 0? */
    OutLen--;                          /* Decrease number of chars in the transmit buffer */
    SerFlag |= (RUNINT_FROM_TX);       /* Set flag "running int from TX"? */
    getReg(QSCI0_STAT);                /* Reset interrupt request flags */
    QSCI0_DATA = (AS1_TComData)*(OutPtrR++); /* Store char to transmitter register */
    /*lint -save  -e946 Disable MISRA rule (17.2) checking. */
    if (OutPtrR >= (OutBuffer + AS1_OUT_BUF_SIZE)) { /* Is the pointer out of the transmit buffer? */
      OutPtrR = OutBuffer;             /* Set pointer on the first item into the transmit buffer */
    }
    /*lint -restore Enable MISRA rule (17.2) checking. */
  } else {
    OnFlags |= ON_FREE_TX;             /* Set flag "OnFreeTxBuf" */
    clrRegBit(QSCI0_CTRL1, TEIE);      /* Disable transmit interrupt */
  }
  if ((OnFlags & ON_TX_CHAR) != 0x00U) { /* Is flag "OnTxChar" set? */
    AS1_OnTxChar();                    /* If yes then invoke user event */
  }
  if ((OnFlags & ON_FREE_TX) != 0x00U) { /* Is flag "OnFreeTxBuf" set? */
    AS1_OnFreeTxBuf();                 /* If yes then invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  AS1_InterruptError (component AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the bean's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma interrupt alignsp saveall
void AS1_InterruptError(void)
{
  register word StatReg = getReg(QSCI0_STAT); /* Read status register */
  register word Data = getReg(QSCI0_DATA); /* Read data from the receiver */

  setReg(QSCI0_STAT, 0x00U);           /* Reset error request flags */
  if(StatReg & (QSCI0_STAT_OR_MASK|QSCI0_STAT_NF_MASK|QSCI0_STAT_FE_MASK|QSCI0_STAT_PF_MASK)) { /* Is an error detected? */
    SerFlag |= COMMON_ERR;             /* If yes then set an internal flag */
    if ((StatReg & QSCI0_STAT_FE_MASK) != 0x00U) { /* Is framing error flag set? */
      if (Data == 0x00U) {             /* When a zero character is received */
        SerFlag |= BREAK_ERR;          /* When a zero character is received and framing error ocurred the break character is received */
      } else {
        SerFlag |= FRAMING_ERR;
      }
    }
  }
  if ((SerFlag & COMMON_ERR) != 0x00U) { /* Was any error set? */
    AS1_OnError();                     /* If yes then invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  AS1_Init (component AsynchroSerial)
**
**     Description :
**         Initializes the associated peripheral(s) and the bean internal 
**         variables. The method is called automatically as a part of the 
**         application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AS1_Init(void)
{
  SerFlag = 0x00U;                     /* Reset flags */
  /* QSCI0_CTRL1: LOOP=0,SWAI=0,RSRC=0,M=0,WAKE=0,POL=0,PE=0,PT=0,TEIE=0,TIIE=0,RFIE=0,REIE=0,TE=0,RE=0,RWU=0,SBK=0 */
  setReg(QSCI0_CTRL1, 0x00U);          /* Set the SCI configuration */
  InpLen = 0x00U;                      /* No char in the receive buffer */
  InpPtrR = InpBuffer;                 /* Set pointer on the first item in the receive buffer */
  InpPtrW = InpBuffer;
  OutLen = 0x00U;                      /* No char in the transmit buffer */
  OutPtrR = OutBuffer;                 /* Set pointer on the first item in the transmit buffer */
  OutPtrW = OutBuffer;
  /* QSCI0_RATE: SBR=0x0186,FRAC_SBR=5 */
  setReg(QSCI0_RATE, 0x0C35U);         /* Set prescaler bits */
  HWEnDi();                            /* Enable/disable device according to status flags */
}


/* END AS1. */


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
