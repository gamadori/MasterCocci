/* ###################################################################
**     Filename    : main.c
**     Project     : CocciMaster
**     Processor   : MC56F84451VLF
**     Version     : Driver 01.16
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-11, 22:10, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.16
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "CAN1.h"
#include "TI1.h"
#include "SM1.h"
#include "CSOutput.h"
#include "CSInput.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "Timer.h"
#include "Plc.h"
#include "NMT.h"
#include "CAN.h"
#include "CanOpen.h"
#include "Serial.h"

// Prova commit
void main(void)
{
  /* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */

	TimersInit();
	InitPlc();
	NMTInit();
	CAN1_Enable();
	
	for(;;)
	{
		CanServerTrasmit();
		
		IOServer();
		
		SerialReceve();
		CANOPENServer();
		
		
	}
}

/* END main */
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
