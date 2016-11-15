/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : Cpu.c
**     Project     : CocciMaster
**     Processor   : MC56F84451VLF
**     Component   : MC56F84451VLF
**     Version     : Component 01.004, Driver 01.01, CPU db: 3.50.001
**     Datasheet   : MC56F844XXRM Rev.1 06/2012
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-14, 19:55, # CodeGen: 9
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         EnableInt   - void Cpu_EnableInt(void);
**         DisableInt  - void Cpu_DisableInt(void);
**         SetWaitMode - void Cpu_SetWaitMode(void);
**         SetStopMode - void Cpu_SetStopMode(void);
**
**     (c) Freescale Semiconductor, Inc.
**     2004 All Rights Reserved
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
** @file Cpu.c
** @version 01.01
** @brief
**
*/         
/*!
**  @addtogroup Cpu_module Cpu module documentation
**  @{
*/         

/* MODULE Cpu. */
#include "AS1.h"
#include "CAN1.h"
#include "TI1.h"
#include "SM1.h"
#include "CSOutput.h"
#include "CSInput.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"


/* Global variables */
volatile word SR_lock = 0U;            /* Lock */
volatile word SR_reg;                  /* Current value of the SR register */

/*Definition of global shadow variables*/
word Shadow_GPIOE_DR;
word Shadow_GPIOC_DR;

/*
** ===================================================================
**     Method      :  Cpu_Interrupt (component MC56F84451VLF)
**
**     Description :
**         The method services unhandled interrupt vectors.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma interrupt alignsp
void Cpu_Interrupt(void)
{
/* This code can be changed using the CPU bean property "Build Options / Unhandled int code" */
  asm(DEBUGHLT);                       /* Halt the core and placing it in the debug processing state. */
  /* Here should be two NOPs if DEBUGHLT is removed.
  asm(nop);
  asm(nop); */
}

/*
** ===================================================================
**     Method      :  Cpu_DisableInt (component MC56F84451VLF)
**     Description :
**         Disables all maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_DisableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_EnableInt (component MC56F84451VLF)
**     Description :
**         Enables all maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_EnableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_SetStopMode (component MC56F84451VLF)
**     Description :
**         Sets low power mode - Stop mode.
**         For more information about the stop mode see this CPU
**         documentation.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_SetStopMode(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_SetWaitMode (component MC56F84451VLF)
**     Description :
**         Sets low power mode - Wait mode.
**         For more information about the wait mode see this CPU
**         documentation.
**         Release from wait mode: Reset or interrupt
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_SetWaitMode(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  _EntryPoint (component MC56F84451VLF)
**
**     Description :
**         Initializes the whole system like timing and so on. At the end 
**         of this function, the C startup is invoked to initialize stack,
**         memory areas and so on.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
extern void init_56800_(void);         /* Forward declaration of external startup function declared in startup file */

/*** !!! Here you can place your own code using property "User data declarations" on the build options tab. !!! ***/

void _EntryPoint(void)
{
  #pragma constarray off

  /*** !!! Here you can place your own code before PE initialization using property "User code before PE initialization" on the build options tab. !!! ***/

  /*** ### MC56F84451VLF "Cpu" init code ... ***/

  /*** PE initialization code after reset ***/

  /* System clock initialization */
  setRegBitGroup(OCCS_OSCTL1, FREQ_TRIM8M, ((*(word *)0xE42C) & 0x03FFU)); /* Trim the 8MHz internal relaxation oscillator, frequency trim value */
  clrSetReg16Bits(OCCS_OSCTL1, OCCS_OSCTL1_ROPD_MASK, OCCS_OSCTL1_CLK_MODE_MASK); /* Enable internal 8MHz oscillator and select an external clock bypass mode */
  setRegBitGroup(OCCS_CTRL, PRECS, 0U); /* Select an internal 8MHz clock source for the CPU core */
  clrSetReg16Bits(OCCS_CTRL, OCCS_CTRL_PLLPD_MASK, OCCS_CTRL_LCKON_MASK); /* Enable PLL, LCKON and select clock source from prescaler */
  /* OCCS_DIVBY: LORTP=2,COD=0,??=0,??=0,PLLDB=0x1D */
  setReg16(OCCS_DIVBY, 0x201DU);       /* Set the clock prescalers */ 
  while(!getRegBit(OCCS_STAT, LCK0)){} /* Wait for PLL lock */
  setReg16Bits(OCCS_CTRL, OCCS_CTRL_ZSRC_MASK); /* Enable PLL, LCKON and select clock source from prescaler */
  /* OCCS_PROT: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,FRQEP=0,OSCEP=0,PLLEP=0 */
  setReg16(OCCS_PROT, 0x00U);          /* Set the OCCS protection register */ 
  /*** End of PE initialization code after reset ***/

  /*** !!! Here you can place your own code after PE initialization using property "User code after PE initialization" on the build options tab. !!! ***/

  asm(JMP init_56800_);                /* Jump to C startup code */
}

/*
** ===================================================================
**     Method      :  PE_low_level_init (component MC56F84451VLF)
**
**     Description :
**         Initializes beans and provides common register initialization. 
**         The method is called automatically as a part of the 
**         application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_low_level_init(void)
{
      /* Initialization of the SIM module */
  /* SIM_MISC0: PIT_MSTR=0 */
  clrReg16Bits(SIM_MISC0, 0x01U);       
  /* SIM_CTRL: RST_FILT=0,DMAEbl=3,OnceEbl=0,STOP_disable=0,WAIT_disable=0 */
  clrSetReg16Bits(SIM_CTRL, 0x052FU, 0xC0U); 
  /* SIM_PWR: SR12STDBY=0,SR27PDN=0,SR27STDBY=0,LRSTDBY=0 */
  clrReg16Bits(SIM_PWR, 0xFFU);         
  /* SIM_CLKOUT: CLKODIV=0,CLKDIS1=1,CLKOSEL1=0,CLKDIS0=1,CLKOSEL0=0 */
  clrSetReg16Bits(SIM_CLKOUT, 0xE387U, 0x1020U); 
  /* SIM_PCE0: TA0=0,TA1=0,TA2=0,TA3=0,TB0=0,TB1=0,TB2=0,TB3=0,GPIOA=0,GPIOB=0,GPIOC=1,GPIOD=0,GPIOE=1,GPIOF=0,GPIOG=0 */
  clrSetReg16Bits(SIM_PCE0, 0xFF6BU, 0x14U); 
  /* SIM_PCE1: SCI0=1,SCI1=0,QSPI0=1,QSPI1=0,IIC0=0,IIC1=0,FLEXCAN=1 */
  clrSetReg16Bits(SIM_PCE1, 0x0960U, 0x1201U); 
  /* SIM_PCE2: CMPA=0,CMPB=0,CMPC=0,CYCADC=0,CRC=0,QDC=0,PIT0=1,PIT1=0,PDB0=0,PDB1=0 */
  clrSetReg16Bits(SIM_PCE2, 0x1CB7U, 0x08U); 
  /* SIM_PCE3: PWMACH0=0,PWMACH1=0,PWMACH2=0,PWMACH3=0 */
  clrReg16Bits(SIM_PCE3, 0xF0U);        
  /* SIM_SD0: TA0=0,TA1=0,TA2=0,TA3=0,TB0=0,TB1=0,TB2=0,TB3=0,GPIOA=0,GPIOB=0,GPIOC=0,GPIOD=0,GPIOE=0,GPIOF=0,GPIOG=0 */
  clrReg16Bits(SIM_SD0, 0xFF7FU);       
  /* SIM_SD1: SCI0=0,SCI1=0,QSPI0=0,QSPI1=0,IIC0=0,IIC1=0,FLEXCAN=0 */
  clrReg16Bits(SIM_SD1, 0x1B61U);       
  /* SIM_SD2: CMPA=0,CMPB=0,CMPC=0,CYCADC=0,CRC=0,QDC=0,PIT0=0,PIT1=0,PDB0=0,PDB1=0 */
  clrReg16Bits(SIM_SD2, 0x1CBFU);       
  /* SIM_SD3: PWMACH0=0,PWMACH1=0,PWMACH2=0,PWMACH3=0 */
  clrReg16Bits(SIM_SD3, 0xF0U);         
  /* SIM_PROT: PMODE=0,GDP=0,PCEP=0,GIPSP=0 */
  clrReg16Bits(SIM_PROT, 0xFFU);        
  /* SIM_PCR: ??=0,??=0,SCI0_CR=1,SCI1_CR=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg16(SIM_PCR, 0x3000U);          /* Set the QSCI0; QSCI1 module clock rates */ 
      /* Initialization of the MCM module */
  /* MCM_UPRAMBAR: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,RBA=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg32(MCM_UPRAMBAR, 0x00UL);       
  /* MCM_UFLASHBAR: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,FBA=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg32(MCM_UFLASHBAR, 0x00UL);      
  /* MCM_CPCR: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,IBDIS=0,SRDIS=0,RCDIS=0,INSDIS=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg32(MCM_CPCR, 0x00UL);           
  /* MCM_RPCR: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,RL=0,RPE=0 */
  setReg32(MCM_RPCR, 0x00UL);           
        /* Initialization of the PMC module */
  /* PMC_STS: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,SR27=1,LVI=1,SLV27F=1,SLV22F=1,LV27F=0,LV22F=0 */
  setReg16(PMC_STS, 0x3CU);             
  /* PMC_CTRL: TRIM=*58413 */
  setReg16(PMC_CTRL, PMC_CTRL_TRIM_MASK & (*(word *)0xE42D));
      /* Initialization of the GPIOA module */
  /* GPIOA_DRIVE: DRIVE4=0,DRIVE3=0,DRIVE2=0,DRIVE1=0,DRIVE0=0 */
  clrReg16Bits(GPIOA_DRIVE, 0x1FU);     
  /* GPIOA_SRE: SRE4=1,SRE3=1,SRE2=1,SRE1=1,SRE0=1 */
  setReg16Bits(GPIOA_SRE, 0x1FU);       
      /* Initialization of the GPIOB module */
  /* GPIOB_DRIVE: DRIVE4=0,DRIVE3=0,DRIVE2=0,DRIVE1=0,DRIVE0=0 */
  clrReg16Bits(GPIOB_DRIVE, 0x1FU);     
  /* GPIOB_SRE: SRE4=1,SRE3=1,SRE2=1,SRE1=1,SRE0=1 */
  setReg16Bits(GPIOB_SRE, 0x1FU);       
      /* Initialization of the GPIOC module */
  /* GPIOC_DRIVE: DRIVE15=0,DRIVE14=0,DRIVE13=0,DRIVE12=0,DRIVE11=0,DRIVE10=0,DRIVE9=0,DRIVE8=0,DRIVE7=0,DRIVE6=0,DRIVE5=0,DRIVE4=0,DRIVE3=0,DRIVE2=0,DRIVE1=0,DRIVE0=0 */
  setReg16(GPIOC_DRIVE, 0x00U);         
  /* GPIOC_SRE: SRE15=1,SRE14=1,SRE13=1,SRE12=1,SRE11=1,SRE10=1,SRE9=1,SRE8=1,SRE7=1,SRE6=1,SRE5=1,SRE4=1,SRE3=1,SRE2=1,SRE1=1,SRE0=1 */
  setReg16(GPIOC_SRE, 0xFFFFU);         
      /* Initialization of the GPIOD module */
  /* GPIOD_DRIVE: DRIVE4=0,DRIVE3=0,DRIVE2=0,DRIVE1=0,DRIVE0=0 */
  clrReg16Bits(GPIOD_DRIVE, 0x1FU);     
  /* GPIOD_SRE: SRE4=1,SRE3=0,SRE2=1,SRE1=0,SRE0=1 */
  clrSetReg16Bits(GPIOD_SRE, 0x0AU, 0x15U); 
      /* Initialization of the GPIOE module */
  /* GPIOE_DRIVE: DRIVE5=0,DRIVE4=0,DRIVE3=0,DRIVE2=0,DRIVE1=0,DRIVE0=0 */
  clrReg16Bits(GPIOE_DRIVE, 0x3FU);     
  /* GPIOE_SRE: SRE5=1,SRE4=1,SRE3=1,SRE2=1,SRE1=1,SRE0=1 */
  setReg16Bits(GPIOE_SRE, 0x3FU);       
      /* Initialization of the GPIOF module */
  /* GPIOF_DRIVE: DRIVE1=0,DRIVE0=0 */
  clrReg16Bits(GPIOF_DRIVE, 0x03U);     
  /* GPIOF_SRE: SRE1=1,SRE0=1 */
  setReg16Bits(GPIOF_SRE, 0x03U);       
/*lint -save  -e586 Disable MISRA rule (2.1) checking. Functionality is implemented using assembler. */
  /* Shadow registers initialization */
  asm {
    swap shadows
    nop
    nop
    move.l #0, R0
    move.l #0, R1
    move.l #0, R2
    move.l #0, R3
    move.l #0, R4
    move.l #0, R5
    move.l #0, N
    moveu.w #65535, N3
    moveu.w #65535, M01
    nop
    nop
    swap shadows
  }
/*lint -restore Enable MISRA rule (2.1) checking. */
  /* Common initialization of the CPU registers */
  /* GPIOC_PER: PE12=1,PE11=1,PE10=1,PE9=1,PE8=1,PE5=0,PE3=1,PE2=1 */
  clrSetReg16Bits(GPIOC_PER, 0x20U, 0x1F0CU); 
  /* SIM_GPSCL: C3=2,C2=0 */
  clrSetReg16Bits(SIM_GPSCL, 0x70U, 0x80U); 
  /* INTC_IPR5: QSCI0_TDRE=2,QSCI0_TIDLE=2,QSCI0_RCV=2,QSCI0_RERR=2 */
  clrSetReg16Bits(INTC_IPR5, 0x0550U, 0x0AA0U); 
  /* SIM_GPSCH: C12=0,C11=0,C10=0,C9=0,C8=0 */
  clrReg16Bits(SIM_GPSCH, 0x03F7U);     
  /* INTC_IPR3: CAN_TX_WARN=2,CAN_ERROR=2,CAN_BUS_OFF=2,CAN_MB_OR=2 */
  clrSetReg16Bits(INTC_IPR3, 0x5500U, 0xAA00U); 
  /* INTC_IPR4: CAN_RX_WARN=2 */
  clrSetReg16Bits(INTC_IPR4, 0x01U, 0x02U); 
  /* INTC_IPR10: PIT0_ROLLOVR=2 */
  clrSetReg16Bits(INTC_IPR10, 0x1000U, 0x2000U); 
  /* INTC_IPR6: QSPI0_RCV=2,QSPI0_XMIT=2 */
  clrSetReg16Bits(INTC_IPR6, 0x50U, 0xA0U); 
  /* GPIOE_IENR: IEN5=0 */
  clrReg16Bits(GPIOE_IENR, 0x20U);      
  /* GPIOE_IAR: IA5=0 */
  clrReg16Bits(GPIOE_IAR, 0x20U);       
  /* GPIOE_IESR: IES5=1 */
  setReg16Bits(GPIOE_IESR, 0x20U);      
  /* GPIOE_PUR: PU5=0 */
  clrReg16Bits(GPIOE_PUR, 0x20U);       
  /* GPIOE_PPMODE: PPMODE5=1 */
  setReg16Bits(GPIOE_PPMODE, 0x20U);    
  /* GPIOE_DR: D5=0 */
  clrReg16Bits(GPIOE_DR, 0x20U);        
  /* GPIOE_DDR: DD5=1 */
  setReg16Bits(GPIOE_DDR, 0x20U);       
  /* GPIOE_PER: PE5=0 */
  clrReg16Bits(GPIOE_PER, 0x20U);       
  /* GPIOC_IENR: IEN5=0 */
  clrReg16Bits(GPIOC_IENR, 0x20U);      
  /* GPIOC_IAR: IA5=0 */
  clrReg16Bits(GPIOC_IAR, 0x20U);       
  /* GPIOC_IESR: IES5=1 */
  setReg16Bits(GPIOC_IESR, 0x20U);      
  /* GPIOC_PUR: PU5=0 */
  clrReg16Bits(GPIOC_PUR, 0x20U);       
  /* GPIOC_PPMODE: PPMODE5=1 */
  setReg16Bits(GPIOC_PPMODE, 0x20U);    
  /* GPIOC_DR: D5=0 */
  clrReg16Bits(GPIOC_DR, 0x20U);        
  /* GPIOC_DDR: DD5=1 */
  setReg16Bits(GPIOC_DDR, 0x20U);       
  /* GPIOD_PER: PE4=1 */
  setReg16Bits(GPIOD_PER, 0x10U);       
  /* Initialization of the SIM module */
  /* ### Asynchro serial "AS1" init code ... */
  AS1_Init();
  /* ###  "CAN1" init code ... */
  CAN1_Init();
  /* ### TimerInt "TI1" init code ... */
  TI1_Init();
  /* ### SynchroMaster "SM1" init code ... */
  SM1_Init();
  /* ### BitIO "CSOutput" init code ... */
  Shadow_GPIOE_DR &= ~(word)0x20U;     /* Initialize shadow variable */
  /* ### BitIO "CSInput" init code ... */
  Shadow_GPIOC_DR &= ~(word)0x20U;     /* Initialize shadow variable */
  __EI(0);                             /* Enable interrupts of the selected priority level */
}

/* END Cpu. */

